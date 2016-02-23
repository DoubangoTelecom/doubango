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

/**@file tdav_codec_h263.c
 * @brief H.263-1996 and H.263-1998 codec plugins.
 * RTP payloader follows RFC 4629 for H263+ and RFC 2190 for H263.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinydav/codecs/h263/tdav_codec_h263.h"

#if HAVE_FFMPEG

#include "tinydav/video/tdav_converter_video.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tnet_endianness.h"

#include "tinymedia/tmedia_params.h"
#include "tinymedia/tmedia_defaults.h"

#include "tsk_string.h"
#include "tsk_time.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <libavcodec/avcodec.h>

#define TDAV_H263_GOP_SIZE_IN_SECONDS		25
#define RTP_PAYLOAD_SIZE	750

#define H263P_HEADER_SIZE		2
#define H263_HEADER_MODE_A_SIZE 4
#define H263_HEADER_MODE_B_SIZE 8
#define H263_HEADER_MODE_C_SIZE 12

#define tdav_codec_h263p_set tdav_codec_h263_set
#define tdav_codec_h263p_open tdav_codec_h263_open
#define tdav_codec_h263p_close tdav_codec_h263_close
#define tdav_codec_h263p_encode tdav_codec_h263_encode
#define tdav_codec_h263p_sdp_att_match tdav_codec_h263_sdp_att_match
#define tdav_codec_h263p_sdp_att_get tdav_codec_h263_sdp_att_get

#define tdav_codec_h263pp_set tdav_codec_h263_set
#define tdav_codec_h263pp_open tdav_codec_h263_open
#define tdav_codec_h263pp_close tdav_codec_h263_close
#define tdav_codec_h263pp_encode tdav_codec_h263_encode
#define tdav_codec_h263pp_decode tdav_codec_h263_decode
#define tdav_codec_h263pp_sdp_att_match tdav_codec_h263_sdp_att_match
#define tdav_codec_h263pp_sdp_att_get tdav_codec_h263_sdp_att_get

#define TDAV_CODEC_H263(self) ((tdav_codec_h263_t*)(self))

typedef enum tdav_codec_h263_type_e {
    tdav_codec_h263_1996,
    tdav_codec_h263_1998,
    tdav_codec_h263_2000,
}
tdav_codec_h263_type_t;

/** H.263-1996 codec */
typedef struct tdav_codec_h263_s {
    TMEDIA_DECLARE_CODEC_VIDEO;

    tdav_codec_h263_type_t type;

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
        int32_t quality; // [1-31]
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
tdav_codec_h263_t;

#define TDAV_DECLARE_CODEC_H263 tdav_codec_h263_t __codec_h263__

static int tdav_codec_h263_init(tdav_codec_h263_t* self, tdav_codec_h263_type_t type, enum CodecID encoder, enum CodecID decoder);
static int tdav_codec_h263_deinit(tdav_codec_h263_t* self);
static int tdav_codec_h263_open_encoder(tdav_codec_h263_t* self);
static int tdav_codec_h263_open_decoder(tdav_codec_h263_t* self);
static int tdav_codec_h263_close_encoder(tdav_codec_h263_t* self);
static int tdav_codec_h263_close_decoder(tdav_codec_h263_t* self);

/** H.263-1998 codec */
typedef struct tdav_codec_h263p_s {
    TDAV_DECLARE_CODEC_H263;
}
tdav_codec_h263p_t;

/** H.263-2000 codec */
typedef struct tdav_codec_h263pp_s {
    TDAV_DECLARE_CODEC_H263;
}
tdav_codec_h263pp_t;


static void tdav_codec_h263_rtp_callback(tdav_codec_h263_t *self, const void *data, tsk_size_t size, tsk_bool_t marker);
static void tdav_codec_h263p_rtp_callback(tdav_codec_h263_t *self, const void *data, tsk_size_t size, tsk_bool_t frag, tsk_bool_t marker);

static void tdav_codec_h263_encap(const tdav_codec_h263_t* h263, const uint8_t* pdata, tsk_size_t size);


/* ============ Common To all H263 codecs ================= */

static int tdav_codec_h263_set(tmedia_codec_t* self, const tmedia_param_t* param)
{
    tdav_codec_h263_t* h263 = (tdav_codec_h263_t*)self;
    if(!self->opened) {
        TSK_DEBUG_ERROR("Codec not opened");
        return -1;
    }
    if(param->value_type == tmedia_pvt_int32) {
        if(tsk_striequals(param->key, "action")) {
            tmedia_codec_action_t action = (tmedia_codec_action_t)TSK_TO_INT32((uint8_t*)param->value);
            switch(action) {
            case tmedia_codec_action_encode_idr: {
                h263->encoder.force_idr = tsk_true;
                break;
            }
            case tmedia_codec_action_bw_down: {
                h263->encoder.quality = TSK_CLAMP(1, (h263->encoder.quality + 1), 31);
                h263->encoder.context->global_quality = FF_QP2LAMBDA * h263->encoder.quality;
                break;
            }
            case tmedia_codec_action_bw_up: {
                h263->encoder.quality = TSK_CLAMP(1, (h263->encoder.quality - 1), 31);
                h263->encoder.context->global_quality = FF_QP2LAMBDA * h263->encoder.quality;
                break;
            }
            }
            return 0;
        }
    }
    return -1;
}

int tdav_codec_h263_init(tdav_codec_h263_t* self, tdav_codec_h263_type_t type, enum CodecID encoder, enum CodecID decoder)
{
    int ret = 0;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    self->type = type;
    self->encoder.quality = 1;

    if(!(self->encoder.codec = avcodec_find_encoder(encoder))) {
        TSK_DEBUG_ERROR("Failed to find [%d]encoder", encoder);
        ret = -2;
    }

    if(!(self->decoder.codec = avcodec_find_decoder(decoder))) {
        TSK_DEBUG_ERROR("Failed to find [%d] decoder", decoder);
        ret = -3;
    }

    self->encoder.max_bw_kpbs = tmedia_defaults_get_bandwidth_video_upload_max();

    /* allocations MUST be done by open() */
    return ret;
}

int tdav_codec_h263_deinit(tdav_codec_h263_t* self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    self->encoder.codec = tsk_null;
    self->decoder.codec = tsk_null;

    // FFMpeg resources are destroyed by close()



    TSK_FREE(self->rtp.ptr);
    self->rtp.size = 0;

    return 0;
}



/* ============ H.263-1996 Plugin interface ================= */

//
//	H.263-1996 object definition
//
static int tdav_codec_h263_open(tmedia_codec_t* self)
{
    int ret;

    tdav_codec_h263_t* h263 = (tdav_codec_h263_t*)self;

    if(!h263) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* the caller (base class) already checked that the codec is not opened */

    //	Encoder
    if((ret = tdav_codec_h263_open_encoder(h263))) {
        return ret;
    }

    //	Decoder
    if((ret = tdav_codec_h263_open_decoder(h263))) {
        return ret;
    }

    return ret;
}

static int tdav_codec_h263_close(tmedia_codec_t* self)
{
    tdav_codec_h263_t* h263 = (tdav_codec_h263_t*)self;
    int ret;

    if(!h263) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* the caller (base class) already checked that the codec is opened */

    //	Encoder
    ret = tdav_codec_h263_close_encoder(h263);
    //	Decoder
    ret = tdav_codec_h263_close_decoder(h263);

    return ret;
}

static tsk_size_t tdav_codec_h263_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
    int ret;
    int size;

    tdav_codec_h263_t* h263 = (tdav_codec_h263_t*)self;

    if(!self || !in_data || !in_size || !out_data) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    // wrap yuv420 buffer
    size = avpicture_fill((AVPicture *)h263->encoder.picture, (uint8_t*)in_data, PIX_FMT_YUV420P, h263->encoder.context->width, h263->encoder.context->height);
    if(size != in_size) {
        /* guard */
        TSK_DEBUG_ERROR("Invalid size");
        return 0;
    }
#if LIBAVCODEC_VERSION_MAJOR <= 53
    h263->encoder.picture->pict_type = h263->encoder.force_idr ? FF_I_TYPE : 0;
#else
    h263->encoder.picture->pict_type = h263->encoder.force_idr ? AV_PICTURE_TYPE_I : AV_PICTURE_TYPE_NONE;
#endif
    h263->encoder.picture->pts = AV_NOPTS_VALUE;
    h263->encoder.picture->quality = h263->encoder.context->global_quality;
    ret = avcodec_encode_video(h263->encoder.context, h263->encoder.buffer, size, h263->encoder.picture);
    if(ret > 0) {
        tdav_codec_h263_encap(h263, h263->encoder.buffer, (tsk_size_t)ret);
    }
    h263->encoder.force_idr = tsk_false;

    return 0;
}

