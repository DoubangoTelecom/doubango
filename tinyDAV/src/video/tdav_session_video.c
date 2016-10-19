/*
 * Copyright (C) 2010-2016 Mamadou DIOP.
 * Copyright (C) 2011-2016 Doubango Telecom.
 *
 *
 * This file is part of Open Source Doubango Framework.
 *
 * DOUBANGO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DOUBANGO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DOUBANGO.
 *
 */

/**@file tdav_session_video.c
 * @brief Video Session plugin.
 *
 */
#include "tinydav/video/tdav_session_video.h"
#include "tinydav/video/tdav_converter_video.h"
#include "tinydav/video/jb/tdav_video_jb.h"
#include "tinydav/codecs/fec/tdav_codec_red.h"
#include "tinydav/codecs/fec/tdav_codec_ulpfec.h"

#include "tinymedia/tmedia_converter_video.h"
#include "tinymedia/tmedia_consumer.h"
#include "tinymedia/tmedia_producer.h"
#include "tinymedia/tmedia_defaults.h"
#include "tinymedia/tmedia_params.h"

#include "tinyrtp/trtp_manager.h"
#include "tinyrtp/rtcp/trtp_rtcp_header.h"
#include "tinyrtp/rtp/trtp_rtp_packet.h"
#include "tinyrtp/rtcp/trtp_rtcp_packet.h"
#include "tinyrtp/rtcp/trtp_rtcp_report_rr.h"
#include "tinyrtp/rtcp/trtp_rtcp_report_sr.h"
#include "tinyrtp/rtcp/trtp_rtcp_report_fb.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

#include <limits.h> /* INT_MAX */

// Minimum time between two incoming FIR. If smaller, the request from the remote party will be ignored
// Tell the encoder to send IDR frame if condition is met
#if METROPOLIS
#	define TDAV_SESSION_VIDEO_AVPF_FIR_HONOR_INTERVAL_MIN		0 // millis
#else
#	define TDAV_SESSION_VIDEO_AVPF_FIR_HONOR_INTERVAL_MIN		750 // millis
#endif
// Minimum time between two outgoing FIR. If smaller, the request from the remote party will be ignored
// Tell the RTCP session to request IDR if condition is met
#if METROPOLIS
#	define TDAV_SESSION_VIDEO_AVPF_FIR_REQUEST_INTERVAL_MIN		0 // millis
#else
#	define TDAV_SESSION_VIDEO_AVPF_FIR_REQUEST_INTERVAL_MIN		1500 // millis
#endif

// Interval to compute average quality metrics
#define TDAV_SESSION_VIDEO_QOS_COMPUTE_INTERVAL					3000

// The maximum number of pakcet loss allowed
#define TDAV_SESSION_VIDEO_PKT_LOSS_MAX_COUNT_TO_REQUEST_FIR	50

static const tmedia_codec_action_t __action_encode_idr = tmedia_codec_action_encode_idr;
static const tmedia_codec_action_t __action_encode_bw_up = tmedia_codec_action_bw_up;
static const tmedia_codec_action_t __action_encode_bw_down = tmedia_codec_action_bw_down;


#define _tdav_session_video_remote_requested_idr(__self, __ssrc_media) { \
uint64_t __now = tsk_time_now(); \
tsk_bool_t too_close = tsk_false; \
if((__now - (__self)->avpf.last_fir_time) > TDAV_SESSION_VIDEO_AVPF_FIR_HONOR_INTERVAL_MIN){ /* guard to avoid sending too many FIR */ \
_tdav_session_video_codec_set_int32((__self), "action", __action_encode_idr); \
}else { too_close = tsk_true; TSK_DEBUG_INFO("***IDR request tooo close(%llu ms)...ignoring****", (__now - (__self)->avpf.last_fir_time)); } \
if((__self)->cb_rtcpevent.func){ \
(__self)->cb_rtcpevent.func((__self)->cb_rtcpevent.context, tmedia_rtcp_event_type_fir, (__ssrc_media)); \
} \
if (!too_close) { /* if too close don't update "last_fir_time" to "now" to be sure interval will increase */ \
(__self)->avpf.last_fir_time = __now; \
} \
}
#define _tdav_session_video_local_request_idr(_session, _reason, _ssrc) \
{ \
tdav_session_av_t* _base = (tdav_session_av_t*)_session; \
if ((_base)->avpf_mode_neg || (_base)->is_fb_fir_neg) { \
/*return*/ trtp_manager_signal_frame_corrupted((_base)->rtp_manager, _ssrc); \
} \
else if ((_session)->rfc5168_cb.fun) { \
/*return*/ (_session)->rfc5168_cb.fun((_session)->rfc5168_cb.usrdata, (_session), (_reason), tmedia_session_rfc5168_cmd_picture_fast_update); \
} \
}
#define _tdav_session_video_bw_up(__self) _tdav_session_video_codec_set_int32(__self, "action", __action_encode_bw_up)
#define _tdav_session_video_bw_down(__self) _tdav_session_video_codec_set_int32(__self, "action", __action_encode_bw_down)
#define _tdav_session_video_bw_kbps(__self, __bw_kbps) _tdav_session_video_codec_set_int32(__self, "bw_kbps", __bw_kbps)


#define _tdav_session_video_reset_loss_prob(__self) \
{ \
(__self)->encoder.pkt_loss_level = tdav_session_video_pkt_loss_level_low; \
(__self)->encoder.pkt_loss_prob_bad = TDAV_SESSION_VIDEO_PKT_LOSS_PROB_BAD; \
(__self)->encoder.pkt_loss_prob_good = TDAV_SESSION_VIDEO_PKT_LOSS_PROB_GOOD; \
}

static int _tdav_session_video_set_defaults(tdav_session_video_t* self);
static int _tdav_session_video_codec_set_int32(tdav_session_video_t* self, const char* key, int32_t value);
static int _tdav_session_video_jb_cb(const tdav_video_jb_cb_data_xt* data);
static int _tdav_session_video_open_decoder(tdav_session_video_t* self, uint8_t payload_type);
static int _tdav_session_video_decode(tdav_session_video_t* self, const trtp_rtp_packet_t* packet);
static int _tdav_session_video_set_callbacks(tmedia_session_t* self);
static int _tdav_session_video_timer_cb(const void* arg, tsk_timer_id_t timer_id);
static int _tdav_session_video_get_bw_usage_est(tdav_session_video_t* self, uint64_t* bw_kbps, tsk_bool_t in, tsk_bool_t reset);
static int _tdav_session_video_report_bw_usage_and_jcng(tdav_session_video_t* self);

