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

/**@file tdav_codec_theora.c
 * @brief Theora codec plugin
 * RTP payloader/depayloader follows draft-barbato-avt-rtp-theora-01.
 * For more information about Theora, http://www.theora.org/doc/Theora.pdf.
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinydav/codecs/theora/tdav_codec_theora.h"

#if HAVE_FFMPEG

#include "tinydav/video/tdav_converter_video.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tinymedia/tmedia_params.h"
#include "tinymedia/tmedia_defaults.h"

#include "tsk_string.h"
#include "tsk_buffer.h"
#include "tsk_time.h"
#include "tsk_params.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <libavcodec/avcodec.h>

#define THEORA_RTP_PAYLOAD_SIZE		900
#define THEORA_GOP_SIZE_IN_SECONDS	25
#define THEORA_PAYLOAD_HEADER_SIZE	4 /* 2.2. Payload Header */
#define THEORA_PAYLOAD_LENGTH_SIZE	2 /* 2.2. Payload Header */
#define THEORA_IDENT_HEADER_SIZE	42 /* 6.2 Identification Header Decode */
#define THEORA_CONF_SEND_COUNT		10 /* at 250ms, 500ms, 1000ms, ....  */

typedef struct tdav_codec_theora_s {
    TMEDIA_DECLARE_CODEC_VIDEO;

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

        uint64_t conf_last;
        int conf_count;
        tsk_bool_t force_idr;
        int quality;
        int rotation;
        int32_t max_bw_kpbs;
    } encoder;

    // decoder
    struct {
        AVCodec* codec;
        AVCodecContext* context;
        AVFrame* picture;

        tsk_bool_t opened;
        uint8_t conf_ident[3];
        tsk_buffer_t* conf_pkt;

        void* accumulator;
        uint8_t ebit;
        tsk_size_t accumulator_pos;
        uint16_t last_seq;
    } decoder;
}
tdav_codec_theora_t;


/* 2.2. Payload Header filed 'F'*/
typedef enum theora_frag_type_e {
    Not_Fragmented = 0,
    Start_Fragment = 1,
    Continuation_Fragment = 2,
    End_Fragment = 3,
}
theora_frag_type_t;

/* 2.2. Payload Header field 'TDT'*/
typedef enum theora_datatype_e {
    Raw_Theora_payload = 0,
    Theora_Packed_Configuration_payload = 1,
    Legacy_Theora_Comment_payload = 2,
    Reserved = 3,
}
theora_datatype_t;

static int tdav_codec_theora_open_encoder(tdav_codec_theora_t* self);
static int tdav_codec_theora_open_decoder(tdav_codec_theora_t* self);
static int tdav_codec_theora_close_encoder(tdav_codec_theora_t* self);
static int tdav_codec_theora_close_decoder(tdav_codec_theora_t* self);

static int tdav_codec_theora_send(tdav_codec_theora_t* self, const uint8_t* data, tsk_size_t size, theora_datatype_t tdt);
static void tdav_codec_theora_rtp_callback(tdav_codec_theora_t *self, const void *data, tsk_size_t size, tsk_bool_t marker);

static void tdav_codec_theora_encap(tdav_codec_theora_t* theora, const uint8_t* pdata, tsk_size_t size);

/* ============ Theora Plugin interface functions ================= */

static int tdav_codec_theora_set(tmedia_codec_t* self, const tmedia_param_t* param)
{
    tdav_codec_theora_t* theora = (tdav_codec_theora_t*)self;
    if(!self->opened) {
        TSK_DEBUG_ERROR("Codec not opened");
        return -1;
    }
    if(param->value_type == tmedia_pvt_int32) {
        if(tsk_striequals(param->key, "action")) {
            tmedia_codec_action_t action = (tmedia_codec_action_t)TSK_TO_INT32((uint8_t*)param->value);
            switch(action) {
            case tmedia_codec_action_encode_idr: {
                theora->encoder.force_idr = tsk_true;
                break;
            }
            case tmedia_codec_action_bw_down: {
                theora->encoder.quality = TSK_CLAMP(1, (theora->encoder.quality + 1), 31);
                theora->encoder.context->global_quality = FF_QP2LAMBDA * theora->encoder.quality;
                break;
            }
            case tmedia_codec_action_bw_up: {
                theora->encoder.quality = TSK_CLAMP(1, (theora->encoder.quality - 1), 31);
                theora->encoder.context->global_quality = FF_QP2LAMBDA * theora->encoder.quality;
                break;
            }
            }
        }
        // FIXME: not working as expected
        /*else if(tsk_striequals(param->key, "rotation")){
        	int rotation = *((int32_t*)param->value);
        	if(theora->encoder.rotation != rotation){
        		if(self->opened){
        			int ret;
        			theora->encoder.rotation = rotation;
        			if((ret = tdav_codec_theora_close_encoder(theora))){
        				return ret;
        			}
        			if((ret = tdav_codec_theora_open_encoder(theora))){
        				return ret;
        			}
        		}
        	}
        	return 0;
        }*/
    }
    return -1;
}