static tsk_size_t tdav_codec_h263_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
    uint8_t F, P, sbit, ebit;
    const uint8_t* pdata = in_data;
    const uint8_t* pay_ptr;
    tsk_size_t pay_size;
    tsk_size_t hdr_size;
    tsk_size_t xsize, retsize = 0;
    int got_picture_ptr;
    int ret;

    tdav_codec_h263_t* h263 = (tdav_codec_h263_t*)self;
    const trtp_rtp_header_t* rtp_hdr = proto_hdr;
    tsk_bool_t is_idr = tsk_false;

    if(!self || !in_data || !in_size || !out_data || !h263->decoder.context) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    /*	RFC 2190
    	get F and P bits, used to determine the header Mode (A, B or C)
    	F: 1 bit
    	The flag bit indicates the mode of the payload header. F=0, mode A;
    	F=1, mode B or mode C depending on P bit defined below.
    	P: 1 bit
    	Optional PB-frames mode as defined by the H.263 [4]. "0" implies
    	normal I or P frame, "1" PB-frames. When F=1, P also indicates modes:
    	mode B if P=0, mode C if P=1.

    	I:  1 bit.
       Picture coding type, bit 9 in PTYPE defined by H.263[4], "0" is
       intra-coded, "1" is inter-coded.
    */
    F = *pdata >> 7;
    P = (*pdata >> 6) & 0x01;

    /* SBIT and EBIT */
    sbit = (*pdata >> 3) & 0x0F;
    ebit = (*pdata & 0x07);

    if(F == 0) {
        /*	MODE A
        	0                   1                   2                   3
        	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
        	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        	|F|P|SBIT |EBIT | SRC |I|U|S|A|R      |DBQ| TRB |    TR         |
        	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        */
        hdr_size = H263_HEADER_MODE_A_SIZE;
        is_idr = (in_size >= 2) && !(pdata[1] & 0x10) /* I==1 */;
    }
    else if(P == 0) { // F=1 and P=0
        /* MODE B
        	 0                   1                   2                   3
        	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
        	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        	|F|P|SBIT |EBIT | SRC | QUANT   |  GOBN   |   MBA           |R  |
        	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        	|I|U|S|A| HMV1        | VMV1        | HMV2        | VMV2        |
        	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        */
        hdr_size = H263_HEADER_MODE_B_SIZE;
        is_idr = (in_size >= 5) && !(pdata[4] & 0x80) /* I==1 */;
    }
    else { // F=1 and P=1
        /* MODE C
        	 0                   1                   2                   3
        	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
        	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        	|F|P|SBIT |EBIT | SRC | QUANT   |  GOBN   |   MBA           |R  |
        	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        	|I|U|S|A| HMV1        | VMV1        | HMV2        | VMV2        |
        	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        	| RR                                  |DBQ| TRB |    TR         |
        	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        */
        hdr_size = H263_HEADER_MODE_C_SIZE;
        is_idr = (in_size >= 5) && !(pdata[4] & 0x80) /* I==1 */;
    }

    /* Check size */
    if(in_size < hdr_size) {
        TSK_DEBUG_ERROR("Too short");
        return 0;
    }

    pay_ptr = (pdata + hdr_size);
    pay_size = (in_size - hdr_size);
    xsize = avpicture_get_size(h263->decoder.context->pix_fmt, h263->decoder.context->width, h263->decoder.context->height);

    /* Packet lost? */
    if(h263->decoder.last_seq != (rtp_hdr->seq_num - 1) && h263->decoder.last_seq) {
        if(h263->decoder.last_seq == rtp_hdr->seq_num) {
            // Could happen on some stupid emulators
            //TSK_DEBUG_INFO("Packet duplicated, seq_num=%d", rtp_hdr->seq_num);
            return 0;
        }
        TSK_DEBUG_INFO("[H.263] Packet loss, seq_num=%d", rtp_hdr->seq_num);
    }
    h263->decoder.last_seq = rtp_hdr->seq_num;

    if((int)(h263->decoder.accumulator_pos + pay_size) <= xsize) {
        if((h263->decoder.ebit + sbit) == 8) { /* Perfect one Byte to clean up */
            if(h263->decoder.accumulator_pos) {
                ((uint8_t*)h263->decoder.accumulator)[h263->decoder.accumulator_pos-1] = (((uint8_t*)h263->decoder.accumulator)[h263->decoder.accumulator_pos-1] & (0xFF << h263->decoder.ebit)) |
                        (*pay_ptr & (0xFF >> sbit));
            }
            pay_ptr++, pay_size--;
        }
        h263->decoder.ebit = ebit;

        memcpy(&((uint8_t*)h263->decoder.accumulator)[h263->decoder.accumulator_pos], pay_ptr, pay_size);
        h263->decoder.accumulator_pos += pay_size;
    }
    else {
        TSK_DEBUG_WARN("Buffer overflow");
        h263->decoder.accumulator_pos = 0;
        return 0;
    }

    if(rtp_hdr->marker) {
        AVPacket packet;
        /* allocate destination buffer */
        if(*out_max_size <xsize) {
            if(!(*out_data = tsk_realloc(*out_data, xsize))) {
                TSK_DEBUG_ERROR("Failed to allocate new buffer");
                h263->decoder.accumulator_pos = 0;
                *out_max_size = 0;
                return 0;
            }
            *out_max_size = xsize;
        }

        av_init_packet(&packet);
        packet.size = (int)h263->decoder.accumulator_pos;
        packet.data = h263->decoder.accumulator;
        ret = avcodec_decode_video2(h263->decoder.context, h263->decoder.picture, &got_picture_ptr, &packet);

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
            // Is it IDR frame?
            if(is_idr && TMEDIA_CODEC_VIDEO(self)->in.callback) {
                TSK_DEBUG_INFO("Decoded H.263 IDR");
                TMEDIA_CODEC_VIDEO(self)->in.result.type = tmedia_video_decode_result_type_idr;
                TMEDIA_CODEC_VIDEO(self)->in.result.proto_hdr = proto_hdr;
                TMEDIA_CODEC_VIDEO(self)->in.callback(&TMEDIA_CODEC_VIDEO(self)->in.result);
            }
            TMEDIA_CODEC_VIDEO(h263)->in.width = h263->decoder.context->width;
            TMEDIA_CODEC_VIDEO(h263)->in.height = h263->decoder.context->height;
            /* copy picture into a linear buffer */
            avpicture_layout((AVPicture *)h263->decoder.picture, h263->decoder.context->pix_fmt, (int)h263->decoder.context->width, (int)h263->decoder.context->height,
                             *out_data, (int)retsize);
        }
        /* in all cases: reset accumulator */
        h263->decoder.accumulator_pos = 0;
    }

    return retsize;
}