// Codec callback (From codec to the network)
// or Producer callback to sendRaw() data "as is"
static int tdav_session_video_raw_cb(const tmedia_video_encode_result_xt* result)
{
    tdav_session_av_t* base = (tdav_session_av_t*)result->usr_data;
    tdav_session_video_t* video = (tdav_session_video_t*)result->usr_data;
    trtp_rtp_header_t* rtp_header = (trtp_rtp_header_t*)result->proto_hdr;
    trtp_rtp_packet_t* packet = tsk_null;
    int ret = 0;
    tsk_size_t s;

    if(base->rtp_manager && base->rtp_manager->is_started) {
        if(rtp_header) {
            // uses negotiated SSRC (SDP)
            rtp_header->ssrc = base->rtp_manager->rtp.ssrc.local;
            // uses negotiated payload type
            if(base->pt_map.local != base->rtp_manager->rtp.payload_type || base->pt_map.remote != rtp_header->payload_type || base->pt_map.neg == -1) {
                if(rtp_header->codec_id == tmedia_codec_id_none) {
                    TSK_DEBUG_WARN("Internal codec id is equal to none");
                }
                else {
                    const tsk_list_item_t* item;
                    tsk_bool_t found = tsk_false;
                    tsk_list_lock(TMEDIA_SESSION(base)->neg_codecs);
                    tsk_list_foreach(item, TMEDIA_SESSION(base)->neg_codecs) {
                        if((item->data) && ((const tmedia_codec_t*)item->data)->id == rtp_header->codec_id) {
                            base->pt_map.local = base->rtp_manager->rtp.payload_type;
                            base->pt_map.remote = rtp_header->payload_type;
                            base->pt_map.neg = atoi(((const tmedia_codec_t*)item->data)->neg_format);
                            found = tsk_true;
                            break;
                        }
                    }
                    tsk_list_unlock(TMEDIA_SESSION(base)->neg_codecs);
                    if(found) {
                        TSK_DEBUG_INFO("Codec PT mapping: local=%d, remote=%d, neg=%d", base->pt_map.local, base->pt_map.remote, base->pt_map.neg);
                    }
                    else {
                        TSK_DEBUG_ERROR("Failed to map codec PT: local=%d, remote=%d", base->rtp_manager->rtp.payload_type, rtp_header->payload_type);
                    }
                }
            }
            rtp_header->payload_type = base->pt_map.neg;
        }
        packet = rtp_header
                 ? trtp_rtp_packet_create_2(rtp_header)
                 : trtp_rtp_packet_create(base->rtp_manager->rtp.ssrc.local, base->rtp_manager->rtp.seq_num, base->rtp_manager->rtp.timestamp, base->rtp_manager->rtp.payload_type, result->last_chunck);

        if(packet ) {
            tsk_size_t rtp_hdr_size;
            if(result->last_chunck) {
#if 1
#if 1
                /*	http://www.cs.columbia.edu/~hgs/rtp/faq.html#timestamp-computed
                 For video, time clock rate is fixed at 90 kHz. The timestamps generated depend on whether the application can determine the frame number or not.
                 If it can or it can be sure that it is transmitting every frame with a fixed frame rate, the timestamp is governed by the nominal frame rate. Thus, for a 30 f/s video, timestamps would increase by 3,000 for each frame, for a 25 f/s video by 3,600 for each frame.
                 If a frame is transmitted as several RTP packets, these packets would all bear the same timestamp.
                 If the frame number cannot be determined or if frames are sampled aperiodically, as is typically the case for software codecs, the timestamp has to be computed from the system clock (e.g., gettimeofday())
                 */

                if(!video->encoder.last_frame_time) {
                    // For the first frame it's not possible to compute the duration as there is no previous one.
                    // In this case, we trust the duration from the result (computed based on the codec fps and rate).
                    video->encoder.last_frame_time = tsk_time_now();
                    base->rtp_manager->rtp.timestamp += result->duration;
                }
                else {
                    uint64_t now = tsk_time_now();
                    uint32_t duration = (uint32_t)(now - video->encoder.last_frame_time);
                    base->rtp_manager->rtp.timestamp += (duration * 90/* 90KHz */);
                    video->encoder.last_frame_time = now;
                }
#else
                base->rtp_manager->rtp.timestamp = (uint32_t)(tsk_gettimeofday_ms() * 90/* 90KHz */);
#endif
#else
                base->rtp_manager->rtp.timestamp += result->duration;
#endif

            }

            packet->payload.data_const = result->buffer.ptr;
            packet->payload.size = result->buffer.size;
            s = trtp_manager_send_rtp_packet(base->rtp_manager, packet, tsk_false); // encrypt and send data
            ++base->rtp_manager->rtp.seq_num; // seq_num must be incremented here (before the bail) because already used by SRTP context
            if(s < TRTP_RTP_HEADER_MIN_SIZE) {
                // without audio session iOS "audio" background mode is useless and UDP sockets will be closed: e.g. GE's video-only sessions
#if TDAV_UNDER_IPHONE
                if (tnet_geterrno() == TNET_ERROR_BROKENPIPE) {
                    TSK_DEBUG_INFO("iOS UDP pipe is broken (restoration in progress): failed to send packet with seqnum=%u. %u expected but only %u sent", (unsigned)packet->header->seq_num, (unsigned)packet->payload.size, (unsigned)s);
                }
#endif /* TDAV_UNDER_IPHONE */
                TSK_DEBUG_ERROR("Failed to send packet with seqnum=%u. %u expected but only %u sent", (unsigned)packet->header->seq_num, (unsigned)packet->payload.size, (unsigned)s);
                // save data expected to be sent in order to honor RTCP-NACK requests
                s = base->rtp_manager->rtp.serial_buffer.index;
            }

            rtp_hdr_size = TRTP_RTP_HEADER_MIN_SIZE + (packet->header->csrc_count << 2);
            // Save packet
            if (base->avpf_mode_neg && (s > TRTP_RTP_HEADER_MIN_SIZE)) {
                trtp_rtp_packet_t* packet_avpf = tsk_object_ref(packet);
                // when SRTP is used, "serial_buffer" will contains the encoded buffer with both RTP header and payload
                // Hack the RTP packet payload to point to the the SRTP data instead of unencrypted ptr
                packet_avpf->payload.size = (s - rtp_hdr_size);
                packet_avpf->payload.data_const = tsk_null;
                if(!(packet_avpf->payload.data = tsk_malloc(packet_avpf->payload.size))) { // FIXME: to be optimized (reuse memory address)
                    TSK_DEBUG_ERROR("failed to allocate buffer");
                    goto bail;
                }
                memcpy(packet_avpf->payload.data, (((const uint8_t*)base->rtp_manager->rtp.serial_buffer.ptr) + rtp_hdr_size), packet_avpf->payload.size);
                tsk_list_lock(video->avpf.packets);
                if(video->avpf.count > video->avpf.max) {
                    tsk_list_remove_first_item(video->avpf.packets);
                }
                else {
                    ++video->avpf.count;
                }

                // The packet must not added 'ascending' but 'back' because the sequence number coult wrap
                // For example:
                //	- send(65533, 65534, 65535, 0, 1)
                //  - will be stored as (if added 'ascending'): 0, 1, 65533, 65534, 65535
                //  - this means there is no benefit (if added 'ascending') as we cannot make 'smart search' using seqnums
                // tsk_list_push_ascending_data(video->avpf.packets, (void**)&packet_avpf); // filtered per seqnum
                tsk_list_push_back_data(video->avpf.packets, (void**)&packet_avpf);
                tsk_list_unlock(video->avpf.packets);
            }

            // Send FEC packet
            // FIXME: protect only Intra and Params packets
            if(base->ulpfec.codec && (s > TRTP_RTP_HEADER_MIN_SIZE)) {
                packet->payload.data_const = (((const uint8_t*)base->rtp_manager->rtp.serial_buffer.ptr) + rtp_hdr_size);
                packet->payload.size = (s - rtp_hdr_size);
                ret = tdav_codec_ulpfec_enc_protect((struct tdav_codec_ulpfec_s*)base->ulpfec.codec, packet);
                if(result->last_chunck) {
                    trtp_rtp_packet_t* packet_fec;
                    if((packet_fec = trtp_rtp_packet_create(base->rtp_manager->rtp.ssrc.local, base->ulpfec.seq_num++, base->ulpfec.timestamp, base->ulpfec.payload_type, tsk_true))) {
                        // serialize the FEC payload packet packet
                        s = tdav_codec_ulpfec_enc_serialize((const struct tdav_codec_ulpfec_s*)base->ulpfec.codec, &video->encoder.buffer, &video->encoder.buffer_size);
                        if(s > 0) {
                            packet_fec->payload.data_const = video->encoder.buffer;
                            packet_fec->payload.size = s;
                            s = trtp_manager_send_rtp_packet(base->rtp_manager, packet_fec, tsk_true/* already encrypted */);
                        }
                        TSK_OBJECT_SAFE_FREE(packet_fec);
                    }
                    base->ulpfec.timestamp += result->duration;
                    ret = tdav_codec_ulpfec_enc_reset((struct tdav_codec_ulpfec_s*)base->ulpfec.codec);
                }
            }
#if 0
            // Send RED Packet
            if (ret == 0 && video->red.codec) {
                // don't need to lock as the buffer is never used by other codecs
                tsk_size_t red_pay_size = video->red.codec->plugin->encode(
                                              video->red.codec,
                                              buffer, size,
                                              &video->encoder.buffer, &video->encoder.buffer_size
                                          );
                if(red_pay_size > 1) {
                    packet->header->payload_type = video->red.payload_type;
                    ((uint8_t*)video->encoder.buffer)[0] = packet->header->payload_type;
                    packet->payload.data_const = video->encoder.buffer;
                    packet->payload.size = red_pay_size;
                    trtp_manager_send_rtp_2(base->rtp_manager, packet);
                }
            }
#endif
        }
        else {
            TSK_DEBUG_ERROR("Failed to create packet");
        }
    }
    else {
        //--TSK_DEBUG_WARN("Session not ready yet");
    }

bail:
    TSK_OBJECT_SAFE_FREE(packet);
    return ret;
}

// Codec Callback after decoding
static int tdav_session_video_decode_cb(const tmedia_video_decode_result_xt* result)
{
    tdav_session_av_t* base = (tdav_session_av_t*)result->usr_data;
    tdav_session_video_t* video = (tdav_session_video_t*)base;

    switch(result->type) {
    case tmedia_video_decode_result_type_idr: {
        if(video->decoder.last_corrupted_timestamp != ((const trtp_rtp_header_t*)result->proto_hdr)->timestamp) {
            TSK_DEBUG_INFO("IDR frame decoded");
            video->decoder.stream_corrupted = tsk_false;
        }
        else {
            TSK_DEBUG_INFO("IDR frame decoded but corrupted :(");
        }
        break;
    }
    case tmedia_video_decode_result_type_error: {
        TSK_DEBUG_INFO("Decoding failed -> request Full Intra Refresh (FIR)");
        _tdav_session_video_local_request_idr(TMEDIA_SESSION(video), "DECODED_FAILED", ((const trtp_rtp_header_t*)result->proto_hdr)->ssrc);
        break;
    }
    default:
        break;
    }
    return 0;
}

// Producer callback (From the producer to the network) => encode data before send()
static int tdav_session_video_producer_enc_cb(const void* callback_data, const void* buffer, tsk_size_t size)
{
    tdav_session_video_t* video = (tdav_session_video_t*)callback_data;
    tdav_session_av_t* base = (tdav_session_av_t*)callback_data;
    tmedia_session_t* session = (tmedia_session_t*)callback_data;
    tsk_size_t yuv420p_size = 0;
    int ret = 0;

    if(!base) {
        TSK_DEBUG_ERROR("Null session");
        return 0;
    }

    // do nothing if session is held
    // when the session is held the end user will get feedback he also has possibilities to put the consumer and producer on pause
    if (session->lo_held) {
        return 0;
    }

    // do nothing if not started yet
    if (!video->started) {
        TSK_DEBUG_INFO("Video session not started yet");
        return 0;
    }

    // get best negotiated codec if not already done
    // the encoder codec could be null when session is renegotiated without re-starting (e.g. hold/resume)
    if (!video->encoder.codec) {
        const tmedia_codec_t* codec;
        tsk_safeobj_lock(base);
        if (!(codec = tdav_session_av_get_best_neg_codec(base))) {
            TSK_DEBUG_ERROR("No codec matched");
            tsk_safeobj_unlock(base);
            return -2;
        }
        video->encoder.codec = tsk_object_ref(TSK_OBJECT(codec));
		// update negotiated video size
		video->neg_width = TMEDIA_CODEC_VIDEO(video->encoder.codec)->out.width;
		video->neg_height = TMEDIA_CODEC_VIDEO(video->encoder.codec)->out.height;
        tsk_safeobj_unlock(base);
    }

    if (base->rtp_manager) {
        //static int __rotation_counter = 0;
        /* encode */
        tsk_size_t out_size = 0;
        tmedia_codec_t* codec_encoder = tsk_null;
        uint64_t encode_start_time, encode_duration; // This time chroma conversion, scaling and encoding

        if (!base->rtp_manager->is_started) {
            TSK_DEBUG_ERROR("Not started");
            goto bail;
        }

        // take a reference to the encoder to make sure it'll not be destroyed while we're using it
        codec_encoder = tsk_object_ref(video->encoder.codec);
        if (!codec_encoder) {
            TSK_DEBUG_ERROR("The encoder is null");
            goto bail;
        }

        encode_start_time = tsk_time_now();

#define PRODUCER_OUTPUT_FIXSIZE (base->producer->video.chroma != tmedia_chroma_mjpeg) // whether the output data has a fixed size/length
#define PRODUCER_OUTPUT_RAW (base->producer->encoder.codec_id == tmedia_codec_id_none) // Otherwise, frames from the producer are already encoded
#define PRODUCER_SIZE_CHANGED ((video->conv.producerWidth && video->conv.producerWidth != base->producer->video.width) || (video->conv.producerHeight && video->conv.producerHeight != base->producer->video.height) \
|| (video->conv.xProducerSize && (video->conv.xProducerSize != size && PRODUCER_OUTPUT_FIXSIZE)))
#define ENCODER_SIZE_CHANGED (video->encoder.size_changed)
#define ENCODED_NEED_FLIP (TMEDIA_CODEC_VIDEO(codec_encoder)->out.flip)
#define ENCODED_NEED_RESIZE (base->producer->video.width != TMEDIA_CODEC_VIDEO(codec_encoder)->out.width || base->producer->video.height != TMEDIA_CODEC_VIDEO(codec_encoder)->out.height)
#define PRODUCED_FRAME_NEED_ROTATION (base->producer->video.rotation != 0)
#define PRODUCED_FRAME_NEED_MIRROR (base->producer->video.mirror != tsk_false)
#define PRODUCED_FRAME_NEED_CHROMA_CONVERSION (base->producer->video.chroma != TMEDIA_CODEC_VIDEO(codec_encoder)->out.chroma)
        // Video codecs only accept YUV420P buffers ==> do conversion if needed or producer doesn't have the right size
        if (PRODUCER_OUTPUT_RAW && (PRODUCED_FRAME_NEED_CHROMA_CONVERSION || PRODUCER_SIZE_CHANGED || ENCODER_SIZE_CHANGED || ENCODED_NEED_FLIP || ENCODED_NEED_RESIZE ||PRODUCED_FRAME_NEED_ROTATION || PRODUCED_FRAME_NEED_MIRROR)) {
            // Create video converter if not already done or producer/encoder size have changed
            if(!video->conv.toYUV420 || PRODUCER_SIZE_CHANGED || ENCODER_SIZE_CHANGED) {
                TSK_OBJECT_SAFE_FREE(video->conv.toYUV420);
                video->conv.producerWidth = base->producer->video.width;
                video->conv.producerHeight = base->producer->video.height;
                video->conv.xProducerSize = size;
				video->encoder.size_changed = tsk_false;

                TSK_DEBUG_INFO("producer size = (%d, %d)", (int)base->producer->video.width, (int)base->producer->video.height);
                if (!(video->conv.toYUV420 = tmedia_converter_video_create(base->producer->video.width, base->producer->video.height, base->producer->video.chroma, TMEDIA_CODEC_VIDEO(codec_encoder)->out.width, TMEDIA_CODEC_VIDEO(codec_encoder)->out.height,
                                             TMEDIA_CODEC_VIDEO(codec_encoder)->out.chroma))) {
                    TSK_DEBUG_ERROR("Failed to create video converter");
                    ret = -5;
                    goto bail;
                }
                // restore/set rotation scaling info because producer size could change
                tmedia_converter_video_set_scale_rotated_frames(video->conv.toYUV420, video->encoder.scale_rotated_frames);
            }
        }

        if(video->conv.toYUV420) {
            video->encoder.scale_rotated_frames = video->conv.toYUV420->scale_rotated_frames;
            // check if rotation have changed and alert the codec
            // we avoid scalling the frame after rotation because it's CPU intensive and keeping the image ratio is difficult
            // it's up to the encoder to swap (w,h) and to track the rotation value
            if(video->encoder.rotation != base->producer->video.rotation) {
                tmedia_param_t* param = tmedia_param_create(tmedia_pat_set,
                                        tmedia_video,
                                        tmedia_ppt_codec,
                                        tmedia_pvt_int32,
                                        "rotation",
                                        (void*)&base->producer->video.rotation);
                if (!param) {
                    TSK_DEBUG_ERROR("Failed to create a media parameter");
                    return -1;
                }
                video->encoder.rotation = base->producer->video.rotation; // update rotation to avoid calling the function several times
                ret = tmedia_codec_set(codec_encoder, param);
                TSK_OBJECT_SAFE_FREE(param);
                // (ret != 0) -> not supported by the codec -> to be done by the converter
                video->encoder.scale_rotated_frames = (ret != 0);
            }

            // update one-shot parameters
            tmedia_converter_video_set(video->conv.toYUV420, base->producer->video.rotation, TMEDIA_CODEC_VIDEO(codec_encoder)->out.flip, base->producer->video.mirror, video->encoder.scale_rotated_frames);

            yuv420p_size = tmedia_converter_video_process(video->conv.toYUV420, buffer, size, &video->encoder.conv_buffer, &video->encoder.conv_buffer_size);
            if (!yuv420p_size || !video->encoder.conv_buffer) {
                TSK_DEBUG_ERROR("Failed to convert XXX buffer to YUV42P");
                ret = -6;
                goto bail;
            }
        }

        // Encode data
        tsk_mutex_lock(video->encoder.h_mutex);
        if (video->started && codec_encoder->opened && !video->encoder.size_changed) { // stop() function locks the encoder mutex before changing "started"
            if (video->encoder.conv_buffer && yuv420p_size) {
                /* producer doesn't support yuv42p */
                out_size = codec_encoder->plugin->encode(codec_encoder, video->encoder.conv_buffer, yuv420p_size, &video->encoder.buffer, &video->encoder.buffer_size);
            }
            else {
                /* producer supports yuv42p */
                out_size = codec_encoder->plugin->encode(codec_encoder, buffer, size, &video->encoder.buffer, &video->encoder.buffer_size);
            }
        }
        tsk_mutex_unlock(video->encoder.h_mutex);

        if (out_size) {
            /* Never called, see tdav_session_video_raw_cb() */
            trtp_manager_send_rtp(base->rtp_manager, video->encoder.buffer, out_size, 6006, tsk_true, tsk_true);
        }

        // This block of code is called on success, otherwise we'd jump to bail
        encode_duration = (tsk_time_now() - encode_start_time);
        if (base->congestion_ctrl_enabled) {
            tsk_mutex_lock(video->h_mutex_qos);
            session->qos_metrics.video_enc_avg_time = (unsigned)((session->qos_metrics.video_enc_avg_time + encode_duration) / (video->enc_avg_time_n++ ? 2 : 1));
            tsk_mutex_unlock(video->h_mutex_qos);
        }
bail:
        TSK_OBJECT_SAFE_FREE(codec_encoder);
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        ret = -1;
    }

    return ret;
}

