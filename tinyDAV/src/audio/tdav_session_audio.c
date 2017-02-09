/*
* Copyright (C) 2010-2015 Mamadou DIOP.
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

/**@file tdav_session_audio.c
* @brief Audio Session plugin.
*
* @author Mamadou Diop <diopmamadou(at)doubango.org>
* @contributors: See $(DOUBANGO_HOME)\contributors.txt
*/
#include "tinydav/audio/tdav_session_audio.h"

//#include "tinydav/codecs/dtmf/tdav_codec_dtmf.h"
#include "tinydav/audio/tdav_consumer_audio.h"

#include "tinymedia/tmedia_resampler.h"
#include "tinymedia/tmedia_denoise.h"
#include "tinymedia/tmedia_jitterbuffer.h"
#include "tinymedia/tmedia_consumer.h"
#include "tinymedia/tmedia_producer.h"
#include "tinymedia/tmedia_defaults.h"

#include "tinyrtp/trtp_manager.h"
#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tsk_timer.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#define TDAV_AUDIO_RESAMPLER_DEFAULT_QUALITY 5

static int _tdav_session_audio_dtmfe_timercb(const void* arg, tsk_timer_id_t timer_id);
static struct tdav_session_audio_dtmfe_s* _tdav_session_audio_dtmfe_create(const tdav_session_audio_t* session, uint8_t event, uint16_t duration, uint32_t seq, uint32_t timestamp, uint8_t format, tsk_bool_t M, tsk_bool_t E);
static void _tdav_session_audio_apply_gain(void* buffer, int len, int bps, int gain);
static tmedia_resampler_t* _tdav_session_audio_resampler_create(int32_t bytes_per_sample, uint32_t in_freq, uint32_t out_freq, uint32_t frame_duration, uint32_t in_channels, uint32_t out_channels, uint32_t quality, void** resampler_buffer, tsk_size_t *resampler_buffer_size);

/* DTMF event object */
typedef struct tdav_session_audio_dtmfe_s {
    TSK_DECLARE_OBJECT;

    tsk_timer_id_t timer_id;
    trtp_rtp_packet_t* packet;

    const tdav_session_audio_t* session;
}
tdav_session_audio_dtmfe_t;
extern const tsk_object_def_t *tdav_session_audio_dtmfe_def_t;

// RTP/RTCP callback (From the network to the consumer)
static int tdav_session_audio_rtp_cb(const void* callback_data, const struct trtp_rtp_packet_s* packet)
{
    tdav_session_audio_t* audio = (tdav_session_audio_t*)callback_data;
    tmedia_codec_t* codec = tsk_null;
    tdav_session_av_t* base = (tdav_session_av_t*)callback_data;
    int ret = -1;

    if (!audio || !packet || !packet->header) {
        TSK_DEBUG_ERROR("Invalid parameter");
        goto bail;
    }

    if (audio->is_started && base->consumer && base->consumer->is_started) {
        tsk_size_t out_size = 0;

        // Find the codec to use to decode the RTP payload
        if (!audio->decoder.codec || audio->decoder.payload_type != packet->header->payload_type) {
            tsk_istr_t format;
            TSK_OBJECT_SAFE_FREE(audio->decoder.codec);
            tsk_itoa(packet->header->payload_type, &format);
            if (!(audio->decoder.codec = tmedia_codec_find_by_format(TMEDIA_SESSION(audio)->neg_codecs, format)) || !audio->decoder.codec->plugin || !audio->decoder.codec->plugin->decode) {
                TSK_DEBUG_ERROR("%s is not a valid payload for this session", format);
                ret = -2;
                goto bail;
            }
            audio->decoder.payload_type = packet->header->payload_type;
        }
        // ref() the codec to be able to use it short time after stop(SAFE_FREE(codec))
        if (!(codec = tsk_object_ref(TSK_OBJECT(audio->decoder.codec)))) {
            TSK_DEBUG_ERROR("Failed to get decoder codec");
            goto bail;
        }

        // Open codec if not already done
        if (!TMEDIA_CODEC(codec)->opened) {
            tsk_safeobj_lock(base);
            if ((ret = tmedia_codec_open(codec))) {
                tsk_safeobj_unlock(base);
                TSK_DEBUG_ERROR("Failed to open [%s] codec", codec->plugin->desc);
                TSK_OBJECT_SAFE_FREE(audio->decoder.codec);
                goto bail;
            }
            tsk_safeobj_unlock(base);
        }
        // Decode data
        out_size = codec->plugin->decode(codec, packet->payload.data, packet->payload.size, &audio->decoder.buffer, &audio->decoder.buffer_size, packet->header);
        if (out_size && audio->is_started) { // check "is_started" again ...to be sure stop() not called by another thread
            void* buffer = audio->decoder.buffer;
            tsk_size_t size = out_size;

            // resample if needed
            if ((base->consumer->audio.out.rate && base->consumer->audio.out.rate != codec->in.rate) || (base->consumer->audio.out.channels && base->consumer->audio.out.channels != TMEDIA_CODEC_AUDIO(codec)->in.channels)) {
                tsk_size_t resampler_result_size = 0;
                int bytesPerSample = (base->consumer->audio.bits_per_sample >> 3);

                if (!audio->decoder.resampler.instance) {
                    TSK_DEBUG_INFO("Create audio resampler(%s) for consumer: rate=%d->%d, channels=%d->%d, bytesPerSample=%d",
                                   codec->plugin->desc,
                                   codec->in.rate, base->consumer->audio.out.rate,
                                   TMEDIA_CODEC_AUDIO(codec)->in.channels, base->consumer->audio.out.channels,
                                   bytesPerSample);
                    audio->decoder.resampler.instance = _tdav_session_audio_resampler_create(
                                                            bytesPerSample,
                                                            codec->in.rate, base->consumer->audio.out.rate,
                                                            base->consumer->audio.ptime,
                                                            TMEDIA_CODEC_AUDIO(codec)->in.channels, base->consumer->audio.out.channels,
                                                            TDAV_AUDIO_RESAMPLER_DEFAULT_QUALITY,
                                                            &audio->decoder.resampler.buffer, &audio->decoder.resampler.buffer_size
                                                        );
                }
                if (!audio->decoder.resampler.instance) {
                    TSK_DEBUG_ERROR("No resampler to handle data");
                    ret = -5;
                    goto bail;
                }
                if (!(resampler_result_size = tmedia_resampler_process(audio->decoder.resampler.instance, buffer, size / bytesPerSample, audio->decoder.resampler.buffer, audio->decoder.resampler.buffer_size / bytesPerSample))) {
                    TSK_DEBUG_ERROR("Failed to process audio resampler input buffer");
                    ret = -6;
                    goto bail;
                }

                buffer = audio->decoder.resampler.buffer;
                size = audio->decoder.resampler.buffer_size;
            }

            // adjust the gain
            if (base->consumer->audio.gain) {
                _tdav_session_audio_apply_gain(buffer, (int)size, base->consumer->audio.bits_per_sample, base->consumer->audio.gain);
            }
            // consume the frame
            tmedia_consumer_consume(base->consumer, buffer, size, packet->header);
        }
    }
    else {
        TSK_DEBUG_INFO("Session audio not ready");
    }

    // everything is ok
    ret = 0;

bail:
    tsk_object_unref(TSK_OBJECT(codec));
    return ret;
}