int tdav_codec_theora_open(tmedia_codec_t* self)
{
    int ret;

    tdav_codec_theora_t* theora = (tdav_codec_theora_t*)self;

    if(!theora) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* the caller (base class) already checked that the codec is not opened */


    //	Encoder
    if((ret = tdav_codec_theora_open_encoder(theora))) {
        return ret;
    }

    //	Decoder
    if((ret = tdav_codec_theora_open_decoder(theora))) {
        return ret;
    }

    return 0;
}

int tdav_codec_theora_close(tmedia_codec_t* self)
{
    tdav_codec_theora_t* theora = (tdav_codec_theora_t*)self;

    if(!theora) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* the caller (base class) already checked that the codec is opened */


    //	Encoder
    tdav_codec_theora_close_encoder(theora);

    //	Decoder
    tdav_codec_theora_close_decoder(theora);

    return 0;
}

//#include "tsk_time.h"
tsk_size_t tdav_codec_theora_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
    int ret;
    int size;

    tdav_codec_theora_t* theora = (tdav_codec_theora_t*)self;

    if(!self || !in_data || !in_size || !out_data) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    // wrap yuv420 buffer
    size = avpicture_fill((AVPicture *)theora->encoder.picture, (uint8_t*)in_data, PIX_FMT_YUV420P, theora->encoder.context->width, theora->encoder.context->height);
    if(size != in_size) {
        /* guard */
        TSK_DEBUG_ERROR("Invalid size");
        return 0;
    }

    // Encode data
#if LIBAVCODEC_VERSION_MAJOR <= 53
    theora->encoder.picture->pict_type = theora->encoder.force_idr ? FF_I_TYPE : 0;
#else
    theora->encoder.picture->pict_type = theora->encoder.force_idr ? AV_PICTURE_TYPE_I : AV_PICTURE_TYPE_NONE;
#endif
    theora->encoder.picture->pts = AV_NOPTS_VALUE;
    theora->encoder.picture->quality = theora->encoder.context->global_quality;
    ret = avcodec_encode_video(theora->encoder.context, theora->encoder.buffer, size, theora->encoder.picture);
    if(ret > 0) {
        tdav_codec_theora_encap(theora, theora->encoder.buffer, (tsk_size_t)ret);
    }
    theora->encoder.force_idr = tsk_false;

    return 0;
}