// RTP callback (Network -> Decoder -> Consumer)
static int tdav_session_video_rtp_cb(const void* callback_data, const trtp_rtp_packet_t* packet)
{
    tdav_session_video_t* video = (tdav_session_video_t*)callback_data;
    tdav_session_av_t* base = (tdav_session_av_t*)callback_data;

    if(!video || !packet || !packet->header) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(packet->header->payload_type == base->red.payload_type) {
        static void* __red_buffer_ptr = tsk_null; // Never used
        static tsk_size_t __red_buffer_size = 0; // Never used
        if(!base->red.codec) {
            TSK_DEBUG_ERROR("No RED codec could be found");
            return -2;
        }
        // Decode RED data
        base->red.codec->plugin->decode(
            base->red.codec,
            (packet->payload.data ? packet->payload.data : packet->payload.data_const), packet->payload.size,
            &__red_buffer_ptr, &__red_buffer_size,
            packet->header
        );
        return 0;
    }
    else if(packet->header->payload_type == base->ulpfec.payload_type) {
        if(!base->ulpfec.codec) {
            TSK_DEBUG_ERROR("No ULPFEC codec could be found");
            return -2;
        }
        // FIXME: do something
        return 0;
    }
    else {
        return video->jb
               ? tdav_video_jb_put(video->jb, (trtp_rtp_packet_t*)packet)
               : _tdav_session_video_decode(video, packet);
    }
}

// RTCP callback (Network -> This)
static int tdav_session_video_rtcp_cb(const void* callback_data, const trtp_rtcp_packet_t* packet)
{
    int ret = 0;
    const trtp_rtcp_report_psfb_t* psfb;
    const trtp_rtcp_report_rtpfb_t* rtpfb;
    const trtp_rtcp_rblocks_L_t* blocks = tsk_null;

    tdav_session_video_t* video = (tdav_session_video_t*)callback_data;
    tdav_session_av_t* base = (tdav_session_av_t*)callback_data;
    tmedia_session_t* session = (tmedia_session_t*)callback_data;
    tsk_size_t i;

    if((blocks = (packet->header->type == trtp_rtcp_packet_type_rr) ? ((const trtp_rtcp_report_rr_t*)packet)->blocks :
                 (packet->header->type == trtp_rtcp_packet_type_sr ? ((const trtp_rtcp_report_sr_t*)packet)->blocks : tsk_null))) {
        const tsk_list_item_t* item;
        const trtp_rtcp_rblock_t* block;
        tsk_list_foreach(item, blocks) {
            if(!(block = item->data)) {
                continue;
            }
            if(base->rtp_manager->rtp.ssrc.local == block->ssrc) {
                TSK_DEBUG_INFO("RTCP pkt loss fraction=%d, congestion_ctrl_enabled=%d", block->fraction, (int)base->congestion_ctrl_enabled);
                // Global packet loss estimation
                if (base->congestion_ctrl_enabled) {
                    float q2;
                    q2 = block->fraction == 0 ? 1.f : ((float)block->fraction / 256.f);
                    tsk_mutex_lock(video->h_mutex_qos);
                    session->qos_metrics.q2 = (session->qos_metrics.q2 + q2) / (video->q2_n++ ? 2.f : 1.f);
                    tsk_mutex_unlock(video->h_mutex_qos);
                }
                break;
            }
        }
    }

    i = 0;
    while((psfb = (const trtp_rtcp_report_psfb_t*)trtp_rtcp_packet_get_at(packet, trtp_rtcp_packet_type_psfb, i++))) {
        switch(psfb->fci_type) {
        case trtp_rtcp_psfb_fci_type_fir: {
            TSK_DEBUG_INFO("Receiving RTCP-FIR (%u)", ((const trtp_rtcp_report_fb_t*)psfb)->ssrc_media);
            _tdav_session_video_remote_requested_idr(video, ((const trtp_rtcp_report_fb_t*)psfb)->ssrc_media);
            break;
        }
        case trtp_rtcp_psfb_fci_type_pli: {
            uint64_t now;
            TSK_DEBUG_INFO("Receiving RTCP-PLI (%u)", ((const trtp_rtcp_report_fb_t*)psfb)->ssrc_media);
            now = tsk_time_now();
            // more than one PLI in 500ms ?
            // "if" removed because PLI really means codec prediction chain is broken
            /*if((now - video->avpf.last_pli_time) < 500)*/{
                _tdav_session_video_remote_requested_idr(video, ((const trtp_rtcp_report_fb_t*)psfb)->ssrc_media);
            }
            video->avpf.last_pli_time = now;
            break;
        }
        case trtp_rtcp_psfb_fci_type_afb: {
            if (psfb->afb.type == trtp_rtcp_psfb_afb_type_remb) {
                uint64_t bw_up_reported_kpbs = ((psfb->afb.remb.mantissa << psfb->afb.remb.exp) >> 10);
                TSK_DEBUG_INFO("Receiving RTCP-AFB-REMB (%u), exp=%u, mantissa=%u, bandwidth=%llukbps", ((const trtp_rtcp_report_fb_t*)psfb)->ssrc_media, psfb->afb.remb.exp, psfb->afb.remb.mantissa, bw_up_reported_kpbs);
                if (base->congestion_ctrl_enabled) {
                    if (session->qos_metrics.bw_up_est_kbps != 0) {
                        float q3 = bw_up_reported_kpbs / (float)session->qos_metrics.bw_up_est_kbps;
                        q3 = TSK_CLAMP(0.f, q3, 1.f);
                        TSK_DEBUG_INFO("bw_up_estimated_kbps=%u, bw_up_reported_kpbs=%llu, q3=%f", session->qos_metrics.bw_up_est_kbps, bw_up_reported_kpbs, q3);
                        tsk_mutex_lock(video->h_mutex_qos);
                        session->qos_metrics.q3 = (session->qos_metrics.q3 + q3) / (video->q3_n++ ? 2.f : 1.f);
                        tsk_mutex_unlock(video->h_mutex_qos);
                    }
                }
            }
            else if (psfb->afb.type == trtp_rtcp_psfb_afb_type_jcng) {
                float jcng_q = ((float)psfb->afb.jcng.q / 255.f);
                TSK_DEBUG_INFO("Receiving RTCP-AFB-JCNG (%u), q_recv=%u, q_dec=%f", ((const trtp_rtcp_report_fb_t*)psfb)->ssrc_media, psfb->afb.jcng.q, jcng_q);
                if (base->congestion_ctrl_enabled) {
                    tsk_mutex_lock(video->h_mutex_qos);
                    session->qos_metrics.q5 = (session->qos_metrics.q5 + jcng_q) / (video->q5_n++ ? 2.f : 1.f);
                    tsk_mutex_unlock(video->h_mutex_qos);
                }
            }
            break;
        }
        default:
            break;
        }
    }
    i = 0;
    while((rtpfb = (const trtp_rtcp_report_rtpfb_t*)trtp_rtcp_packet_get_at(packet, trtp_rtcp_packet_type_rtpfb, i++))) {
        switch(rtpfb->fci_type) {
        default:
            break;
        case trtp_rtcp_rtpfb_fci_type_nack: {
            if(rtpfb->nack.blp && rtpfb->nack.pid) {
                tsk_size_t i;
                int32_t j;
                uint16_t pid, blp;
                uint32_t r = 0; // the number of recoverable packets (lost but recovered using a NACK requests)
                uint32_t u = 0; // the number of unrecoverable packets (lost but not recovered using NACK requests)
                const tsk_list_item_t* item;
                const trtp_rtp_packet_t* pkt_rtp;
                for(i = 0; i < rtpfb->nack.count; ++i) {
                    static const int32_t __Pow2[16] = { 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000 };
                    int32_t blp_count;
                    blp = rtpfb->nack.blp[i];
                    blp_count = blp ? 16 : 0;

                    for(j = -1/*Packet ID (PID)*/; j < blp_count; ++j) {
                        if(j == -1 || (blp & __Pow2[j])) {
                            pid = (rtpfb->nack.pid[i] + (j + 1));
                            tsk_list_lock(video->avpf.packets);
                            tsk_list_foreach(item, video->avpf.packets) {
                                if(!(pkt_rtp = item->data)) {
                                    continue;
                                }

                                // Very Important: the seq_nums are not consecutive because of wrapping.
                                // For example, '65533, 65534, 65535, 0, 1' is a valid sequences which means we have to check all packets (probaly need somthing smarter)
                                if(pkt_rtp->header->seq_num == pid) {
                                    ++r;
                                    TSK_DEBUG_INFO("NACK Found, pid=%d, blp=%u, r=%u", pid, blp, r);
                                    trtp_manager_send_rtp_packet(base->rtp_manager, pkt_rtp, tsk_true);
                                    break;
                                }
                                if(item == video->avpf.packets->tail) {
                                    // should never be called unless the tail is too small
                                    int32_t old_max = (int32_t)video->avpf.max;
                                    int32_t len_drop = (pkt_rtp->header->seq_num - pid);
                                    ++u;
                                    video->avpf.max = TSK_CLAMP((int32_t)tmedia_defaults_get_avpf_tail_min(), (old_max + len_drop), (int32_t)tmedia_defaults_get_avpf_tail_max());
                                    TSK_DEBUG_INFO("**NACK requesting dropped frames. List=[%d-%d], requested=%d, List.Max=%d, List.Count=%d, u=%u. RTT is probably too high.",
                                                   ((const trtp_rtp_packet_t*)TSK_LIST_FIRST_DATA(video->avpf.packets))->header->seq_num,
                                                   ((const trtp_rtp_packet_t*)TSK_LIST_LAST_DATA(video->avpf.packets))->header->seq_num,
                                                   pid,
                                                   (int)video->avpf.max,
                                                   (int)video->avpf.count,
                                                   (unsigned)u);
                                    // FIR not really requested but needed
                                    /*_tdav_session_video_remote_requested_idr(video, ((const trtp_rtcp_report_fb_t*)rtpfb)->ssrc_media);
                                     tsk_list_clear_items(video->avpf.packets);
                                     video->avpf.count = 0;*/
                                } // if(last_item)
                            }// foreach(pkt)
                            tsk_list_unlock(video->avpf.packets);
                        }// if(BLP is set)
                    }// foreach(BIT in BLP)
                }// foreach(nack)
                if (base->congestion_ctrl_enabled) {
                    // Compute q1
                    if (r || u) {
                        float q1 = 1.f - (((r * 0.2f) + (u * 0.8f)) / (r + u));
                        tsk_mutex_lock(video->h_mutex_qos);
                        session->qos_metrics.q1 = (session->qos_metrics.q1 + q1) / (video->q1_n++ ? 2.f : 1.f);
                        tsk_mutex_unlock(video->h_mutex_qos);
                        TSK_DEBUG_INFO("RTCP-NACK: r=%u, u=%u, q1=%f", r, u, q1);
                    }
                }
            }// if(nack-blp and nack-pid are set)
            break;
        }// case
        }// switch
    }// while(rtcp-pkt)

    return ret;
}

