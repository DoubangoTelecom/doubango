/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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

/**@file tdav_codec_h264.c
 * @brief H.264 codec plugin using FFmpeg for decoding and x264 for encoding
 * RTP payloader/depayloader follows RFC 3984
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinydav/codecs/h264/tdav_codec_h264.h"

#if HAVE_FFMPEG || HAVE_H264_PASSTHROUGH

#include "tinydav/codecs/h264/tdav_codec_h264_rtp.h"
#include "tinydav/video/tdav_converter_video.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tinymedia/tmedia_params.h"
#include "tinymedia/tmedia_defaults.h"

#include "tsk_params.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#if HAVE_FFMPEG
#   include <libavcodec/avcodec.h>
#   if (LIBAVUTIL_VERSION_INT >= AV_VERSION_INT(51, 35, 0))
#       include <libavutil/opt.h>
#   endif
#endif

typedef struct tdav_codec_h264_s {
    TDAV_DECLARE_CODEC_H264_COMMON;

    // Encoder
    struct {
#if HAVE_FFMPEG
        AVCodec* codec;
        AVCodecContext* context;
        AVFrame* picture;
#endif
        void* buffer;
        int64_t frame_count;
        tsk_bool_t force_idr;
        int32_t quality; // [1-31]
        int rotation;
        int32_t max_bw_kpbs;
        tsk_bool_t passthrough; // whether to bypass encoding
    } encoder;

    // decoder
    struct {
#if HAVE_FFMPEG
        AVCodec* codec;
        AVCodecContext* context;
        AVFrame* picture;
#endif
        void* accumulator;
        tsk_size_t accumulator_pos;
        tsk_size_t accumulator_size;
        uint16_t last_seq;
        tsk_bool_t passthrough; // whether to bypass decoding
    } decoder;
}
tdav_codec_h264_t;

#if !defined(TDAV_H264_GOP_SIZE_IN_SECONDS)
#   define TDAV_H264_GOP_SIZE_IN_SECONDS		25
#endif

#define kResetRotationTrue tsk_true
#define kResetRotationFalse tsk_false

static int tdav_codec_h264_init(tdav_codec_h264_t* self, profile_idc_t profile);
static int tdav_codec_h264_deinit(tdav_codec_h264_t* self);
static int tdav_codec_h264_open_encoder(tdav_codec_h264_t* self);
static int tdav_codec_h264_close_encoder(tdav_codec_h264_t* self, tsk_bool_t reset_rotation);
static int tdav_codec_h264_open_decoder(tdav_codec_h264_t* self);
static int tdav_codec_h264_close_decoder(tdav_codec_h264_t* self);

/* ============ H.264 Base/Main Profile X.X Plugin interface functions ================= */