tsk_size_t tdav_codec_theora_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
    const uint8_t* pdata = in_data;
    int pkts;
    const uint8_t* pay_ptr;
    tsk_size_t pay_size;
    //tsk_size_t hdr_size;
    tsk_size_t xsize, retsize = 0;
    int got_picture_ptr;
    int ret;

    tdav_codec_theora_t* theora = (tdav_codec_theora_t*)self;
    const trtp_rtp_header_t* rtp_hdr = proto_hdr;

    if(!self || !in_data || (in_size<(THEORA_PAYLOAD_HEADER_SIZE + THEORA_PAYLOAD_LENGTH_SIZE)) || !out_data || !theora->decoder.context) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    /* Packet lost? */
    if(theora->decoder.last_seq != (rtp_hdr->seq_num - 1) && theora->decoder.last_seq) {
        if(theora->decoder.last_seq == rtp_hdr->seq_num) {
            // Could happen on some stupid emulators
            //TSK_DEBUG_INFO("Packet duplicated, seq_num=%d", rtp_hdr->seq_num);
            return 0;
        }
        TSK_DEBUG_INFO("Packet lost, seq_num=%d", rtp_hdr->seq_num);
    }
    theora->decoder.last_seq = rtp_hdr->seq_num;

    xsize = avpicture_get_size(theora->decoder.context->pix_fmt, theora->decoder.context->width, theora->decoder.context->height);

    /* 2.2. Payload Header
    	0                   1                   2                   3
    	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    	|               Configuration Ident             | F |TDT|# pkts.|
    	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    */
    /*	2.3. Payload Data
    	 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    	|        Payload Length         |          Theora Data         ..
    	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    */
    pkts = (pdata[3] & 0x0F);
    pay_ptr = (pdata + THEORA_PAYLOAD_HEADER_SIZE);

    do { /* pkts=0 for fragmented packets */

        pay_size = pay_ptr[0], pay_size<<=8, pay_size |= pay_ptr[1]; /* Big Endian read */
        pay_ptr += THEORA_PAYLOAD_LENGTH_SIZE;
        /* check size validity */
        if((pay_ptr + pay_size)>(pdata + in_size)) {
            TSK_DEBUG_ERROR("Too short");
            break;
        }

        switch((pdata[3]>>4) & 0x03) {
        case Raw_Theora_payload: {
            /* ====== Theora data (2.2. Payload Header, 2.3. Payload Data) ====== */
            /* append buffer */
            if((int)(theora->decoder.accumulator_pos + pay_size) <= xsize) {
                memcpy(&((uint8_t*)theora->decoder.accumulator)[theora->decoder.accumulator_pos], pay_ptr, pay_size);
                theora->decoder.accumulator_pos += pay_size;
            }
            else {
                TSK_DEBUG_WARN("Buffer overflow");
                theora->decoder.accumulator_pos = 0;
                break;
            }
            /* only take care if last packet (What about the RTP marker?) */
            if(((pdata[3]>>6) == Not_Fragmented || (pdata[3]>>6) == End_Fragment /*|| rtp_hdr->marker*/) && theora->decoder.opened) {
                AVPacket packet;
                /* Perform decoding */
                av_init_packet(&packet);
                packet.size = (int)theora->decoder.accumulator_pos;
                packet.data = theora->decoder.accumulator;
                ret = avcodec_decode_video2(theora->decoder.context, theora->decoder.picture, &got_picture_ptr, &packet);

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
                    TMEDIA_CODEC_VIDEO(theora)->in.width = theora->decoder.context->width;
                    TMEDIA_CODEC_VIDEO(theora)->in.height = theora->decoder.context->height;

                    /* allocate buffer */
                    if(*out_max_size <xsize) {
                        if((*out_data = tsk_realloc(*out_data, xsize))) {
                            *out_max_size = xsize;
                        }
                        else {
                            TSK_DEBUG_ERROR("Failed to allocate new buffer");
                            *out_max_size = 0;
                            return 0;
                        }
                    }
                    /* copy picture into a linear buffer */
                    avpicture_layout((AVPicture *)theora->decoder.picture, theora->decoder.context->pix_fmt, (int)theora->decoder.context->width, (int)theora->decoder.context->height,
                                     *out_data, (int)retsize);
                }
                /* in all cases: reset accumulator */
                theora->decoder.accumulator_pos = 0;
            }
            break;
        }
        case Theora_Packed_Configuration_payload: {
            /* ====== Configuration packet (3.1.1. Packed Configuration) ====== */
            static uint8_t __theora_comment_hdr[] = {0x81, 0x74, 0x68, 0x65, 0x6F, 0x72, 0x61,
                                                     0x00, 0x00, 0x00, 0x08, /* 4-byte length */
                                                     'd', 'o', 'u', 'b', 'a', 'n', 'g', 'o', /* UTF-8 encoded string */
                                                    };

            /*	http://www.theora.org/doc/Theora.pdf - Chapter 6
            A Theora bitstream begins with three header packets. The header packets
            are, in order, the identifcation header, the comment header, and the setup
            header. All are required for decode compliance. An end-of-packet condition
            encountered while decoding the identification or setup header packets renders
            the stream undecodable. An end-of-packet condition encountered while decode
            the comment header is a non-fatal error condition, and MAY be ignored by a
            decoder.

            Decode continues according to HEADERTYPE. The identification header
            is type 0x80, the comment header is type 0x81, and the setup header is type
            0x82.
            */
            /*TSK_DEBUG_INFO("Theora_Packed_Configuration_payload");*/

            if(!theora->decoder.opened /*|| (conf_ident changed)*/) {
                if(!theora->decoder.conf_pkt) {
                    theora->decoder.conf_pkt = tsk_buffer_create(pay_ptr, pay_size);
                }
                else {
                    tsk_buffer_append(theora->decoder.conf_pkt, pay_ptr, pay_size);
                }

                if((pdata[3]>>6) == Not_Fragmented || (pdata[3]>>6) == End_Fragment || rtp_hdr->marker) {
                    if(theora->decoder.conf_pkt->size > THEORA_IDENT_HEADER_SIZE) {
                        const uint8_t* conf_ptr = theora->decoder.conf_pkt->data;
                        int setup_size = (int)theora->decoder.conf_pkt->size - THEORA_IDENT_HEADER_SIZE;
                        int extradata_size = (2 + THEORA_IDENT_HEADER_SIZE) + (2 + setup_size) + (2 + sizeof(__theora_comment_hdr));
                        if(conf_ptr[0] == 0x80 && conf_ptr[THEORA_IDENT_HEADER_SIZE] == 0x82) { /* Do not check for 't'h'e'o'r'a' */
                            /* save configration identification */
                            memcpy(theora->decoder.conf_ident, &pdata[0], sizeof(theora->decoder.conf_ident));
                            if(theora->decoder.context->extradata) {
                                TSK_FREE(theora->decoder.context->extradata);
                            }
                            if((theora->decoder.context->extradata = tsk_calloc(extradata_size + FF_INPUT_BUFFER_PADDING_SIZE, 1))) {
                                int index = 0;
                                /* Because of endianess pb. do not use uint16_t or uint32_t */
                                theora->decoder.context->extradata[index++] = 0x00;
                                theora->decoder.context->extradata[index++] = THEORA_IDENT_HEADER_SIZE;
                                memcpy(&theora->decoder.context->extradata[index], &conf_ptr[0], THEORA_IDENT_HEADER_SIZE);
                                index += THEORA_IDENT_HEADER_SIZE;

                                theora->decoder.context->extradata[index++] = (setup_size >>8) & 0xFF;
                                theora->decoder.context->extradata[index++] = (setup_size & 0xFF);
                                memcpy(&theora->decoder.context->extradata[index], &conf_ptr[THEORA_IDENT_HEADER_SIZE], setup_size);
                                index+=setup_size;

                                theora->decoder.context->extradata[index++] = 0x00;
                                theora->decoder.context->extradata[index++] = sizeof(__theora_comment_hdr);/* <0xFF */
                                memcpy(&theora->decoder.context->extradata[index], __theora_comment_hdr, sizeof(__theora_comment_hdr));

                                theora->decoder.context->extradata_size = extradata_size;

                                if((ret = avcodec_open(theora->decoder.context, theora->decoder.codec)) == 0) {
                                    theora->decoder.opened = tsk_true;
                                }
                                else {
                                    TSK_DEBUG_ERROR("Failed to open theora decoder %d", ret);
                                    TSK_FREE(theora->decoder.context->extradata);
                                    theora->decoder.context->extradata_size = 0;
                                }
                            }
                        }
                        else {
                            TSK_DEBUG_ERROR("Invalid configuration packet");
                        }
                    }
                    else {
                        TSK_DEBUG_ERROR("Too short");
                    }
                    tsk_buffer_cleanup(theora->decoder.conf_pkt);
                }
            }
            break;
        }
        case Legacy_Theora_Comment_payload:
            /*TSK_DEBUG_INFO("Legacy_Theora_Comment_payload");*/
            break;
        case Reserved:
            /*TSK_DEBUG_INFO("Reserved");*/
            break;
        }
    }
    while(--pkts>0);



    return retsize;
}