static int _tdav_session_video_set_defaults(tdav_session_video_t* self)
{
    tmedia_session_t* session = (tmedia_session_t*)self;
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->jb_enabled = tmedia_defaults_get_videojb_enabled();
    self->zero_artifacts = tmedia_defaults_get_video_zeroartifacts_enabled();
    self->avpf.max = tmedia_defaults_get_avpf_tail_min();
    self->encoder.last_frame_time = 0;
	self->last_sendreport_time = 0;
    // Quality metrics
    session->qos_metrics.q1 = 0.f, self->q1_n = 0;
    session->qos_metrics.q2 = 0.f, self->q2_n = 0;
    session->qos_metrics.q3 = 0.f, self->q3_n = 0;
    session->qos_metrics.q4 = 0.f, self->q4_n = 0;
    session->qos_metrics.qvag = 1.f;
    session->qos_metrics.bw_up_est_kbps = 0;
    session->qos_metrics.bw_down_est_kbps = 0;
    session->qos_metrics.last_update_time = 0;
    session->qos_metrics.video_out_width = 0;
    session->qos_metrics.video_out_height = 0;
    session->qos_metrics.video_in_width = 0;
    session->qos_metrics.video_in_height = 0;
    session->qos_metrics.video_in_avg_fps = tmedia_defaults_get_video_fps(), self->in_avg_fps_n = 1;
    session->qos_metrics.video_dec_avg_time = 0, self->dec_avg_time_n = 0 ;
    session->qos_metrics.video_enc_avg_time = 0, self->enc_avg_time_n = 0;
	self->qavg_lowest = 1.f;
	self->num_qavg_down = 0;
	self->num_enc_avg_time_high = 0;

	// set negotiated video size to default pref-size
	tmedia_video_get_size(tmedia_defaults_get_pref_video_size(), &self->neg_width, &self->neg_height);

    // reset rotation info (MUST for reINVITE when mobile device in portrait[90 degrees])
    self->encoder.rotation = 0;

    TSK_DEBUG_INFO("Video 'zero-artifacts' option = %s", self->zero_artifacts  ? "yes" : "no");

    return 0;
}

static int _tdav_session_video_codec_set_int32(tdav_session_video_t* self, const char* key, int32_t value) 
{ 
	int ret = -1;
	tmedia_param_t* param = tmedia_param_create(tmedia_pat_set, tmedia_video, tmedia_ppt_codec, tmedia_pvt_int32, key, (void*)&value);
	if (self->encoder.codec && param) { 
		tdav_session_av_t* base = (tdav_session_av_t*)self;
		if (base->producer && base->producer->encoder.codec_id == self->encoder.codec->id) { /* Whether the producer output encoded frames */ 
			ret = tmedia_producer_set(base->producer, param); 
		} 
		else { 
			if (tsk_strniequals(key, "out-size", 8)) {
				self->encoder.size_changed = tsk_true;
			}
			ret = tmedia_codec_set((tmedia_codec_t*)(self)->encoder.codec, param); 
		}
	}
	TSK_OBJECT_SAFE_FREE(param);
	return ret;
}

// From jitter buffer to codec
static int _tdav_session_video_jb_cb(const tdav_video_jb_cb_data_xt* data)
{
    tdav_session_video_t* video = (tdav_session_video_t*)data->usr_data;
    tdav_session_av_t* base = (tdav_session_av_t*)data->usr_data;
    tmedia_session_t* session = (tmedia_session_t*)data->usr_data;

    switch (data->type) {
    default:
        break;
    case tdav_video_jb_cb_data_type_rtp: {
        return _tdav_session_video_decode(video, data->rtp.pkt);
    }
    case tdav_video_jb_cb_data_type_tmfr: {
        base->time_last_frame_loss_report = tsk_time_now();
        _tdav_session_video_local_request_idr(session, "TMFR", data->ssrc);
    }
    case tdav_video_jb_cb_data_type_fl: {
        base->time_last_frame_loss_report = tsk_time_now();
        if(data->fl.count > TDAV_SESSION_VIDEO_PKT_LOSS_MAX_COUNT_TO_REQUEST_FIR) {
            _tdav_session_video_local_request_idr(session, "TMFR", data->ssrc);
        }
        else {
            if (base->avpf_mode_neg || base->is_fb_nack_neg) { // AVPF?
                // Send RTCP-NACK
                tsk_size_t i, j, k;
                uint16_t seq_nums[16];
                for (i = 0; i < data->fl.count; i+=16) {
                    for(j = 0, k = i; j < 16 && k < data->fl.count; ++j, ++k) {
                        seq_nums[j] = (uint16_t)(data->fl.seq_num + i + j);
                        TSK_DEBUG_INFO("Request re-send(%u)", seq_nums[j]);
                    }
                    trtp_manager_signal_pkt_loss(base->rtp_manager, data->ssrc, seq_nums, j);
                }
            }
        }

        break;
    }
    case tdav_video_jb_cb_data_type_fps_changed: {
        if (base->congestion_ctrl_enabled) {
            video->fps_changed = tsk_true;
            if (video->decoder.codec) {
                TSK_DEBUG_INFO("Congestion control enabled and fps updated from %u to %u", data->fps.old, data->fps.new);
                TMEDIA_CODEC_VIDEO(video->decoder.codec)->in.fps = data->fps.new;
            }
            tsk_mutex_lock(video->h_mutex_qos);
            session->qos_metrics.video_in_avg_fps = (session->qos_metrics.video_in_avg_fps + data->fps.new) / (video->in_avg_fps_n++ ? 2 : 1);
            tsk_mutex_unlock(video->h_mutex_qos);
        }
        break;
    }
    }

    return 0;
}

int _tdav_session_video_open_decoder(tdav_session_video_t* self, uint8_t payload_type)
{
    int ret = 0;

    if ((self->decoder.codec_payload_type != payload_type) || !self->decoder.codec) {
        tsk_istr_t format;
        TSK_OBJECT_SAFE_FREE(self->decoder.codec);
        tsk_itoa(payload_type, &format);
        if (!(self->decoder.codec = tmedia_codec_find_by_format(TMEDIA_SESSION(self)->neg_codecs, format)) || !self->decoder.codec->plugin || !self->decoder.codec->plugin->decode) {
            TSK_DEBUG_ERROR("%s is not a valid payload for this session", format);
            ret = -2;
            goto bail;
        }
        self->decoder.codec_payload_type = payload_type;
        self->decoder.codec_decoded_frames_count = 0; // because we switched the codecs
    }
    // Open codec if not already done
    if (!TMEDIA_CODEC(self->decoder.codec)->opened) {
        if ((ret = tmedia_codec_open(self->decoder.codec))) {
            TSK_DEBUG_ERROR("Failed to open [%s] codec", self->decoder.codec->plugin->desc);
            goto bail;
        }
        self->decoder.codec_decoded_frames_count = 0; // because first time to use
    }

bail:
    return ret;
}

