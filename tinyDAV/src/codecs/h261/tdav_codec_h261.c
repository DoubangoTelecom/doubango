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

/**@file tdav_codec_h261.c
 * @brief H.261 codec plugin.
 * RTP payloader follows RFC 4587
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinydav/codecs/h261/tdav_codec_h261.h"

#if HAVE_FFMPEG

#include "tinydav/video/tdav_converter_video.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tnet_endianness.h"

#include "tsk_string.h"
#include "tsk_time.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#define RTP_PAYLOAD_SIZE	700
#define H261_HEADER_SIZE	4

static void *run(void* self);
static void tdav_codec_h261_rtp_callback(tdav_codec_h261_t *self, const void *data, tsk_size_t size, tsk_bool_t marker);
static void tdav_codec_h261_encap(const tdav_codec_h261_t* h261, const uint8_t* pdata, tsk_size_t size);

/* ============ H.261 Plugin interface ================= */

//
//	H.261 object definition
//
int tdav_codec_h261_open(tmedia_codec_t* self)
{
    int ret;
    int size;

    tdav_codec_h261_t* h261 = (tdav_codec_h261_t*)self;

    if(!h261) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* the caller (base class) already checked that the codec is not opened */

    //
    //	Encoder
    //
    if(!(h261->encoder.codec = avcodec_find_encoder(CODEC_ID_H261))) {
        TSK_DEBUG_ERROR("Failed to find H.261 encoder");
        return -2;
    }
    h261->encoder.context = avcodec_alloc_context();
    avcodec_get_context_defaults(h261->encoder.context);

    h261->encoder.context->pix_fmt		= PIX_FMT_YUV420P;
    h261->encoder.context->time_base.num  = 1;
    h261->encoder.context->time_base.den  = TMEDIA_CODEC_VIDEO(h261)->out.fps;
    h261->encoder.context->width = TMEDIA_CODEC_VIDEO(h261)->out.width;
    h261->encoder.context->height = TMEDIA_CODEC_VIDEO(h261)->out.height;

    /*h261->encoder.context->mb_qmin =*/ h261->encoder.context->qmin = 4;
    /*h261->encoder.context->mb_qmax =*/ h261->encoder.context->qmax = 31;
    h261->encoder.context->mb_decision = FF_MB_DECISION_SIMPLE;

    h261->encoder.context->thread_count = 1;
    h261->encoder.context->rtp_payload_size = RTP_PAYLOAD_SIZE;
    h261->encoder.context->opaque = tsk_null;
    h261->encoder.context->bit_rate = (float) (500000) * 0.80f;
    h261->encoder.context->bit_rate_tolerance = (int) (500000 * 0.20f);
    h261->encoder.context->gop_size = TMEDIA_CODEC_VIDEO(h261)->out.fps*4; /* each 4 seconds */

    // Picture (YUV 420)
    if(!(h261->encoder.picture = avcodec_alloc_frame())) {
        TSK_DEBUG_ERROR("Failed to create encoder picture");
        return -2;
    }
    avcodec_get_frame_defaults(h261->encoder.picture);
    //if((ret = avpicture_alloc((AVPicture*)h261->encoder.picture, PIX_FMT_YUV420P, h261->encoder.context->width, h261->encoder.context->height))){
    //	TSK_DEBUG_ERROR("Failed to allocate encoder picture");
    //	return ret;
    //}

    size = avpicture_get_size(PIX_FMT_YUV420P, h261->encoder.context->width, h261->encoder.context->height);
    if(!(h261->encoder.buffer = tsk_calloc(size, sizeof(uint8_t)))) {
        TSK_DEBUG_ERROR("Failed to allocate encoder buffer");
        return -2;
    }

    // Open encoder
    if((ret = avcodec_open(h261->encoder.context, h261->encoder.codec)) < 0) {
        TSK_DEBUG_ERROR("Failed to open H.261 encoder");
        return ret;
    }

    //
    //	Decoder
    //
    if(!(h261->decoder.codec = avcodec_find_decoder(CODEC_ID_H261))) {
        TSK_DEBUG_ERROR("Failed to find H.261 decoder");
    }
    h261->decoder.context = avcodec_alloc_context();
    avcodec_get_context_defaults(h261->decoder.context);

    h261->decoder.context->pix_fmt = PIX_FMT_YUV420P;
    h261->decoder.context->width = TMEDIA_CODEC_VIDEO(h261)->in.width;
    h261->decoder.context->height = TMEDIA_CODEC_VIDEO(h261)->in.height;

    // Picture (YUV 420)
    if(!(h261->decoder.picture = avcodec_alloc_frame())) {
        TSK_DEBUG_ERROR("Failed to create decoder picture");
        return -2;
    }
    avcodec_get_frame_defaults(h261->decoder.picture);

    size = avpicture_get_size(PIX_FMT_YUV420P, h261->decoder.context->width, h261->decoder.context->height);
    if(!(h261->decoder.accumulator = tsk_calloc((size + FF_INPUT_BUFFER_PADDING_SIZE), sizeof(uint8_t)))) {
        TSK_DEBUG_ERROR("Failed to allocate decoder buffer");
        return -2;
    }

    // Open decoder
    if((ret = avcodec_open(h261->decoder.context, h261->decoder.codec)) < 0) {
        TSK_DEBUG_ERROR("Failed to open H.261 decoder");
        return ret;
    }

    return 0;
}