static int tdav_codec_h264_set(tmedia_codec_t* self, const tmedia_param_t* param)
{
    tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)self;
    if (param->value_type == tmedia_pvt_int32) {
        if(tsk_striequals(param->key, "action")) {
            tmedia_codec_action_t action = (tmedia_codec_action_t)TSK_TO_INT32((uint8_t*)param->value);
            switch(action) {
            case tmedia_codec_action_encode_idr: {
                h264->encoder.force_idr = tsk_true;
                break;
            }
            case tmedia_codec_action_bw_down: {
                h264->encoder.quality = TSK_CLAMP(1, (h264->encoder.quality + 1), 31);
#if HAVE_FFMPEG
                if (h264->encoder.context) {
                    h264->encoder.context->global_quality = FF_QP2LAMBDA * h264->encoder.quality;
                }
#endif
                break;
            }
            case tmedia_codec_action_bw_up: {
                h264->encoder.quality = TSK_CLAMP(1, (h264->encoder.quality - 1), 31);
#if HAVE_FFMPEG
                if (h264->encoder.context) {
                    h264->encoder.context->global_quality = FF_QP2LAMBDA * h264->encoder.quality;
                }
#endif
                break;
            }
            }
            return 0;
        }
        else if(tsk_striequals(param->key, "bw_kbps")) {
            int32_t max_bw_userdefine = self->bandwidth_max_upload;
            int32_t max_bw_new = *((int32_t*)param->value);
            if (max_bw_userdefine > 0) {
                // do not use more than what the user defined in it's configuration
                h264->encoder.max_bw_kpbs = TSK_MIN(max_bw_new, max_bw_userdefine);
            }
            else {
                h264->encoder.max_bw_kpbs = max_bw_new;
            }
            // dynamic bandwidth change not implemented for x264
            // please use openh264 or msf264
            TSK_DEBUG_INFO("Bandwidth change not supported using x264 ...ignoring new bandwidth value");
            return 0;
        }
        else if(tsk_striequals(param->key, "bypass-encoding")) {
            h264->encoder.passthrough = *((int32_t*)param->value) ? tsk_true : tsk_false;
            TSK_DEBUG_INFO("[H.264] bypass-encoding = %d", h264->encoder.passthrough);
            return 0;
        }
        else if(tsk_striequals(param->key, "bypass-decoding")) {
            h264->decoder.passthrough = *((int32_t*)param->value) ? tsk_true : tsk_false;
            TSK_DEBUG_INFO("[H.264] bypass-decoding = %d", h264->decoder.passthrough);
            return 0;
        }
        else if(tsk_striequals(param->key, "rotation")) {
            int32_t rotation = *((int32_t*)param->value);
            if(h264->encoder.rotation != rotation) {
                h264->encoder.rotation = rotation;
                if (self->opened) {
                    int ret;
                    if ((ret = tdav_codec_h264_close_encoder(h264, kResetRotationFalse))) {
                        return ret;
                    }
                    if ((ret = tdav_codec_h264_open_encoder(h264))) {
                        return ret;
                    }
#if 0 // Not working
                    if((ret = avcodec_close(h264->encoder.context))) {
                        TSK_DEBUG_ERROR("Failed to close [%s] codec", TMEDIA_CODEC(h264)->plugin->desc);
                        return ret;
                    }
                    h264->encoder.context->width = (rotation == 90 || rotation == 270) ? TMEDIA_CODEC_VIDEO(h264)->out.height : TMEDIA_CODEC_VIDEO(h264)->out.width;
                    h264->encoder.context->height = (rotation == 90 || rotation == 270) ? TMEDIA_CODEC_VIDEO(h264)->out.width : TMEDIA_CODEC_VIDEO(h264)->out.height;
                    if((ret = avcodec_open(h264->encoder.context, h264->encoder.codec)) < 0) {
                        TSK_DEBUG_ERROR("Failed to open [%s] codec", TMEDIA_CODEC(h264)->plugin->desc);
                        return ret;
                    }
                    h264->encoder.force_idr = tsk_true;
#endif
                }
            }
            return 0;
        }
    }
    return -1;
}


static int tdav_codec_h264_open(tmedia_codec_t* self)
{
    int ret;
    tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)self;

    if(!h264) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* the caller (base class) already checked that the codec is not opened */

    //	Encoder
    if((ret = tdav_codec_h264_open_encoder(h264))) {
        return ret;
    }

    //	Decoder
    if((ret = tdav_codec_h264_open_decoder(h264))) {
        return ret;
    }

    return 0;
}

static int tdav_codec_h264_close(tmedia_codec_t* self)
{
    tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)self;

    if(!h264) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* the caller (base class) alreasy checked that the codec is opened */

    //	Encoder
    tdav_codec_h264_close_encoder(h264, kResetRotationTrue);

    //	Decoder
    tdav_codec_h264_close_decoder(h264);

    return 0;
}

static tsk_size_t tdav_codec_h264_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
    int ret = 0;

#if HAVE_FFMPEG
    int size;
    tsk_bool_t send_idr, send_hdr;