static int _tdav_session_video_decode(tdav_session_video_t* self, const trtp_rtp_packet_t* packet)
{
    tdav_session_av_t* base = (tdav_session_av_t*)self;
    static const trtp_rtp_header_t* __rtp_header = tsk_null;
    static const tmedia_codec_id_t __codecs_supporting_zero_artifacts = (tmedia_codec_id_vp8 | tmedia_codec_id_h264_bp | tmedia_codec_id_h264_mp | tmedia_codec_id_h263);
    int ret = 0;

    if(!self || !packet || !packet->header) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(base);

    if (self->started && base->consumer && base->consumer->is_started) {
        tsk_size_t out_size, _size;
        const void* _buffer;
        tdav_session_video_t* video = (tdav_session_video_t*)base;
        tmedia_session_t* session = (tmedia_session_t*)base;
        uint64_t time_start, time_duration;

        // Find the codec to use to decode the RTP payload
        if (!self->decoder.codec || self->decoder.codec_payload_type != packet->header->payload_type) {
            if ((ret = _tdav_session_video_open_decoder(self, packet->header->payload_type))) {
                goto bail;
            }
        }

        // check whether bypassing is enabled (e.g. rtcweb breaker ON and media coder OFF)
        if(TMEDIA_SESSION(self)->bypass_decoding) {
            // set codec id for internal use (useful to find codec with dynamic payload type)
            TRTP_RTP_HEADER(packet->header)->codec_id = self->decoder.codec->id;
            // consume the frame
            ret = tmedia_consumer_consume(base->consumer, (packet->payload.data ? packet->payload.data : packet->payload.data_const), packet->payload.size, packet->header);
            goto bail;
        }

        time_start = tsk_time_now();

        // Check if stream is corrupted or not
        if(video->decoder.last_seqnum && (video->decoder.last_seqnum + 1) != packet->header->seq_num) {
            TSK_DEBUG_INFO("/!\\Video stream corrupted because of packet loss [%u - %u]. Pause rendering if 'zero_artifacts' (supported = %s, enabled = %s).",
                           video->decoder.last_seqnum,
                           packet->header->seq_num,
                           (__codecs_supporting_zero_artifacts & self->decoder.codec->id) ? "yes" : "no",
                           self->zero_artifacts ? "yes" : "no"
                          );
            if(!video->decoder.stream_corrupted) { // do not do the job twice
                if(self->zero_artifacts && (__codecs_supporting_zero_artifacts & self->decoder.codec->id)) {
                    // request IDR now and every time after 'TDAV_SESSION_VIDEO_AVPF_FIR_REQUEST_INTERVAL_MIN' ellapsed
                    // 'zero-artifacts' not enabled then, we'll request IDR when decoding fails
                    TSK_DEBUG_INFO("Sending FIR to request IDR...");
                    _tdav_session_video_local_request_idr(TMEDIA_SESSION(video), "ZERO_ART_CORRUPTED", packet->header->ssrc);
                }
                // value will be updated when we decode an IDR frame
                video->decoder.stream_corrupted = tsk_true;
                video->decoder.stream_corrupted_since = tsk_time_now();
            }
            // will be used as guard to avoid redering corrupted IDR
            video->decoder.last_corrupted_timestamp = packet->header->timestamp;
        }
        video->decoder.last_seqnum = packet->header->seq_num; // update last seqnum

        // Decode data
        out_size = self->decoder.codec->plugin->decode(
                       self->decoder.codec,
                       (packet->payload.data ? packet->payload.data : packet->payload.data_const), packet->payload.size,
                       &self->decoder.buffer, &self->decoder.buffer_size,
                       packet->header
                   );

        // report to the remote party the bandwidth usage and jitter buffer congestion info
        // this must be done here to make sure it won't be skipped by decoding issues or any other failure
        ret = _tdav_session_video_report_bw_usage_and_jcng(self);

        // inc() frame count
        ++self->decoder.codec_decoded_frames_count;

        // check
        if (!out_size || !self->decoder.buffer) {
            goto bail;
        }
        // check if stream is corrupted
        // the above decoding process is required in order to reset stream corruption status when IDR frame is decoded
        if (self->zero_artifacts && self->decoder.stream_corrupted && (__codecs_supporting_zero_artifacts & self->decoder.codec->id)) {
            TSK_DEBUG_INFO("Do not render video frame because stream is corrupted and 'zero-artifacts' is enabled. Last seqnum=%u", video->decoder.last_seqnum);
            if (video->decoder.stream_corrupted && (tsk_time_now() - video->decoder.stream_corrupted_since) > TDAV_SESSION_VIDEO_AVPF_FIR_REQUEST_INTERVAL_MIN) {
                TSK_DEBUG_INFO("Sending FIR to request IDR because frame corrupted since %llu...", video->decoder.stream_corrupted_since);
                _tdav_session_video_local_request_idr(TMEDIA_SESSION(video), "ZERO_ART_CORRUPTED", packet->header->ssrc);
            }
            goto bail;
        }

        // important: do not override the display size (used by the end-user) unless requested
        if(base->consumer->video.display.auto_resize) {
            base->consumer->video.display.width = TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.width;//decoded width
            base->consumer->video.display.height = TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.height;//decoded height
        }

        // Convert decoded data to the consumer chroma and size
#define CONSUMER_NEED_DECODER (base->consumer->decoder.codec_id == tmedia_codec_id_none) // Otherwise, the consumer requires encoded frames
#define CONSUMER_IN_N_DISPLAY_MISMATCH		(!base->consumer->video.display.auto_resize && (base->consumer->video.in.width != base->consumer->video.display.width || base->consumer->video.in.height != base->consumer->video.display.height))
#define CONSUMER_DISPLAY_N_CODEC_MISMATCH		(base->consumer->video.display.width != TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.width || base->consumer->video.display.height != TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.height)
#define CONSUMER_DISPLAY_N_CONVERTER_MISMATCH	( (self->conv.fromYUV420 && self->conv.fromYUV420->dstWidth != base->consumer->video.display.width) || (self->conv.fromYUV420 && self->conv.fromYUV420->dstHeight != base->consumer->video.display.height) )
#define CONSUMER_CHROMA_MISMATCH	(base->consumer->video.display.chroma != TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.chroma)
#define DECODED_NEED_FLIP	(TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.flip)

        if(CONSUMER_NEED_DECODER && (CONSUMER_CHROMA_MISMATCH || CONSUMER_DISPLAY_N_CODEC_MISMATCH || CONSUMER_IN_N_DISPLAY_MISMATCH || CONSUMER_DISPLAY_N_CONVERTER_MISMATCH || DECODED_NEED_FLIP)) {

            // Create video converter if not already done
            if(!self->conv.fromYUV420 || CONSUMER_DISPLAY_N_CONVERTER_MISMATCH) {
                TSK_OBJECT_SAFE_FREE(self->conv.fromYUV420);

                // create converter
                if(!(self->conv.fromYUV420 = tmedia_converter_video_create(TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.width, TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.height, TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.chroma, base->consumer->video.display.width, base->consumer->video.display.height,
                                             base->consumer->video.display.chroma))) {
                    TSK_DEBUG_ERROR("Failed to create video converter");
                    ret = -3;
                    goto bail;
                }
            }
        }

        // update consumer size using the codec decoded values
        // must be done here to avoid fooling "CONSUMER_IN_N_DISPLAY_MISMATCH" unless "auto_resize" option is enabled
        base->consumer->video.in.width = TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.width;//decoded width
        base->consumer->video.in.height = TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.height;//decoded height

        if(self->conv.fromYUV420) {
            // update one-shot parameters
            tmedia_converter_video_set_flip(self->conv.fromYUV420, TMEDIA_CODEC_VIDEO(self->decoder.codec)->in.flip);
            // convert data to the consumer's chroma
            out_size = tmedia_converter_video_process(self->conv.fromYUV420, self->decoder.buffer, self->decoder.buffer_size, &self->decoder.conv_buffer, &self->decoder.conv_buffer_size);
            if(!out_size || !self->decoder.conv_buffer) {
                TSK_DEBUG_ERROR("Failed to convert YUV420 buffer to consumer's chroma");
                ret = -4;
                goto bail;
            }

            _buffer = self->decoder.conv_buffer;
            _size = out_size;
        }
        else {
            _buffer = self->decoder.buffer;
            _size = out_size;
        }

        time_duration = tsk_time_now() - time_start;
        if (base->congestion_ctrl_enabled) {
            tsk_mutex_lock(video->h_mutex_qos);
            session->qos_metrics.video_dec_avg_time = (unsigned)((session->qos_metrics.video_dec_avg_time + time_duration) / (video->dec_avg_time_n++ ? 2 : 1));
            tsk_mutex_unlock(video->h_mutex_qos);
        }

        // consume decoded video
        ret = tmedia_consumer_consume(base->consumer, _buffer, _size, __rtp_header);
    }
    else if (!base->consumer || !base->consumer->is_started) {
        TSK_DEBUG_INFO("Consumer not started (is_null=%d)", !base->consumer);
    }

bail:
    tsk_safeobj_unlock(base);

    return ret;
}

/* ============ Plugin interface ================= */

static int tdav_session_video_set(tmedia_session_t* self, const tmedia_param_t* param)
{
    int ret = 0;
    tdav_session_video_t* video;
    tdav_session_av_t* base;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // try with the base class to see if this option is supported or not
    if (tdav_session_av_set(TDAV_SESSION_AV(self), param) == tsk_true) {
        return 0;
    }

    video = (tdav_session_video_t*)self;
    base = (tdav_session_av_t*)self;

    if(param->plugin_type == tmedia_ppt_codec) {
        tsk_mutex_lock(video->encoder.h_mutex);
        ret = tmedia_codec_set((tmedia_codec_t*)video->encoder.codec, param);
        tsk_mutex_unlock(video->encoder.h_mutex);
    }
    else if(param->plugin_type == tmedia_ppt_consumer) {
        if(!base->consumer) {
            TSK_DEBUG_ERROR("No consumer associated to this session");
            return -1;
        }
        if(param->value_type == tmedia_pvt_int32) {
            if(tsk_striequals(param->key, "flip")) {
                tsk_list_item_t* item;
                tsk_bool_t flip = (tsk_bool_t)TSK_TO_INT32((uint8_t*)param->value);
                tmedia_codecs_L_t *codecs = tsk_object_ref(self->codecs);
                tsk_list_foreach(item, codecs) {
                    TMEDIA_CODEC_VIDEO(item->data)->in.flip = flip;
                }
                tsk_object_unref(codecs);
            }
        }
        ret = tmedia_consumer_set(base->consumer, param);
    }
    else if(param->plugin_type == tmedia_ppt_producer) {
        if(!base->producer) {
            TSK_DEBUG_ERROR("No producer associated to this session");
            return -1;
        }
        if(param->value_type == tmedia_pvt_int32) {
            if(tsk_striequals(param->key, "flip")) {
                tsk_list_item_t* item;
                tsk_bool_t flip = (tsk_bool_t)TSK_TO_INT32((uint8_t*)param->value);
                tmedia_codecs_L_t *codecs = tsk_object_ref(self->codecs);
                tsk_list_foreach(item, codecs) {
                    TMEDIA_CODEC_VIDEO(item->data)->out.flip = flip;
                }
                tsk_object_unref(codecs);
            }
        }
        ret = tmedia_producer_set(base->producer, param);
    }
    else {
        if (param->value_type == tmedia_pvt_int32) {
            if (tsk_striequals(param->key, "bandwidth-level")) {
                tsk_list_item_t* item;
                self->bl = (tmedia_bandwidth_level_t)TSK_TO_INT32((uint8_t*)param->value);
                self->codecs = tsk_object_ref(self->codecs);
                tsk_list_foreach(item, self->codecs) {
                    ((tmedia_codec_t*)item->data)->bl = self->bl;
                }
                tsk_object_unref(self->codecs);
            }
        }
    }

    return ret;
}