int tdav_codec_h261_close(tmedia_codec_t* self)
{
    tdav_codec_h261_t* h261 = (tdav_codec_h261_t*)self;

    if(!h261) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* the caller (base class) already checked that the codec is opened */

    //
    //	Encoder
    //
    if(h261->encoder.context) {
        avcodec_close(h261->encoder.context);
        av_free(h261->encoder.context);
        h261->encoder.context = tsk_null;
    }
    if(h261->encoder.picture) {
        av_free(h261->encoder.picture);
    }
    if(h261->encoder.buffer) {
        TSK_FREE(h261->encoder.buffer);
    }

    //
    //	Decoder
    //
    if(h261->decoder.context) {
        avcodec_close(h261->decoder.context);
        av_free(h261->decoder.context);
        h261->decoder.context = tsk_null;
    }
    if(h261->decoder.picture) {
        av_free(h261->decoder.picture);
        h261->decoder.picture = tsk_null;
    }
    if(h261->decoder.accumulator) {
        TSK_FREE(h261->decoder.accumulator);
        h261->decoder.accumulator_pos = 0;
    }

    return 0;
}

tsk_size_t tdav_codec_h261_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
    int ret;
    int size;

    tdav_codec_h261_t* h261 = (tdav_codec_h261_t*)self;

    if(!self || !in_data || !in_size || !out_data) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    // delete old buffer
    if(*out_data) {
        TSK_FREE(*out_data);
    }

    // wrap yuv420 buffer
    size = avpicture_fill((AVPicture *)h261->encoder.picture, (uint8_t*)in_data, PIX_FMT_YUV420P, h261->encoder.context->width, h261->encoder.context->height);
    if(size != in_size) {
        /* guard */
        TSK_DEBUG_ERROR("Invalid size");
        return 0;
    }

    // Encode data
    h261->encoder.picture->pts = AV_NOPTS_VALUE;
    //h261->encoder.picture->pict_type = FF_I_TYPE;
    ret = avcodec_encode_video(h261->encoder.context, h261->encoder.buffer, size, h261->encoder.picture);
    if(ret > 0) {
        tdav_codec_h261_encap(h261, h261->encoder.buffer, (tsk_size_t)ret);
    }

    return 0;
}

