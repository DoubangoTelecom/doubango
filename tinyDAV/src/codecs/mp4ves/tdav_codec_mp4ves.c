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

/**@file tdav_codec_mp4ves.c
 * @brief MP4V-ES codec plugin
 * RTP payloader/depayloader follows RFC 3016.
 * ISO-IEC-14496-2: http://www.csus.edu/indiv/p/pangj/aresearch/video_compression/presentation/ISO-IEC-14496-2_2001_MPEG4_Visual.pdf
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Th Dec 2 16:54:58 2010 mdiop
 */
#include "tinydav/codecs/mp4ves/tdav_codec_mp4ves.h"

#if HAVE_FFMPEG

#include "tinydav/video/tdav_converter_video.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tnet_endianness.h"

#include "tinymedia/tmedia_params.h"
#include "tinymedia/tmedia_defaults.h"

#include "tsk_string.h"
#include "tsk_params.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <libavcodec/avcodec.h>

#define DEFAULT_PROFILE_LEVEL_ID	Simple_Profile_Level_1

#define MP4V_GOP_SIZE_IN_SECONDS		25
#define MP4V_RTP_PAYLOAD_SIZE		900

typedef struct tdav_codec_mp4ves_s {
    TMEDIA_DECLARE_CODEC_VIDEO;

    int profile;

    struct {
        uint8_t* ptr;
        tsk_size_t size;
    } rtp;

    // Encoder
    struct {
        AVCodec* codec;
        AVCodecContext* context;
        AVFrame* picture;
        void* buffer;
        tsk_bool_t force_idr;
        int quality; // [1-31]
        int rotation;
        int32_t max_bw_kpbs;
    } encoder;

    // decoder
    struct {
        AVCodec* codec;
        AVCodecContext* context;
        AVFrame* picture;

        void* accumulator;
        uint8_t ebit;
        tsk_size_t accumulator_pos;
        uint16_t last_seq;
    } decoder;
}
tdav_codec_mp4ves_t;

// From ISO-IEC-14496-2
typedef enum mp4v_codes_e {
    // To initiate a visual session (6.3.2)
    visual_object_sequence_start_code = 0x000001B0,
    // To terminate a visual session (6.3.2)
    visual_object_sequence_end_code = 0x000001B1,
    // To initiate a visual object (6.3.2)
    visual_object_start_code = 0x000001B5,
    // To identify the beginning of user data. The user data continues until receipt of another start code. (6.3.2.1)
    user_data_start_code = 0x000001B2,
    // The video_object_layer_start_code is a string of 32 bits. The first 28 bits are
    // ‘0000 0000 0000 0000 0000 0001 0010‘ in binary and the last 4-bits represent one of the values in the range of
    // ‘0000’ to ‘1111’ in binary. The video_object_layer_start_code marks a new video object layer. (6.3.3)
    video_object_layer_start_code = 0x0000012,
    // To identify the beginning of a GOV header (6.3.4)
    group_of_vop_start_code = 0x000001B3,
    // To mark the start of a video object plane (6.3.5 )
    vop_start_code = 0x000001B6,
}
mp4v_start_code_t;