#endif

    tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)self;

    if(!self || !in_data || !in_size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    if(!self->opened) {
        TSK_DEBUG_ERROR("Codec not opened");
        return 0;
    }

    if(h264->encoder.passthrough) {
        tdav_codec_h264_rtp_encap(TDAV_CODEC_H264_COMMON(h264), (const uint8_t*)in_data, in_size);
    }
    else { // !h264->encoder.passthrough
#if HAVE_FFMPEG		// wrap yuv420 buffer
        size = avpicture_fill((AVPicture *)h264->encoder.picture, (uint8_t*)in_data, PIX_FMT_YUV420P, h264->encoder.context->width, h264->encoder.context->height);
        if (size != in_size) {
            /* guard */
            TSK_DEBUG_ERROR("Invalid size: %u<>%u", size, in_size);
            return 0;
        }

        // send IDR for:
        //	- the first frame
        //  - remote peer requested an IDR
        //	- every second within the first 4seconds
        send_idr = (
                       h264->encoder.frame_count++ == 0
                       || h264 ->encoder.force_idr
                       //|| ( (h264->encoder.frame_count < (int)TMEDIA_CODEC_VIDEO(h264)->out.fps * 4) && ((h264->encoder.frame_count % TMEDIA_CODEC_VIDEO(h264)->out.fps)==0) )
                   );

        // send SPS and PPS headers for:
        //  - IDR frames (not required but it's the easiest way to deal with pkt loss)
        //  - every 5 seconds after the first 4seconds
        send_hdr = (
                       send_idr
                       //|| ( (h264->encoder.frame_count % (TMEDIA_CODEC_VIDEO(h264)->out.fps * 5))==0 )
                   );
        if(send_hdr) {
            tdav_codec_h264_rtp_encap(TDAV_CODEC_H264_COMMON(h264), h264->encoder.context->extradata, (tsk_size_t)h264->encoder.context->extradata_size);
        }

        // Encode data
#if LIBAVCODEC_VERSION_MAJOR <= 53
        h264->encoder.picture->pict_type = send_idr ? FF_I_TYPE : 0;
#else
        h264->encoder.picture->pict_type = send_idr ? AV_PICTURE_TYPE_I : AV_PICTURE_TYPE_NONE;
#endif
        h264->encoder.picture->key_frame = send_idr ? 1 : 0;
        h264->encoder.picture->pts = AV_NOPTS_VALUE;
        h264->encoder.picture->quality = h264->encoder.context->global_quality;
        // h264->encoder.picture->pts = h264->encoder.frame_count; MUST NOT
        ret = avcodec_encode_video(h264->encoder.context, h264->encoder.buffer, size, h264->encoder.picture);
        if(ret > 0) {
            tdav_codec_h264_rtp_encap(TDAV_CODEC_H264_COMMON(h264), h264->encoder.buffer, (tsk_size_t)ret);
        }
        h264 ->encoder.force_idr = tsk_false;
#endif
    }// else(!h264->encoder.passthrough)

    return 0;
}

static tsk_size_t tdav_codec_h264_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
    tdav_codec_h264_t* h264 = (tdav_codec_h264_t*)self;
    const trtp_rtp_header_t* rtp_hdr = (const trtp_rtp_header_t*)proto_hdr;

    const uint8_t* pay_ptr = tsk_null;
    tsk_size_t pay_size = 0;
    int ret;
    tsk_bool_t sps_or_pps, append_scp, end_of_unit;
    tsk_size_t retsize = 0, size_to_copy = 0;
    static const tsk_size_t xmax_size = (3840 * 2160 * 3) >> 3; // >>3 instead of >>1 (not an error)
    static tsk_size_t start_code_prefix_size = sizeof(H264_START_CODE_PREFIX);
#if HAVE_FFMPEG
    int got_picture_ptr = 0;