tsk_size_t tdav_codec_h261_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
    uint8_t sbit, ebit;
    const uint8_t* pdata = in_data;
    const uint8_t* pay_ptr;
    tsk_size_t pay_size;
    tsk_size_t xsize, retsize = 0;
    int got_picture_ptr;
    int ret;

    tdav_codec_h261_t* h261 = (tdav_codec_h261_t*)self;
    const trtp_rtp_header_t* rtp_hdr = proto_hdr;

    if(!self || !in_data || !in_size || !out_data || !h261->decoder.context) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    /* RFC 4587
    	0                   1                   2                   3
    	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    	|SBIT |EBIT |I|V| GOBN  |   MBAP  |  QUANT  |  HMVD   |  VMVD   |
    	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    */
    sbit = *pdata >> 5;
    ebit = (*pdata >> 2) & 0x07;

    /* Check size */
    if(in_size < H261_HEADER_SIZE) {
        TSK_DEBUG_ERROR("Too short");
        return 0;
    }

    pay_ptr = (pdata + H261_HEADER_SIZE);
    pay_size = (in_size - H261_HEADER_SIZE);
    xsize = avpicture_get_size(h261->decoder.context->pix_fmt, h261->decoder.context->width, h261->decoder.context->height);

    /* Packet lost? */
    if(h261->decoder.last_seq != (rtp_hdr->seq_num - 1) && h261->decoder.last_seq) {
        TSK_DEBUG_INFO("Packet lost");
    }
    h261->decoder.last_seq = rtp_hdr->seq_num;

    if((int)(h261->decoder.accumulator_pos + pay_size) <= xsize) {

        if((h261->decoder.ebit + sbit) == 8) { /* Perfect one Byte to clean up */
            if(h261->decoder.accumulator_pos) {
                ((uint8_t*)h261->decoder.accumulator)[h261->decoder.accumulator_pos-1] =
                    (((uint8_t*)h261->decoder.accumulator)[h261->decoder.accumulator_pos-1] & (0xFF << h261->decoder.ebit)) |
                    (*pay_ptr << sbit);
            }
            pay_ptr++, pay_size--;
        }
        h261->decoder.ebit = ebit;

        memcpy(&((uint8_t*)h261->decoder.accumulator)[h261->decoder.accumulator_pos], pay_ptr, pay_size);
        h261->decoder.accumulator_pos += pay_size;
    }
    else {
        TSK_DEBUG_WARN("Buffer overflow");
        h261->decoder.accumulator_pos = 0;
        return 0;
    }

    if(rtp_hdr->marker) {
        AVPacket packet;
        /* allocate destination buffer */
        if(*out_max_size <xsize) {
            if(!(*out_data = tsk_realloc(*out_data, xsize))) {
                TSK_DEBUG_ERROR("Failed to allocate new buffer");
                *out_max_size = 0;
                h261->decoder.accumulator_pos = 0;
                return 0;
            }
            *out_max_size = xsize;
        }

        /* decode the picture */
        av_init_packet(&packet);
        packet.size = (int)h261->decoder.accumulator_pos;
        packet.data = h261->decoder.accumulator;
        ret = avcodec_decode_video2(h261->decoder.context, h261->decoder.picture, &got_picture_ptr, &packet);

        if(ret <0 || !got_picture_ptr) {
            TSK_DEBUG_WARN("Failed to decode the buffer");
        }
        else {
            retsize = xsize;
            TMEDIA_CODEC_VIDEO(h261)->in.width = h261->decoder.context->width;
            TMEDIA_CODEC_VIDEO(h261)->in.height = h261->decoder.context->height;
            /* copy picture into a linear buffer */
            avpicture_layout((AVPicture *)h261->decoder.picture, h261->decoder.context->pix_fmt, (int)h261->decoder.context->width, (int)h261->decoder.context->height,
                             *out_data, (int)retsize);
        }
        /* in all cases: reset accumulator */
        h261->decoder.accumulator_pos = 0;
    }

    return retsize;
}

tsk_bool_t tdav_codec_h261_sdp_att_match(const tmedia_codec_t* codec, const char* att_name, const char* att_value)
{
    int ret;
    unsigned maxbr, fps, width, height;
    tmedia_codec_video_t* h261 = (tmedia_codec_video_t*)codec;

    if(tsk_striequals(att_value, "fmtp")) {
        if(!(ret = tmedia_codec_parse_fmtp(att_value, &maxbr, &fps, &width, &height))) {
            h261->in.max_br = h261->out.max_br = maxbr * 1000;
            h261->in.fps = h261->out.fps = fps;
            h261->in.width = h261->out.width = width;
            h261->in.height = h261->out.height = height;
            return tsk_true;
        }
        else {
            TSK_DEBUG_WARN("Failed to match fmtp [%s]", att_value);
        }
    }
    return tsk_false;
}

char* tdav_codec_h261_sdp_att_get(const tmedia_codec_t* self, const char* att_name)
{
#if 0
    return tsk_strdup("CIF=2/MaxBR=3840;QCIF=2/MaxBR=1920");
#else
    return tsk_strdup("QCIF=2");
#endif
}