tsk_bool_t tdav_codec_theora_sdp_att_match(const tmedia_codec_t* codec, const char* att_name, const char* att_value)
{
    tsk_bool_t ret = tsk_true; // accept decoding any size

    if(tsk_striequals(att_name, "fmtp")) {
        tsk_params_L_t* params;
        if((params = tsk_params_fromstring(att_value, ";", tsk_true))) {
            int pref_width, pref_height;
            int prop_width = tsk_params_get_param_value_as_int(params, "width");
            int prop_height = tsk_params_get_param_value_as_int(params, "height");

            if(prop_width > 0 && prop_height > 0) {
                if(tmedia_video_get_size(TMEDIA_CODEC_VIDEO(codec)->pref_size, (unsigned *)&pref_width, (unsigned *)&pref_height) != 0) {
                    TSK_OBJECT_SAFE_FREE(params);
                    return tsk_false;
                }
                TMEDIA_CODEC_VIDEO(codec)->in.width = TMEDIA_CODEC_VIDEO(codec)->out.width = TSK_MIN(pref_width, prop_width);
                TMEDIA_CODEC_VIDEO(codec)->in.height = TMEDIA_CODEC_VIDEO(codec)->out.height = TSK_MIN(pref_height, prop_height);
            }
            TSK_OBJECT_SAFE_FREE(params);
        }
    }
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

    return ret;
}