// Producer callback (From the producer to the network). Will encode() data before sending
static int tdav_session_audio_producer_enc_cb(const void* callback_data, const void* buffer, tsk_size_t size)
{
    int ret = 0;

    tdav_session_audio_t* audio = (tdav_session_audio_t*)callback_data;
    tdav_session_av_t* base = (tdav_session_av_t*)callback_data;
	struct tmedia_codec_s* codec_copy = tsk_null; // to ensure thread-safeness (crash reported by Chunghwa Telecom), SIP reINVITE or UPDATE could update the encoder's codec

    if (!audio) {
        TSK_DEBUG_ERROR("Null session");
        return 0;
    }

    // do nothing if session is held
    // when the session is held the end user will get feedback he also has possibilities to put the consumer and producer on pause
    if (TMEDIA_SESSION(audio)->lo_held) {
        return 0;
    }

    // get best negotiated codec if not already done
    // the encoder codec could be null when session is renegotiated without re-starting (e.g. hold/resume)
    if (!audio->encoder.codec) {
        const tmedia_codec_t* codec;
        tsk_safeobj_lock(base);
        if (!(codec = tdav_session_av_get_best_neg_codec(base))) {
            TSK_DEBUG_ERROR("No codec matched");
            tsk_safeobj_unlock(base);
            return -2;
        }
        audio->encoder.codec = tsk_object_ref(TSK_OBJECT(codec));
        tsk_safeobj_unlock(base);
    }

    if (audio->is_started && base->rtp_manager && base->rtp_manager->is_started) {
        /* encode */
        tsk_size_t out_size = 0;

        // Open codec if not already done
        if (!audio->encoder.codec->opened) {
            tsk_safeobj_lock(base);
            if ((ret = tmedia_codec_open(audio->encoder.codec))) {
                tsk_safeobj_unlock(base);
                TSK_DEBUG_ERROR("Failed to open [%s] codec", audio->encoder.codec->plugin->desc);
                return -4;
            }
            tsk_safeobj_unlock(base);
        }

		// get a copy
		if (!(codec_copy = tsk_object_ref(audio->encoder.codec))) {
			TSK_DEBUG_ERROR("Null encoder");
			goto done;
		}

        // check if we're sending DTMF or not
        if (audio->is_sending_dtmf_events) {
            if (base->rtp_manager) {
                // increment the timestamp
                base->rtp_manager->rtp.timestamp += TMEDIA_CODEC_PCM_FRAME_SIZE_AUDIO_ENCODING(codec_copy)/*duration*/;
            }
            TSK_DEBUG_INFO("Skiping audio frame as we're sending DTMF...");
            goto done;
        }

        // resample if needed
        if (base->producer->audio.rate != codec_copy->out.rate || base->producer->audio.channels != TMEDIA_CODEC_AUDIO(codec_copy)->out.channels) {
            tsk_size_t resampler_result_size = 0;
            int bytesPerSample = (base->producer->audio.bits_per_sample >> 3);

            if (!audio->encoder.resampler.instance) {
                TSK_DEBUG_INFO("Create audio resampler(%s) for producer: rate=%d->%d, channels=%d->%d, bytesPerSample=%d",
                               codec_copy->plugin->desc,
                               base->producer->audio.rate, codec_copy->out.rate,
                               base->producer->audio.channels, TMEDIA_CODEC_AUDIO(codec_copy)->out.channels,
                               bytesPerSample);
                audio->encoder.resampler.instance = _tdav_session_audio_resampler_create(
                                                        bytesPerSample,
                                                        base->producer->audio.rate, codec_copy->out.rate,
                                                        base->producer->audio.ptime,
                                                        base->producer->audio.channels, TMEDIA_CODEC_AUDIO(codec_copy)->out.channels,
                                                        TDAV_AUDIO_RESAMPLER_DEFAULT_QUALITY,
                                                        &audio->encoder.resampler.buffer, &audio->encoder.resampler.buffer_size
                                                    );
            }
            if (!audio->encoder.resampler.instance) {
                TSK_DEBUG_ERROR("No resampler to handle data");
                ret = -1;
                goto done;
            }
            if (!(resampler_result_size = tmedia_resampler_process(audio->encoder.resampler.instance, buffer, size / bytesPerSample, audio->encoder.resampler.buffer, audio->encoder.resampler.buffer_size / bytesPerSample))) {
                TSK_DEBUG_ERROR("Failed to process audio resampler input buffer");
                ret = -1;
                goto done;
            }

            buffer = audio->encoder.resampler.buffer;
            size = audio->encoder.resampler.buffer_size;
        }

        // Denoise (VAD, AGC, Noise suppression, ...)
        // Must be done after resampling
        if (audio->denoise) {
            tsk_bool_t silence_or_noise = tsk_false;
            if (audio->denoise->echo_supp_enabled) {
                ret = tmedia_denoise_process_record(TMEDIA_DENOISE(audio->denoise), (void*)buffer, (uint32_t)size, &silence_or_noise);
            }
        }
        // adjust the gain
        // Must be done after resampling
        if (base->producer->audio.gain) {
            _tdav_session_audio_apply_gain((void*)buffer, (int)size, base->producer->audio.bits_per_sample, base->producer->audio.gain);
        }

        // Encode data
        out_size = codec_copy->plugin->encode(codec_copy, buffer, size, &audio->encoder.buffer, &audio->encoder.buffer_size);
        if (out_size) {
            trtp_manager_send_rtp(base->rtp_manager, audio->encoder.buffer, out_size, TMEDIA_CODEC_FRAME_DURATION_AUDIO_ENCODING(codec_copy), tsk_false/*Marker*/, tsk_true/*lastPacket*/);
        }
    }

done:
	tsk_object_unref(codec_copy);
    return ret;
}