/* constructor */
static tsk_object_t* tdav_codec_h261_ctor(tsk_object_t * self, va_list * app)
{
    tdav_codec_h261_t *h261 = self;
    if(h261) {
        /* init base: called by tmedia_codec_create() */
        /* init self */
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h261_dtor(tsk_object_t * self)
{
    tdav_codec_h261_t *h261 = self;
    if(h261) {
        /* deinit base */
        tmedia_codec_video_deinit(h261); // will call close()
        /* deinit self */
        TSK_FREE(h261->rtp.ptr);
        h261->rtp.size = 0;
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h261_def_s = {
    sizeof(tdav_codec_h261_t),
    tdav_codec_h261_ctor,
    tdav_codec_h261_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h261_plugin_def_s = {
    &tdav_codec_h261_def_s,

    tmedia_video,
    tmedia_codec_id_h261,
    "H261",
    "H261 codec (FFmpeg)",
    TMEDIA_CODEC_FORMAT_H261,
    tsk_false,
    90000, // rate

    /* audio */
    { 0 },

    /* video */
    {176, 144, 15},

    tsk_null, // set()
    tdav_codec_h261_open,
    tdav_codec_h261_close,
    tdav_codec_h261_encode,
    tdav_codec_h261_decode,
    tdav_codec_h261_sdp_att_match,
    tdav_codec_h261_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_h261_plugin_def_t = &tdav_codec_h261_plugin_def_s;



















/* ============ Callbacks ================= */

static void tdav_codec_h261_encap(const tdav_codec_h261_t* h261, const uint8_t* pdata, tsk_size_t size)
{
    uint32_t i, last_index = 0;

    if(size < RTP_PAYLOAD_SIZE) {
        goto last;
    }

    for(i = 4; i<(size - 4); i++) {
        if(pdata[i] == 0x00 && pdata[i+1] == 0x00 && pdata[i+2]>=0x80) { /* PSC or (GBSC) found */
            if((i - last_index) >= RTP_PAYLOAD_SIZE) {
                tdav_codec_h261_rtp_callback((tdav_codec_h261_t*)h261, pdata+last_index,
                                             (i - last_index), (last_index == size));
            }
            last_index = i;
        }
    }
last:
    if(last_index < size - 3/*PSC/GBSC size*/) {
        tdav_codec_h261_rtp_callback((tdav_codec_h261_t*)h261, pdata + last_index,
                                     (size - last_index), tsk_true);
    }
}

//static void *run(void* self)
//{
//	uint32_t i, last_index;
//	tsk_list_item_t *curr;
//
//	const uint8_t* pdata;
//	tsk_size_t size;
//
//	const tdav_codec_h261_t* h261 = ((tdav_runnable_video_t*)self)->userdata;
//
//	TSK_DEBUG_INFO("H261 thread === START");
//
//	TSK_RUNNABLE_RUN_BEGIN(self);
//
//	if((curr = TSK_RUNNABLE_POP_FIRST(self))){
//		/* 4 is sizeof(uint32_t) */
//		pdata = ((const tsk_buffer_t*)curr->data)->data;
//		size = ((const tsk_buffer_t*)curr->data)->size;
//		last_index = 0;
//
//		if(size < RTP_PAYLOAD_SIZE){
//			goto last;
//		}
//
//		for(i = 4; i<(size - 4); i++){
//			if(pdata[i] == 0x00 && pdata[i+1] == 0x00 && pdata[i+2]>=0x80){  /* PSC or (GBSC) found */
//				if((i - last_index) >= RTP_PAYLOAD_SIZE){
//					tdav_codec_h261_rtp_callback((tdav_codec_h261_t*)h261, pdata+last_index,
//						(i - last_index), (last_index == size));
//				}
//				last_index = i;
//			}
//		}
//last:
//		if(last_index < size - 3/*PSC/GBSC size*/){
//			tdav_codec_h261_rtp_callback((tdav_codec_h261_t*)h261, pdata + last_index,
//				(size - last_index), tsk_true);
//		}
//
//		tsk_object_unref(curr);
//	}
//
//	TSK_RUNNABLE_RUN_END(self);
//
//	TSK_DEBUG_INFO("H261 thread === STOP");
//
//	return tsk_null;
//}

static void tdav_codec_h261_rtp_callback(tdav_codec_h261_t *self, const void *data, tsk_size_t size, tsk_bool_t marker)
{

}

tsk_bool_t tdav_codec_ffmpeg_h261_is_supported()
{
    return /*(avcodec_find_encoder(CODEC_ID_H261) && avcodec_find_decoder(CODEC_ID_H261))*/tsk_false /* @deprecated */;
}


#endif /* HAVE_FFMPEG */