/*
* Copyright (C) 2009 Mamadou Diop.
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
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/codecs/theora/tdav_codec_theora.h"

#include "tinydav/video/tdav_converter_video.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tsk_params.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#define THEORA_RTP_PAYLOAD_SIZE		700
#define THEORA_PAYLOAD_HEADER_SIZE	4 /* 2.2. Payload Header */
#define THEORA_IDENT_HEADER_SIZE	42 /* 6.2 Identification Header Decode */
#define THEORA_

/* 2.2. Payload Header filed 'F'*/
typedef enum theora_frag_type_e{
	Not_Fragmented = 0,
	Start_Fragment = 1,
	Continuation_Fragment = 2,
	End_Fragment = 3,
}
theora_frag_type_t;

/* 2.2. Payload Header field 'TDT'*/
typedef enum theora_datatype_e{
	Raw_Theora_payload = 0,
	Theora_Packed_Configuration_payload = 1,
	Legacy_Theora_Comment_payload = 2,
	Reserved = 3,
}
theora_datatype_t;

static void *run(void* self);
static void tdav_codec_theora_rtp_callback(tdav_codec_theora_t *self, const void *data, tsk_size_t size, tsk_bool_t marker);

/* ============ Theora Plugin interface functions ================= */

#define tdav_codec_theora_fmtp_set tsk_null