/* ============ Plugin interface ================= */

static int tdav_session_audio_set(tmedia_session_t* self, const tmedia_param_t* param)
{
    int ret = 0;
    tdav_session_audio_t* audio;

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (tdav_session_av_set(TDAV_SESSION_AV(self), param) == tsk_true) {
        return 0;
    }

    audio = (tdav_session_audio_t*)self;

    if (param->plugin_type == tmedia_ppt_consumer) {
        TSK_DEBUG_ERROR("Not expected consumer_set(%s)", param->key);
    }
    else if (param->plugin_type == tmedia_ppt_producer) {
        TSK_DEBUG_ERROR("Not expected producer_set(%s)", param->key);
    }
    else {
        if (param->value_type == tmedia_pvt_int32) {
            if (tsk_striequals(param->key, "echo-supp")) {
                if (audio->denoise) {
                    audio->denoise->echo_supp_enabled = (TSK_TO_INT32((uint8_t*)param->value) != 0);
                }
            }
            else if (tsk_striequals(param->key, "echo-tail")) {
                if (audio->denoise) {
                    return tmedia_denoise_set(audio->denoise, param);
                }
            }
        }
    }

    return ret;
}

static int tdav_session_audio_get(tmedia_session_t* self, tmedia_param_t* param)
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
        // the codec information is held by the session even if the user is authorized to request it for the consumer/producer
        if (param->value_type == tmedia_pvt_pobject) {
            if (param->plugin_type == tmedia_ppt_consumer) {
                TSK_DEBUG_ERROR("Not implemented");
                return -4;
            }
            else if (param->plugin_type == tmedia_ppt_producer) {
                if (tsk_striequals("codec", param->key)) {
                    const tmedia_codec_t* codec;
                    if (!(codec = TDAV_SESSION_AUDIO(self)->encoder.codec)) {
                        codec = tdav_session_av_get_best_neg_codec((const tdav_session_av_t*)self); // up to the caller to release the object
                    }
                    *((tsk_object_t**)param->value) = tsk_object_ref(TSK_OBJECT(codec));
                    return 0;
                }
            }
            else if (param->plugin_type == tmedia_ppt_session) {
                if (tsk_striequals(param->key, "codec-encoder")) {
                    *((tsk_object_t**)param->value) = tsk_object_ref(TDAV_SESSION_AUDIO(self)->encoder.codec); // up to the caller to release the object
                    return 0;
                }
            }
        }
    }

    TSK_DEBUG_WARN("This session doesn't support get(%s)", param->key);
    return -2;
}