// From ISO-IEC-14496-2 Annex G
typedef enum mp4v_profiles_e {
    /* Reserved = 0x00000000 */
    Simple_Profile_Level_1 = 1,
    Simple_Profile_Level_2 = 2,
    Simple_Profile_Level_3 = 3,
    /* Reserved 00000100 ? 00010000 */
    Simple_Scalable_Profile_Level_1 = 17,
    Simple_Scalable_Profile_Level_2 = 18,
    /* Reserved 00010011 ? = 0x00100000 */
    Core_Profile_Level_1 = 33,
    Core_Profile_Level_2 = 34,
    /* Reserved 00100011 ? = 0x00110001 */
    Main_Profile_Level_2 = 50,
    Main_Profile_Level_3 = 51,
    Main_Profile_Level_4 = 52,
    /* Reserved 00110101 ? = 0x01000001 */
    N_bit_Profile_Level_2 = 66,
    /* Reserved 01000011 ? = 0x01010000 */
    Scalable_Texture_Profile_Level_1 = 81,
    /* Reserved 01010010 ? 01100000 */
    Simple_Face_Animation_Profile_Level_1 = 97,
    Simple_Face_Animation_Profile_Level_2 = 98,
    Simple_FBA_Profile_Level_1 = 99,
    Simple_FBA_Profile_Level_2 = 100,
    /* Reserved 01100101 ? 01110000 */
    Basic_Animated_Texture_Profile_Level_1 = 113,
    Basic_Animated_Texture_Profile_Level_2 = 114,
    /* Reserved 01110011 ? 10000000 */
    Hybrid_Profile_Level_1 = 129,
    Hybrid_Profile_Level_2 = 130,
    /* Reserved 10000011 ? 10010000 */
    Advanced_Real_Time_Simple_Profile_Level_1 = 145,
    Advanced_Real_Time_Simple_Profile_Level_2 = 146,
    Advanced_Real_Time_Simple_Profile_Level_3 = 147,
    Advanced_Real_Time_Simple_Profile_Level_4 = 148,
    /* Reserved 10010101 ? 10100000 */
}
mp4v_profiles_t;

static int tdav_codec_mp4ves_open_encoder(tdav_codec_mp4ves_t* self);
static int tdav_codec_mp4ves_open_decoder(tdav_codec_mp4ves_t* self);
static int tdav_codec_mp4ves_close_encoder(tdav_codec_mp4ves_t* self);
static int tdav_codec_mp4ves_close_decoder(tdav_codec_mp4ves_t* self);

static void tdav_codec_mp4ves_encap(tdav_codec_mp4ves_t* mp4v, const uint8_t* pdata, tsk_size_t size);
static void tdav_codec_mp4ves_rtp_callback(tdav_codec_mp4ves_t *mp4v, const void *data, tsk_size_t size, tsk_bool_t marker);

/* ============ MP4V-ES Plugin interface functions ================= */

static int tdav_codec_mp4ves_set(tmedia_codec_t* self, const tmedia_param_t* param)
{
    tdav_codec_mp4ves_t* mp4ves = (tdav_codec_mp4ves_t*)self;
    if(!self->opened) {
        TSK_DEBUG_ERROR("Codec not opened");
        return -1;
    }
    if(param->value_type == tmedia_pvt_int32) {
        if(tsk_striequals(param->key, "action")) {
            tmedia_codec_action_t action = (tmedia_codec_action_t)TSK_TO_INT32((uint8_t*)param->value);
            switch(action) {
            case tmedia_codec_action_encode_idr: {
                mp4ves->encoder.force_idr = tsk_true;
                break;
            }
            case tmedia_codec_action_bw_down: {
                mp4ves->encoder.quality = TSK_CLAMP(1, (mp4ves->encoder.quality + 1), 31);
                mp4ves->encoder.context->global_quality = FF_QP2LAMBDA * mp4ves->encoder.quality;
                break;
            }
            case tmedia_codec_action_bw_up: {
                mp4ves->encoder.quality = TSK_CLAMP(1, (mp4ves->encoder.quality - 1), 31);
                mp4ves->encoder.context->global_quality = FF_QP2LAMBDA * mp4ves->encoder.quality;
                break;
            }
            }
        }
        else if(tsk_striequals(param->key, "rotation")) {
            int rotation = *((int32_t*)param->value);
            if(mp4ves->encoder.rotation != rotation) {
                if(self->opened) {
                    int ret;
                    mp4ves->encoder.rotation = rotation;
                    if((ret = tdav_codec_mp4ves_close_encoder(mp4ves))) {
                        return ret;
                    }
                    if((ret = tdav_codec_mp4ves_open_encoder(mp4ves))) {
                        return ret;
                    }
                }
            }
            return 0;
        }
    }
    return -1;
}