#endif

    if(!h264 || !in_data || !in_size || !out_data
#if HAVE_FFMPEG
            || !h264->decoder.context
#endif
      ) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    //TSK_DEBUG_INFO("SeqNo=%hu", rtp_hdr->seq_num);

    /* Packet lost? */
    if((h264->decoder.last_seq + 1) != rtp_hdr->seq_num && h264->decoder.last_seq) {
        TSK_DEBUG_INFO("[H.264] Packet loss, seq_num=%d", (h264->decoder.last_seq + 1));
    }
    h264->decoder.last_seq = rtp_hdr->seq_num;


    /* 5.3. NAL Unit Octet Usage
      +---------------+
      |0|1|2|3|4|5|6|7|
      +-+-+-+-+-+-+-+-+
      |F|NRI|  Type   |
      +---------------+
    */
    if(*((uint8_t*)in_data) & 0x80) {
        TSK_DEBUG_WARN("F=1");
        /* reset accumulator */
        h264->decoder.accumulator_pos = 0;
        return 0;
    }

    /* get payload */
    if((ret = tdav_codec_h264_get_pay(in_data, in_size, (const void**)&pay_ptr, &pay_size, &append_scp, &end_of_unit)) || !pay_ptr || !pay_size) {
        TSK_DEBUG_ERROR("Depayloader failed to get H.264 content");
        return 0;
    }
    //append_scp = tsk_true;
    size_to_copy = pay_size + (append_scp ? start_code_prefix_size : 0);
    // whether it's SPS or PPS (append_scp is false for subsequent FUA chuncks)
    sps_or_pps = append_scp && pay_ptr && ((pay_ptr[0] & 0x1F) == 7 || (pay_ptr[0] & 0x1F) == 8);

    // start-accumulator
    if(!h264->decoder.accumulator) {
        if(size_to_copy > xmax_size) {
            TSK_DEBUG_ERROR("%u too big to contain valid encoded data. xmax_size=%u", size_to_copy, xmax_size);
            return 0;
        }
        if(!(h264->decoder.accumulator = tsk_calloc(size_to_copy, sizeof(uint8_t)))) {
            TSK_DEBUG_ERROR("Failed to allocated new buffer");
            return 0;
        }
        h264->decoder.accumulator_size = size_to_copy;
    }
    if((h264->decoder.accumulator_pos + size_to_copy) >= xmax_size) {
        TSK_DEBUG_ERROR("BufferOverflow");
        h264->decoder.accumulator_pos = 0;
        return 0;
    }
    if((h264->decoder.accumulator_pos + size_to_copy) > h264->decoder.accumulator_size) {
        if(!(h264->decoder.accumulator = tsk_realloc(h264->decoder.accumulator, (h264->decoder.accumulator_pos + size_to_copy)))) {
            TSK_DEBUG_ERROR("Failed to reallocated new buffer");
            h264->decoder.accumulator_pos = 0;
            h264->decoder.accumulator_size = 0;
            return 0;
        }
        h264->decoder.accumulator_size = (h264->decoder.accumulator_pos + size_to_copy);
    }

    if(append_scp) {
        memcpy(&((uint8_t*)h264->decoder.accumulator)[h264->decoder.accumulator_pos], H264_START_CODE_PREFIX, start_code_prefix_size);
        h264->decoder.accumulator_pos += start_code_prefix_size;
    }
    memcpy(&((uint8_t*)h264->decoder.accumulator)[h264->decoder.accumulator_pos], pay_ptr, pay_size);
    h264->decoder.accumulator_pos += pay_size;
    // end-accumulator

    if(sps_or_pps) {
        // http://libav-users.943685.n4.nabble.com/Decode-H264-streams-how-to-fill-AVCodecContext-from-SPS-PPS-td2484472.html
        // SPS and PPS should be bundled with IDR
        TSK_DEBUG_INFO("Receiving SPS or PPS ...to be tied to an IDR");
    }
    else if(rtp_hdr->marker) {
        if(h264->decoder.passthrough) {
            if(*out_max_size < h264->decoder.accumulator_pos) {
                if((*out_data = tsk_realloc(*out_data, h264->decoder.accumulator_pos))) {
                    *out_max_size = h264->decoder.accumulator_pos;
                }
                else {
                    *out_max_size = 0;
                    return 0;
                }
            }
            memcpy(*out_data, h264->decoder.accumulator, h264->decoder.accumulator_pos);
            retsize = h264->decoder.accumulator_pos;
        }
        else { // !h264->decoder.passthrough
#if HAVE_FFMPEG
            AVPacket packet;

            /* decode the picture */
            av_init_packet(&packet);
            packet.dts = packet.pts = AV_NOPTS_VALUE;
            packet.size = (int)h264->decoder.accumulator_pos;
            packet.data = h264->decoder.accumulator;
            ret = avcodec_decode_video2(h264->decoder.context, h264->decoder.picture, &got_picture_ptr, &packet);

            if(ret <0) {
                TSK_DEBUG_INFO("Failed to decode the buffer with error code =%d, size=%u, append=%s", ret, h264->decoder.accumulator_pos, append_scp ? "yes" : "no");
                if(TMEDIA_CODEC_VIDEO(self)->in.callback) {
                    TMEDIA_CODEC_VIDEO(self)->in.result.type = tmedia_video_decode_result_type_error;
                    TMEDIA_CODEC_VIDEO(self)->in.result.proto_hdr = proto_hdr;
                    TMEDIA_CODEC_VIDEO(self)->in.callback(&TMEDIA_CODEC_VIDEO(self)->in.result);
                }
            }
            else if(got_picture_ptr) {
                tsk_size_t xsize;

                /* IDR ? */
                if(((pay_ptr[0] & 0x1F) == 0x05) && TMEDIA_CODEC_VIDEO(self)->in.callback) {
                    TSK_DEBUG_INFO("Decoded H.264 IDR");
                    TMEDIA_CODEC_VIDEO(self)->in.result.type = tmedia_video_decode_result_type_idr;
                    TMEDIA_CODEC_VIDEO(self)->in.result.proto_hdr = proto_hdr;
                    TMEDIA_CODEC_VIDEO(self)->in.callback(&TMEDIA_CODEC_VIDEO(self)->in.result);
                }
                /* fill out */
                xsize = avpicture_get_size(h264->decoder.context->pix_fmt, h264->decoder.context->width, h264->decoder.context->height);
                if(*out_max_size<xsize) {
                    if((*out_data = tsk_realloc(*out_data, (xsize + FF_INPUT_BUFFER_PADDING_SIZE)))) {
                        *out_max_size = xsize;
                    }
                    else {
                        *out_max_size = 0;
                        return 0;
                    }
                }
                retsize = xsize;
                TMEDIA_CODEC_VIDEO(h264)->in.width = h264->decoder.context->width;
                TMEDIA_CODEC_VIDEO(h264)->in.height = h264->decoder.context->height;
                avpicture_layout((AVPicture *)h264->decoder.picture, h264->decoder.context->pix_fmt, (int)h264->decoder.context->width, (int)h264->decoder.context->height,
                                 *out_data, (int)retsize);
            }
#endif /* HAVE_FFMPEG */
        } // else(h264->decoder.passthrough)

        h264->decoder.accumulator_pos = 0;
    } // else if(rtp_hdr->marker)

    return retsize;
}