static tsk_bool_t tdav_codec_h263_sdp_att_match(const tmedia_codec_t* codec, const char* att_name, const char* att_value)
{
    if(tsk_striequals(att_name, "fmtp")) {
        unsigned width, height, fps;
        if(tmedia_parse_video_fmtp(att_value, TMEDIA_CODEC_VIDEO(codec)->pref_size, &width, &height, &fps)) {
            TSK_DEBUG_ERROR("Failed to match fmtp=%s", att_value);
            return tsk_false;
        }
        TMEDIA_CODEC_VIDEO(codec)->in.width = TMEDIA_CODEC_VIDEO(codec)->out.width = width;
        TMEDIA_CODEC_VIDEO(codec)->in.height = TMEDIA_CODEC_VIDEO(codec)->out.height = height;
        TMEDIA_CODEC_VIDEO(codec)->in.fps = TMEDIA_CODEC_VIDEO(codec)->out.fps = fps;
    }
#if 0
    else if(tsk_striequals(att_name, "imageattr")) {
        unsigned in_width, in_height, out_width, out_height;
        if(tmedia_parse_video_imageattr(att_value, TMEDIA_CODEC_VIDEO(codec)->pref_size, &in_width, &in_height, &out_width, &out_height) != 0) {
            return tsk_false;
        }
        TMEDIA_CODEC_VIDEO(codec)->in.width = in_width;
        TMEDIA_CODEC_VIDEO(codec)->in.height = in_height;
        TMEDIA_CODEC_VIDEO(codec)->out.width = out_width;
        TMEDIA_CODEC_VIDEO(codec)->out.height = out_height;
    }
#endif

    return tsk_true;
}

static char* tdav_codec_h263_sdp_att_get(const tmedia_codec_t* codec, const char* att_name)
{
    if(tsk_striequals(att_name, "fmtp")) {
        tmedia_pref_video_size_t cif_vs;
        if(tmedia_video_get_closest_cif_size(TMEDIA_CODEC_VIDEO(codec)->pref_size, &cif_vs)) {
            TSK_DEBUG_ERROR("Failed to get closest CIF family size");
            return tsk_null;
        }
        return tmedia_get_video_fmtp(cif_vs);
    }
#if 0
    else if(tsk_striequals(att_name, "imageattr")) {
        return tmedia_get_video_imageattr(TMEDIA_CODEC_VIDEO(codec)->pref_size,
                                          TMEDIA_CODEC_VIDEO(codec)->in.width, TMEDIA_CODEC_VIDEO(codec)->in.height, TMEDIA_CODEC_VIDEO(codec)->out.width, TMEDIA_CODEC_VIDEO(codec)->out.height);
    }
#endif
    return tsk_null;
}

