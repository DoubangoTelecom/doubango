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

#if HAVE_FFMPEG && (!defined(HAVE_THEORA) || HAVE_THEORA)

#include "tinydav/video/tdav_converter_video.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tsk_time.h"
#include "tsk_params.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#define THEORA_RTP_PAYLOAD_SIZE		900
#define THEORA_PAYLOAD_HEADER_SIZE	4 /* 2.2. Payload Header */
#define THEORA_PAYLOAD_LENGTH_SIZE	2 /* 2.2. Payload Header */
#define THEORA_IDENT_HEADER_SIZE	42 /* 6.2 Identification Header Decode */
#define THEORA_CONF_SEND_COUNT		10 /* at 250ms, 500ms, 1000ms, ....  */

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

static int tdav_codec_theora_send(tdav_codec_theora_t* self, const uint8_t* data, tsk_size_t size, theora_datatype_t tdt);
static void tdav_codec_theora_rtp_callback(tdav_codec_theora_t *self, const void *data, tsk_size_t size, tsk_bool_t marker);

static void tdav_codec_theora_encap(tdav_codec_theora_t* theora, const uint8_t* pdata, tsk_size_t size);

/* ============ Theora Plugin interface functions ================= */

#define tdav_codec_theora_fmtp_set tsk_null

int tdav_codec_theora_open(tmedia_codec_t* self)
{
	int ret;
	int size;
	float bitRate = 64000.f;
	
	tdav_codec_theora_t* theora = (tdav_codec_theora_t*)self;

	if(!theora){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* the caller (base class) already checked that the codec is not opened */

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
	theora->encoder.context->time_base.den  = TMEDIA_CODEC_VIDEO(theora)->out.fps;
	theora->encoder.context->width = TMEDIA_CODEC_VIDEO(theora)->out.width;
	theora->encoder.context->height = TMEDIA_CODEC_VIDEO(theora)->out.height;
	
	theora->encoder.context->mb_decision = FF_MB_DECISION_RD;
	
	// Theoraenc doesn't honor 'CODEC_FLAG_QSCALE'
	switch(self->bl){
		case tmedia_bl_low:
		default:
			bitRate = 72000.f;
			break;
		case tmedia_bl_medium:
			bitRate = 150000.f;
			break;
		case tmedia_bl_hight:
			bitRate = 320000.f;
			break;
		case tmedia_bl_unrestricted:
			bitRate = 510000.f;
			break;
	}

	theora->encoder.context->thread_count = 1;
	theora->encoder.context->rtp_payload_size = THEORA_RTP_PAYLOAD_SIZE;
	theora->encoder.context->opaque = tsk_null;
	theora->encoder.context->bit_rate = (int) (bitRate * 0.80f);
	theora->encoder.context->bit_rate_tolerance = (int) (bitRate * 0.20f);
	theora->encoder.context->gop_size = TMEDIA_CODEC_VIDEO(theora)->out.fps*2; // each 2 seconds

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
		return -2;
	}
	theora->decoder.context = avcodec_alloc_context();
	avcodec_get_context_defaults(theora->decoder.context);
	
	theora->decoder.context->pix_fmt = PIX_FMT_YUV420P;
	theora->decoder.context->width = TMEDIA_CODEC_VIDEO(theora)->in.width;
	theora->decoder.context->height = TMEDIA_CODEC_VIDEO(theora)->in.height;

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

	// Open decoder
	//if((ret = avcodec_open(theora->decoder.context, theora->decoder.codec)) < 0){
	//	TSK_DEBUG_ERROR("Failed to open Theora decoder");
	//	return ret;
	//}

	return 0;
}