int tdav_codec_mp4ves_open(tmedia_codec_t* self)
{
    int ret;

    tdav_codec_mp4ves_t* mp4v = (tdav_codec_mp4ves_t*)self;

    if(!mp4v) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* the caller (base class) already checked that the codec is not opened */


    //	Encoder
    if((ret = tdav_codec_mp4ves_open_encoder(mp4v))) {
        return ret;
    }

    //	Decoder
    if((ret = tdav_codec_mp4ves_open_decoder(mp4v))) {
        return ret;
    }

    return 0;
}

int tdav_codec_mp4ves_close(tmedia_codec_t* self)
{
    tdav_codec_mp4ves_t* mp4v = (tdav_codec_mp4ves_t*)self;

    if(!mp4v) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* the caller (base class) already checked that the codec is opened */

    //	Encoder
    tdav_codec_mp4ves_close_encoder(mp4v);

    //	Decoder
    tdav_codec_mp4ves_close_decoder(mp4v);

    return 0;
}

tsk_size_t tdav_codec_mp4ves_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
    int ret;
    int size;

    tdav_codec_mp4ves_t* mp4v = (tdav_codec_mp4ves_t*)self;

    if(!self || !in_data || !in_size || !out_data) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    // wrap yuv420 buffer
    size = avpicture_fill((AVPicture *)mp4v->encoder.picture, (uint8_t*)in_data, PIX_FMT_YUV420P, mp4v->encoder.context->width, mp4v->encoder.context->height);
    if(size != in_size) {
        /* guard */
        TSK_DEBUG_ERROR("Invalid size");
        return 0;
    }

    if(mp4v->encoder.force_idr) {
#if LIBAVCODEC_VERSION_MAJOR <= 53
        mp4v->encoder.picture->pict_type = FF_I_TYPE;
#else
        mp4v->encoder.picture->pict_type = AV_PICTURE_TYPE_I;
#endif
        mp4v->encoder.force_idr = tsk_false;
    }
    else {
        mp4v->encoder.picture->pict_type = 0;// reset
    }
    mp4v->encoder.picture->pts = AV_NOPTS_VALUE;
    mp4v->encoder.picture->quality = mp4v->encoder.context->global_quality;
    ret = avcodec_encode_video(mp4v->encoder.context, mp4v->encoder.buffer, size, mp4v->encoder.picture);
    if(ret > 0) {
        tdav_codec_mp4ves_encap(mp4v, mp4v->encoder.buffer, (tsk_size_t)ret);
    }

    return 0;
}