static int tdav_session_audio_prepare(tmedia_session_t* self)
{
    tdav_session_av_t* base = (tdav_session_av_t*)(self);
    int ret;

    if ((ret = tdav_session_av_prepare(base))) {
        TSK_DEBUG_ERROR("tdav_session_av_prepare(audio) failed");
        return ret;
    }

    if (base->rtp_manager) {
        ret = trtp_manager_set_rtp_callback(base->rtp_manager, tdav_session_audio_rtp_cb, base);
    }

    return ret;
}

static int tdav_session_audio_start(tmedia_session_t* self)
{
    int ret;
    tdav_session_audio_t* audio;
    const tmedia_codec_t* codec;
    tdav_session_av_t* base;

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    audio = (tdav_session_audio_t*)self;
    base = (tdav_session_av_t*)self;

    if (audio->is_started) {
        TSK_DEBUG_INFO("Audio session already started");
        return 0;
    }

    if (!(codec = tdav_session_av_get_best_neg_codec(base))) {
        TSK_DEBUG_ERROR("No codec matched");
        return -2;
    }

    TSK_OBJECT_SAFE_FREE(audio->encoder.codec);
    audio->encoder.codec = tsk_object_ref((tsk_object_t*)codec);

    if ((ret = tdav_session_av_start(base, codec))) {
        TSK_DEBUG_ERROR("tdav_session_av_start(audio) failed");
        return ret;
    }

    if (base->rtp_manager) {
        /* Denoise (AEC, Noise Suppression, AGC)
        * tmedia_denoise_process_record() is called after resampling and before encoding which means sampling rate is equal to codec's rate
        * tmedia_denoise_echo_playback() is called before playback which means sampling rate is equal to consumer's rate
        */
        if (audio->denoise) {
            uint32_t record_frame_size_samples = TMEDIA_CODEC_PCM_FRAME_SIZE_AUDIO_ENCODING(audio->encoder.codec);
            uint32_t record_sampling_rate = TMEDIA_CODEC_RATE_ENCODING(audio->encoder.codec);
            uint32_t record_channels = TMEDIA_CODEC_CHANNELS_AUDIO_ENCODING(audio->encoder.codec);

            uint32_t playback_frame_size_samples = (base->consumer && base->consumer->audio.ptime && base->consumer->audio.out.rate && base->consumer->audio.out.channels)
                                                   ? ((base->consumer->audio.ptime * base->consumer->audio.out.rate) / 1000) * base->consumer->audio.out.channels
                                                   : TMEDIA_CODEC_PCM_FRAME_SIZE_AUDIO_DECODING(audio->encoder.codec);
            uint32_t playback_sampling_rate = (base->consumer && base->consumer->audio.out.rate)
                                              ? base->consumer->audio.out.rate
                                              : TMEDIA_CODEC_RATE_DECODING(audio->encoder.codec);
            uint32_t playback_channels = (base->consumer && base->consumer->audio.out.channels)
                                         ? base->consumer->audio.out.channels
                                         : TMEDIA_CODEC_CHANNELS_AUDIO_DECODING(audio->encoder.codec);

            TSK_DEBUG_INFO("Audio denoiser to be opened(record_frame_size_samples=%u, record_sampling_rate=%u, record_channels=%u, playback_frame_size_samples=%u, playback_sampling_rate=%u, playback_channels=%u)",
                           record_frame_size_samples, record_sampling_rate, record_channels, playback_frame_size_samples, playback_sampling_rate, playback_channels);

            // close()
            tmedia_denoise_close(audio->denoise);
            // open() with new values
            tmedia_denoise_open(audio->denoise,
                                record_frame_size_samples, record_sampling_rate, TSK_CLAMP(1, record_channels, 2),
                                playback_frame_size_samples, playback_sampling_rate, TSK_CLAMP(1, playback_channels, 2));
        }
    }

    audio->is_started = (ret == 0);

    return ret;
}

static int tdav_session_audio_stop(tmedia_session_t* self)
{
    tdav_session_audio_t* audio = TDAV_SESSION_AUDIO(self);
    tdav_session_av_t* base = TDAV_SESSION_AV(self);
    int ret = tdav_session_av_stop(base);
    audio->is_started = tsk_false;
    TSK_OBJECT_SAFE_FREE(audio->encoder.codec);
    TSK_OBJECT_SAFE_FREE(audio->decoder.codec);

    // close the jitter buffer and denoiser to be sure it will be reopened and reinitialized if reINVITE or UPDATE
    // this is a "must" when the initial and updated sessions use codecs with different rate
    if (audio->jitterbuffer && audio->jitterbuffer->opened) {
        ret = tmedia_jitterbuffer_close(audio->jitterbuffer);
    }
    if (audio->denoise && audio->denoise->opened) {
        ret = tmedia_denoise_close(audio->denoise);
    }
    return ret;
}