/* constructor */
static tsk_object_t* tdav_codec_h263_ctor(tsk_object_t * self, va_list * app)
{
    tdav_codec_h263_t *h263 = self;
    if(h263) {
        /* init base: called by tmedia_codec_create() */
        /* init self */
        tdav_codec_h263_init(TDAV_CODEC_H263(self), tdav_codec_h263_1996, CODEC_ID_H263, CODEC_ID_H263);
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h263_dtor(tsk_object_t * self)
{
    tdav_codec_h263_t *h263 = self;
    if(h263) {
        /* deinit base */
        tmedia_codec_video_deinit(h263);
        /* deinit self */
        tdav_codec_h263_deinit(TDAV_CODEC_H263(self));

    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h263_def_s = {
    sizeof(tdav_codec_h263_t),
    tdav_codec_h263_ctor,
    tdav_codec_h263_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h263_plugin_def_s = {
    &tdav_codec_h263_def_s,

    tmedia_video,
    tmedia_codec_id_h263,
    "H263",
    "H263-1996 codec (FFmpeg)",
    TMEDIA_CODEC_FORMAT_H263,
    tsk_false,
    90000, // rate

    /* audio */
    { 0 },

    /* video */
    {176, 144, 15},

    tdav_codec_h263_set,
    tdav_codec_h263_open,
    tdav_codec_h263_close,
    tdav_codec_h263_encode,
    tdav_codec_h263_decode,
    tdav_codec_h263_sdp_att_match,
    tdav_codec_h263_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_h263_plugin_def_t = &tdav_codec_h263_plugin_def_s;























/* ============ H.263-1998 Plugin interface ================= */

//
//	H.263-1998 object definition
//

static tsk_size_t tdav_codec_h263p_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
    uint8_t P, V, PLEN, PEBIT;
    uint8_t* pdata = (uint8_t*)in_data;
    const uint8_t* pay_ptr;
    tsk_size_t pay_size;
    int hdr_size = H263P_HEADER_SIZE;
    tsk_size_t xsize, retsize = 0;
    int got_picture_ptr;
    int ret;

    tdav_codec_h263_t* h263 = (tdav_codec_h263_t*)self;
    const trtp_rtp_header_t* rtp_hdr = proto_hdr;

    if(!self || !in_data || !in_size || ((int)in_size <= hdr_size) || !out_data || !h263->decoder.context) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    /*
    	rfc4629 - 5.1.  General H.263+ Payload Header

             0                   1
             0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |   RR    |P|V|   PLEN    |PEBIT|
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    */
    P = (pdata[0] & 0x04)>>2;
    V = (pdata[0] & 0x02)>>1;
    PLEN = (((pdata[0] & 0x01)<<5) | pdata[1]>>3);
    PEBIT = pdata[1] & 0x07;

    if(V) {
        /*
        	Indicates the presence of an 8-bit field containing information
        	for Video Redundancy Coding (VRC), which follows immediately after
        	the initial 16 bits of the payload header, if present.  For syntax
        	and semantics of that 8-bit VRC field, see Section 5.2.
        */
    }
    if(PLEN) {
        /*
        	Length, in bytes, of the extra picture header.  If no extra
        	picture header is attached, PLEN is 0.  If PLEN>0, the extra
        	picture header is attached immediately following the rest of the
        	payload header.  Note that the length reflects the omission of the
        	first two bytes of the picture start code (PSC).  See Section 6.1.
        */
        hdr_size += PLEN;
        if(PEBIT) {
            /*
            	Indicates the number of bits that shall be ignored in the last
            	byte of the picture header.  If PLEN is not zero, the ignored bits
            	shall be the least significant bits of the byte.  If PLEN is zero,
            	then PEBIT shall also be zero.
            */
            TSK_DEBUG_WARN("PEBIT ignored");
        }
    }
    if(P) { /* MUST be done after PLEN and PEBIT */
        /*
        	Indicates the picture start or a picture segment (GOB/Slice) start
        	or a video sequence end (EOS or EOSBS).  Two bytes of zero bits
        	then have to be prefixed to the payload of such a packet to
        	compose a complete picture/GOB/slice/EOS/EOSBS start code.  This
        	bit allows the omission of the two first bytes of the start codes,
        	thus improving the compression ratio.
        */
        hdr_size -= 2;
        pdata[hdr_size] = 0x00, pdata[hdr_size + 1] = 0x00;
    }

    pay_ptr = (pdata + hdr_size);
    pay_size = (in_size - hdr_size);
    xsize = avpicture_get_size(h263->decoder.context->pix_fmt, h263->decoder.context->width, h263->decoder.context->height);

    /* Packet lost? */
    if(h263->decoder.last_seq != (rtp_hdr->seq_num - 1) && h263->decoder.last_seq) {
        if(h263->decoder.last_seq == rtp_hdr->seq_num) {
            // Could happen on some stupid emulators
            //TSK_DEBUG_INFO("Packet duplicated, seq_num=%d", rtp_hdr->seq_num);
            return 0;
        }
        TSK_DEBUG_INFO("[H.263+] Packet loss, seq_num=%d", rtp_hdr->seq_num);
    }
    h263->decoder.last_seq = rtp_hdr->seq_num;

    if((int)(h263->decoder.accumulator_pos + pay_size) <= xsize) {
        /* PEBIT is ignored */
        memcpy(&((uint8_t*)h263->decoder.accumulator)[h263->decoder.accumulator_pos], pay_ptr, pay_size);
        h263->decoder.accumulator_pos += pay_size;
    }
    else {
        TSK_DEBUG_WARN("Buffer overflow");
        h263->decoder.accumulator_pos = 0;
        return 0;
    }

    if(rtp_hdr->marker) {
        AVPacket packet;
        /* allocate destination buffer */
        if(*out_max_size < xsize) {
            if(!(*out_data = tsk_realloc(*out_data, xsize))) {
                TSK_DEBUG_ERROR("Failed to allocate new buffer");
                *out_max_size = 0;
                h263->decoder.accumulator_pos = 0;
                return 0;
            }
            *out_max_size = xsize;
        }

        /* decode the picture */
        av_init_packet(&packet);
        packet.size = (int)h263->decoder.accumulator_pos;
        packet.data = h263->decoder.accumulator;
        ret = avcodec_decode_video2(h263->decoder.context, h263->decoder.picture, &got_picture_ptr, &packet);

        if(ret <0 || !got_picture_ptr) {
            TSK_DEBUG_WARN("Failed to decode the buffer");
        }
        else {
            retsize = xsize;
            TMEDIA_CODEC_VIDEO(h263)->in.width = h263->decoder.context->width;
            TMEDIA_CODEC_VIDEO(h263)->in.height = h263->decoder.context->height;
            /* copy picture into a linear buffer */
            avpicture_layout((AVPicture *)h263->decoder.picture, h263->decoder.context->pix_fmt, (int)h263->decoder.context->width, (int)h263->decoder.context->height,
                             *out_data, (int)retsize);
        }
        /* in all cases: reset accumulator */
        h263->decoder.accumulator_pos = 0;
    }

    return retsize;
}

/* constructor */
static tsk_object_t* tdav_codec_h263p_ctor(tsk_object_t * self, va_list * app)
{
    tdav_codec_h263p_t *h263p = self;
    if(h263p) {
        /* init base: called by tmedia_codec_create() */
        /* init self */
        tdav_codec_h263_init(TDAV_CODEC_H263(self), tdav_codec_h263_1998, CODEC_ID_H263P, CODEC_ID_H263);
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h263p_dtor(tsk_object_t * self)
{
    tdav_codec_h263p_t *h263p = self;
    if(h263p) {
        /* deinit base */
        tmedia_codec_video_deinit(h263p);
        /* deinit self */
        tdav_codec_h263_deinit(TDAV_CODEC_H263(self));
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h263p_def_s = {
    sizeof(tdav_codec_h263p_t),
    tdav_codec_h263p_ctor,
    tdav_codec_h263p_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h263p_plugin_def_s = {
    &tdav_codec_h263p_def_s,

    tmedia_video,
    tmedia_codec_id_h263p,
    "H263-1998",
    "H263-1998 codec (FFmpeg)",
    TMEDIA_CODEC_FORMAT_H263_1998,
    tsk_true,
    90000, // rate

    /* audio */
    { 0 },

    /* video (width, height, fps) */
    {176, 144, 0},// fps is @deprecated

    tdav_codec_h263p_set,
    tdav_codec_h263p_open,
    tdav_codec_h263p_close,
    tdav_codec_h263p_encode,
    tdav_codec_h263p_decode,
    tdav_codec_h263p_sdp_att_match,
    tdav_codec_h263p_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_h263p_plugin_def_t = &tdav_codec_h263p_plugin_def_s;














/* ============ H.263-2000 Plugin interface ================= */

//
//	H.263-2000 object definition
//

/* constructor */
static tsk_object_t* tdav_codec_h263pp_ctor(tsk_object_t * self, va_list * app)
{
    tdav_codec_h263pp_t *h263pp = self;
    if(h263pp) {
        /* init base: called by tmedia_codec_create() */
        /* init self */
        tdav_codec_h263_init(TDAV_CODEC_H263(self), tdav_codec_h263_2000, CODEC_ID_H263P, CODEC_ID_H263);
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h263pp_dtor(tsk_object_t * self)
{
    tdav_codec_h263pp_t *h263pp = self;
    if(h263pp) {
        /* deinit base */
        tmedia_codec_video_deinit(h263pp);
        /* deinit self */
        tdav_codec_h263_deinit(TDAV_CODEC_H263(self));
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h263pp_def_s = {
    sizeof(tdav_codec_h263pp_t),
    tdav_codec_h263pp_ctor,
    tdav_codec_h263pp_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h263pp_plugin_def_s = {
    &tdav_codec_h263pp_def_s,

    tmedia_video,
    tmedia_codec_id_h263pp,
    "H263-2000",
    "H263-2000 codec (FFmpeg)",
    TMEDIA_CODEC_FORMAT_H263_2000,
    tsk_true,
    90000, // rate

    /* audio */
    { 0 },

    /* video (width, height, fps)*/
    {176, 144, 0},// fps is @deprecated

    tdav_codec_h263pp_set,
    tdav_codec_h263pp_open,
    tdav_codec_h263pp_close,
    tdav_codec_h263pp_encode,
    tdav_codec_h263pp_decode,
    tdav_codec_h263pp_sdp_att_match,
    tdav_codec_h263pp_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_h263pp_plugin_def_t = &tdav_codec_h263pp_plugin_def_s;



int tdav_codec_h263_open_encoder(tdav_codec_h263_t* self)
{
    int ret;
    int size;
    int32_t max_bw_kpbs;
    if(self->encoder.context) {
        TSK_DEBUG_ERROR("Encoder already opened");
        return -1;
    }

    self->encoder.context = avcodec_alloc_context();
    avcodec_get_context_defaults(self->encoder.context);

    self->encoder.context->pix_fmt		= PIX_FMT_YUV420P;
    self->encoder.context->time_base.num  = 1;
    self->encoder.context->time_base.den  = TMEDIA_CODEC_VIDEO(self)->out.fps;
    self->encoder.context->width = TMEDIA_CODEC_VIDEO(self)->out.width;
    self->encoder.context->height = TMEDIA_CODEC_VIDEO(self)->out.height;

    self->encoder.context->qmin = 10;
    self->encoder.context->qmax = 51;
#if LIBAVCODEC_VERSION_MAJOR <= 53
    self->encoder.context->mb_qmin = self->encoder.context->qmin;
    self->encoder.context->mb_qmax = self->encoder.context->qmax;
#endif
    self->encoder.context->mb_decision = FF_MB_DECISION_RD;
    max_bw_kpbs = TSK_CLAMP(
                      0,
                      tmedia_get_video_bandwidth_kbps_2(TMEDIA_CODEC_VIDEO(self)->out.width, TMEDIA_CODEC_VIDEO(self)->out.height, TMEDIA_CODEC_VIDEO(self)->out.fps),
                      self->encoder.max_bw_kpbs
                  );
    self->encoder.context->bit_rate = (max_bw_kpbs * 1024);// bps
    //self->encoder.context->rc_lookahead = 0;
    self->encoder.context->rtp_payload_size = RTP_PAYLOAD_SIZE;
    self->encoder.context->opaque = tsk_null;
    self->encoder.context->gop_size = (TMEDIA_CODEC_VIDEO(self)->out.fps * TDAV_H263_GOP_SIZE_IN_SECONDS);
    self->encoder.context->flags |= CODEC_FLAG_QSCALE;
    self->encoder.context->global_quality = FF_QP2LAMBDA * self->encoder.quality;
    self->encoder.context->max_b_frames = 0;

    // Picture (YUV 420)
    if(!(self->encoder.picture = avcodec_alloc_frame())) {
        TSK_DEBUG_ERROR("Failed to create encoder picture");
        return -2;
    }
    avcodec_get_frame_defaults(self->encoder.picture);
    //if((ret = avpicture_alloc((AVPicture*)self->encoder.picture, PIX_FMT_YUV420P, self->encoder.context->width, self->encoder.context->height))){
    //	TSK_DEBUG_ERROR("Failed to allocate encoder picture");
    //	return ret;
    //}

    size = avpicture_get_size(PIX_FMT_YUV420P, self->encoder.context->width, self->encoder.context->height);
    if(!(self->encoder.buffer = tsk_calloc(size, sizeof(uint8_t)))) {
        TSK_DEBUG_ERROR("Failed to allocate encoder buffer");
        return -2;
    }


    // RTP Callback
    switch(self->type) {
    case tdav_codec_h263_1996: {
        // H263 - 1996
        break;
    }
    case tdav_codec_h263_1998: {
        // H263 - 1998
#if defined(CODEC_FLAG_H263P_UMV)
        self->encoder.context->flags |= CODEC_FLAG_H263P_UMV;		// Annex D+
#endif
        self->encoder.context->flags |= CODEC_FLAG_AC_PRED;			// Annex I and T
        self->encoder.context->flags |= CODEC_FLAG_LOOP_FILTER;		// Annex J
#if defined(CODEC_FLAG_H263P_SLICE_STRUCT)
        self->encoder.context->flags |= CODEC_FLAG_H263P_SLICE_STRUCT;	// Annex K
#endif
#if defined(CODEC_FLAG_H263P_AIV)
        self->encoder.context->flags |= CODEC_FLAG_H263P_AIV;			// Annex S
#endif
        break;
    }
    case tdav_codec_h263_2000: {
        // H263 - 2000
#if defined(CODEC_FLAG_H263P_UMV)
        self->encoder.context->flags |= CODEC_FLAG_H263P_UMV;		// Annex D+
#endif
        self->encoder.context->flags |= CODEC_FLAG_AC_PRED;			// Annex I and T
        self->encoder.context->flags |= CODEC_FLAG_LOOP_FILTER;		// Annex J
#if defined(CODEC_FLAG_H263P_SLICE_STRUCT)
        self->encoder.context->flags |= CODEC_FLAG_H263P_SLICE_STRUCT;	// Annex K
#endif
#if defined(CODEC_FLAG_H263P_AIV)
        self->encoder.context->flags |= CODEC_FLAG_H263P_AIV;			// Annex S
#endif
        break;
    }
    }
    // Open encoder
    if((ret = avcodec_open(self->encoder.context, self->encoder.codec)) < 0) {
        TSK_DEBUG_ERROR("Failed to open [%s] codec", TMEDIA_CODEC(self)->plugin->desc);
        return ret;
    }

    TSK_DEBUG_INFO("[H.263] bitrate=%d bps", self->encoder.context->bit_rate);

    return ret;
}

int tdav_codec_h263_open_decoder(tdav_codec_h263_t* self)
{
    int ret, size;

    if(self->decoder.context) {
        TSK_DEBUG_ERROR("Decoder already opened");
        return -1;
    }

    self->decoder.context = avcodec_alloc_context();
    avcodec_get_context_defaults(self->decoder.context);

    self->decoder.context->pix_fmt = PIX_FMT_YUV420P;
    self->decoder.context->width = TMEDIA_CODEC_VIDEO(self)->in.width;
    self->decoder.context->height = TMEDIA_CODEC_VIDEO(self)->in.height;

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

    // Open decoder
    if((ret = avcodec_open(self->decoder.context, self->decoder.codec)) < 0) {
        TSK_DEBUG_ERROR("Failed to open [%s] codec", TMEDIA_CODEC(self)->plugin->desc);
        return ret;
    }

    self->decoder.last_seq = 0;

    return ret;
}

int tdav_codec_h263_close_encoder(tdav_codec_h263_t* self)
{
    if(self->encoder.context) {
        avcodec_close(self->encoder.context);
        av_free(self->encoder.context);
        self->encoder.context = tsk_null;
    }
    if(self->encoder.picture) {
        av_free(self->encoder.picture);
        self->encoder.picture = tsk_null;
    }
    if(self->encoder.buffer) {
        TSK_FREE(self->encoder.buffer);
    }
    return 0;
}

int tdav_codec_h263_close_decoder(tdav_codec_h263_t* self)
{
    if(self->decoder.context) {
        avcodec_close(self->decoder.context);
        av_free(self->decoder.context);
        self->decoder.context = tsk_null;
    }
    if(self->decoder.picture) {
        av_free(self->decoder.picture);
        self->decoder.picture = tsk_null;
    }
    if(self->decoder.accumulator) {
        TSK_FREE(self->decoder.accumulator);
        self->decoder.accumulator_pos = 0;
    }
    return 0;
}

/* ============ Callbacks ================= */

static void tdav_codec_h263_encap(const tdav_codec_h263_t* h263, const uint8_t* pdata, tsk_size_t size)
{
    tsk_bool_t frag = tsk_false;
    register uint32_t i, last_index = 0;

    if(size < RTP_PAYLOAD_SIZE) {
        goto last;
    }

    for(i = 4; i<(size - 4); i++) {
        if(pdata[i] == 0x00 && pdata[i+1] == 0x00 && pdata[i+2]>=0x80) { /* PSC or (GBSC) found */
            if((i - last_index) >= RTP_PAYLOAD_SIZE || tsk_true/* FIXME */) {
                switch(h263->type) {
                case tdav_codec_h263_1996:
                    tdav_codec_h263_rtp_callback((tdav_codec_h263_t*) h263, pdata+last_index,
                                                 (i - last_index), (last_index == size));
                    break;
                default:
                    tdav_codec_h263p_rtp_callback((tdav_codec_h263_t*) h263, pdata + last_index,
                                                  (i - last_index), frag, (last_index == size));
                    frag = tsk_true;
                    break;
                }
                last_index = i;
            }
        }
    }
last:
    if(last_index < size) {
        switch(h263->type) {
        case tdav_codec_h263_1996:
            tdav_codec_h263_rtp_callback((tdav_codec_h263_t*) h263, pdata + last_index,
                                         (size - last_index), tsk_true);
            break;
        default:
            tdav_codec_h263p_rtp_callback((tdav_codec_h263_t*) h263, pdata + last_index,
                                          (size - last_index), frag, tsk_true);
            break;
        }
    }
}


static void tdav_codec_h263_rtp_callback(tdav_codec_h263_t *self, const void *data, tsk_size_t size, tsk_bool_t marker)
{
    uint8_t* pdata = (uint8_t*)data;

    if(self->rtp.size < (size + H263_HEADER_MODE_A_SIZE)) {
        if(!(self->rtp.ptr = tsk_realloc(self->rtp.ptr, (size + H263_HEADER_MODE_A_SIZE)))) {
            TSK_DEBUG_ERROR("Failed to allocate new buffer");
            return;
        }
        self->rtp.size = (size + H263_HEADER_MODE_A_SIZE);
    }
    memcpy((self->rtp.ptr + H263_HEADER_MODE_A_SIZE), data, size);

    /* http://eu.sabotage.org/www/ITU/H/H0263e.pdf section 5.1
    * 5.1.1 Picture Start Code (PSC) (22 bits) - PSC is a word of 22 bits. Its value is 0000 0000 0000 0000 1 00000.

    *
    * 5.1.1 Picture Start Code (PSC) (22 bits)
    * 5.1.2 Temporal Reference (TR) (8 bits)
    * 5.1.3 Type Information (PTYPE) (Variable Length)
    *	– Bit 1: Always "1", in order to avoid start code emulation.
    *	– Bit 2: Always "0", for distinction with Recommendation H.261.

    *	– Bit 3: Split screen indicator, "0" off, "1" on.
    *	– Bit 4: Document camera indicator, "0" off, "1" on.
    *	– Bit 5: Full Picture Freeze Release, "0" off, "1" on.
    *	– Bits 6-8: Source Format, "000" forbidden, "001" sub-QCIF, "010" QCIF, "011" CIF,
    	"100" 4CIF, "101" 16CIF, "110" reserved, "111" extended PTYPE.
    	If bits 6-8 are not equal to "111", which indicates an extended PTYPE (PLUSPTYPE), the following
    	five bits are also present in PTYPE:
    	– Bit 9: Picture Coding Type, "0" INTRA (I-picture), "1" INTER (P-picture).
    	– Bit 10: Optional Unrestricted Motion Vector mode (see Annex D), "0" off, "1" on.
    	– Bit 11: Optional Syntax-based Arithmetic Coding mode (see Annex E), "0" off, "1" on.
    	– Bit 12: Optional Advanced Prediction mode (see Annex F), "0" off, "1" on.
    	– Bit 13: Optional PB-frames mode (see Annex G), "0" normal I- or P-picture, "1" PB-frame.
    */
    if(pdata[0] == 0x00 && pdata[1] == 0x00 && (pdata[2] & 0xfc)==0x80) { /* PSC */
        /* RFC 2190 -5.1 Mode A
        	0                   1                   2                   3
        	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
        	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        	|F|P|SBIT |EBIT | SRC |I|U|S|A|R      |DBQ| TRB |    TR         |
        	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

        SRC : 3 bits
        Source format, bit 6,7 and 8 in PTYPE defined by H.263 [4], specifies
        the resolution of the current picture.

        I:  1 bit.
        Picture coding type, bit 9 in PTYPE defined by H.263[4], "0" is
        intra-coded, "1" is inter-coded.
        */

        // PDATA[4] ======> Bits 3-10 of PTYPE
        uint32_t rtp_hdr = 0;
        uint8_t format, pict_type;

        // Source Format = 4,5,6
        format = (pdata[4] & 0x3C)>>2;
        // Picture Coding Type = 7
        pict_type = (pdata[4] & 0x02)>>1;
        // RTP mode A header
        ((uint8_t*)&rtp_hdr)[1] = (format <<5) | (pict_type << 4);
        //rtp_hdr = tnet_htonl(rtp_hdr);
        memcpy(self->rtp.ptr, &rtp_hdr, sizeof(rtp_hdr));
    }

    // Send data over the network
    if(TMEDIA_CODEC_VIDEO(self)->out.callback) {
        TMEDIA_CODEC_VIDEO(self)->out.result.buffer.ptr = self->rtp.ptr;
        TMEDIA_CODEC_VIDEO(self)->out.result.buffer.size = (size + H263_HEADER_MODE_A_SIZE);
        TMEDIA_CODEC_VIDEO(self)->out.result.duration =  (uint32_t)((1./(double)TMEDIA_CODEC_VIDEO(self)->out.fps) * TMEDIA_CODEC(self)->plugin->rate);
        TMEDIA_CODEC_VIDEO(self)->out.result.last_chunck = marker;
        TMEDIA_CODEC_VIDEO(self)->out.callback(&TMEDIA_CODEC_VIDEO(self)->out.result);
    }
}

static void tdav_codec_h263p_rtp_callback(tdav_codec_h263_t *self, const void *data, tsk_size_t size, tsk_bool_t frag, tsk_bool_t marker)
{
    uint8_t* pdata = (uint8_t*)data;
    //uint8_t rtp_hdr[2] = {0x00, 0x00};
    //tsk_bool_t eos = tsk_false;

    const void* _ptr = tsk_null;
    tsk_size_t _size = 0;
    //static tsk_bool_t frag = tsk_false;
    //tsk_bool_t found_gob = tsk_false;

    /* RFC 4629 - 5.1. General H.263+ Payload Header
    	The H.263+ payload header is structured as follows:
         0                   1
         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |   RR    |P|V|   PLEN    |PEBIT|
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    */

    /* http://eu.sabotage.org/www/ITU/H/H0263e.pdf
    *
    * 5.1.1 Picture Start Code (PSC) (22 bits)
    *	->PSC is a word of 22 bits. Its value is 0000 0000 0000 0000 1 00000.
    * 5.1.27 End Of Sequence (EOS) (22 bits)
    *	->A codeword of 22 bits. Its value is 0000 0000 0000 0000 1 11111
    * 5.2.2 Group of Block Start Code (GBSC) (17 bits)
    *	->A word of 17 bits. Its value is 0000 0000 0000 0000 1
    * C.4.1 End Of Sub-Bitstream code (EOSBS) (23 bits)
    *	->The EOSBS code is a codeword of 23 bits. Its value is 0000 0000 0000 0000 1 11110 0
    *
    *
    * 5.2.3 Group Number (GN) (5 bits)
    *	-> last 5 bits
    */
    //if(pdata[0] == 0x00 && pdata[1] == 0x00 && pdata[2] >= 0x80){ /* PSC or EOS or GBSC */
    //	uint8_t GN = ((pdata[2]>>2) & 0x1F);
    //	found_gob = tsk_true;
    //	//TSK_DEBUG_INFO("GN=%u", pdata[2]);
    //
    //	/*	RFC 4629 - 6.1.1. Packets that begin with a Picture Start Code
    //		A packet that begins at the location of a Picture, GOB, slice, EOS,
    //		or EOSBS start code shall omit the first two (all zero) bytes from
    //		the H.263+ bitstream and signify their presence by setting P=1 in the
    //		payload header.
    //	*/

    //	if(GN == 0x00){ /* PSC 00000 */
    //		/* Use the two first bytes as RTP header */
    //		//pdata[0] |= 0x04; // P=1

    //		/*
    //		 0                   1                   2                   3
    //		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    //		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //		|   RR    |1|V|0|0|0|0|0|0|0|0|0| bitstream data without the    :
    //		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //		: first two 0 bytes of the PSC
    //		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //		*/

    //		//TSK_DEBUG_INFO("H263 - PSC");
    //	}
    //	else if(GN == 0x1F){ /* EOS 11111 */
    //		/* Use the two first bytes as RTP header */
    //		//pdata[0] |= 0x04; // P=1
    //		eos = tsk_true;
    //		/* RFC 4629 - 6.1.3. Packets that begin with an EOS or EOSBS Code
    //			0                   1                   2
    //			0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
    //			+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //			|   RR    |1|V|0|0|0|0|0|0|0|0|0|1|1|1|1|1|1|0|0|
    //			+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //		*/
    //		//TSK_DEBUG_INFO("H263 - EOS");
    //	}
    //	else /*if((GN >> 4) == 0x01)*/{ /* GBSC  10000 */
    //		/* Use the two first bytes as RTP header */
    //		//pdata[0] |= 0x04; // P=1
    //
    //		/* RFC 4629 - 6.1.2. Packets that begin with GBSC or SSC
    //	0                   1                   2                   3
    //	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    //	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //	|   RR    |1|V|0 0 1 0 0 1|PEBIT|1 0 0 0 0 0| picture header    :
    //	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //	: starting with TR, PTYPE ...                                   |
    //	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //	| ...                                           | bitstream     :
    //	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //	: data starting with GBSC/SSC without its first two 0 bytes
    //	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //		*/
    //		//TSK_DEBUG_INFO("H263 - GBSC");
    //		found_gob = tsk_false;
    //	}
    //	//else if(EOSBS) -> Not Supported
    //}
    //else{
    //	/* 6.2. Encapsulating Follow-on Packet (P=0) */
    //	int i = 0;
    //	i++;
    //}

    //if(/*eos*/!found_gob && frag){
    //	if(self->rtp.size < (size + 2/* H263+ Header size */)){
    //		if(!(self->rtp.ptr = tsk_realloc(self->rtp.ptr, (size + 2)))){
    //			TSK_DEBUG_ERROR("Failed to allocate new buffer");
    //			return;
    //		}
    //		self->rtp.size = (size + 2);
    //	}
    //	/* RFC 4629 - 6. Packetization Schemes */
    //	//rtp_hdr[0] |= 0x00;
    //	//memcpy(self->rtp.ptr, rtp_hdr/* zeros-> is it corretc? */, 2);
    //	//memcpy((self->rtp.ptr + 2), pdata, size);
    //	//_ptr = self->rtp.ptr;
    //	//_size = (size + 2);

    //	pdata[0] |= pdata[2] > 0x80 ? 0x04 : 0x04;
    //	_ptr = pdata;
    //	_size = size;
    //}
    //else{
    //	pdata[0] |= pdata[2] > 0x80 ? 0x04 : 0x04;
    //	_ptr = pdata;
    //	_size = size;
    //}

// FIXME
    pdata[0] |= pdata[2] > 0x80 ? 0x04 : 0x04;
    _ptr = pdata;
    _size = size;


    // Send data over the network
    if(TMEDIA_CODEC_VIDEO(self)->out.callback) {
        TMEDIA_CODEC_VIDEO(self)->out.result.buffer.ptr = _ptr;
        TMEDIA_CODEC_VIDEO(self)->out.result.buffer.size = _size;
        TMEDIA_CODEC_VIDEO(self)->out.result.duration =  (uint32_t)((1./(double)TMEDIA_CODEC_VIDEO(self)->out.fps) * TMEDIA_CODEC(self)->plugin->rate);
        TMEDIA_CODEC_VIDEO(self)->out.result.last_chunck = marker;
        TMEDIA_CODEC_VIDEO(self)->out.callback(&TMEDIA_CODEC_VIDEO(self)->out.result);
    }
}

tsk_bool_t tdav_codec_ffmpeg_h263_is_supported()
{
    return (avcodec_find_encoder(CODEC_ID_H263) && avcodec_find_decoder(CODEC_ID_H263));
}

tsk_bool_t tdav_codec_ffmpeg_h263p_is_supported()
{
    return (avcodec_find_encoder(CODEC_ID_H263P) && avcodec_find_decoder(CODEC_ID_H263));
}

tsk_bool_t tdav_codec_ffmpeg_h263pp_is_supported()
{
    return tdav_codec_ffmpeg_h263p_is_supported();
}


#endif /* HAVE_FFMPEG */
