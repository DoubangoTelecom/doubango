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

#include "tsk_params.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#define DEFAULT_PROFILE_LEVEL_ID	Simple_Profile_Level_1

#define MP4V_RTP_PAYLOAD_SIZE		900


// From ISO-IEC-14496-2
typedef enum mp4v_codes_e
{
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
typedef enum mp4v_profiles_e
{
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

static void tdav_codec_mp4ves_encap(tdav_codec_mp4ves_t* mp4v, const uint8_t* pdata, tsk_size_t size);
static void tdav_codec_mp4ves_rtp_callback(tdav_codec_mp4ves_t *mp4v, const void *data, tsk_size_t size, tsk_bool_t marker);

/* ============ MP4V-ES Plugin interface functions ================= */
#define tdav_codec_mp4ves_fmtp_set tsk_null /* MUST be removed from all codecs */

int tdav_codec_mp4ves_open(tmedia_codec_t* self)
{
	int ret;
	int size;
	
	tdav_codec_mp4ves_t* mp4v = (tdav_codec_mp4ves_t*)self;

	if(!mp4v){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* the caller (base class) already checked that the codec is not opened */

	//
	//	Encoder
	//
	if(!(mp4v->encoder.codec = avcodec_find_encoder(CODEC_ID_MPEG4))){
		TSK_DEBUG_ERROR("Failed to find mp4v encoder");
		return -2;
	}
	mp4v->encoder.context = avcodec_alloc_context();
	avcodec_get_context_defaults(mp4v->encoder.context);
	
	mp4v->encoder.context->pix_fmt		= PIX_FMT_YUV420P;
	mp4v->encoder.context->time_base.num  = 1;
	mp4v->encoder.context->time_base.den  = TMEDIA_CODEC_VIDEO(mp4v)->in.fps;
	mp4v->encoder.context->width = TMEDIA_CODEC_VIDEO(mp4v)->in.width;
	mp4v->encoder.context->height = TMEDIA_CODEC_VIDEO(mp4v)->in.height;
	mp4v->encoder.context->mb_decision = FF_MB_DECISION_RD;
	mp4v->encoder.context->noise_reduction = 250;
	mp4v->encoder.context->flags |= CODEC_FLAG_QSCALE;
	mp4v->encoder.context->global_quality = FF_QP2LAMBDA * tmedia_get_video_qscale(self->bl);

	mp4v->encoder.context->thread_count = 1;
	mp4v->encoder.context->rtp_payload_size = MP4V_RTP_PAYLOAD_SIZE;
	mp4v->encoder.context->opaque = tsk_null;
	//mp4v->encoder.context->bit_rate = (int) (bitRate * 0.80f);
	//mp4v->encoder.context->bit_rate_tolerance = (int) (bitRate * 0.20f);
	mp4v->encoder.context->profile = mp4v->profile>>4;
	mp4v->encoder.context->level = mp4v->profile & 0x0F;
	mp4v->encoder.context->gop_size = TMEDIA_CODEC_VIDEO(mp4v)->in.fps*2; // each 2 seconds
	mp4v->encoder.context->max_b_frames = 0;
	mp4v->encoder.context->b_frame_strategy = 1;
    mp4v->encoder.context->flags |= CODEC_FLAG_AC_PRED;

	// Picture (YUV 420)
	if(!(mp4v->encoder.picture = avcodec_alloc_frame())){
		TSK_DEBUG_ERROR("Failed to create MP4V-ES encoder picture");
		return -2;
	}
	avcodec_get_frame_defaults(mp4v->encoder.picture);
	
	size = avpicture_get_size(PIX_FMT_YUV420P, mp4v->encoder.context->width, mp4v->encoder.context->height);
	if(!(mp4v->encoder.buffer = tsk_calloc(size, sizeof(uint8_t)))){
		TSK_DEBUG_ERROR("Failed to allocate MP4V-ES encoder buffer");
		return -2;
	}
	
	// Open encoder
	if((ret = avcodec_open(mp4v->encoder.context, mp4v->encoder.codec)) < 0){
		TSK_DEBUG_ERROR("Failed to open MP4V-ES encoder");
		return ret;
	}
	
	//
	//	Decoder
	//
	if(!(mp4v->decoder.codec = avcodec_find_decoder(CODEC_ID_MPEG4))){
		TSK_DEBUG_ERROR("Failed to find MP4V-ES decoder");
	}
	mp4v->decoder.context = avcodec_alloc_context();
	avcodec_get_context_defaults(mp4v->decoder.context);
	
	mp4v->decoder.context->pix_fmt = PIX_FMT_YUV420P;
	mp4v->decoder.context->width = TMEDIA_CODEC_VIDEO(mp4v)->out.width;
	mp4v->decoder.context->height = TMEDIA_CODEC_VIDEO(mp4v)->out.height;

	// Picture (YUV 420)
	if(!(mp4v->decoder.picture = avcodec_alloc_frame())){
		TSK_DEBUG_ERROR("Failed to create decoder picture");
		return -2;
	}
	avcodec_get_frame_defaults(mp4v->decoder.picture);

	size = avpicture_get_size(PIX_FMT_YUV420P, mp4v->decoder.context->width, mp4v->decoder.context->height);
	if(!(mp4v->decoder.accumulator = tsk_calloc((size + FF_INPUT_BUFFER_PADDING_SIZE), sizeof(uint8_t)))){
		TSK_DEBUG_ERROR("Failed to allocate decoder buffer");
		return -2;
	}

	if(!(mp4v->decoder.accumulator = tsk_calloc((size + FF_INPUT_BUFFER_PADDING_SIZE), sizeof(uint8_t)))){
		TSK_DEBUG_ERROR("Failed to allocate decoder buffer");
		return -2;
	}

	// Open decoder
	if((ret = avcodec_open(mp4v->decoder.context, mp4v->decoder.codec)) < 0){
		TSK_DEBUG_ERROR("Failed to open MP4V-ES decoder");
		return ret;
	}

	return 0;
}

int tdav_codec_mp4ves_close(tmedia_codec_t* self)
{
	tdav_codec_mp4ves_t* mp4v = (tdav_codec_mp4ves_t*)self;

	if(!mp4v){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* the caller (base class) already checked that the codec is opened */

	//
	//	Encoder
	//
	if(mp4v->encoder.context){
		avcodec_close(mp4v->encoder.context);
		av_free(mp4v->encoder.context);
		mp4v->encoder.context = tsk_null;
	}
	if(mp4v->encoder.picture){
		av_free(mp4v->encoder.picture);
	}
	if(mp4v->encoder.buffer){
		TSK_FREE(mp4v->encoder.buffer);
	}

	//
	//	Decoder
	//
	if(mp4v->decoder.context){
		avcodec_close(mp4v->decoder.context);
		if(mp4v->decoder.context->extradata){
			TSK_FREE(mp4v->decoder.context->extradata);
			mp4v->decoder.context->extradata_size = 0;
		}
		av_free(mp4v->decoder.context);
		mp4v->decoder.context = tsk_null;
	}
	if(mp4v->decoder.picture){
		av_free(mp4v->decoder.picture);
		mp4v->decoder.picture = tsk_null;
	}
	if(mp4v->decoder.accumulator){
		TSK_FREE(mp4v->decoder.accumulator);
	}

	return 0;
}

tsk_size_t tdav_codec_mp4ves_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
	int ret;
	int size;

	tdav_codec_mp4ves_t* mp4v = (tdav_codec_mp4ves_t*)self;

	if(!self || !in_data || !in_size || !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	// wrap yuv420 buffer
	size = avpicture_fill((AVPicture *)mp4v->encoder.picture, (uint8_t*)in_data, PIX_FMT_YUV420P, mp4v->encoder.context->width, mp4v->encoder.context->height);
	if(size != in_size){
		/* guard */
		TSK_DEBUG_ERROR("Invalid size");
		return 0;
	}
	
	// Flip
	if(self->video.flip.encoded){
		tdav_converter_video_flip(mp4v->encoder.picture, mp4v->encoder.context->height);
	}

	mp4v->encoder.picture->pts = AV_NOPTS_VALUE;
	mp4v->encoder.picture->quality = mp4v->encoder.context->global_quality;
	ret = avcodec_encode_video(mp4v->encoder.context, mp4v->encoder.buffer, size, mp4v->encoder.picture);
	if(ret > 0){
		tdav_codec_mp4ves_encap(mp4v, mp4v->encoder.buffer, (tsk_size_t)ret);
	}

	return 0;
}

tsk_size_t tdav_codec_mp4ves_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{ 
	tdav_codec_mp4ves_t* mp4v = (tdav_codec_mp4ves_t*)self;
	const trtp_rtp_header_t* rtp_hdr = proto_hdr;

	tsk_size_t xsize, retsize = 0;
	int got_picture_ptr;
	int ret;

	if(!self || !in_data || !in_size || !out_data || !mp4v->decoder.context){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	// get expected size
	xsize = avpicture_get_size(mp4v->decoder.context->pix_fmt, mp4v->decoder.context->width, mp4v->decoder.context->height);

	/* Packet lost? */
	if(mp4v->decoder.last_seq != (rtp_hdr->seq_num - 1) && mp4v->decoder.last_seq){
		if(mp4v->decoder.last_seq == rtp_hdr->seq_num){
			// Could happen on some stupid emulators
			TSK_DEBUG_INFO("Packet duplicated, seq_num=%d", rtp_hdr->seq_num);
			return 0;
		}
		TSK_DEBUG_INFO("Packet lost, seq_num=%d", rtp_hdr->seq_num);
	}
	mp4v->decoder.last_seq = rtp_hdr->seq_num;

	if((mp4v->decoder.accumulator_pos + in_size) <= xsize){
		memcpy(&((uint8_t*)mp4v->decoder.accumulator)[mp4v->decoder.accumulator_pos], in_data, in_size);
		mp4v->decoder.accumulator_pos += in_size;
	}
	else{
		TSK_DEBUG_WARN("Buffer overflow");
		mp4v->decoder.accumulator_pos = 0;
		return 0;
	}

	if(rtp_hdr->marker){
		AVPacket packet;
		/* allocate destination buffer */
		if(*out_max_size <xsize){
			if(!(*out_data = tsk_realloc(*out_data, xsize))){
				TSK_DEBUG_ERROR("Failed to allocate new buffer");
				mp4v->decoder.accumulator_pos = 0;
				*out_max_size = 0;
				return 0;
			}
			*out_max_size = xsize;
		}		

		av_init_packet(&packet);
		packet.size = mp4v->decoder.accumulator_pos;
		packet.data = mp4v->decoder.accumulator;
		ret = avcodec_decode_video2(mp4v->decoder.context, mp4v->decoder.picture, &got_picture_ptr, &packet);

		if(ret <0 || !got_picture_ptr){
			TSK_DEBUG_WARN("Failed to decode the buffer");
		}
		else{
			retsize = xsize;
			TMEDIA_CODEC_VIDEO(mp4v)->in.width = mp4v->decoder.context->width;
			TMEDIA_CODEC_VIDEO(mp4v)->in.height = mp4v->decoder.context->height;
			// flip
			if(self->video.flip.decoded){
				tdav_converter_video_flip(mp4v->decoder.picture, mp4v->decoder.context->height);
			}
			/* copy picture into a linear buffer */
			avpicture_layout((AVPicture *)mp4v->decoder.picture, mp4v->decoder.context->pix_fmt, mp4v->decoder.context->width, mp4v->decoder.context->height,
				*out_data, retsize);
		}
		/* in all cases: reset accumulator */
		mp4v->decoder.accumulator_pos = 0;		
	}

	return retsize;
}

tsk_bool_t tdav_codec_mp4ves_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{
	tsk_params_L_t* params = tsk_null;
	tdav_codec_mp4ves_t *mp4v = (tdav_codec_mp4ves_t *)codec;

	if(!mp4v){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_false;
	}

	/* e.g. profile-level-id=1; xx=yy */
	if((params = tsk_params_fromstring(fmtp, ";", tsk_true))){
		int val_int;
		if((val_int = tsk_params_get_param_value_as_int(params, "profile-level-id")) != -1){
			TSK_DEBUG_INFO("Proposed profile-level-id=%d", val_int);
			mp4v->profile = val_int; // FIXME: Take the remote profile-level-id even if the bandwidth level doesn't match
		}
	}
	
	switch (mp4v->profile ) {
		case Simple_Profile_Level_1:
			TMEDIA_CODEC_VIDEO(mp4v)->out.width = TMEDIA_CODEC_VIDEO(mp4v)->in.width = 176, TMEDIA_CODEC_VIDEO(mp4v)->in.height = TMEDIA_CODEC_VIDEO(mp4v)->out.height = 144;
			break;
		case Simple_Profile_Level_2:
		case Simple_Profile_Level_3:
		default:
			TMEDIA_CODEC_VIDEO(mp4v)->out.width = TMEDIA_CODEC_VIDEO(mp4v)->in.width = 352, TMEDIA_CODEC_VIDEO(mp4v)->in.height = TMEDIA_CODEC_VIDEO(mp4v)->out.height = 288;
			break;
	}

	TSK_OBJECT_SAFE_FREE(params);

	return tsk_true;
}

char* tdav_codec_mp4ves_fmtp_get(const tmedia_codec_t* self)
{
	tdav_codec_mp4ves_t *mp4v = (tdav_codec_mp4ves_t *)self;
	char* fmtp = tsk_null;

	switch(self->bl){
		case tmedia_bl_low:
		default:
			mp4v->profile = Simple_Profile_Level_1;
			break;
		case tmedia_bl_medium:
			mp4v->profile = Simple_Profile_Level_2;
			break;
		case tmedia_bl_hight:
		case tmedia_bl_unrestricted:
			mp4v->profile = Simple_Profile_Level_3;
			break;
	}

	tsk_sprintf(&fmtp, "profile-level-id=%d", mp4v->profile);
	return fmtp;
}


/* ============ Internal functions ================= */
static void tdav_codec_mp4ves_encap(tdav_codec_mp4ves_t* mp4v, const uint8_t* pdata, tsk_size_t size)
{
	uint32_t scode; // start code

	if(size <= 4/*32bits: start code size*/){
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

	switch(scode){
		case visual_object_sequence_start_code:
		case visual_object_start_code:
		case user_data_start_code:
		case video_object_layer_start_code:
		case group_of_vop_start_code:
		case vop_start_code:
			{
				register uint32_t i, last_index = 0;
				int startcode = 0xffffffff;

				if(scode == visual_object_sequence_start_code && size >=5){
					uint8_t profile_and_level_indication = pdata[4]; /* IEC 14496-2: 6.3.2 Visual Object Sequence and Visual Object */
					TSK_DEBUG_INFO("profile_and_level_indication=%d", profile_and_level_indication);
				}

				if(size < MP4V_RTP_PAYLOAD_SIZE){
					goto last;
				}

				for(i = 4; i<(size - 4); i++){
					startcode = (startcode <<8) | pdata[i];
					switch(startcode){
						case visual_object_sequence_start_code:
						case group_of_vop_start_code:
						case vop_start_code:
							tdav_codec_mp4ves_rtp_callback(mp4v, pdata + last_index, (i - last_index), (last_index == size));
							last_index = i;
					}
				}
last:
				if(last_index < size){
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
	if(TMEDIA_CODEC_VIDEO(mp4v)->callback){
		TMEDIA_CODEC_VIDEO(mp4v)->callback(TMEDIA_CODEC_VIDEO(mp4v)->callback_data, data, size, (3003* (30/TMEDIA_CODEC_VIDEO(mp4v)->out.fps)), marker);
	}
}


/* ============ MP4V-ES Plugin interface ================= */

/* constructor */
static tsk_object_t* tdav_codec_mp4ves_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_mp4ves_t *mp4v = self;
	if(mp4v){
		/* init base: called by tmedia_codec_create() */
		/* init self */
		mp4v->profile = DEFAULT_PROFILE_LEVEL_ID;
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_mp4ves_dtor(tsk_object_t * self)
{ 
	tdav_codec_mp4ves_t *mp4v = self;
	if(mp4v){
		/* deinit base */
		tmedia_codec_video_deinit(self); // will close the codec if opened
		/* deinit self */
		TSK_FREE(mp4v->rtp.ptr);
		mp4v->rtp.size = 0;
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_mp4ves_def_s = 
{
	sizeof(tdav_codec_mp4ves_t),
	tdav_codec_mp4ves_ctor, 
	tdav_codec_mp4ves_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_mp4ves_plugin_def_s = 
{
	&tdav_codec_mp4ves_def_s,

	tmedia_video,
	"MP4V-ES",
	"MP4V-ES Codec",
	TMEDIA_CODEC_FORMAT_MP4V_ES,
	tsk_true,
	90000, // rate
	
	/* audio */
	{ 0 },

	/* video */
	{176, 144, 15},

	tdav_codec_mp4ves_open,
	tdav_codec_mp4ves_close,
	tdav_codec_mp4ves_encode,
	tdav_codec_mp4ves_decode,
	tdav_codec_mp4ves_fmtp_match,
	tdav_codec_mp4ves_fmtp_get,
	tdav_codec_mp4ves_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_mp4ves_plugin_def_t = &tdav_codec_mp4ves_plugin_def_s;


#endif /* HAVE_FFMPEG */