static int tdav_session_audio_send_dtmf(tmedia_session_t* self, uint8_t event)
{
    tdav_session_audio_t* audio;
    tdav_session_av_t* base;
    tmedia_codec_t* codec;
    int ret, rate = 8000, ptime = 20;
    uint16_t duration;
    tdav_session_audio_dtmfe_t *dtmfe, *copy;
    int format = 101;

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    audio = (tdav_session_audio_t*)self;
    base = (tdav_session_av_t*)self;

    // Find the DTMF codec to use to use the RTP payload
    if ((codec = tmedia_codec_find_by_format(TMEDIA_SESSION(audio)->codecs, TMEDIA_CODEC_FORMAT_DTMF))) {
        rate = (int)codec->out.rate;
        format = atoi(codec->neg_format ? codec->neg_format : codec->format);
        TSK_OBJECT_SAFE_FREE(codec);
    }

    /* do we have an RTP manager? */
    if (!base->rtp_manager) {
        TSK_DEBUG_ERROR("No RTP manager associated to this session");
        return -2;
    }

    /* Create Events list */
    if (!audio->dtmf_events) {
        audio->dtmf_events = tsk_list_create();
    }

    /* Create global reference to the timer manager */
    if (!audio->timer.handle_mgr_global) {
        if (!(audio->timer.handle_mgr_global = tsk_timer_mgr_global_ref())) {
            TSK_DEBUG_ERROR("Failed to create Global Timer Manager");
            return -3;
        }
    }

    /* Start the timer manager */
    if (!audio->timer.started) {
        if ((ret = tsk_timer_manager_start(audio->timer.handle_mgr_global))) {
            TSK_DEBUG_ERROR("Failed to start Global Timer Manager");
            return ret;
        }
        audio->timer.started = tsk_true;
    }


    /*	RFC 4733 - 5.  Examples

    +-------+-----------+------+--------+------+--------+--------+------+
    |  Time | Event     |   M  |  Time- |  Seq |  Event |  Dura- |   E  |
    |  (ms) |           |  bit |  stamp |   No |  Code  |   tion |  bit |
    +-------+-----------+------+--------+------+--------+--------+------+
    |     0 | "9"       |      |        |      |        |        |      |
    |       | starts    |      |        |      |        |        |      |
    |    50 | RTP       |  "1" |      0 |    1 |    9   |    400 |  "0" |
    |       | packet 1  |      |        |      |        |        |      |
    |       | sent      |      |        |      |        |        |      |
    |   100 | RTP       |  "0" |      0 |    2 |    9   |    800 |  "0" |
    |       | packet 2  |      |        |      |        |        |      |
    |       | sent      |      |        |      |        |        |      |
    |   150 | RTP       |  "0" |      0 |    3 |    9   |   1200 |  "0" |
    |       | packet 3  |      |        |      |        |        |      |
    |       | sent      |      |        |      |        |        |      |
    |   200 | RTP       |  "0" |      0 |    4 |    9   |   1600 |  "0" |
    |       | packet 4  |      |        |      |        |        |      |
    |       | sent      |      |        |      |        |        |      |
    |   200 | "9" ends  |      |        |      |        |        |      |
    |   250 | RTP       |  "0" |      0 |    5 |    9   |   1600 |  "1" |
    |       | packet 4  |      |        |      |        |        |      |
    |       | first     |      |        |      |        |        |      |
    |       | retrans-  |      |        |      |        |        |      |
    |       | mission   |      |        |      |        |        |      |
    |   300 | RTP       |  "0" |      0 |    6 |    9   |   1600 |  "1" |
    |       | packet 4  |      |        |      |        |        |      |
    |       | second    |      |        |      |        |        |      |
    |       | retrans-  |      |        |      |        |        |      |
    |       | mission   |      |        |      |        |        |      |
    =====================================================================
    |   880 | First "1" |      |        |      |        |        |      |
    |       | starts    |      |        |      |        |        |      |
    |   930 | RTP       |  "1" |   7040 |    7 |    1   |    400 |  "0" |
    |       | packet 5  |      |        |      |        |        |      |
    |       | sent      |      |        |      |        |        |      |
    */

    // ref()(thread safeness)
    audio = tsk_object_ref(audio);

    // says we're sending DTMF digits to avoid mixing with audio (SRTP won't let this happen because of senquence numbers)
    // flag will be turned OFF when the list is empty
    audio->is_sending_dtmf_events = tsk_true;

    duration = TMEDIA_CODEC_PCM_FRAME_SIZE_AUDIO_ENCODING(audio->encoder.codec);

    // lock() list
    tsk_list_lock(audio->dtmf_events);

    copy = dtmfe = _tdav_session_audio_dtmfe_create(audio, event, duration * 1, ++base->rtp_manager->rtp.seq_num, base->rtp_manager->rtp.timestamp, (uint8_t)format, tsk_true, tsk_false);
    tsk_list_push_back_data(audio->dtmf_events, (void**)&dtmfe);
    tsk_timer_mgr_global_schedule(ptime * 0, _tdav_session_audio_dtmfe_timercb, copy);
    copy = dtmfe = _tdav_session_audio_dtmfe_create(audio, event, duration * 2, ++base->rtp_manager->rtp.seq_num, base->rtp_manager->rtp.timestamp, (uint8_t)format, tsk_false, tsk_false);
    tsk_list_push_back_data(audio->dtmf_events, (void**)&dtmfe);
    tsk_timer_mgr_global_schedule(ptime * 1, _tdav_session_audio_dtmfe_timercb, copy);
    copy = dtmfe = _tdav_session_audio_dtmfe_create(audio, event, duration * 3, ++base->rtp_manager->rtp.seq_num, base->rtp_manager->rtp.timestamp, (uint8_t)format, tsk_false, tsk_false);
    tsk_list_push_back_data(audio->dtmf_events, (void**)&dtmfe);
    tsk_timer_mgr_global_schedule(ptime * 2, _tdav_session_audio_dtmfe_timercb, copy);
    copy = dtmfe = _tdav_session_audio_dtmfe_create(audio, event, duration * 4, ++base->rtp_manager->rtp.seq_num, base->rtp_manager->rtp.timestamp, (uint8_t)format, tsk_false, tsk_false);
    tsk_list_push_back_data(audio->dtmf_events, (void**)&dtmfe);
    tsk_timer_mgr_global_schedule(ptime * 3, _tdav_session_audio_dtmfe_timercb, copy);

    copy = dtmfe = _tdav_session_audio_dtmfe_create(audio, event, duration * 4, ++base->rtp_manager->rtp.seq_num, base->rtp_manager->rtp.timestamp, (uint8_t)format, tsk_false, tsk_true);
    tsk_list_push_back_data(audio->dtmf_events, (void**)&dtmfe);
    tsk_timer_mgr_global_schedule(ptime * 4, _tdav_session_audio_dtmfe_timercb, copy);
    copy = dtmfe = _tdav_session_audio_dtmfe_create(audio, event, duration * 4, ++base->rtp_manager->rtp.seq_num, base->rtp_manager->rtp.timestamp, (uint8_t)format, tsk_false, tsk_true);
    tsk_list_push_back_data(audio->dtmf_events, (void**)&dtmfe);
    tsk_timer_mgr_global_schedule(ptime * 5, _tdav_session_audio_dtmfe_timercb, copy);

    // unlock() list
    tsk_list_unlock(audio->dtmf_events);

    // increment timestamp
    base->rtp_manager->rtp.timestamp += duration;

    // unref()(thread safeness)
    audio = tsk_object_unref(audio);

    return 0;
}