static tsk_bool_t tdav_codec_h264_sdp_att_match(const tmedia_codec_t* self, const char* att_name, const char* att_value)
{
    return tdav_codec_h264_common_sdp_att_match((tdav_codec_h264_common_t*)self, att_name, att_value);
}

static char* tdav_codec_h264_sdp_att_get(const tmedia_codec_t* self, const char* att_name)
{
    char* att = tdav_codec_h264_common_sdp_att_get((const tdav_codec_h264_common_t*)self, att_name);
    if(att && tsk_striequals(att_name, "fmtp")) {
        tsk_strcat_2(&att, "; impl=%s",
#if HAVE_FFMPEG
                     "FFMPEG"
#elif HAVE_H264_PASSTHROUGH
                     "PASSTHROUGH"
#endif
                    );
    }
    return att;
}




/* ============ H.264 Base Profile Plugin interface ================= */

/* constructor */
static tsk_object_t* tdav_codec_h264_base_ctor(tsk_object_t * self, va_list * app)
{
    tdav_codec_h264_t *h264 = (tdav_codec_h264_t*)self;
    if(h264) {
        /* init base: called by tmedia_codec_create() */
        /* init self */
        if(tdav_codec_h264_init(h264, profile_idc_baseline) != 0) {
            return tsk_null;
        }
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h264_base_dtor(tsk_object_t * self)
{
    tdav_codec_h264_t *h264 = (tdav_codec_h264_t*)self;
    if(h264) {
        /* deinit base */
        tdav_codec_h264_common_deinit((tdav_codec_h264_common_t*)self);
        /* deinit self */
        tdav_codec_h264_deinit(h264);

    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h264_base_def_s = {
    sizeof(tdav_codec_h264_t),
    tdav_codec_h264_base_ctor,
    tdav_codec_h264_base_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h264_base_plugin_def_s = {
    &tdav_codec_h264_base_def_s,

    tmedia_video,
    tmedia_codec_id_h264_bp,
    "H264",
    "H264 Base Profile (FFmpeg, x264)",
    TMEDIA_CODEC_FORMAT_H264_BP,
    tsk_true,
    90000, // rate

    /* audio */
    { 0 },

    /* video (width, height, fps) */
    {176, 144, 0}, // fps is @deprecated

    tdav_codec_h264_set,
    tdav_codec_h264_open,
    tdav_codec_h264_close,
    tdav_codec_h264_encode,
    tdav_codec_h264_decode,
    tdav_codec_h264_sdp_att_match,
    tdav_codec_h264_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_h264_base_plugin_def_t = &tdav_codec_h264_base_plugin_def_s;

/* ============ H.264 Main Profile Plugin interface ================= */

/* constructor */
static tsk_object_t* tdav_codec_h264_main_ctor(tsk_object_t * self, va_list * app)
{
    tdav_codec_h264_t *h264 = (tdav_codec_h264_t*)self;
    if(h264) {
        /* init base: called by tmedia_codec_create() */
        /* init self */
        if(tdav_codec_h264_init(h264, profile_idc_main) != 0) {
            return tsk_null;
        }
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h264_main_dtor(tsk_object_t * self)
{
    tdav_codec_h264_t *h264 = (tdav_codec_h264_t*)self;
    if(h264) {
        /* deinit base */
        tdav_codec_h264_common_deinit((tdav_codec_h264_common_t*)self);
        /* deinit self */
        tdav_codec_h264_deinit(h264);

    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h264_main_def_s = {
    sizeof(tdav_codec_h264_t),
    tdav_codec_h264_main_ctor,
    tdav_codec_h264_main_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h264_main_plugin_def_s = {
    &tdav_codec_h264_main_def_s,

    tmedia_video,
    tmedia_codec_id_h264_mp,
    "H264",
    "H264 Main Profile (FFmpeg, x264)",
    TMEDIA_CODEC_FORMAT_H264_MP,
    tsk_true,
    90000, // rate

    /* audio */
    { 0 },

    /* video (width, height, fps)*/
    {176, 144, 0},// fps is @deprecated

    tdav_codec_h264_set,
    tdav_codec_h264_open,
    tdav_codec_h264_close,
    tdav_codec_h264_encode,
    tdav_codec_h264_decode,
    tdav_codec_h264_sdp_att_match,
    tdav_codec_h264_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_h264_main_plugin_def_t = &tdav_codec_h264_main_plugin_def_s;





































/* ============ Common To all H264 codecs ================= */

int tdav_codec_h264_open_encoder(tdav_codec_h264_t* self)
{
#if HAVE_FFMPEG
    int ret;
    tsk_size_t size;

    if(self->encoder.context) {
        TSK_DEBUG_ERROR("Encoder already opened");
        return -1;
    }

#if (LIBAVUTIL_VERSION_INT >= AV_VERSION_INT(51, 35, 0))
    if((self->encoder.context = avcodec_alloc_context3(self->encoder.codec))) {
        avcodec_get_context_defaults3(self->encoder.context, self->encoder.codec);
    }
#else
    if((self->encoder.context = avcodec_alloc_context())) {
        avcodec_get_context_defaults(self->encoder.context);
    }
#endif

    if(!self->encoder.context) {
        TSK_DEBUG_ERROR("Failed to allocate context");
        return -1;
    }

#if TDAV_UNDER_X86 && LIBAVCODEC_VERSION_MAJOR <= 53
    self->encoder.context->dsp_mask = (FF_MM_MMX | FF_MM_MMXEXT | FF_MM_SSE);
#endif

    self->encoder.context->pix_fmt		= PIX_FMT_YUV420P;
    self->encoder.context->time_base.num  = 1;
    self->encoder.context->time_base.den  = TMEDIA_CODEC_VIDEO(self)->out.fps;
    self->encoder.context->width = (self->encoder.rotation == 90 || self->encoder.rotation == 270) ? TMEDIA_CODEC_VIDEO(self)->out.height : TMEDIA_CODEC_VIDEO(self)->out.width;
    self->encoder.context->height = (self->encoder.rotation == 90 || self->encoder.rotation == 270) ? TMEDIA_CODEC_VIDEO(self)->out.width : TMEDIA_CODEC_VIDEO(self)->out.height;
    self->encoder.max_bw_kpbs = TSK_CLAMP(
                                    0,
                                    tmedia_get_video_bandwidth_kbps_2(TMEDIA_CODEC_VIDEO(self)->out.width, TMEDIA_CODEC_VIDEO(self)->out.height, TMEDIA_CODEC_VIDEO(self)->out.fps),
                                    TMEDIA_CODEC(self)->bandwidth_max_upload
                                );
    self->encoder.context->bit_rate = (self->encoder.max_bw_kpbs * 1024);// bps

    self->encoder.context->rc_min_rate = (self->encoder.context->bit_rate >> 3);
    self->encoder.context->rc_max_rate = self->encoder.context->bit_rate;

#if LIBAVCODEC_VERSION_MAJOR <= 53
    self->encoder.context->rc_lookahead = 0;
#endif
    self->encoder.context->global_quality = FF_QP2LAMBDA * self->encoder.quality;

#if LIBAVCODEC_VERSION_MAJOR <= 53
    self->encoder.context->partitions = X264_PART_I4X4 | X264_PART_I8X8 | X264_PART_P8X8 | X264_PART_B8X8;
#endif
    self->encoder.context->me_method = ME_UMH;
    self->encoder.context->me_range = 16;
    self->encoder.context->qmin = 10;
    self->encoder.context->qmax = 51;
#if LIBAVCODEC_VERSION_MAJOR <= 53
    self->encoder.context->mb_qmin = self->encoder.context->qmin;
    self->encoder.context->mb_qmax = self->encoder.context->qmax;
#endif
    /* METROPOLIS = G2J.COM TelePresence client. Check Issue 378: No video when calling "TANDBERG/4129 (X8.1.1)" */
#if !METROPOLIS  && 0
    self->encoder.context->flags |= CODEC_FLAG_GLOBAL_HEADER;
#endif
    self->encoder.context->flags |= CODEC_FLAG_LOW_DELAY;
    if (self->encoder.context->profile == FF_PROFILE_H264_BASELINE) {
        self->encoder.context->max_b_frames = 0;
    }

    switch(TDAV_CODEC_H264_COMMON(self)->profile) {
    case profile_idc_baseline:
    default:
        self->encoder.context->profile = FF_PROFILE_H264_BASELINE;
        self->encoder.context->level = TDAV_CODEC_H264_COMMON(self)->level;
        break;
    case profile_idc_main:
        self->encoder.context->profile = FF_PROFILE_H264_MAIN;
        self->encoder.context->level = TDAV_CODEC_H264_COMMON(self)->level;
        break;
    }

    /* Comment from libavcodec/libx264.c:
     * Allow x264 to be instructed through AVCodecContext about the maximum
     * size of the RTP payload. For example, this enables the production of
     * payload suitable for the H.264 RTP packetization-mode 0 i.e. single
     * NAL unit per RTP packet.
     */
    self->encoder.context->rtp_payload_size = H264_RTP_PAYLOAD_SIZE;
    self->encoder.context->opaque = tsk_null;
    self->encoder.context->gop_size = (TMEDIA_CODEC_VIDEO(self)->out.fps * TDAV_H264_GOP_SIZE_IN_SECONDS);

#if (LIBAVUTIL_VERSION_INT >= AV_VERSION_INT(51, 35, 0))
    if((ret = av_opt_set_int(self->encoder.context->priv_data, "slice-max-size", H264_RTP_PAYLOAD_SIZE, 0))) {
        TSK_DEBUG_ERROR("Failed to set x264 slice-max-size to %d", H264_RTP_PAYLOAD_SIZE);
    }
    if((ret = av_opt_set(self->encoder.context->priv_data, "profile", (self->encoder.context->profile == FF_PROFILE_H264_BASELINE ? "baseline" : "main"), 0))) {
        TSK_DEBUG_ERROR("Failed to set x264 profile");
    }
    if((ret = av_opt_set(self->encoder.context->priv_data, "preset", "veryfast", 0))) {
        TSK_DEBUG_ERROR("Failed to set x264 preset to veryfast");
    }
    if((ret = av_opt_set_int(self->encoder.context->priv_data, "rc-lookahead", 0, 0)) && (ret = av_opt_set_int(self->encoder.context->priv_data, "rc_lookahead", 0, 0))) {
        TSK_DEBUG_ERROR("Failed to set x264 rc_lookahead=0");
    }
    if((ret = av_opt_set(self->encoder.context->priv_data, "tune", "animation+zerolatency", 0))) {
        TSK_DEBUG_ERROR("Failed to set x264 tune to zerolatency");
    }
#endif

    // Picture (YUV 420)
    if(!(self->encoder.picture = avcodec_alloc_frame())) {
        TSK_DEBUG_ERROR("Failed to create encoder picture");
        return -2;
    }
    avcodec_get_frame_defaults(self->encoder.picture);


    size = avpicture_get_size(PIX_FMT_YUV420P, self->encoder.context->width, self->encoder.context->height);
    if(!(self->encoder.buffer = tsk_calloc(size, sizeof(uint8_t)))) {
        TSK_DEBUG_ERROR("Failed to allocate encoder buffer");
        return -2;
    }

    // Open encoder
    if((ret = avcodec_open(self->encoder.context, self->encoder.codec)) < 0) {
        TSK_DEBUG_ERROR("Failed to open [%s] codec", TMEDIA_CODEC(self)->plugin->desc);
        return ret;
    }

    self->encoder.frame_count = 0;

    TSK_DEBUG_INFO("[H.264] bitrate=%d bps", self->encoder.context->bit_rate);

    return ret;
#elif HAVE_H264_PASSTHROUGH
    self->encoder.frame_count = 0;
    return 0;
#endif

    TSK_DEBUG_ERROR("Not expected code called");
    return -1;
}

int tdav_codec_h264_close_encoder(tdav_codec_h264_t* self, tsk_bool_t reset_rotation)
{
#if HAVE_FFMPEG
    if(self->encoder.context) {
        avcodec_close(self->encoder.context);
        av_free(self->encoder.context);
        self->encoder.context = tsk_null;
    }
    if(self->encoder.picture) {
        av_free(self->encoder.picture);
        self->encoder.picture = tsk_null;
    }
#endif
    if(self->encoder.buffer) {
        TSK_FREE(self->encoder.buffer);
    }
    self->encoder.frame_count = 0;
    if (reset_rotation) {
        self->encoder.rotation = 0; // reset rotation
    }

    return 0;
}

int tdav_codec_h264_open_decoder(tdav_codec_h264_t* self)
{
#if HAVE_FFMPEG
    int ret;

    if(self->decoder.context) {
        TSK_DEBUG_ERROR("Decoder already opened");
        return -1;
    }

    self->decoder.context = avcodec_alloc_context();
    avcodec_get_context_defaults(self->decoder.context);

    self->decoder.context->pix_fmt = PIX_FMT_YUV420P;
    self->decoder.context->flags2 |= CODEC_FLAG2_FAST;
    self->decoder.context->width = TMEDIA_CODEC_VIDEO(self)->in.width;
    self->decoder.context->height = TMEDIA_CODEC_VIDEO(self)->in.height;

    // Picture (YUV 420)
    if(!(self->decoder.picture = avcodec_alloc_frame())) {
        TSK_DEBUG_ERROR("Failed to create decoder picture");
        return -2;
    }
    avcodec_get_frame_defaults(self->decoder.picture);

    // Open decoder
    if((ret = avcodec_open(self->decoder.context, self->decoder.codec)) < 0) {
        TSK_DEBUG_ERROR("Failed to open [%s] codec", TMEDIA_CODEC(self)->plugin->desc);
        return ret;
    }
    self->decoder.last_seq = 0;

    return ret;

#elif HAVE_H264_PASSTHROUGH
    return 0;
#endif

    TSK_DEBUG_ERROR("Unexpected code called");
    return -1;

}

int tdav_codec_h264_close_decoder(tdav_codec_h264_t* self)
{
#if HAVE_FFMPEG
    if(self->decoder.context) {
        avcodec_close(self->decoder.context);
        av_free(self->decoder.context);
        self->decoder.context = tsk_null;
    }
    if(self->decoder.picture) {
        av_free(self->decoder.picture);
        self->decoder.picture = tsk_null;
    }
#endif
    TSK_FREE(self->decoder.accumulator);
    self->decoder.accumulator_pos = 0;

    return 0;
}

int tdav_codec_h264_init(tdav_codec_h264_t* self, profile_idc_t profile)
{
    int ret = 0;
    level_idc_t level;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if((ret = tdav_codec_h264_common_init(TDAV_CODEC_H264_COMMON(self)))) {
        TSK_DEBUG_ERROR("tdav_codec_h264_common_init() faile with error code=%d", ret);
        return ret;
    }

    if((ret = tdav_codec_h264_common_level_from_size(TMEDIA_CODEC_VIDEO(self)->out.width, TMEDIA_CODEC_VIDEO(self)->out.height, &level))) {
        TSK_DEBUG_ERROR("Failed to find level for size=[%u, %u]", TMEDIA_CODEC_VIDEO(self)->out.width, TMEDIA_CODEC_VIDEO(self)->out.height);
        return ret;
    }

    (self)->encoder.max_bw_kpbs = TMEDIA_CODEC(self)->bandwidth_max_upload;
    TDAV_CODEC_H264_COMMON(self)->pack_mode_local = H264_PACKETIZATION_MODE;
    TDAV_CODEC_H264_COMMON(self)->profile = profile;
    TDAV_CODEC_H264_COMMON(self)->level = level;
    TMEDIA_CODEC_VIDEO(self)->in.max_mbps = TMEDIA_CODEC_VIDEO(self)->out.max_mbps = H264_MAX_MBPS*1000;
    TMEDIA_CODEC_VIDEO(self)->in.max_br = TMEDIA_CODEC_VIDEO(self)->out.max_br = H264_MAX_BR*1000;

#if HAVE_FFMPEG
    if(!(self->encoder.codec = avcodec_find_encoder(CODEC_ID_H264))) {
        TSK_DEBUG_ERROR("Failed to find H.264 encoder");
        ret = -2;
    }

    if(!(self->decoder.codec = avcodec_find_decoder(CODEC_ID_H264))) {
        TSK_DEBUG_ERROR("Failed to find H.264 decoder");
        ret = -3;
    }
#endif
#if HAVE_H264_PASSTHROUGH
    TMEDIA_CODEC(self)->passthrough = tsk_true;
    self->decoder.passthrough = tsk_true;
    self->encoder.passthrough = tsk_true;
#endif

    self->encoder.quality = 1;

    /* allocations MUST be done by open() */
    return ret;
}

int tdav_codec_h264_deinit(tdav_codec_h264_t* self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

#if HAVE_FFMPEG
    self->encoder.codec = tsk_null;
    self->decoder.codec = tsk_null;

    // FFMpeg resources are destroyed by close()
#endif

    return 0;
}

tsk_bool_t tdav_codec_ffmpeg_h264_is_supported()
{
#if HAVE_FFMPEG
    return (avcodec_find_encoder(CODEC_ID_H264) && avcodec_find_decoder(CODEC_ID_H264));
#else
    return tsk_false;
#endif
}

tsk_bool_t tdav_codec_passthrough_h264_is_supported()
{
#if HAVE_H264_PASSTHROUGH
    return tsk_true;
#else
    return tsk_false;
#endif
}

#endif /* HAVE_FFMPEG || HAVE_H264_PASSTHROUGH */