static int tdav_session_video_get(tmedia_session_t* self, tmedia_param_t* param)
{
    if (!self || !param) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // try with the base class to see if this option is supported or not
    if (tdav_session_av_get(TDAV_SESSION_AV(self), param) == tsk_true) {
        return 0;
    }
    else {
        if (param->plugin_type == tmedia_ppt_session) {
            if (param->value_type == tmedia_pvt_pobject) {
                if (tsk_striequals(param->key, "codec-encoder")) {
                    *((tsk_object_t**)param->value) = tsk_object_ref(TDAV_SESSION_VIDEO(self)->encoder.codec); // up to the caller to release the object
                    return 0;
                }
            }
        }
    }

    TSK_DEBUG_WARN("This session doesn't support get(%s)", param->key);
    return -2;
}

static int tdav_session_video_prepare(tmedia_session_t* self)
{
    tdav_session_av_t* base = (tdav_session_av_t*)(self);
    tdav_session_video_t* video = (tdav_session_video_t*)self;
    int ret;

    if((ret = tdav_session_av_prepare(base))) {
        TSK_DEBUG_ERROR("tdav_session_av_prepare(video) failed");
        return ret;
    }

    if(base->rtp_manager) {
        ret = trtp_manager_set_rtp_callback(base->rtp_manager, tdav_session_video_rtp_cb, base);
        ret = trtp_manager_set_rtcp_callback(base->rtp_manager, tdav_session_video_rtcp_cb, base);
    }

    return ret;
}

static int tdav_session_video_start(tmedia_session_t* self)
{
    int ret;
    tdav_session_video_t* video;
    const tmedia_codec_t* codec;
    tdav_session_av_t* base;

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    video = (tdav_session_video_t*)self;
    base = (tdav_session_av_t*)self;

    if (video->started) {
        TSK_DEBUG_INFO("Video session already started");
        return 0;
    }

    // ENCODER codec
    if (!(codec = tdav_session_av_get_best_neg_codec(base))) {
        TSK_DEBUG_ERROR("No codec matched");
        return -2;
    }
    tsk_mutex_lock(video->encoder.h_mutex);
    TSK_OBJECT_SAFE_FREE(video->encoder.codec);
    video->encoder.codec = tsk_object_ref((tsk_object_t*)codec);
    // initialize the encoder using user-defined values
    if ((ret = tdav_session_av_init_encoder(base, video->encoder.codec))) {
		tsk_mutex_unlock(video->encoder.h_mutex);
        TSK_DEBUG_ERROR("Failed to initialize the encoder [%s] codec", video->encoder.codec->plugin->desc);
        return ret;
    }
    if (!TMEDIA_CODEC(video->encoder.codec)->opened) {
        if((ret = tmedia_codec_open(video->encoder.codec))) {
            tsk_mutex_unlock(video->encoder.h_mutex);
            TSK_DEBUG_ERROR("Failed to open [%s] codec", video->encoder.codec->plugin->desc);
            return ret;
        }
    }
	// update negotiated video size
	video->neg_width = TMEDIA_CODEC_VIDEO(video->encoder.codec)->out.width;
	video->neg_height = TMEDIA_CODEC_VIDEO(video->encoder.codec)->out.height;

    tsk_mutex_unlock(video->encoder.h_mutex);

    if (video->jb) {
        if ((ret = tdav_video_jb_start(video->jb))) {
            TSK_DEBUG_ERROR("Failed to start jitter buffer");
            return ret;
        }
    }

    if ((ret = tdav_session_av_start(base, video->encoder.codec))) {
        TSK_DEBUG_ERROR("tdav_session_av_start(video) failed");
        return ret;
    }
    video->started = tsk_true;

    // start timer manager (must be after setting "started" to true)
    if (tsk_timer_mgr_global_start() == 0) {
        video->timer.id_qos = tsk_timer_mgr_global_schedule(TDAV_SESSION_VIDEO_QOS_COMPUTE_INTERVAL, _tdav_session_video_timer_cb, video);
    }

    return ret;
}

static int tdav_session_video_stop(tmedia_session_t* self)
{
    int ret;
    tdav_session_video_t* video;
    tdav_session_av_t* base;

    TSK_DEBUG_INFO("tdav_session_video_stop");

    video = (tdav_session_video_t*)self;
    base = (tdav_session_av_t*)self;

    // unschedule qos timer
    if (video->timer.mgr) {
        tsk_timer_mgr_global_cancel(video->timer.id_qos);
        video->timer.id_qos = TSK_INVALID_TIMER_ID;
        // must not stop global timer manager as it's used by other functions
    }

    // must be here to make sure no other thread will lock the encoder once we have done it
    tsk_mutex_lock(video->encoder.h_mutex); // encoder thread will check "started" var right after the lock is passed
    video->started = tsk_false;
    tsk_mutex_unlock(video->encoder.h_mutex);
    // at this step we're sure that encode() will no longer be called which means we can safely close the codec

    if (video->jb) {
        ret = tdav_video_jb_stop(video->jb);
    }
    // clear AVPF packets and wait for the dtor() before destroying the list
    tsk_list_lock(video->avpf.packets);
    tsk_list_clear_items(video->avpf.packets);
    tsk_list_unlock(video->avpf.packets);

    // tdav_session_av_stop() : stop producer and consumer, close encoder and all other codecs, stop rtpManager...
    // no need to lock the encoder to avoid using a closed codec (see above)
    // no need to lock the decoder as the rtpManager will be stop before closing the codec
    // lock-free stop() may avoid deadlock issue (cannot reproduce it myself) on Hovis
    ret = tdav_session_av_stop(base);
    tsk_mutex_lock(video->encoder.h_mutex);
    TSK_OBJECT_SAFE_FREE(video->encoder.codec);
    tsk_mutex_unlock(video->encoder.h_mutex);
    TSK_OBJECT_SAFE_FREE(video->decoder.codec);

    // reset default values to make sure next start will be called with right defaults
    // do not call this function in start to avoid overriding values defined between prepare() and start()
    _tdav_session_video_set_defaults(video);

    return ret;
}

static int tdav_session_video_pause(tmedia_session_t* self)
{
    return tdav_session_av_pause(TDAV_SESSION_AV(self));
}

static const tsdp_header_M_t* tdav_session_video_get_lo(tmedia_session_t* self)
{
    tsk_bool_t updated = tsk_false;
    const tsdp_header_M_t* ret;
    tdav_session_av_t* base = TDAV_SESSION_AV(self);

    if(!(ret = tdav_session_av_get_lo(base, &updated))) {
        TSK_DEBUG_ERROR("tdav_session_av_get_lo(video) failed");
        return tsk_null;
    }

    if(updated) {
        // set callbacks
        _tdav_session_video_set_callbacks(self);
    }

    return ret;
}

static int tdav_session_video_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m)
{
    int ret;
    tsk_bool_t updated = tsk_false;
    tdav_session_av_t* base = TDAV_SESSION_AV(self);

    if((ret = tdav_session_av_set_ro(base, m, &updated))) {
        TSK_DEBUG_ERROR("tdav_session_av_set_ro(video) failed");
        return ret;
    }

    // Check if "RTCP-NACK" and "RTC-FIR" are supported
    {
        const tmedia_codec_t* codec;
        base->is_fb_fir_neg = base->is_fb_nack_neg = base->is_fb_googremb_neg = base->is_fb_doubsjcng_neg = tsk_false;
        if ((codec = tdav_session_av_get_best_neg_codec(base))) {
            // a=rtcp-fb:* ccm fir
            // a=rtcp-fb:* nack
            // a=rtcp-fb:* goog-remb
            // a=rtcp-fb:* doubs-jcng
            char attr_fir[256], attr_nack[256], attr_goog_remb[256], attr_doubs_jcng[256];
            int index = 0;
            const tsdp_header_A_t* A;

            sprintf(attr_fir, "%s ccm fir", codec->neg_format);
            sprintf(attr_nack, "%s nack", codec->neg_format);
            sprintf(attr_goog_remb, "%s goog-remb", codec->neg_format);
            sprintf(attr_doubs_jcng, "%s doubs-jcng", codec->neg_format);

            while ((A = tsdp_header_M_findA_at(m, "rtcp-fb", index++))) {
                if (!base->is_fb_fir_neg) {
                    base->is_fb_fir_neg = (tsk_striequals(A->value, "* ccm fir") || tsk_striequals(A->value, attr_fir));
                }
                if (!base->is_fb_nack_neg) {
                    base->is_fb_nack_neg = (tsk_striequals(A->value, "* nack") || tsk_striequals(A->value, attr_nack));
                }
                if (!base->is_fb_googremb_neg) {
                    base->is_fb_googremb_neg = (tsk_striequals(A->value, "* goog-remb") || tsk_striequals(A->value, attr_goog_remb));
                }
                if (!base->is_fb_doubsjcng_neg) {
                    base->is_fb_doubsjcng_neg = (tsk_striequals(A->value, "* doubs-jcng") || tsk_striequals(A->value, attr_doubs_jcng));
                }
            }
        }
    }

    if (updated) {
        // set callbacks
        ret = _tdav_session_video_set_callbacks(self);
    }

    return ret;
}

// Plugin interface: callback from the end-user to set rtcp event callback (should be called only when encoding is bypassed)
static int tdav_session_video_rtcp_set_onevent_cbfn(tmedia_session_t* self, const void* context, tmedia_session_rtcp_onevent_cb_f func)
{
    tdav_session_video_t* video;
    tdav_session_av_t* base;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    video = (tdav_session_video_t*)self;
    base = (tdav_session_av_t*)self;

    tsk_safeobj_lock(base);
    video->cb_rtcpevent.context = context;
    video->cb_rtcpevent.func = func;
    tsk_safeobj_unlock(base);

    return 0;
}

// Plugin interface: called by the end-user to send rtcp event (should be called only when encoding is bypassed)
static int tdav_session_video_rtcp_send_event(tmedia_session_t* self, tmedia_rtcp_event_type_t event_type, uint32_t ssrc_media)
{
    tdav_session_video_t* video;
    tdav_session_av_t* base;
    int ret = -1;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    video = (tdav_session_video_t*)self;
    base = (tdav_session_av_t*)self;

    tsk_safeobj_lock(base);
    switch(event_type) {
    case tmedia_rtcp_event_type_fir: {
        if(base->rtp_manager && base->rtp_manager->is_started) {
            if(!ssrc_media) { // when called from C++/Java/C# bindings "ssrc_media" is a default parameter with value=0
                ssrc_media = base->rtp_manager->rtp.ssrc.remote;
            }
            TSK_DEBUG_INFO("Send FIR(%u)", ssrc_media);
            _tdav_session_video_local_request_idr(self, "CALLBACK", ssrc_media);
        }
        break;
    }
    }
    tsk_safeobj_unlock(base);

    return ret;
}

// Plugin interface: called by the end-user to recv rtcp event
static int tdav_session_video_rtcp_recv_event(tmedia_session_t* self, tmedia_rtcp_event_type_t event_type, uint32_t ssrc_media)
{
    tdav_session_video_t* video;
    tdav_session_av_t* base;
    int ret = -1;

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    video = (tdav_session_video_t*)self;
    base = (tdav_session_av_t*)self;

    tsk_safeobj_lock(base);
    switch(event_type) {
    case tmedia_rtcp_event_type_fir: {
        _tdav_session_video_remote_requested_idr(video, ssrc_media);
        ret = 0;
        break;
    }
    }
    tsk_safeobj_unlock(base);

    return ret;
}