static int tdav_session_audio_pause(tmedia_session_t* self)
{
    return tdav_session_av_pause(TDAV_SESSION_AV(self));
}

static const tsdp_header_M_t* tdav_session_audio_get_lo(tmedia_session_t* self)
{
    tsk_bool_t updated = tsk_false;
    const tsdp_header_M_t* ret;
    tdav_session_av_t* base = TDAV_SESSION_AV(self);


    if (!(ret = tdav_session_av_get_lo(base, &updated))) {
        TSK_DEBUG_ERROR("tdav_session_av_get_lo(audio) failed");
        return tsk_null;
    }

    if (updated) {
        tsk_safeobj_lock(base);
        TSK_OBJECT_SAFE_FREE(TDAV_SESSION_AUDIO(self)->encoder.codec);
        tsk_safeobj_unlock(base);
    }

    return ret;
}

static int tdav_session_audio_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m)
{
    int ret;
    tsk_bool_t updated = tsk_false;
    tdav_session_av_t* base = TDAV_SESSION_AV(self);

    if ((ret = tdav_session_av_set_ro(base, m, &updated))) {
        TSK_DEBUG_ERROR("tdav_session_av_set_ro(audio) failed");
        return ret;
    }

    if (updated) {
        tsk_safeobj_lock(base);
        // reset audio jitter buffer (new Offer probably comes with new seq_nums or timestamps)
        if (base->consumer) {
            ret = tdav_consumer_audio_reset(TDAV_CONSUMER_AUDIO(base->consumer));
        }
        // destroy encoder to force requesting new one
        TSK_OBJECT_SAFE_FREE(TDAV_SESSION_AUDIO(self)->encoder.codec);
        tsk_safeobj_unlock(base);
    }

    return ret;
}

/* apply gain */
static void _tdav_session_audio_apply_gain(void* buffer, int len, int bps, int gain)
{
    register int i;
    int max_val;

    max_val = (1 << (bps - 1 - gain)) - 1;

    if (bps == 8) {
        int8_t *buff = buffer;
        for (i = 0; i < len; i++) {
            if (buff[i] > -max_val && buff[i] < max_val) {
                buff[i] = buff[i] << gain;
            }
        }
    }
    else if (bps == 16) {
        int16_t *buff = buffer;
        for (i = 0; i < len / 2; i++) {
            if (buff[i] > -max_val && buff[i] < max_val) {
                buff[i] = buff[i] << gain;
            }
        }
    }
}