tsk_size_t tdav_codec_mp4ves_decode(tmedia_codec_t* _self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
    tdav_codec_mp4ves_t* self = (tdav_codec_mp4ves_t*)_self;
    const trtp_rtp_header_t* rtp_hdr = proto_hdr;

    tsk_size_t xsize, retsize = 0;
    int got_picture_ptr;
    int ret;

    if(!self || !in_data || !in_size || !out_data || !self->decoder.context) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    // get expected size
    xsize = avpicture_get_size(self->decoder.context->pix_fmt, self->decoder.context->width, self->decoder.context->height);

    /* Packet lost? */
    if(self->decoder.last_seq != (rtp_hdr->seq_num - 1) && self->decoder.last_seq) {
        if(self->decoder.last_seq == rtp_hdr->seq_num) {
            // Could happen on some stupid emulators
            TSK_DEBUG_INFO("Packet duplicated, seq_num=%d", rtp_hdr->seq_num);
            return 0;
        }
        TSK_DEBUG_INFO("Packet lost, seq_num=%d", rtp_hdr->seq_num);
    }
    self->decoder.last_seq = rtp_hdr->seq_num;

    if((self->decoder.accumulator_pos + in_size) <= xsize) {
        memcpy(&((uint8_t*)self->decoder.accumulator)[self->decoder.accumulator_pos], in_data, in_size);
        self->decoder.accumulator_pos += in_size;
    }
    else {
        TSK_DEBUG_WARN("Buffer overflow");
        self->decoder.accumulator_pos = 0;
        return 0;
    }

    if(rtp_hdr->marker) {
        AVPacket packet;
        /* allocate destination buffer */
        if(*out_max_size <xsize) {
            if(!(*out_data = tsk_realloc(*out_data, xsize))) {
                TSK_DEBUG_ERROR("Failed to allocate new buffer");
                self->decoder.accumulator_pos = 0;
                *out_max_size = 0;
                return 0;
            }
            *out_max_size = xsize;
        }

        av_init_packet(&packet);
        packet.size = (int)self->decoder.accumulator_pos;
        packet.data = self->decoder.accumulator;
        ret = avcodec_decode_video2(self->decoder.context, self->decoder.picture, &got_picture_ptr, &packet);

        if(ret < 0) {
            TSK_DEBUG_WARN("Failed to decode the buffer with error code = %d", ret);
            if(TMEDIA_CODEC_VIDEO(self)->in.callback) {
                TMEDIA_CODEC_VIDEO(self)->in.result.type = tmedia_video_decode_result_type_error;
                TMEDIA_CODEC_VIDEO(self)->in.result.proto_hdr = proto_hdr;
                TMEDIA_CODEC_VIDEO(self)->in.callback(&TMEDIA_CODEC_VIDEO(self)->in.result);
            }
        }
        else if(got_picture_ptr) {
            retsize = xsize;
            TMEDIA_CODEC_VIDEO(self)->in.width = self->decoder.context->width;
            TMEDIA_CODEC_VIDEO(self)->in.height = self->decoder.context->height;

            /* copy picture into a linear buffer */
            avpicture_layout((AVPicture *)self->decoder.picture, self->decoder.context->pix_fmt, (int)self->decoder.context->width, (int)self->decoder.context->height,
                             *out_data, (int)retsize);
        }
        /* in all cases: reset accumulator */
        self->decoder.accumulator_pos = 0;
    }

    return retsize;
}

tsk_bool_t tdav_codec_mp4ves_sdp_att_match(const tmedia_codec_t* _self, const char* att_name, const char* att_value)
{
    tdav_codec_mp4ves_t *self = (tdav_codec_mp4ves_t *)_self;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_false;
    }

    if(tsk_striequals(att_name, "fmtp")) {
        tsk_params_L_t* params ;
        /* e.g. profile-level-id=1; xx=yy */
        if((params = tsk_params_fromstring(att_value, ";", tsk_true))) {
            int val_int;
            if((val_int = tsk_params_get_param_value_as_int(params, "profile-level-id")) != -1) {
                TSK_DEBUG_INFO("Proposed profile-level-id=%d", val_int);
                self->profile = val_int; // FIXME: Take the remote profile-level-id even if the bandwidth level doesn't match
            }
            TSK_OBJECT_SAFE_FREE(params);
        }

        switch (self->profile ) {
        case Simple_Profile_Level_1:
            TMEDIA_CODEC_VIDEO(self)->out.width = TMEDIA_CODEC_VIDEO(self)->in.width = 176;
            TMEDIA_CODEC_VIDEO(self)->in.height = TMEDIA_CODEC_VIDEO(self)->out.height = 144;
            break;
        case Simple_Profile_Level_2:
        case Simple_Profile_Level_3:
        default:
            TMEDIA_CODEC_VIDEO(self)->out.width = TMEDIA_CODEC_VIDEO(self)->in.width = 352;
            TMEDIA_CODEC_VIDEO(self)->in.height = TMEDIA_CODEC_VIDEO(self)->out.height = 288;
            break;
        }
    }
    else if(tsk_striequals(att_name, "imageattr")) {
        unsigned in_width, in_height, out_width, out_height;
        if(tmedia_parse_video_imageattr(att_value, TMEDIA_CODEC_VIDEO(self)->pref_size, &in_width, &in_height, &out_width, &out_height) != 0) {
            return tsk_false;
        }
        TMEDIA_CODEC_VIDEO(self)->in.width = in_width;
        TMEDIA_CODEC_VIDEO(self)->in.height = in_height;
        TMEDIA_CODEC_VIDEO(self)->out.width = out_width;
        TMEDIA_CODEC_VIDEO(self)->out.height = out_height;
    }

    return tsk_true;
}