int tdav_codec_theora_open(tmedia_codec_t* self)
{
	int ret;
	int size;
	uint8_t* extradata = tsk_null;

	tdav_codec_theora_t* theora = (tdav_codec_theora_t*)self;

	if(!theora){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* the caller (base class) already checked that the codec is not opened */

	//
	//	Runnable
	//
	theora->runnable = tdav_runnable_video_create(run, theora);
	if((ret = tdav_runnable_video_start(theora->runnable))){
		TSK_DEBUG_ERROR("Failed to create runnable (H263 codec)");
		return ret;
	}

	//
	//	Encoder
	//
	if(!(theora->encoder.codec = avcodec_find_encoder(CODEC_ID_THEORA))){
		TSK_DEBUG_ERROR("Failed to find Theora encoder");
		return -2;
	}
	theora->encoder.context = avcodec_alloc_context();
	avcodec_get_context_defaults(theora->encoder.context);
	
	theora->encoder.context->pix_fmt		= PIX_FMT_YUV420P;
	theora->encoder.context->time_base.num  = 1;
	theora->encoder.context->time_base.den  = TMEDIA_CODEC_VIDEO(theora)->fps;
	theora->encoder.context->width = TMEDIA_CODEC_VIDEO(theora)->width;
	theora->encoder.context->height = TMEDIA_CODEC_VIDEO(theora)->height;

	theora->encoder.context->mb_qmin = theora->encoder.context->qmin = 4;
	theora->encoder.context->mb_qmax = theora->encoder.context->qmax = 31;
	theora->encoder.context->mb_decision = FF_MB_DECISION_SIMPLE;

	theora->encoder.context->thread_count = 1;
	theora->encoder.context->rtp_payload_size = THEORA_RTP_PAYLOAD_SIZE;
	theora->encoder.context->opaque = tsk_null;
	theora->encoder.context->bit_rate = (float) (500000) * 0.80f;
	theora->encoder.context->bit_rate_tolerance = (int) (500000 * 0.20f);
	theora->encoder.context->gop_size = TMEDIA_CODEC_VIDEO(theora)->fps*4; // each 4 seconds

	// Picture (YUV 420)
	if(!(theora->encoder.picture = avcodec_alloc_frame())){
		TSK_DEBUG_ERROR("Failed to create encoder picture");
		return -2;
	}
	avcodec_get_frame_defaults(theora->encoder.picture);
	
	size = avpicture_get_size(PIX_FMT_YUV420P, theora->encoder.context->width, theora->encoder.context->height);
	if(!(theora->encoder.buffer = tsk_calloc(size, sizeof(uint8_t)))){
		TSK_DEBUG_ERROR("Failed to allocate encoder buffer");
		return -2;
	}
	
	// Open encoder
	if((ret = avcodec_open(theora->encoder.context, theora->encoder.codec)) < 0){
		TSK_DEBUG_ERROR("Failed to open Theora encoder");
		return ret;
	}

	//
	//	Decoder
	//
	if(!(theora->decoder.codec = avcodec_find_decoder(CODEC_ID_THEORA))){
		TSK_DEBUG_ERROR("Failed to find Theora decoder");
	}
	theora->decoder.context = avcodec_alloc_context();
	avcodec_get_context_defaults(theora->decoder.context);
	
	theora->decoder.context->pix_fmt = PIX_FMT_YUV420P;
	theora->decoder.context->width = TMEDIA_CODEC_VIDEO(theora)->width;
	theora->decoder.context->height = TMEDIA_CODEC_VIDEO(theora)->height;

	// Picture (YUV 420)
	if(!(theora->decoder.picture = avcodec_alloc_frame())){
		TSK_DEBUG_ERROR("Failed to create decoder picture");
		return -2;
	}
	avcodec_get_frame_defaults(theora->decoder.picture);

	size = avpicture_get_size(PIX_FMT_YUV420P, theora->decoder.context->width, theora->decoder.context->height);
	if(!(theora->decoder.accumulator = tsk_calloc((size + FF_INPUT_BUFFER_PADDING_SIZE), sizeof(uint8_t)))){
		TSK_DEBUG_ERROR("Failed to allocate decoder buffer");
		return -2;
	}

	if(!(theora->decoder.accumulator = tsk_calloc((size + FF_INPUT_BUFFER_PADDING_SIZE), sizeof(uint8_t)))){
		TSK_DEBUG_ERROR("Failed to allocate decoder buffer");
		return -2;
	}
	/*	http://www.theora.org/doc/Theora.pdf - Chapter 6
		A Theora bitstream begins with three header packets. The header packets
		are, in order, the identifcation header, the comment header, and the setup
		header. All are required for decode compliance. An end-of-packet condition
		encountered while decoding the identification or setup header packets renders
		the stream undecodable. An end-of-packet condition encountered while decode
		the comment header is a non-fatal error condition, and MAY be ignored by a
		decoder.

		Decode continues according to HEADERTYPE. The identication header
		is type 0x80, the comment header is type 0x81, and the setup header is type
		0x82.
	*/

	// Open decoder
	//if((ret = avcodec_open(theora->decoder.context, theora->decoder.codec)) < 0){
	//	TSK_DEBUG_ERROR("Failed to open Theora decoder");
	//	return ret;
	//}

	return 0;
}

int tdav_codec_theora_close(tmedia_codec_t* self)
{

	//if(theora->decoder.context->extradata){
	//	TSK_FREE(theora->decoder.context->extradata);
	//	theora->decoder.context->extradata_size = 0;
	//}
	return 0;
}

tsk_size_t tdav_codec_theora_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
{
	return 0;
}

tsk_size_t tdav_codec_theora_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, const tsk_object_t* proto_hdr)
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

	if(!self || !in_data || (in_size<(THEORA_PAYLOAD_HEADER_SIZE + 2/*"Payload Length" field*/)) || !out_data || !theora->decoder.context){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	// delete old buffer
	if(*out_data){
		TSK_FREE(*out_data);
	}

	/* Packet lost? */
	if(theora->decoder.last_seq != (rtp_hdr->seq_num - 1) && theora->decoder.last_seq){
		TSK_DEBUG_INFO("Packet lost");
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

	do{ /* pkts=0 for fragmented packets */
		
		pay_size = pay_ptr[0]; pay_size<<=8, pay_size |= pay_ptr[1]; /* Big Endian read */
		pay_ptr += 2 /* "Payload Length" field */;
		/* check size validity */
		if((pay_ptr + pay_size)>(pdata + in_size)){
			TSK_DEBUG_ERROR("Too short");
			break;
		}

		switch((pdata[3]>>4) & 0x03){
			case Raw_Theora_payload:
				{	/* ====== Theora data (2.2. Payload Header, 2.3. Payload Data) ====== */
					/* append buffer */
					if((int)(theora->decoder.accumulator_pos + pay_size) <= xsize){
						memcpy(&((uint8_t*)theora->decoder.accumulator)[theora->decoder.accumulator_pos], pay_ptr, pay_size);
						theora->decoder.accumulator_pos += pay_size;
					}
					else{
						TSK_DEBUG_WARN("Buffer overflow");
						theora->decoder.accumulator_pos = 0;
						break;
					}
					/* only take care if last packet (What about the RTP marker?) */
					if(((pdata[3]>>6) == Not_Fragmented || (pdata[3]>>6) == End_Fragment /*|| rtp_hdr->marker*/) && theora->decoder.opened){
						AVPacket packet;
						/* Perform decoding */
						av_init_packet(&packet);
						packet.size = theora->decoder.accumulator_pos;
						packet.data = theora->decoder.accumulator;
						ret = avcodec_decode_video2(theora->decoder.context, theora->decoder.picture, &got_picture_ptr, &packet);
						
						if(ret <0 || !got_picture_ptr){
							TSK_DEBUG_WARN("Failed to decode the buffer");
						}
						else{
							retsize = xsize;
#if FLIP_DECODED_PICT
							tdav_converter_video_flip(theora->decoder.picture, theora->decoder.context->height);
#endif
							/* allocate buffer */
							if((*out_data = tsk_calloc(xsize, sizeof(uint8_t)))){
								/* copy picture into a linear buffer */
								avpicture_layout((AVPicture *)theora->decoder.picture, theora->decoder.context->pix_fmt, theora->decoder.context->width, theora->decoder.context->height,
									*out_data, retsize);
							}
							else{
								TSK_DEBUG_ERROR("Failed to allocate new buffer");
							}
						}
						/* in all cases: reset accumulator */
						theora->decoder.accumulator_pos = 0;
					}
					break;
				}
			case Theora_Packed_Configuration_payload:
				{/* ====== Configuration packet (3.1.1. Packed Configuration) ====== */
					static uint8_t __theora_comment_hdr[] = {0x81, 0x74, 0x68, 0x65, 0x6F, 0x72, 0x61,
						0x00, 0x00, 0x00, 0x08, /* 4-byte length */
						'd', 'o', 'u', 'b', 'a', 'n', 'g', 'o', /* UTF-8 encoded string */
					};

					TSK_DEBUG_INFO("Theora_Packed_Configuration_payload");

					if(!theora->decoder.opened /*|| (conf_ident changed)*/){
						if(!theora->decoder.conf_pkt){
							theora->decoder.conf_pkt = tsk_buffer_create(pay_ptr, pay_size);
						}
						else{
							tsk_buffer_append(theora->decoder.conf_pkt, pay_ptr, pay_size);
						}

						if((pdata[3]>>6) == Not_Fragmented || (pdata[3]>>6) == End_Fragment || rtp_hdr->marker){
							if(theora->decoder.conf_pkt->size > THEORA_IDENT_HEADER_SIZE){
								const uint8_t* conf_ptr = theora->decoder.conf_pkt->data;
								int setup_size = theora->decoder.conf_pkt->size - THEORA_IDENT_HEADER_SIZE;
								int extradata_size = (2 + THEORA_IDENT_HEADER_SIZE) + (2 + setup_size) + (2 + sizeof(__theora_comment_hdr));
								if(conf_ptr[0] == 0x80 && conf_ptr[THEORA_IDENT_HEADER_SIZE] == 0x82){ /* Do not check for 't'h'e'o'r'a' */
									/* save configration identification */
									memcpy(theora->decoder.conf_ident, &pdata[0], sizeof(theora->decoder.conf_ident));
									if(theora->decoder.context->extradata){
										TSK_FREE(theora->decoder.context->extradata);
									}
									if((theora->decoder.context->extradata = tsk_calloc(extradata_size + FF_INPUT_BUFFER_PADDING_SIZE, 1))){
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

										if((ret = avcodec_open(theora->decoder.context, theora->decoder.codec)) == 0){
											theora->decoder.opened = tsk_true;
										}
										else{
											TSK_DEBUG_ERROR("Failed to open theora decoder %d", ret);
											TSK_FREE(theora->decoder.context->extradata);
											theora->decoder.context->extradata_size = 0;
										}
									}
								}
								else{
									TSK_DEBUG_ERROR("Invalid configuration packet");
								}
							}
							else{
								TSK_DEBUG_ERROR("Too short");
							}
							tsk_buffer_cleanup(theora->decoder.conf_pkt);
						}
					}
					break;
				}
			case Legacy_Theora_Comment_payload:
				TSK_DEBUG_INFO("Legacy_Theora_Comment_payload");
				break;
			case Reserved:
				TSK_DEBUG_INFO("Reserved");
				break;
		}
	}
	while(--pkts>0);

	

	return retsize;
}

tsk_bool_t tdav_codec_theora_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{
	return tsk_true;
}

char* tdav_codec_theora_fmtp_get(const tmedia_codec_t* self)
{
	return tsk_strdup("sampling=YCbCr-4:2:0; width=176; height=144");
}



/* constructor */
static tsk_object_t* tdav_codec_theora_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_theora_t *theora = self;
	if(theora){
		/* init base: called by tmedia_codec_create() */
		/* init self */
		//TSK_OBJECT_SAFE_FREE(theora->conf_data);
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_theora_dtor(tsk_object_t * self)
{ 
	tdav_codec_theora_t *theora = self;
	if(theora){
		/* deinit base */
		tmedia_codec_video_deinit(self);
		/* deinit self */
		TSK_OBJECT_SAFE_FREE(theora->decoder.conf_pkt);
		
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_theora_def_s = 
{
	sizeof(tdav_codec_theora_t),
	tdav_codec_theora_ctor, 
	tdav_codec_theora_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_theora_plugin_def_s = 
{
	&tdav_codec_theora_def_s,

	tmedia_video,
	"theora",
	"Theora Codec",
	TMEDIA_CODEC_FORMAT_THEORA,
	tsk_true,
	90000, // rate
	
	/* audio */
	{ 0 },

	/* video */
	{176, 144, 15},

	tdav_codec_theora_open,
	tdav_codec_theora_close,
	tdav_codec_theora_encode,
	tdav_codec_theora_decode,
	tdav_codec_theora_fmtp_match,
	tdav_codec_theora_fmtp_get,
	tdav_codec_theora_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_theora_plugin_def_t = &tdav_codec_theora_plugin_def_s;




static void *run(void* self)
{
	return tsk_null;
}