/* Internal function used to create new DTMF event */
static tdav_session_audio_dtmfe_t* _tdav_session_audio_dtmfe_create(const tdav_session_audio_t* session, uint8_t event, uint16_t duration, uint32_t seq, uint32_t timestamp, uint8_t format, tsk_bool_t M, tsk_bool_t E)
{
    tdav_session_audio_dtmfe_t* dtmfe;
    const tdav_session_av_t* base = (const tdav_session_av_t*)session;
    static uint8_t volume = 10;
    static uint32_t ssrc = 0x5234A8;

    uint8_t pay[4] = { 0 };

    /* RFC 4733 - 2.3.  Payload Format
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |     event     |E|R| volume    |          duration             |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    */

    if (!(dtmfe = tsk_object_new(tdav_session_audio_dtmfe_def_t))) {
        TSK_DEBUG_ERROR("Failed to create new DTMF event");
        return tsk_null;
    }
    dtmfe->session = session;

    if (!(dtmfe->packet = trtp_rtp_packet_create((session && base->rtp_manager) ? base->rtp_manager->rtp.ssrc.local : ssrc, seq, timestamp, format, M))) {
        TSK_DEBUG_ERROR("Failed to create DTMF RTP packet");
        TSK_OBJECT_SAFE_FREE(dtmfe);
        return tsk_null;
    }

    pay[0] = event;
    pay[1] |= ((E << 7) | (volume & 0x3F));
    pay[2] = (duration >> 8);
    pay[3] = (duration & 0xFF);

    /* set data */
    if ((dtmfe->packet->payload.data = tsk_calloc(sizeof(pay), sizeof(uint8_t)))) {
        memcpy(dtmfe->packet->payload.data, pay, sizeof(pay));
        dtmfe->packet->payload.size = sizeof(pay);
    }

    return dtmfe;
}

static int _tdav_session_audio_dtmfe_timercb(const void* arg, tsk_timer_id_t timer_id)
{
    tdav_session_audio_dtmfe_t* dtmfe = (tdav_session_audio_dtmfe_t*)arg;
    tdav_session_audio_t *audio;

    if (!dtmfe || !dtmfe->session || !dtmfe->session->dtmf_events) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* Send the data */
    TSK_DEBUG_INFO("Sending DTMF event...");
    trtp_manager_send_rtp_packet(TDAV_SESSION_AV(dtmfe->session)->rtp_manager, dtmfe->packet, tsk_false);


    audio = tsk_object_ref(TSK_OBJECT(dtmfe->session));
    tsk_list_lock(audio->dtmf_events);
    /* Remove and delete the event from the queue */
    tsk_list_remove_item_by_data(audio->dtmf_events, dtmfe);
    /* Check if there are pending events */
    audio->is_sending_dtmf_events = !TSK_LIST_IS_EMPTY(audio->dtmf_events);
    tsk_list_unlock(audio->dtmf_events);
    tsk_object_unref(audio);

    return 0;
}

static tmedia_resampler_t* _tdav_session_audio_resampler_create(int32_t bytes_per_sample, uint32_t in_freq, uint32_t out_freq, uint32_t frame_duration, uint32_t in_channels, uint32_t out_channels, uint32_t quality, void** resampler_buffer, tsk_size_t *resampler_buffer_size)
{
    uint32_t resampler_buff_size;
    tmedia_resampler_t* resampler;
    int ret;

    if (out_channels > 2 || in_channels > 2) {
        TSK_DEBUG_ERROR("Invalid parameter: out_channels=%u, in_channels=%u", out_channels, in_channels);
        return tsk_null;
    }

    resampler_buff_size = (((out_freq * frame_duration) / 1000) * bytes_per_sample) << (out_channels == 2 ? 1 : 0);

    if (!(resampler = tmedia_resampler_create())) {
        TSK_DEBUG_ERROR("Failed to create audio resampler");
        return tsk_null;
    }
    else {
        if ((ret = tmedia_resampler_open(resampler, in_freq, out_freq, frame_duration, in_channels, out_channels, quality, 16))) {
            TSK_DEBUG_ERROR("Failed to open audio resampler (%d, %d, %d, %d, %d,%d) with retcode=%d", in_freq, out_freq, frame_duration, in_channels, out_channels, quality, ret);
            TSK_OBJECT_SAFE_FREE(resampler);
            goto done;
        }
    }
    // create temp resampler buffer
    if ((*resampler_buffer = tsk_realloc(*resampler_buffer, resampler_buff_size))) {
        *resampler_buffer_size = resampler_buff_size;
    }
    else {
        *resampler_buffer_size = 0;
        TSK_DEBUG_ERROR("Failed to allocate resampler buffer with size = %d", resampler_buff_size);
        TSK_OBJECT_SAFE_FREE(resampler);
        goto done;
    }
done:
    return resampler;
}