char* tdav_codec_mp4ves_sdp_att_get(const tmedia_codec_t* _self, const char* att_name)
{
    tdav_codec_mp4ves_t *self = (tdav_codec_mp4ves_t *)_self;

    if(tsk_striequals(att_name, "fmtp")) {
        char* fmtp = tsk_null;
        switch(_self->bl) { //FIXME: deprecated
        case tmedia_bl_low:
        default:
            self->profile = Simple_Profile_Level_1;
            break;
        case tmedia_bl_medium:
            self->profile = Simple_Profile_Level_2;
            break;
        case tmedia_bl_hight:
        case tmedia_bl_unrestricted:
            self->profile = Simple_Profile_Level_3;
            break;
        }
        tsk_sprintf(&fmtp, "profile-level-id=%d", self->profile);
        return fmtp;
    }
    else if(tsk_striequals(att_name, "imageattr")) {
        return tmedia_get_video_imageattr(TMEDIA_CODEC_VIDEO(self)->pref_size,
                                          TMEDIA_CODEC_VIDEO(self)->in.width, TMEDIA_CODEC_VIDEO(self)->in.height, TMEDIA_CODEC_VIDEO(self)->out.width, TMEDIA_CODEC_VIDEO(self)->out.height);
    }
    return tsk_null;
}


/* ============ Internal functions ================= */
int tdav_codec_mp4ves_open_encoder(tdav_codec_mp4ves_t* self)
{
    int ret, size;
    int32_t max_bw_kpbs;
    if(!self->encoder.codec && !(self->encoder.codec = avcodec_find_encoder(CODEC_ID_MPEG4))) {
        TSK_DEBUG_ERROR("Failed to find mp4v encoder");
        return -1;
    }

    if(self->encoder.context) {
        TSK_DEBUG_ERROR("Encoder already opened");
        return -1;
    }
    self->encoder.context = avcodec_alloc_context();
    avcodec_get_context_defaults(self->encoder.context);

    self->encoder.context->pix_fmt		= PIX_FMT_YUV420P;
    self->encoder.context->time_base.num  = 1;
    self->encoder.context->time_base.den  = TMEDIA_CODEC_VIDEO(self)->in.fps;
    self->encoder.context->width = (self->encoder.rotation == 90 || self->encoder.rotation == 270) ? TMEDIA_CODEC_VIDEO(self)->out.height : TMEDIA_CODEC_VIDEO(self)->out.width;
    self->encoder.context->height = (self->encoder.rotation == 90 || self->encoder.rotation == 270) ? TMEDIA_CODEC_VIDEO(self)->out.width : TMEDIA_CODEC_VIDEO(self)->out.height;
    self->encoder.context->mb_decision = FF_MB_DECISION_RD;
    self->encoder.context->noise_reduction = 250;
    self->encoder.context->flags |= CODEC_FLAG_QSCALE;
    self->encoder.context->global_quality = FF_QP2LAMBDA * self->encoder.quality;

    max_bw_kpbs = TSK_CLAMP(
                      0,
                      tmedia_get_video_bandwidth_kbps_2(TMEDIA_CODEC_VIDEO(self)->out.width, TMEDIA_CODEC_VIDEO(self)->out.height, TMEDIA_CODEC_VIDEO(self)->out.fps),
                      self->encoder.max_bw_kpbs
                  );
    self->encoder.context->bit_rate = (max_bw_kpbs * 1024);// bps
    self->encoder.context->rtp_payload_size = MP4V_RTP_PAYLOAD_SIZE;
    self->encoder.context->opaque = tsk_null;
    self->encoder.context->profile = self->profile>>4;
    self->encoder.context->level = self->profile & 0x0F;
    self->encoder.context->gop_size = (TMEDIA_CODEC_VIDEO(self)->in.fps * MP4V_GOP_SIZE_IN_SECONDS);
    self->encoder.context->max_b_frames = 0;
    self->encoder.context->b_frame_strategy = 1;
    self->encoder.context->flags |= CODEC_FLAG_AC_PRED;

    // Picture (YUV 420)
    if(!(self->encoder.picture = avcodec_alloc_frame())) {
        TSK_DEBUG_ERROR("Failed to create MP4V-ES encoder picture");
        return -2;
    }
    avcodec_get_frame_defaults(self->encoder.picture);

    size = avpicture_get_size(PIX_FMT_YUV420P, self->encoder.context->width, self->encoder.context->height);
    if(!(self->encoder.buffer = tsk_calloc(size, sizeof(uint8_t)))) {
        TSK_DEBUG_ERROR("Failed to allocate MP4V-ES encoder buffer");
        return -2;
    }

    // Open encoder
    if((ret = avcodec_open(self->encoder.context, self->encoder.codec)) < 0) {
        TSK_DEBUG_ERROR("Failed to open MP4V-ES encoder");
        return ret;
    }

    TSK_DEBUG_INFO("[MP4V-ES] bitrate=%d bps", self->encoder.context->bit_rate);

    return ret;
}