char* tdav_codec_theora_sdp_att_get(const tmedia_codec_t* codec, const char* att_name)
{
    if(tsk_striequals(att_name, "fmtp")) {
        char* fmtp = tsk_null;
        tsk_sprintf(&fmtp, "sampling=YCbCr-4:2:0; width=%u; height=%u", TMEDIA_CODEC_VIDEO(codec)->out.width, TMEDIA_CODEC_VIDEO(codec)->out.height);
        return fmtp;
    }
    else if(tsk_striequals(att_name, "imageattr")) {
        return tmedia_get_video_imageattr(TMEDIA_CODEC_VIDEO(codec)->pref_size,
                                          TMEDIA_CODEC_VIDEO(codec)->in.width, TMEDIA_CODEC_VIDEO(codec)->in.height, TMEDIA_CODEC_VIDEO(codec)->out.width, TMEDIA_CODEC_VIDEO(codec)->out.height);
    }
    return tsk_null;
}



/* constructor */
static tsk_object_t* tdav_codec_theora_ctor(tsk_object_t * self, va_list * app)
{
    tdav_codec_theora_t *theora = self;
    if(theora) {
        /* init base: called by tmedia_codec_create() */
        /* init self */
        theora->encoder.quality = 1;
        theora->encoder.max_bw_kpbs = tmedia_defaults_get_bandwidth_video_upload_max();
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_codec_theora_dtor(tsk_object_t * self)
{
    tdav_codec_theora_t *theora = self;
    if(theora) {
        /* deinit base */
        tmedia_codec_video_deinit(self);
        /* deinit self */
        TSK_OBJECT_SAFE_FREE(theora->decoder.conf_pkt);
        TSK_FREE(theora->rtp.ptr);
        theora->rtp.size = 0;
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_theora_def_s = {
    sizeof(tdav_codec_theora_t),
    tdav_codec_theora_ctor,
    tdav_codec_theora_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_theora_plugin_def_s = {
    &tdav_codec_theora_def_s,

    tmedia_video,
    tmedia_codec_id_theora,
    "theora",
    "Theora Codec",
    TMEDIA_CODEC_FORMAT_THEORA,
    tsk_true,
    90000, // rate

    /* audio */
    { 0 },

    /* video (width, height, fps)*/
    {176, 144, 0},// fps is @deprecated

    tdav_codec_theora_set,
    tdav_codec_theora_open,
    tdav_codec_theora_close,
    tdav_codec_theora_encode,
    tdav_codec_theora_decode,
    tdav_codec_theora_sdp_att_match,
    tdav_codec_theora_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_theora_plugin_def_t = &tdav_codec_theora_plugin_def_s;



int tdav_codec_theora_open_encoder(tdav_codec_theora_t* self)
{
    int ret, size;
    int32_t max_bw_kpbs;
    if(!self->encoder.codec && !(self->encoder.codec = avcodec_find_encoder(CODEC_ID_THEORA))) {
        TSK_DEBUG_ERROR("Failed to find Theora encoder");
        return -1;
    }
    if(self->encoder.context) {
        TSK_DEBUG_ERROR("Encoder already initialized");
        return -1;
    }
    self->encoder.context = avcodec_alloc_context();
    avcodec_get_context_defaults(self->encoder.context);

    self->encoder.context->pix_fmt		= PIX_FMT_YUV420P;
    self->encoder.context->time_base.num  = 1;
    self->encoder.context->time_base.den  = TMEDIA_CODEC_VIDEO(self)->out.fps;
    self->encoder.context->width = (self->encoder.rotation == 90 || self->encoder.rotation == 270) ? TMEDIA_CODEC_VIDEO(self)->out.height : TMEDIA_CODEC_VIDEO(self)->out.width;
    self->encoder.context->height = (self->encoder.rotation == 90 || self->encoder.rotation == 270) ? TMEDIA_CODEC_VIDEO(self)->out.width : TMEDIA_CODEC_VIDEO(self)->out.height;
    self->encoder.context->mb_decision = FF_MB_DECISION_RD;

    // Theoraenc doesn't honor 'CODEC_FLAG_QSCALE'
    max_bw_kpbs = TSK_CLAMP(
                      0,
                      tmedia_get_video_bandwidth_kbps_2(TMEDIA_CODEC_VIDEO(self)->out.width, TMEDIA_CODEC_VIDEO(self)->out.height, TMEDIA_CODEC_VIDEO(self)->out.fps),
                      self->encoder.max_bw_kpbs
                  );
    self->encoder.context->bit_rate = (max_bw_kpbs * 1024);// bps
#if LIBAVCODEC_VERSION_MAJOR <= 53
    self->encoder.context->rc_lookahead = 0;
#endif
    self->encoder.context->global_quality = FF_QP2LAMBDA * self->encoder.quality;

    self->encoder.context->thread_count = 0;
    self->encoder.context->rtp_payload_size = THEORA_RTP_PAYLOAD_SIZE;
    self->encoder.context->opaque = tsk_null;
    self->encoder.context->gop_size = (TMEDIA_CODEC_VIDEO(self)->out.fps * THEORA_GOP_SIZE_IN_SECONDS);

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
        TSK_DEBUG_ERROR("Failed to open Theora encoder");
        return ret;
    }

    self->encoder.conf_last = 0;
    self->encoder.conf_count = 0;

    TSK_DEBUG_INFO("[THEORA] bitrate=%d bps", self->encoder.context->bit_rate);

    return ret;
}

int tdav_codec_theora_open_decoder(tdav_codec_theora_t* self)
{
    int size;
    if(!self->decoder.codec && !(self->decoder.codec = avcodec_find_decoder(CODEC_ID_THEORA))) {
        TSK_DEBUG_ERROR("Failed to find Theora decoder");
        return -1;
    }
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

    if(!(self->decoder.accumulator = tsk_calloc((size + FF_INPUT_BUFFER_PADDING_SIZE), sizeof(uint8_t)))) {
        TSK_DEBUG_ERROR("Failed to allocate decoder buffer");
        return -2;
    }

    // Open decoder
    //if((ret = avcodec_open(self->decoder.context, self->decoder.codec)) < 0){
    //	TSK_DEBUG_ERROR("Failed to open Theora decoder");
    //	return ret;
    //}

    return 0;
}

int tdav_codec_theora_close_encoder(tdav_codec_theora_t* self)
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

int tdav_codec_theora_close_decoder(tdav_codec_theora_t* self)
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

static void tdav_codec_theora_encap(tdav_codec_theora_t* theora, const uint8_t* pdata, tsk_size_t size)
{
    if((theora->encoder.conf_count < THEORA_CONF_SEND_COUNT) && theora->encoder.context && theora->encoder.context->extradata) {
        if((theora->encoder.conf_last + (250 *theora->encoder.conf_count)) < tsk_time_now()) {
            int hdr_size, i, exd_size = theora->encoder.context->extradata_size, conf_pkt_size = 0;
            uint8_t *conf_pkt_ptr = tsk_null, *exd_ptr = theora->encoder.context->extradata;
            for(i=0; i<3 && exd_size; i++) {
                hdr_size = exd_ptr[0], hdr_size<<=8, hdr_size |= exd_ptr[1];
                exd_ptr += 2;
                exd_size -= 2;
                if(hdr_size > exd_size) {
                    TSK_DEBUG_ERROR("Invalid extradata");
                    TSK_FREE(conf_pkt_ptr);
                    conf_pkt_size = 0;
                }

                if(exd_ptr[0] == 0x80 || exd_ptr[0] == 0x82) { /* Ignore 'comment' which is equal to '0x81' */
                    if((conf_pkt_ptr = tsk_realloc(conf_pkt_ptr, (conf_pkt_size + hdr_size)))) {
                        memcpy((conf_pkt_ptr + conf_pkt_size), exd_ptr, hdr_size);
                        conf_pkt_size += hdr_size;
                    }
                }
                exd_size -= hdr_size;
                exd_ptr += hdr_size;
            }

            /* Send the conf pack */
            if(conf_pkt_ptr && conf_pkt_size) {
                /*TSK_DEBUG_INFO("Sending Configuration Packet");*/
                tdav_codec_theora_send(theora, conf_pkt_ptr, conf_pkt_size, Theora_Packed_Configuration_payload);
                TSK_FREE(conf_pkt_ptr);
            }

            theora->encoder.conf_last = tsk_time_now();
            theora->encoder.conf_count++;
        }
    }

    /* Send Theora Raw data */
    tdav_codec_theora_send(theora, pdata, size, Raw_Theora_payload);
}

int tdav_codec_theora_send(tdav_codec_theora_t* self, const uint8_t* data, tsk_size_t size, theora_datatype_t tdt)
{
    /* 2.2. Payload Header
    	0                   1                   2                   3
    	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    	|               Configuration Ident             | F |TDT|# pkts.|
    	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    */
    uint8_t pay_hdr[THEORA_PAYLOAD_HEADER_SIZE/*4*/ + THEORA_PAYLOAD_LENGTH_SIZE/*2*/] = {0x01, 0x19, 0x83, 0x00, 0x00, 0x00};
    //uint8_t* pay_ptr = tsk_null;
    tsk_size_t pay_size;
    tsk_bool_t frag, first = tsk_true;

    pay_hdr[3] = (tdt & 0xFF) <<4;

    /* whether the packet will be fragmented or not */
    frag = (size > THEORA_RTP_PAYLOAD_SIZE);

    while(size) {
        pay_size = TSK_MIN(THEORA_RTP_PAYLOAD_SIZE, size);
        pay_hdr[4] = (uint8_t)(pay_size >> 8), pay_hdr[5] = (uint8_t)(pay_size & 0xFF);

        if(frag) {
            if(first) {
                first = tsk_false;
                pay_hdr[3] &= 0x3F, pay_hdr[3] |= (Start_Fragment <<6);
            }
            else { /* could not be 'first' and 'last' */
                if(size<=THEORA_RTP_PAYLOAD_SIZE) {
                    /* Last frag */
                    pay_hdr[3] &= 0x3F, pay_hdr[3] |= (End_Fragment <<6);
                }
                else {
                    /* Continuation frag */
                    pay_hdr[3] &= 0x3F, pay_hdr[3] |= (Continuation_Fragment <<6);
                }
            }
        }
        else {
            pay_hdr[3] |= 0x01; /* 'pkts' */
            pay_hdr[3] &= 0x3F, pay_hdr[3] |= (Not_Fragmented <<6);
        }

        if(self->rtp.size < (pay_size + sizeof(pay_hdr))) {
            if(!(self->rtp.ptr = tsk_realloc(self->rtp.ptr, (pay_size + sizeof(pay_hdr))))) {
                TSK_DEBUG_ERROR("Failed to allocate new buffer");
                return -2;
            }
            self->rtp.size = (pay_size + sizeof(pay_hdr));
        }

        memcpy(self->rtp.ptr, pay_hdr, sizeof(pay_hdr));
        memcpy((self->rtp.ptr + sizeof(pay_hdr)), data, pay_size);
        data += pay_size;
        size -= pay_size;

        // Send data over the network
        if(TMEDIA_CODEC_VIDEO(self)->out.callback) {
            TMEDIA_CODEC_VIDEO(self)->out.result.buffer.ptr = self->rtp.ptr;
            TMEDIA_CODEC_VIDEO(self)->out.result.buffer.size = (pay_size + sizeof(pay_hdr));
            TMEDIA_CODEC_VIDEO(self)->out.result.duration =  (1./(double)TMEDIA_CODEC_VIDEO(self)->out.fps) * TMEDIA_CODEC(self)->plugin->rate;
            TMEDIA_CODEC_VIDEO(self)->out.result.last_chunck = (size == 0);
            TMEDIA_CODEC_VIDEO(self)->out.callback(&TMEDIA_CODEC_VIDEO(self)->out.result);
        }
    }

    return 0;
}

tsk_bool_t tdav_codec_ffmpeg_theora_is_supported()
{
    return (avcodec_find_encoder(CODEC_ID_THEORA) && avcodec_find_decoder(CODEC_ID_THEORA));
}

#endif /* HAVE_FFMPEG */