//=================================================================================================
//	Session Audio Plugin object definition
//
/* constructor */
static tsk_object_t* tdav_session_audio_ctor(tsk_object_t * self, va_list * app)
{
    tdav_session_audio_t *audio = self;
    if (audio) {
        int ret;
        tdav_session_av_t *base = TDAV_SESSION_AV(self);

        /* init() base */
        if ((ret = tdav_session_av_init(base, tmedia_audio)) != 0) {
            TSK_DEBUG_ERROR("tdav_session_av_init(audio) failed");
            return tsk_null;
        }

        /* init() self */
        if (base->producer) {
            tmedia_producer_set_enc_callback(base->producer, tdav_session_audio_producer_enc_cb, audio);
        }
        if (base->consumer) {
            // It's important to create the denoiser and jitter buffer here as dynamic plugins (from shared libs) don't have access to the registry
            if (!(audio->denoise = tmedia_denoise_create())) {
                TSK_DEBUG_WARN("No Audio denoiser found");
            }
            else {
                // IMPORTANT: This means that the consumer must be child of "tdav_consumer_audio_t" object
                tdav_consumer_audio_set_denoise(TDAV_CONSUMER_AUDIO(base->consumer), audio->denoise);
            }

            if (!(audio->jitterbuffer = tmedia_jitterbuffer_create(tmedia_audio))) {
                TSK_DEBUG_ERROR("Failed to create jitter buffer");
            }
            else {
                ret = tmedia_jitterbuffer_init(TMEDIA_JITTER_BUFFER(audio->jitterbuffer));
                tdav_consumer_audio_set_jitterbuffer(TDAV_CONSUMER_AUDIO(base->consumer), audio->jitterbuffer);
            }
        }
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_session_audio_dtor(tsk_object_t * self)
{
    tdav_session_audio_t *audio = self;
    TSK_DEBUG_INFO("*** tdav_session_audio_t destroyed ***");
    if (audio) {
        tdav_session_audio_stop((tmedia_session_t*)audio);
        // Do it in this order (deinit self first)

        /* Timer manager */
        if (audio->timer.started) {
            if (audio->dtmf_events) {
                /* Cancel all events */
                tsk_list_item_t* item;
                tsk_list_foreach(item, audio->dtmf_events) {
                    tsk_timer_mgr_global_cancel(((tdav_session_audio_dtmfe_t*)item->data)->timer_id);
                }
            }
        }

        tsk_timer_mgr_global_unref(&audio->timer.handle_mgr_global);

        /* CleanUp the DTMF events */
        TSK_OBJECT_SAFE_FREE(audio->dtmf_events);

        TSK_OBJECT_SAFE_FREE(audio->denoise);
        TSK_OBJECT_SAFE_FREE(audio->jitterbuffer);

        TSK_OBJECT_SAFE_FREE(audio->encoder.codec);
        TSK_FREE(audio->encoder.buffer);
        TSK_OBJECT_SAFE_FREE(audio->decoder.codec);
        TSK_FREE(audio->decoder.buffer);

        // free resamplers
        TSK_FREE(audio->encoder.resampler.buffer);
        TSK_OBJECT_SAFE_FREE(audio->encoder.resampler.instance);
        TSK_FREE(audio->decoder.resampler.buffer);
        TSK_OBJECT_SAFE_FREE(audio->decoder.resampler.instance);

        /* deinit base */
        tdav_session_av_deinit(TDAV_SESSION_AV(self));

        TSK_DEBUG_INFO("*** Audio session destroyed ***");
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_session_audio_def_s = {
    sizeof(tdav_session_audio_t),
    tdav_session_audio_ctor,
    tdav_session_audio_dtor,
    tmedia_session_cmp,
};
/* plugin definition*/
static const tmedia_session_plugin_def_t tdav_session_audio_plugin_def_s = {
    &tdav_session_audio_def_s,

    tmedia_audio,
    "audio",

    tdav_session_audio_set,
    tdav_session_audio_get,
    tdav_session_audio_prepare,
    tdav_session_audio_start,
    tdav_session_audio_pause,
    tdav_session_audio_stop,

    /* Audio part */
    {
        tdav_session_audio_send_dtmf
    },

    tdav_session_audio_get_lo,
    tdav_session_audio_set_ro
};
const tmedia_session_plugin_def_t *tdav_session_audio_plugin_def_t = &tdav_session_audio_plugin_def_s;
static const tmedia_session_plugin_def_t tdav_session_bfcpaudio_plugin_def_s = {
    &tdav_session_audio_def_s,

    tmedia_bfcp_audio,
    "audio",

    tdav_session_audio_set,
    tdav_session_audio_get,
    tdav_session_audio_prepare,
    tdav_session_audio_start,
    tdav_session_audio_pause,
    tdav_session_audio_stop,

    /* Audio part */
    {
        tdav_session_audio_send_dtmf
    },

    tdav_session_audio_get_lo,
    tdav_session_audio_set_ro
};
const tmedia_session_plugin_def_t *tdav_session_bfcpaudio_plugin_def_t = &tdav_session_bfcpaudio_plugin_def_s;



//=================================================================================================
//	DTMF event object definition
//
static tsk_object_t* tdav_session_audio_dtmfe_ctor(tsk_object_t * self, va_list * app)
{
    tdav_session_audio_dtmfe_t *event = self;
    if (event) {
        event->timer_id = TSK_INVALID_TIMER_ID;
    }
    return self;
}

static tsk_object_t* tdav_session_audio_dtmfe_dtor(tsk_object_t * self)
{
    tdav_session_audio_dtmfe_t *event = self;
    if (event) {
        TSK_OBJECT_SAFE_FREE(event->packet);
    }

    return self;
}

static int tdav_session_audio_dtmfe_cmp(const tsk_object_t *_e1, const tsk_object_t *_e2)
{
    int ret;
    tsk_subsat_int32_ptr(_e1, _e2, &ret);
    return ret;
}

static const tsk_object_def_t tdav_session_audio_dtmfe_def_s = {
    sizeof(tdav_session_audio_dtmfe_t),
    tdav_session_audio_dtmfe_ctor,
    tdav_session_audio_dtmfe_dtor,
    tdav_session_audio_dtmfe_cmp,
};
const tsk_object_def_t *tdav_session_audio_dtmfe_def_t = &tdav_session_audio_dtmfe_def_s;