int tdav_codec_mp4ves_open_decoder(tdav_codec_mp4ves_t* self)
{
    int ret, size;

    if(!self->decoder.codec  && !(self->decoder.codec = avcodec_find_decoder(CODEC_ID_MPEG4))) {
        TSK_DEBUG_ERROR("Failed to find MP4V-ES decoder");
        return -1;
    }

    if(self->decoder.context) {
        TSK_DEBUG_ERROR("Decoder already opened");
        return -1;
    }

    self->decoder.context = avcodec_alloc_context();
    avcodec_get_context_defaults(self->decoder.context);

    self->decoder.context->pix_fmt = PIX_FMT_YUV420P;
    self->decoder.context->width = TMEDIA_CODEC_VIDEO(self)->out.width;
    self->decoder.context->height = TMEDIA_CODEC_VIDEO(self)->out.height;

    // Picture (YUV 420)
    if(!(self->decoder.picture = avcodec_alloc_frame())) {
        TSK_DEBUG_ERROR("Failed to create decoder picture");
        return -2;
    }
    avcodec_get_frame_defaults(self->decoder.picture);

    size = avpicture_get_size(PIX_FMT_YUV420P, self->decoder.context->width, self->decoder.context->height);
    if(!(self->decoder.accumulator = tsk_calloc((size + FF_INPUT_BUFFER_PADDING_SIZE), sizeof(uint8_t)))) {
        TSK_DEBUG_ERROR("Failed to allocate decoder buffer");
        return -2;
    }

    if(!(self->decoder.accumulator = tsk_calloc((size + FF_INPUT_BUFFER_PADDING_SIZE), sizeof(uint8_t)))) {
        TSK_DEBUG_ERROR("Failed to allocate decoder buffer");
        return -2;
    }

    // Open decoder
    if((ret = avcodec_open(self->decoder.context, self->decoder.codec)) < 0) {
        TSK_DEBUG_ERROR("Failed to open MP4V-ES decoder");
        return ret;
    }

    self->decoder.last_seq = 0;

    return ret;
}

int tdav_codec_mp4ves_close_encoder(tdav_codec_mp4ves_t* self)
{
    if(self->encoder.context) {
        avcodec_close(self->encoder.context);
        av_free(self->encoder.context);
        self->encoder.context = tsk_null;
    }
    if(self->encoder.picture) {
        av_free(self->encoder.picture);
    }
    if(self->encoder.buffer) {
        TSK_FREE(self->encoder.buffer);
    }
    return 0;
}