int tdav_codec_theora_close(tmedia_codec_t* self)
{
	tdav_codec_theora_t* theora = (tdav_codec_theora_t*)self;

	if(!theora){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* the caller (base class) already checked that the codec is opened */

	//
	//	Encoder
	//
	if(theora->encoder.context){
		avcodec_close(theora->encoder.context);
		av_free(theora->encoder.context);
		theora->encoder.context = tsk_null;
	}
	if(theora->encoder.picture){
		av_free(theora->encoder.picture);
	}
	if(theora->encoder.buffer){
		TSK_FREE(theora->encoder.buffer);
	}

	//
	//	Decoder
	//
	if(theora->decoder.context){
		avcodec_close(theora->decoder.context);
		if(theora->decoder.context->extradata){
			TSK_FREE(theora->decoder.context->extradata);
			theora->decoder.context->extradata_size = 0;
		}
		av_free(theora->decoder.context);
		theora->decoder.context = tsk_null;
	}
	if(theora->decoder.picture){
		av_free(theora->decoder.picture);
		theora->decoder.picture = tsk_null;
	}
	if(theora->decoder.accumulator){
		TSK_FREE(theora->decoder.accumulator);
	}

	return 0;
}

//#include "tsk_time.h"
tsk_size_t tdav_codec_theora_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
	int ret;
	int size;

	tdav_codec_theora_t* theora = (tdav_codec_theora_t*)self;

	if(!self || !in_data || !in_size || !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	// wrap yuv420 buffer
	size = avpicture_fill((AVPicture *)theora->encoder.picture, (uint8_t*)in_data, PIX_FMT_YUV420P, theora->encoder.context->width, theora->encoder.context->height);
	if(size != in_size){
		/* guard */
		TSK_DEBUG_ERROR("Invalid size");
		return 0;
	}
	
	// Flip 
	if(self->video.flip.encoded){
		tdav_converter_video_flip(theora->encoder.picture, theora->encoder.context->height);
	}
	
	// Encode data
	//theora->encoder.picture->pts = tsk_time_epoch();
	theora->encoder.picture->pts = AV_NOPTS_VALUE;
	//theora->encoder.picture->pict_type = FF_I_TYPE;
	ret = avcodec_encode_video(theora->encoder.context, theora->encoder.buffer, size, theora->encoder.picture);
	if(ret > 0){
		tdav_codec_theora_encap(theora, theora->encoder.buffer, (tsk_size_t)ret);
	}

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

	if(!self || !in_data || (in_size<(THEORA_PAYLOAD_HEADER_SIZE + THEORA_PAYLOAD_LENGTH_SIZE)) || !out_data || !theora->decoder.context){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	/* Packet lost? */
	if(theora->decoder.last_seq != (rtp_hdr->seq_num - 1) && theora->decoder.last_seq){
		if(theora->decoder.last_seq == rtp_hdr->seq_num){
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

	do{ /* pkts=0 for fragmented packets */
		
		pay_size = pay_ptr[0], pay_size<<=8, pay_size |= pay_ptr[1]; /* Big Endian read */
		pay_ptr += THEORA_PAYLOAD_LENGTH_SIZE;
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
							TMEDIA_CODEC_VIDEO(theora)->in.width = theora->decoder.context->width;
							TMEDIA_CODEC_VIDEO(theora)->in.height = theora->decoder.context->height;
							if(self->video.flip.decoded){
								tdav_converter_video_flip(theora->decoder.picture, theora->decoder.context->height);
							}
							/* allocate buffer */
							if(*out_max_size <xsize){
								if((*out_data = tsk_realloc(*out_data, xsize))){
									*out_max_size = xsize;
								}
								else{
									TSK_DEBUG_ERROR("Failed to allocate new buffer");
									*out_max_size = 0;
									return 0;
								}
							}
							/* copy picture into a linear buffer */
							avpicture_layout((AVPicture *)theora->decoder.picture, theora->decoder.context->pix_fmt, theora->decoder.context->width, theora->decoder.context->height,
								*out_data, retsize);
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

tsk_bool_t tdav_codec_theora_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{
	tsk_bool_t ret = tsk_true; // accept decoding any size
	tsk_params_L_t* params = tsk_null;
	tmedia_codec_video_t* theora = (tmedia_codec_video_t*)codec;

	if((params = tsk_params_fromstring(fmtp, ";", tsk_true))){
		int width = tsk_params_get_param_value_as_int(params, "width");
		int height = tsk_params_get_param_value_as_int(params, "height");		
		
		// Set Encoding size
		switch(codec->bl){
			case tmedia_bl_low:
			default:
				if(width<=176 && height<=144){
					theora->in.width = theora->out.width = width, theora->in.height = theora->out.height = height;
				}
				else{
					theora->in.width = theora->out.width = 176, theora->in.height = theora->out.height = 144;
				}
				break;

			case tmedia_bl_medium:
			case tmedia_bl_hight:
			case tmedia_bl_unrestricted:
				if(width<=352 && height<=288){
					theora->in.width = theora->out.width = width, theora->in.height = theora->out.height = height;
				}
				else{
					theora->in.width = theora->out.width = 352, theora->in.height = theora->out.height = 288;
				}
				break;
		}
	}
	TSK_OBJECT_SAFE_FREE(params);

	return ret;
}

char* tdav_codec_theora_fmtp_get(const tmedia_codec_t* self)
{
	switch(self->bl){
		case tmedia_bl_low:
		default:
			return tsk_strdup("sampling=YCbCr-4:2:0; width=176; height=144");
			break;
		case tmedia_bl_medium:
		case tmedia_bl_hight:
			return tsk_strdup("sampling=YCbCr-4:2:0; width=352; height=288");
			break;
	}
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
		TSK_FREE(theora->rtp.ptr);
		theora->rtp.size = 0;
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



static void tdav_codec_theora_encap(tdav_codec_theora_t* theora, const uint8_t* pdata, tsk_size_t size)
{
	if((theora->encoder.conf_count < THEORA_CONF_SEND_COUNT) && theora->encoder.context && theora->encoder.context->extradata){
		if((theora->encoder.conf_last + (250 *theora->encoder.conf_count)) < tsk_time_epoch()){
			int hdr_size, i, exd_size = theora->encoder.context->extradata_size, conf_pkt_size = 0;
			uint8_t *conf_pkt_ptr = tsk_null, *exd_ptr = theora->encoder.context->extradata;
			for(i=0; i<3 && exd_size; i++){
				hdr_size = exd_ptr[0], hdr_size<<=8, hdr_size |= exd_ptr[1];
				exd_ptr += 2;
				exd_size -= 2;
				if(hdr_size > exd_size){
					TSK_DEBUG_ERROR("Invalid extradata");
					TSK_FREE(conf_pkt_ptr);
					conf_pkt_size = 0;
				}

				if(exd_ptr[0] == 0x80 || exd_ptr[0] == 0x82){ /* Ignore 'comment' which is equal to '0x81' */
					if((conf_pkt_ptr = tsk_realloc(conf_pkt_ptr, (conf_pkt_size + hdr_size)))){
						memcpy((conf_pkt_ptr + conf_pkt_size), exd_ptr, hdr_size);
						conf_pkt_size += hdr_size;
					}
				}
				exd_size -= hdr_size;
				exd_ptr += hdr_size;
			}

			/* Send the conf pack */
			if(conf_pkt_ptr && conf_pkt_size){
				/*TSK_DEBUG_INFO("Sending Configuration Packet");*/
				tdav_codec_theora_send(theora, conf_pkt_ptr, conf_pkt_size, Theora_Packed_Configuration_payload);
				TSK_FREE(conf_pkt_ptr);
			}

			theora->encoder.conf_last = tsk_time_epoch();
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

	while(size){
		pay_size = TSK_MIN(THEORA_RTP_PAYLOAD_SIZE, size);
		pay_hdr[4] = pay_size>>8, pay_hdr[5] = pay_size & 0xFF;

		if(frag){
			if(first){
				first = tsk_false;
				pay_hdr[3] &= 0x3F, pay_hdr[3] |= (Start_Fragment <<6);
			}
			else{ /* could not be 'first' and 'last' */
				if(size<=THEORA_RTP_PAYLOAD_SIZE){
					/* Last frag */
					pay_hdr[3] &= 0x3F, pay_hdr[3] |= (End_Fragment <<6);
				}
				else{
					/* Continuation frag */
					pay_hdr[3] &= 0x3F, pay_hdr[3] |= (Continuation_Fragment <<6);
				}
			}
		}
		else{
			pay_hdr[3] |= 0x01; /* 'pkts' */
			pay_hdr[3] &= 0x3F, pay_hdr[3] |= (Not_Fragmented <<6);
		}
		
		if(self->rtp.size < (pay_size + sizeof(pay_hdr))){
			if(!(self->rtp.ptr = tsk_realloc(self->rtp.ptr, (pay_size + sizeof(pay_hdr))))){
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
		if(TMEDIA_CODEC_VIDEO(self)->callback){
			TMEDIA_CODEC_VIDEO(self)->callback(TMEDIA_CODEC_VIDEO(self)->callback_data, self->rtp.ptr, (pay_size + sizeof(pay_hdr)), (3003* (30/TMEDIA_CODEC_VIDEO(self)->out.fps)), (size == 0));
		}
	}

	return 0;
}

#endif /* HAVE_FFMPEG */