static int _tdav_session_video_timer_cb(const void* arg, tsk_timer_id_t timer_id)
{
    /*const*/ tdav_session_video_t* video = (/*const*/ tdav_session_video_t*)tsk_object_ref(TSK_OBJECT(arg));
    tdav_session_av_t* base = (tdav_session_av_t*)video;
    tmedia_session_t* session = (tmedia_session_t*)video;
    if (video->timer.id_qos == timer_id) {
        if (base->congestion_ctrl_enabled) {
            tmedia_codec_video_t* codec = tsk_object_ref(TSK_OBJECT(video->encoder.codec));
            if (codec && video->started) {
                float q1, q2, q3, q4, q5, qavg, cavg;
                uint64_t bw_est_kbps;
				tsk_bool_t cavgneg, update_qavg = tsk_false, update_size = tsk_false;
				unsigned enc_avg_time;
				int32_t bw_up_base_kbps, bw_up_ref_kbps, bw_up_new_kbps;

                tsk_mutex_lock(video->h_mutex_qos);
                q1 = video->q1_n ? session->qos_metrics.q1 : 1.f;
                q2 = video->q2_n ? session->qos_metrics.q2 : 1.f;
                q3 = video->q3_n ? session->qos_metrics.q3 : 1.f;
                q4 = video->q4_n ? session->qos_metrics.q4 : 1.f;
                q5 = video->q5_n ? session->qos_metrics.q5 : 1.f;

				enc_avg_time = session->qos_metrics.video_enc_avg_time;

                // update bw info
                if (_tdav_session_video_get_bw_usage_est(video, &bw_est_kbps, /*in=*/tsk_true, /*reset*/tsk_true) == 0 && bw_est_kbps != 0) {
                    session->qos_metrics.bw_down_est_kbps = (unsigned)bw_est_kbps;
                }
                if (_tdav_session_video_get_bw_usage_est(video, &bw_est_kbps, /*in=*/tsk_false, /*reset*/tsk_true) == 0 && bw_est_kbps != 0) {
                    session->qos_metrics.bw_up_est_kbps = (unsigned)bw_est_kbps;
                }

                // update video size
                session->qos_metrics.video_out_width = codec->out.width;
                session->qos_metrics.video_out_height = codec->out.height;
                session->qos_metrics.video_in_width = codec->in.width;
                session->qos_metrics.video_in_height = codec->in.height;
                
                // continue averaging
				video->q1_n = TSK_CLAMP(0, video->q1_n, 1);
				video->q2_n = TSK_CLAMP(0, video->q2_n, 1);
				video->q3_n = TSK_CLAMP(0, video->q3_n, 1);
				video->q4_n = TSK_CLAMP(0, video->q4_n, 1);
				video->q5_n = TSK_CLAMP(0, video->q5_n, 1);
                video->in_avg_fps_n = TSK_CLAMP(0, video->in_avg_fps_n, 1);
                video->dec_avg_time_n = TSK_CLAMP(0, video->dec_avg_time_n, 1);
                video->enc_avg_time_n = TSK_CLAMP(0, video->enc_avg_time_n, 1);

                // reset time
                session->qos_metrics.last_update_time = tsk_time_now();
                tsk_mutex_unlock(video->h_mutex_qos);

				// Compute the new qvag value
                qavg = q1 * 0.1f + q2 * 0.4f + q3 * 0.1f + q4 * 0.0f + q5 * 0.4f;
                cavg = (qavg - session->qos_metrics.qvag);
				cavgneg = cavg < 0.f ? tsk_true : tsk_false;
                cavg = cavg < 0.f ? -cavg : +cavg;
                TSK_DEBUG_INFO("_tdav_session_video_timer_cb: q1=%f, q2=%f, q3=%f, q4=%f, q5=%f, qavg=%f, qavg_lowest=%f cavg=%f cavgneg=%d congestion_ctrl_enabled=true", q1, q2, q3, q4, q5, qavg, video->qavg_lowest, cavg, cavgneg);				
				
				if (cavgneg) { /* Quality is down */
					++video->num_qavg_down;
					video->qavg_lowest = TSK_MIN(qavg, video->qavg_lowest);
					
					// If we have more than #2 successive quality downgrade then we update qvag
					// We also update qavg when c > 5%
					if (video->num_qavg_down >= 2 || (cavg > 0.05f)) {
						qavg = video->qavg_lowest;
						update_qavg = tsk_true;
					}
				}
				else { /* Quality is up */
					video->num_qavg_down = 0;
					video->qavg_lowest = qavg;
					
					// We only increase the bandwidth if the the upgrade set the new avg to more than 10% of the old value
					update_qavg = (cavg > 0.1f);
				}

				// Do not use "codec->out.width" and "codec->out.height" to compute "bw_up_base_kbps" as these values could be changed when 
				// adapt. resolution is enabled
				bw_up_base_kbps = tmedia_get_video_bandwidth_kbps_2(video->neg_width, video->neg_height, codec->out.fps);
				bw_up_ref_kbps = TMEDIA_CODEC(codec)->bandwidth_max_upload;
				if (bw_up_ref_kbps < 0 || bw_up_ref_kbps == INT_MAX) {
					bw_up_ref_kbps = bw_up_base_kbps;
				}
				bw_up_new_kbps = bw_up_ref_kbps; // default value will be update depending on qavg

				// Unconditionally decrease the bandwidth when the quality is < 90%
				// bandwidth will decrease regardless qavg value as the ref. value is recursive
				update_qavg |= (qavg < 0.9f);
				// Also update the bandwidth when quality is > 90% and saved qvag is < 90% or ref bw is < base bw
				update_qavg |= (qavg > 0.9f && (session->qos_metrics.qvag < 0.9f || bw_up_ref_kbps < bw_up_base_kbps));

                if (update_qavg) {
                    // Update the upload bandwidth
                    int32_t bw_up_max_kbps = base->bandwidth_max_upload_kbps; // user-defined maximum

					// When the qavg is within ]90-100]% we increase the ref. bw
					if (qavg > 0.9f) {
						bw_up_ref_kbps += (int32_t)((bw_up_base_kbps - bw_up_ref_kbps) * 0.1f);
						bw_up_ref_kbps = TSK_CLAMP(5, bw_up_ref_kbps, bw_up_base_kbps);
						bw_up_new_kbps = bw_up_ref_kbps;
					}
					else {
						bw_up_ref_kbps = TSK_CLAMP(5, bw_up_ref_kbps, bw_up_max_kbps);
						bw_up_new_kbps = (int32_t)(bw_up_ref_kbps * qavg);
					}
                    TSK_DEBUG_INFO("Video quality change(%d%%), changing bw_up from ref=%dkbps to new=%dkbps", (int)(cavg*100), bw_up_ref_kbps, bw_up_new_kbps);
                    _tdav_session_video_bw_kbps(video, bw_up_new_kbps);
                    session->qos_metrics.qvag = qavg;
                }
                //!\ update qavg only if condition "c" is true


				/* Update size depending on bw */
#if BUILD_TYPE_TCH
				{
					// A video quality is defined by MR (Motion Rank) like this:
					// MR(#1)->poor, #2->acceptable, #4->good, #6->excellent ....
					// Higher the MR is higher is the bandwidth and the quality
					// The algorithm to change the video size depending on the bandwidth is straightforward:
					//	1/ Compute the right bandwidth (bw_up_new_kbps) using Qavg as done above
					//  2/ From the current video resolution (width, height), framerate(fps) and bandwidth(bw_up_new_kbps) compute the MR (a.k.a quality)
					//  3/ If the computed MR is <=#1(poor) then, downsample the resolution to have MR=#2(acceptable)
					//  4/ If the computed MR is >=#4 and the current resolution is lower than what was negotiated then, upsample the resolution to have MR=#2 (at most)
					// In short: we want MR to stay within [2, %[
					int32_t mr = tmedia_get_video_motion_rank(codec->out.width, codec->out.height, codec->out.fps, bw_up_new_kbps);
					int32_t mr_diff = 0;
					update_size = ((mr <= 1) || (mr >= 4 && (codec->out.width * codec->out.height) < (video->neg_width * video->neg_height)));
					if (update_size) {
						mr_diff = (mr - 2);
					}
					if (mr_diff != 0) {
						unsigned new_w, new_h, min_w, min_h, max_w, max_h;
						if (mr_diff > 0) {
							new_w = ((codec->out.width << mr_diff) + 15) & -16;
							new_h = ((codec->out.height << mr_diff) + 15) & -16;
						}
						else {
							new_w = ((codec->out.width >> (-mr_diff)) + 15) & -16;
							new_h = ((codec->out.height >> (-mr_diff)) + 15) & -16;
						}
						min_w = 16;
						min_h = 16;
						max_w = video->neg_width;
						max_h = video->neg_height;
						if (tmedia_defaults_get_adapt_video_size_range_enabled()) {
							tmedia_pref_video_size_t min, max;
							if (tmedia_defaults_get_pref_video_size_range(&min, &max) == 0) {
								tmedia_video_get_size(min, &min_w, &min_h);
								tmedia_video_get_size(max, &max_w, &max_h);
							}
						}
						new_w = TSK_CLAMP(min_w, new_w, max_w);
						new_h = TSK_CLAMP(min_h, new_h, max_h);
						TSK_DEBUG_INFO("_tdav_session_video_timer_cb: [[size change based on bandwidth]] new video size: (%u, %u), neg size: (%u, %u)", new_w, new_h, video->neg_width, video->neg_height);
						tsk_mutex_lock(video->encoder.h_mutex);
						_tdav_session_video_codec_set_int32(video, "out-size", (int32_t)((new_w & 0xFFFF) | (new_h << 16)));
						tsk_mutex_unlock(video->encoder.h_mutex);
					}
				}
#endif /* BUILD_TYPE_TCH */

				/* Update size depending cpu usage  */
#if BUILD_TYPE_TCH
				if (!update_size) {
					unsigned best_enc_time;
					float vs_weight;

					// Check if encoding time is too high or low
					best_enc_time = 1000 / (codec->out.fps);
					if (enc_avg_time > (best_enc_time + (best_enc_time >> 2))) { /* Too slow */
						if (++video->num_enc_avg_time_high >= 2) {
							update_size = tsk_true;
							vs_weight = ((best_enc_time + (best_enc_time >> 2)) / (float)enc_avg_time);
							TSK_DEBUG_INFO("_tdav_session_video_timer_cb: upsample the video size: %f", vs_weight);
							video->num_enc_avg_time_high = 0;
						}
					}
					else {  /* Too fast */
						if (enc_avg_time < (best_enc_time - (best_enc_time >> 1))) {
							if (--video->num_enc_avg_time_high <= -2) {
								if (enc_avg_time != 0.f && (codec->out.width * codec->out.height) < (video->neg_width * video->neg_height)) { // video size downsampled in the past?
									update_size = tsk_true;
									vs_weight = ((best_enc_time - (best_enc_time >> 1)) / (float)enc_avg_time);
									TSK_DEBUG_INFO("_tdav_session_video_timer_cb: downsample the video size: %f", vs_weight);
								}
								video->num_enc_avg_time_high = 0;
							}
						}
					}

					// Update video size
					if (update_size) {
						unsigned new_w, new_h, new_s;
						char size[128] = {'\0'};
						new_w = (((unsigned)(codec->out.width * vs_weight)) + 16) & -16; // +16 instead of +15 to make sure the size will be >= 16
						new_h = (((unsigned)(codec->out.height * vs_weight)) + 16) & -16;
						new_s = (new_w & 0xFFFF) | (new_h << 16);
						TSK_DEBUG_INFO("_tdav_session_video_timer_cb: [[size change based on cpu]] new video size: (%u, %u), neg size: (%u, %u)", new_w, new_h, video->neg_width, video->neg_height);
						tsk_mutex_lock(video->encoder.h_mutex);
						_tdav_session_video_codec_set_int32(video, "out-size", (int32_t)new_s);
						video->encoder.size_changed = tsk_true;
						tsk_mutex_unlock(video->encoder.h_mutex);
					}
				}
#endif /* BUILD_TYPE_TCH */
            }
            tsk_object_unref(TSK_OBJECT(codec));
        }
        else {
            TSK_DEBUG_INFO("_tdav_session_video_timer_cb: congestion_ctrl_enabled=false");
        }

        if (video->started) {
            video->timer.id_qos = tsk_timer_mgr_global_schedule(TDAV_SESSION_VIDEO_QOS_COMPUTE_INTERVAL, _tdav_session_video_timer_cb, video);
        }
    }
    tsk_object_unref(TSK_OBJECT(video));
    return 0;
}