int tdav_codec_mp4ves_close_decoder(tdav_codec_mp4ves_t* self)
{
    if(self->decoder.context) {
        avcodec_close(self->decoder.context);
        if(self->decoder.context->extradata) {
            TSK_FREE(self->decoder.context->extradata);
            self->decoder.context->extradata_size = 0;
        }
        av_free(self->decoder.context);
        self->decoder.context = tsk_null;
    }
    if(self->decoder.picture) {
        av_free(self->decoder.picture);
        self->decoder.picture = tsk_null;
    }
    if(self->decoder.accumulator) {
        TSK_FREE(self->decoder.accumulator);
    }

    return 0;
}

static void tdav_codec_mp4ves_encap(tdav_codec_mp4ves_t* mp4v, const uint8_t* pdata, tsk_size_t size)
{
    uint32_t scode; // start code

    if(size <= 4/*32bits: start code size*/) {
        TSK_DEBUG_ERROR("Too short");
        return;
    }
    // first 32bits
    scode = tnet_htonl_2(pdata);

    /* RFC 3016 - 3.3 Examples of packetized MPEG-4 Visual bitstream

    	VS= Visual Object Sequence
    	VO= Visual Object
    	VOL= Visual Object Layer
    	VOP= Visual Object Plane
    	GOV= Group of Visual Object Plane
    	VP= Video Plane

    	 +------+------+------+------+
    (a) | RTP  |  VS  |  VO  | VOL  |
        |header|header|header|header|
        +------+------+------+------+

        +------+------+------+------+------------+
    (b) | RTP  |  VS  |  VO  | VOL  |Video Packet|
        |header|header|header|header|            |
        +------+------+------+------+------------+

        +------+-----+------------------+
    (c) | RTP  | GOV |Video Object Plane|
        |header|     |                  |
        +------+-----+------------------+

        +------+------+------------+  +------+------+------------+
    (d) | RTP  | VOP  |Video Packet|  | RTP  |  VP  |Video Packet|
        |header|header|    (1)     |  |header|header|    (2)     |
        +------+------+------------+  +------+------+------------+

        +------+------+------------+------+------------+------+------------+
    (e) | RTP  |  VP  |Video Packet|  VP  |Video Packet|  VP  |Video Packet|
        |header|header|     (1)    |header|    (2)     |header|    (3)     |
        +------+------+------------+------+------------+------+------------+

        +------+------+------------+  +------+------------+
    (f) | RTP  | VOP  |VOP fragment|  | RTP  |VOP fragment|
        |header|header|    (1)     |  |header|    (2)     | ___
        +------+------+------------+  +------+------------+

         Figure 2 - Examples of RTP packetized MPEG-4 Visual bitstream
    */

    /* RFC 3016 - 3.2 Fragmentation of MPEG-4 Visual bitstream

       A fragmented MPEG-4 Visual bitstream is mapped directly onto the RTP
       payload without any addition of extra header fields or any removal of
       Visual syntax elements.  The Combined Configuration/Elementary
       streams mode is used.

       In the following, header means one of the following:

       -  Configuration information (Visual Object Sequence Header, Visual
          Object Header and Video Object Layer Header)
       -  visual_object_sequence_end_code
       -  The header of the entry point function for an elementary stream
          (Group_of_VideoObjectPlane() or the header of VideoObjectPlane(),
          video_plane_with_short_header(), MeshObject() or FaceObject())
       -  The video packet header (video_packet_header() excluding
          next_resync_marker())
       -  The header of gob_layer()
          See 6.2.1 "Start codes" of ISO/IEC 14496-2 [2][9][4] for the
          definition of the configuration information and the entry point
          functions.
    */

    switch(scode) {
    case visual_object_sequence_start_code:
    case visual_object_start_code:
    case user_data_start_code:
    case video_object_layer_start_code:
    case group_of_vop_start_code:
    case vop_start_code: {
        register uint32_t i, last_index = 0;
        int startcode = 0xffffffff;

        if(scode == visual_object_sequence_start_code && size >=5) {
            //uint8_t profile_and_level_indication = pdata[4]; /* IEC 14496-2: 6.3.2 Visual Object Sequence and Visual Object */
            // TSK_DEBUG_INFO("profile_and_level_indication=%d", profile_and_level_indication);
        }

        if(size < MP4V_RTP_PAYLOAD_SIZE) {
            goto last;
        }

        for(i = 4; i<(size - 4); i++) {
            startcode = (startcode <<8) | pdata[i];
            switch(startcode) {
            case visual_object_sequence_start_code:
            case group_of_vop_start_code:
            case vop_start_code:
                tdav_codec_mp4ves_rtp_callback(mp4v, pdata + last_index, (i - last_index), (last_index == size));
                last_index = i;
            }
        }
last:
        if(last_index < size) {
            tdav_codec_mp4ves_rtp_callback(mp4v, pdata + last_index, (size - last_index), tsk_true);
        }
        break;
    }
    default:
        TSK_DEBUG_ERROR("%x is an invalide start code", scode);
        break;
    }
}

static void tdav_codec_mp4ves_rtp_callback(tdav_codec_mp4ves_t *mp4v, const void *data, tsk_size_t size, tsk_bool_t marker)
{
    // Send data over the network
    if(TMEDIA_CODEC_VIDEO(mp4v)->out.callback) {
        TMEDIA_CODEC_VIDEO(mp4v)->out.result.buffer.ptr = data;
        TMEDIA_CODEC_VIDEO(mp4v)->out.result.buffer.size = size;
        TMEDIA_CODEC_VIDEO(mp4v)->out.result.duration =  (uint32_t)((1./(double)TMEDIA_CODEC_VIDEO(mp4v)->out.fps) * TMEDIA_CODEC(mp4v)->plugin->rate);
        TMEDIA_CODEC_VIDEO(mp4v)->out.result.last_chunck = marker;
        TMEDIA_CODEC_VIDEO(mp4v)->out.callback(&TMEDIA_CODEC_VIDEO(mp4v)->out.result);
    }
}


/* ============ MP4V-ES Plugin interface ================= */

/* constructor */
static tsk_object_t* tdav_codec_mp4ves_ctor(tsk_object_t * _self, va_list * app)
{
    tdav_codec_mp4ves_t *self = _self;
    if(self) {
        /* init base: called by tmedia_codec_create() */
        /* init self */
        self->profile = DEFAULT_PROFILE_LEVEL_ID;
        self->encoder.quality = 1;
        self->encoder.max_bw_kpbs = tmedia_defaults_get_bandwidth_video_upload_max();
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_codec_mp4ves_dtor(tsk_object_t * _self)
{
    tdav_codec_mp4ves_t *self = _self;
    if(self) {
        /* deinit base */
        tmedia_codec_video_deinit(self); // will close the codec if opened
        /* deinit self */
        TSK_FREE(self->rtp.ptr);
        self->rtp.size = 0;
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_mp4ves_def_s = {
    sizeof(tdav_codec_mp4ves_t),
    tdav_codec_mp4ves_ctor,
    tdav_codec_mp4ves_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_mp4ves_plugin_def_s = {
    &tdav_codec_mp4ves_def_s,

    tmedia_video,
    tmedia_codec_id_mp4ves_es,
    "MP4V-ES",
    "MP4V-ES Codec",
    TMEDIA_CODEC_FORMAT_MP4V_ES,
    tsk_true,
    90000, // rate

    /* audio */
    { 0 },

    /* video (width, height, fps) */
    {176, 144, 0},// fps is @deprecated

    tdav_codec_mp4ves_set,
    tdav_codec_mp4ves_open,
    tdav_codec_mp4ves_close,
    tdav_codec_mp4ves_encode,
    tdav_codec_mp4ves_decode,
    tdav_codec_mp4ves_sdp_att_match,
    tdav_codec_mp4ves_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_mp4ves_plugin_def_t = &tdav_codec_mp4ves_plugin_def_s;

tsk_bool_t tdav_codec_ffmpeg_mp4ves_is_supported()
{
    return (avcodec_find_encoder(CODEC_ID_MPEG4) && avcodec_find_decoder(CODEC_ID_MPEG4));
}

#endif /* HAVE_FFMPEG */