static int _tdav_session_video_get_bw_usage_est(tdav_session_video_t* self, uint64_t* bw_kbps, tsk_bool_t in, tsk_bool_t reset)
{
    int ret;

    tdav_session_av_t* base = (tdav_session_av_t*)self;
    uint64_t bw_bytes = 0;
    uint64_t* bytes_in = in ? &bw_bytes : tsk_null;
    uint64_t* bytes_out = in ? tsk_null : &bw_bytes;
    uint64_t* count_last_time = in ? &base->bytes_in.count_last_time : &base->bytes_out.count_last_time;
    uint64_t* count = in ? &base->bytes_in.count : &base->bytes_out.count;

    if ((ret = trtp_manager_get_bytes_count(base->rtp_manager, bytes_in, bytes_out)) == 0) {
        uint64_t duration, bytes_count_now;
        bytes_count_now = tsk_time_now();
        duration = (bytes_count_now - *count_last_time);
        if (*count_last_time != 0 && duration > 0) {
            *bw_kbps = ((((bw_bytes - *count) * 8 * 1000) / 1024) / duration);
        }
        else {
            ret = -2;
        }
        if (reset) {
            *count_last_time = bytes_count_now;
            *count = bw_bytes;
        }
    }

    return ret;
}

static int _tdav_session_video_report_bw_usage_and_jcng(tdav_session_video_t* self)
{
    tdav_session_av_t* base = (tdav_session_av_t*)self;
    tmedia_session_t* session = (tmedia_session_t*)self;
    int ret = 0;
    // congetion control
    // send RTCP-REMB if:
    //  - congestion control is enabled and
    //  - fps changed or
    //	- first frame
    if (base->congestion_ctrl_enabled && base->rtp_manager && session->qos_metrics.bw_down_est_kbps != 0) {
        tsk_bool_t update_info = (tsk_time_now() - self->last_sendreport_time) > TDAV_SESSION_VIDEO_QOS_COMPUTE_INTERVAL;
        if (update_info) {
            float jcng_q = 1.f;
            if (self->jb && self->jb_enabled) {
                float q5 = 1.f;
                if ((ret = tdav_video_jb_get_qcong(self->jb, &q5)) == 0) {
                    jcng_q = q5;
                }
            }
            TSK_DEBUG_INFO("video with congestion control enabled: est_bw_down=%ukbps, est_jcng=%f", session->qos_metrics.bw_down_est_kbps, jcng_q);
            ret = trtp_manager_set_app_bw_and_jcng(base->rtp_manager, INT_MAX/* unused */, (int32_t)session->qos_metrics.bw_down_est_kbps, jcng_q);
			self->last_sendreport_time = tsk_time_now();
        }
    }
    return ret;
}

static int _tdav_session_video_set_callbacks(tmedia_session_t* self)
{
    if(self) {
        tsk_list_item_t* item;
        tsk_list_foreach(item, TMEDIA_SESSION(self)->neg_codecs) {
            // set codec callbacks
            tmedia_codec_video_set_enc_callback(TMEDIA_CODEC_VIDEO(item->data), tdav_session_video_raw_cb, self);
            tmedia_codec_video_set_dec_callback(TMEDIA_CODEC_VIDEO(item->data), tdav_session_video_decode_cb, self);
            // set RED callback: redundant data to decode and send to the consumer
            if(TMEDIA_CODEC(item->data)->plugin == tdav_codec_red_plugin_def_t) {
                tdav_codec_red_set_callback((struct tdav_codec_red_s *)(item->data), tdav_session_video_rtp_cb, self);
            }
        }
    }
    return 0;
}

static int _tdav_session_video_init(tdav_session_video_t *p_self, tmedia_type_t e_media_type)
{
    int ret;
    tdav_session_av_t *p_base = TDAV_SESSION_AV(p_self);
    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* init() base */
    if ((ret = tdav_session_av_init(p_base, e_media_type)) != 0) {
        TSK_DEBUG_ERROR("tdav_session_av_init(video) failed");
        return ret;
    }

    /* init() self */
    _tdav_session_video_set_defaults(p_self);
    if (!p_self->encoder.h_mutex && !(p_self->encoder.h_mutex = tsk_mutex_create())) {
        TSK_DEBUG_ERROR("Failed to create encode mutex");
        return -4;
    }
    if (!p_self->avpf.packets && !(p_self->avpf.packets = tsk_list_create())) {
        TSK_DEBUG_ERROR("Failed to create list");
        return -2;
    }
    if (p_self->jb_enabled) {
        if (!p_self->jb && !(p_self->jb = tdav_video_jb_create())) {
            TSK_DEBUG_ERROR("Failed to create jitter buffer");
            return -3;
        }
        tdav_video_jb_set_callback(p_self->jb, _tdav_session_video_jb_cb, p_self);
    }

    /* producer's callbacks */
    if (p_base->producer) {
        tmedia_producer_set_enc_callback(p_base->producer, tdav_session_video_producer_enc_cb, p_self);
        tmedia_producer_set_raw_callback(p_base->producer, tdav_session_video_raw_cb, p_self);
    }

    if (!p_self->h_mutex_qos && !(p_self->h_mutex_qos = tsk_mutex_create())) {
        TSK_DEBUG_ERROR("Failed to create qos mutex");
        return -5;
    }

    /* timer manager */
    if(!p_self->timer.mgr) {
        p_self->timer.mgr = tsk_timer_mgr_global_ref();
    }

    return 0;
}


//=================================================================================================
//	Session Video Plugin object definition
//
/* constructor */
static tsk_object_t* tdav_session_video_ctor(tsk_object_t * self, va_list * app)
{
    tdav_session_video_t *video = self;
    if(video) {
        if (_tdav_session_video_init(video, tmedia_video)) {
            return tsk_null;
        }
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_session_video_dtor(tsk_object_t * self)
{
    tdav_session_video_t *video = self;
    TSK_DEBUG_INFO("*** tdav_session_video_t destroyed ***");
    if(video) {
        tdav_session_video_stop((tmedia_session_t*)video);
        // deinit self (rtp manager should be destroyed after the producer)
        TSK_OBJECT_SAFE_FREE(video->conv.toYUV420);
        TSK_OBJECT_SAFE_FREE(video->conv.fromYUV420);

        TSK_FREE(video->encoder.buffer);
        TSK_FREE(video->encoder.conv_buffer);
        TSK_FREE(video->decoder.buffer);
        TSK_FREE(video->decoder.conv_buffer);

        TSK_OBJECT_SAFE_FREE(video->encoder.codec);
        TSK_OBJECT_SAFE_FREE(video->decoder.codec);

        TSK_OBJECT_SAFE_FREE(video->avpf.packets);

        TSK_OBJECT_SAFE_FREE(video->jb);

        /* timer manager */
        if (video->timer.mgr) {
            tsk_timer_mgr_global_unref(&video->timer.mgr);
        }

        if(video->encoder.h_mutex) {
            tsk_mutex_destroy(&video->encoder.h_mutex);
        }

        if (video->h_mutex_qos) {
            tsk_mutex_destroy(&video->h_mutex_qos);
        }

        /* deinit() base */
        tdav_session_av_deinit(TDAV_SESSION_AV(video));

        TSK_DEBUG_INFO("*** Video session destroyed ***");
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_session_video_def_s = {
    sizeof(tdav_session_video_t),
    tdav_session_video_ctor,
    tdav_session_video_dtor,
    tmedia_session_cmp,
};
/* plugin definition*/
static const tmedia_session_plugin_def_t tdav_session_video_plugin_def_s = {
    &tdav_session_video_def_s,

    tmedia_video,
    "video",

    tdav_session_video_set,
    tdav_session_video_get,
    tdav_session_video_prepare,
    tdav_session_video_start,
    tdav_session_video_pause,
    tdav_session_video_stop,

    /* Audio part */
    { tsk_null },

    tdav_session_video_get_lo,
    tdav_session_video_set_ro,

    /* T.140 */
    { tsk_null },

    /* Rtcp */
    {
        tdav_session_video_rtcp_set_onevent_cbfn,
        tdav_session_video_rtcp_send_event,
        tdav_session_video_rtcp_recv_event
    }
};
const tmedia_session_plugin_def_t *tdav_session_video_plugin_def_t = &tdav_session_video_plugin_def_s;

//=================================================================================================
//	Session BfcpVideo Plugin object definition
//
/* constructor */
static tsk_object_t* tdav_session_bfcpvideo_ctor(tsk_object_t * self, va_list * app)
{
    tdav_session_video_t *video = self;
    if(video) {
        if (_tdav_session_video_init(video, tmedia_bfcp_video)) {
            return tsk_null;
        }
    }
    return self;
}
/* object definition */
static const tsk_object_def_t tdav_session_bfcpvideo_def_s = {
    sizeof(tdav_session_video_t),
    tdav_session_bfcpvideo_ctor,
    tdav_session_video_dtor,
    tmedia_session_cmp,
};
static const tmedia_session_plugin_def_t tdav_session_bfcpvideo_plugin_def_s = {
    &tdav_session_bfcpvideo_def_s,

    tmedia_bfcp_video,
    "video",

    tdav_session_video_set,
    tdav_session_video_get,
    tdav_session_video_prepare,
    tdav_session_video_start,
    tdav_session_video_pause,
    tdav_session_video_stop,

    /* Audio part */
    { tsk_null },

    tdav_session_video_get_lo,
    tdav_session_video_set_ro,

    /* T.140 */
    { tsk_null },

    /* Rtcp */
    {
        tdav_session_video_rtcp_set_onevent_cbfn,
        tdav_session_video_rtcp_send_event,
        tdav_session_video_rtcp_recv_event
    }
};
const tmedia_session_plugin_def_t *tdav_session_bfcpvideo_plugin_def_t = &tdav_session_bfcpvideo_plugin_def_s;
