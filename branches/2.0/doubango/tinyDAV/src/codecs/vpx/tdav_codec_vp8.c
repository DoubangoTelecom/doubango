/*
* Copyright (C) 2011 Doubango Telecom <http://www.doubango.org>
*
* Contact: Mamadou Diop <diopmamadou(at)doubango(dot)org>
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

/**@file tdav_codec_vp8.c
 * @brief VP8 codec
 * The RTP packetizer/depacketizer follows draft-ietf-payload-vp8 and draft-bankoski-vp8-bitstream-05
 * Google's VP8 (http://www.webmproject.org/) encoder/decoder
 *
 * @author Mamadou Diop <diopmamadou(at)doubango(dot)org>
 *
 */
#include "tinydav/codecs/vpx/tdav_codec_vp8.h"

#if HAVE_LIBVPX

#if TDAV_UNDER_WINDOWS
#	include <windows.h>
#endif

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tinymedia/tmedia_params.h"

#include "tsk_memory.h"
#include "tsk_time.h"
#include "tsk_debug.h"

#define TDAV_VP8_PAY_DESC_SIZE				4
#define TDAV_SYSTEM_CORES_COUNT				0
#define TDAV_VP8_GOP_SIZE_IN_SECONDS		25
#define TDAV_VP8_RTP_PAYLOAD_MAX_SIZE		1050
#if !defined(TDAV_VP8_MAX_BANDWIDTH_KB)
#	define TDAV_VP8_MAX_BANDWIDTH_KB			6000
#endif
#if !defined(TDAV_VP8_MIN_BANDWIDTH_KB)
#	define TDAV_VP8_MIN_BANDWIDTH_KB			100
#endif

/* VP8 codec */
typedef struct tdav_codec_vp8_s
{
	TMEDIA_DECLARE_CODEC_VIDEO;

	// Encoder
	struct{
		vpx_codec_enc_cfg_t cfg;
		tsk_bool_t initialized;
		vpx_codec_pts_t pts;
		vpx_codec_ctx_t context;
		uint16_t pic_id;
		uint64_t frame_count;
		tsk_bool_t force_idr;
		uint32_t target_bitrate;
		int rotation;

		struct{
			uint8_t* ptr;
			tsk_size_t size;
		} rtp;
	} encoder;

	// decoder
	struct{
		vpx_codec_dec_cfg_t cfg;
		unsigned initialized:1;
		vpx_codec_ctx_t context;
		void* accumulator;
		tsk_size_t accumulator_pos;
		tsk_size_t accumulator_size;
		uint16_t last_seq;
		unsigned last_PartID:4;
		unsigned last_S:1;
		unsigned last_N:1;
	} decoder;
}
tdav_codec_vp8_t;

#define vp8_interface_enc (vpx_codec_vp8_cx())
#define vp8_interface_dec (vpx_codec_vp8_dx())

static int tdav_codec_vp8_open_encoder(tdav_codec_vp8_t* self);
static int tdav_codec_vp8_open_decoder(tdav_codec_vp8_t* self);
static int tdav_codec_vp8_close_encoder(tdav_codec_vp8_t* self);
static int tdav_codec_vp8_close_decoder(tdav_codec_vp8_t* self);

static void tdav_codec_vp8_encap(tdav_codec_vp8_t* self, const vpx_codec_cx_pkt_t *pkt);
static void tdav_codec_vp8_rtp_callback(tdav_codec_vp8_t *self, const void *data, tsk_size_t size, uint32_t partID, tsk_bool_t part_start, tsk_bool_t non_ref, tsk_bool_t last);

/* ============ VP8 Plugin interface ================= */

static int tdav_codec_vp8_set(tmedia_codec_t* self, const tmedia_param_t* param)
{
	tdav_codec_vp8_t* vp8 = (tdav_codec_vp8_t*)self;
	vpx_codec_err_t vpx_ret;

	if(!vp8->encoder.initialized){
		TSK_DEBUG_ERROR("Codec not initialized");
		return -1;
	}
	if(param->value_type == tmedia_pvt_int32){
		if(tsk_striequals(param->key, "action")){
			tmedia_codec_action_t action = (tmedia_codec_action_t)TSK_TO_INT32((uint8_t*)param->value);
			tsk_bool_t reconf = tsk_false;
			switch(action){
				case tmedia_codec_action_encode_idr:
					{
						vp8->encoder.force_idr = tsk_true;
						break;
					}
				case tmedia_codec_action_bw_down:
					{
						vp8->encoder.cfg.rc_target_bitrate = ((vp8->encoder.cfg.rc_target_bitrate << 1) / 3);
						reconf = tsk_true;
						break;
					}
				case tmedia_codec_action_bw_up:
					{
						vp8->encoder.cfg.rc_target_bitrate = ((vp8->encoder.cfg.rc_target_bitrate * 3) >> 1);
						reconf = tsk_true;
						break;
					}
			}

			if(reconf){
				if((vpx_ret = vpx_codec_enc_config_set(&vp8->encoder.context, &vp8->encoder.cfg)) != VPX_CODEC_OK){
					TSK_DEBUG_ERROR("vpx_codec_enc_config_set failed with error =%s", vpx_codec_err_to_string(vpx_ret));
				}
			}
		}
		else if(tsk_striequals(param->key, "rotation")){
			// IMPORTANT: changing resolution requires at least libvpx v1.1.0 "Eider"
			int rotation = *((int32_t*)param->value);
			if(vp8->encoder.rotation != rotation){
				vp8->encoder.rotation = rotation;
				if(vp8->encoder.initialized){
#if 1
					vp8->encoder.cfg.g_w = (rotation == 90 || rotation == 270) ? TMEDIA_CODEC_VIDEO(vp8)->out.height : TMEDIA_CODEC_VIDEO(vp8)->out.width;
					vp8->encoder.cfg.g_h = (rotation == 90 || rotation == 270) ? TMEDIA_CODEC_VIDEO(vp8)->out.width : TMEDIA_CODEC_VIDEO(vp8)->out.height;
					if((vpx_ret = vpx_codec_enc_config_set(&vp8->encoder.context, &vp8->encoder.cfg)) != VPX_CODEC_OK){
						TSK_DEBUG_ERROR("vpx_codec_enc_config_set failed with error =%s", vpx_codec_err_to_string(vpx_ret));
						return -1;
					}
#else
					int ret;
					if((ret = tdav_codec_vp8_close_encoder(vp8))){
						return ret;
					}
					if((ret = tdav_codec_vp8_open_encoder(vp8))){
						return ret;
					}
#endif
				}
				return 0;
			}
		}
	}
	return -1;
}

static int tdav_codec_vp8_open(tmedia_codec_t* self)
{
	tdav_codec_vp8_t* vp8 = (tdav_codec_vp8_t*)self;
	int ret;

	if(!vp8){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* the caller (base class) already checked that the codec is not opened */

	
	//	Encoder
	if((ret = tdav_codec_vp8_open_encoder(vp8))){
		return ret;
	}
	
	//	Decoder
	if((ret = tdav_codec_vp8_open_decoder(vp8))){
		return ret;
	}
	
	return ret;
}

static int tdav_codec_vp8_close(tmedia_codec_t* self)
{
	tdav_codec_vp8_t* vp8 = (tdav_codec_vp8_t*)self;

	if(!vp8){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tdav_codec_vp8_close_encoder(vp8);
	tdav_codec_vp8_close_decoder(vp8);
	
	return 0;
}

static tsk_size_t tdav_codec_vp8_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
	tdav_codec_vp8_t* vp8 = (tdav_codec_vp8_t*)self;
	vpx_enc_frame_flags_t flags = 0;
	vpx_codec_err_t vpx_ret;
	const vpx_codec_cx_pkt_t *pkt;
	vpx_codec_iter_t iter = tsk_null;
	vpx_image_t image;

	if(!vp8 || !in_data || !in_size || !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	if(in_size != (vp8->encoder.context.config.enc->g_w * vp8->encoder.context.config.enc->g_h * 3)>>1){
		TSK_DEBUG_ERROR("Invalid size");
		return 0;
	}

	// wrap yuv420 buffer
	if(!vpx_img_wrap(&image, VPX_IMG_FMT_I420, vp8->encoder.context.config.enc->g_w, vp8->encoder.context.config.enc->g_h, 1, (unsigned char*)in_data)){
		TSK_DEBUG_ERROR("vpx_img_wrap failed");
		return 0;
	}

	// encode data
	++vp8->encoder.pts;
	if(vp8->encoder.force_idr){
		flags |= VPX_EFLAG_FORCE_KF;
		vp8->encoder.force_idr = tsk_false;
	}
	if((vpx_ret = vpx_codec_encode(&vp8->encoder.context, &image, vp8->encoder.pts, 1, flags, VPX_DL_REALTIME)) != VPX_CODEC_OK){
		TSK_DEBUG_ERROR("vpx_codec_encode failed with error =%s", vpx_codec_err_to_string(vpx_ret));
		vpx_img_free(&image);
		return 0;
	}

	++vp8->encoder.frame_count;
	++vp8->encoder.pic_id;

	while((pkt = vpx_codec_get_cx_data(&vp8->encoder.context, &iter))){
		switch(pkt->kind){
			case VPX_CODEC_CX_FRAME_PKT:
				{
					tdav_codec_vp8_encap(vp8, pkt);
					break;
				}
			default:
			case VPX_CODEC_STATS_PKT:       /**< Two-pass statistics for this frame */
			case VPX_CODEC_PSNR_PKT:        /**< PSNR statistics for this frame */
			case VPX_CODEC_CUSTOM_PKT: /**< Algorithm extensions  */
				{
					TSK_DEBUG_INFO("pkt->kind=%d not supported", (int)pkt->kind);
					break;
				}
		}
	}

	vpx_img_free(&image);
	return 0;
}

static tsk_size_t tdav_codec_vp8_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
	tdav_codec_vp8_t* vp8 = (tdav_codec_vp8_t*)self;
	const trtp_rtp_header_t* rtp_hdr = proto_hdr;
	const uint8_t* pdata = in_data;
	const uint8_t* pdata_end = (pdata + in_size);
	tsk_size_t ret = 0;
	static const tsk_size_t xmax_size = (1920 * 1080 * 3) >> 3;

	if(!self || !in_data || in_size<1 || !out_data || !vp8->decoder.initialized){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	{	/* 4.2. VP8 Payload Descriptor */
		uint8_t X, R, N, S, I, L, T, K, PartID;//FIXME: store
		
		X = (*pdata & 0x80)>>7;
		R = (*pdata & 0x40)>>6;
		if(R){
			TSK_DEBUG_ERROR("R<>0");
			return 0;
		}
		N = (*pdata & 0x20)>>5;
		S = (*pdata & 0x10)>>4;
		PartID = (*pdata & 0x0F);
		// skip "REQUIRED" header
		if(++pdata >= pdata_end){ TSK_DEBUG_ERROR("Too short"); goto bail; }
		// check "OPTIONAL" headers
		if(X){
			I = (*pdata & 0x80);
			L = (*pdata & 0x40);
			T = (*pdata & 0x20);
			K = (*pdata & 0x10);
			if(++pdata >= pdata_end){ TSK_DEBUG_ERROR("Too short"); goto bail; }

			if(I){
				if(*pdata & 0x80){ // M
					// PictureID on 16bits 
					if((pdata += 2) >= pdata_end){ TSK_DEBUG_ERROR("Too short"); goto bail; }
				}
				else{
					// PictureID on 8bits
					if(++pdata >= pdata_end){ TSK_DEBUG_ERROR("Too short"); goto bail; }
				}
			}
			if(L){
				if(++pdata >= pdata_end){ TSK_DEBUG_ERROR("Too short"); goto bail; }
			}
			if(T || K){
				if(++pdata >= pdata_end){ TSK_DEBUG_ERROR("Too short"); goto bail; }
			}
		}
	}

	in_size = (pdata_end - pdata);

	// Packet lost?
	if(vp8->decoder.last_seq && (vp8->decoder.last_seq + 1) != rtp_hdr->seq_num){
		TSK_DEBUG_INFO("Packet lost, seq_num=%d", (vp8->decoder.last_seq + 1));
	}
	vp8->decoder.last_seq = rtp_hdr->seq_num;

	if(in_size > xmax_size){
		TSK_DEBUG_ERROR("%u too big to contain valid encoded data. xmax_size=%u", in_size, xmax_size);
		goto bail;
	}
	// start-accumulator
	if(!vp8->decoder.accumulator){
		if(!(vp8->decoder.accumulator = tsk_calloc(in_size, sizeof(uint8_t)))){
			TSK_DEBUG_ERROR("Failed to allocated new buffer");
			goto bail;
		}
		vp8->decoder.accumulator_size = in_size;
	}
	if((vp8->decoder.accumulator_pos + in_size) >= xmax_size){
		TSK_DEBUG_ERROR("BufferOverflow");
		vp8->decoder.accumulator_pos = 0;
		goto bail;
	}
	if((vp8->decoder.accumulator_pos + in_size) > vp8->decoder.accumulator_size){
		if(!(vp8->decoder.accumulator = tsk_realloc(vp8->decoder.accumulator, (vp8->decoder.accumulator_pos + in_size)))){
			TSK_DEBUG_ERROR("Failed to reallocated new buffer");
			vp8->decoder.accumulator_pos = 0;
			vp8->decoder.accumulator_size = 0;
			goto bail;
		}
		vp8->decoder.accumulator_size = (vp8->decoder.accumulator_pos + in_size);
	}

	memcpy(&((uint8_t*)vp8->decoder.accumulator)[vp8->decoder.accumulator_pos], pdata, in_size);
	vp8->decoder.accumulator_pos += in_size;
	// end-accumulator

	// FIXME: First partition is decodable
	// for better error handling we should decode it
	// (vp8->decoder.last_PartID == 0 && vp8->decoder.last_S && S) => previous was "first decodable" and current is new one
	if(rtp_hdr->marker /*|| (vp8->decoder.last_PartID == 0 && vp8->decoder.last_S)*/){
		vpx_image_t *img;
		vpx_codec_iter_t iter = tsk_null;
		vpx_codec_err_t vpx_ret;
		const uint8_t* pay_ptr = (const uint8_t*)vp8->decoder.accumulator;
		const tsk_size_t pay_size = vp8->decoder.accumulator_pos;

		// in all cases: reset accumulator
		vp8->decoder.accumulator_pos = 0;

#if 0 /* http://groups.google.com/a/webmproject.org/group/apps-devel/browse_thread/thread/c84438e70fe122fa/2dfc322018aa22a8 */
		// libvpx will crash very ofen when the frame is corrupted => for now we decided not to decode such frame
		// according to the latest release there is a function to check if the frame 
		// is corrupted or not => To be checked
		if(vp8->decoder.frame_corrupted){
			vp8->decoder.frame_corrupted = tsk_false;
			goto bail;
		}
#endif
		
		vpx_ret = vpx_codec_decode(&vp8->decoder.context, pay_ptr, pay_size, tsk_null, 0);
		
		if(vpx_ret != VPX_CODEC_OK){
			TSK_DEBUG_WARN("vpx_codec_decode failed with error =%s", vpx_codec_err_to_string(vpx_ret));
			if(TMEDIA_CODEC_VIDEO(self)->in.callback){
				TMEDIA_CODEC_VIDEO(self)->in.result.type = tmedia_video_decode_result_type_error;
				TMEDIA_CODEC_VIDEO(self)->in.result.proto_hdr = proto_hdr;
				TMEDIA_CODEC_VIDEO(self)->in.callback(&TMEDIA_CODEC_VIDEO(self)->in.result);
			}
			goto bail;
		}
		
		// copy decoded data
		ret = 0;
		while((img = vpx_codec_get_frame(&vp8->decoder.context, &iter))){
			unsigned int plane, y;
			tsk_size_t xsize;

			// update sizes
			TMEDIA_CODEC_VIDEO(vp8)->in.width = img->d_w;
			TMEDIA_CODEC_VIDEO(vp8)->in.height = img->d_h;
			xsize = (TMEDIA_CODEC_VIDEO(vp8)->in.width * TMEDIA_CODEC_VIDEO(vp8)->in.height * 3) >> 1;
			// allocate destination buffer
			if(*out_max_size < xsize){
				if(!(*out_data = tsk_realloc(*out_data, xsize))){
					TSK_DEBUG_ERROR("Failed to allocate new buffer");
					vp8->decoder.accumulator_pos = 0;
					*out_max_size = 0;
					goto bail;
				}
				*out_max_size = xsize;
			}

			// layout picture
			for(plane=0; plane < 3; plane++) {
                unsigned char *buf =img->planes[plane];
                for(y=0; y<img->d_h >> (plane ? 1 : 0); y++) {
					unsigned int w_count = img->d_w >> (plane ? 1 : 0);
					if((ret + w_count) > *out_max_size){
						TSK_DEBUG_ERROR("BufferOverflow");
						ret = 0;
						goto bail;
					}
					memcpy(((uint8_t*)*out_data) + ret, buf, w_count);
					ret += w_count;
                    buf += img->stride[plane];
                }
            }  
		}
	}

bail:

//	vp8->decoder.last_PartID = PartID;
//	vp8->decoder.last_S = S;
//	vp8->decoder.last_N = N;
	return ret;
}

static tsk_bool_t tdav_codec_vp8_sdp_att_match(const tmedia_codec_t* codec, const char* att_name, const char* att_value)
{
#if 0
	if(tsk_striequals(att_name, "fmtp")){
		unsigned width, height, fps;
		if(tmedia_parse_video_fmtp(att_value, TMEDIA_CODEC_VIDEO(codec)->pref_size, &width, &height, &fps)){
			TSK_DEBUG_ERROR("Failed to match fmtp=%s", att_value);
			return tsk_false;
		}
		TMEDIA_CODEC_VIDEO(codec)->in.width = TMEDIA_CODEC_VIDEO(codec)->out.width = width;
		TMEDIA_CODEC_VIDEO(codec)->in.height = TMEDIA_CODEC_VIDEO(codec)->out.height = height;
		TMEDIA_CODEC_VIDEO(codec)->in.fps = TMEDIA_CODEC_VIDEO(codec)->out.fps = fps;
	}
	else
#endif
		if(tsk_striequals(att_name, "imageattr")){
		unsigned in_width, in_height, out_width, out_height;
		if(tmedia_parse_video_imageattr(att_value, TMEDIA_CODEC_VIDEO(codec)->pref_size, &in_width, &in_height, &out_width, &out_height) != 0){
			return tsk_false;
		}
		TMEDIA_CODEC_VIDEO(codec)->in.width = in_width;
		TMEDIA_CODEC_VIDEO(codec)->in.height = in_height;
		TMEDIA_CODEC_VIDEO(codec)->out.width = out_width;
		TMEDIA_CODEC_VIDEO(codec)->out.height = out_height;
	}
	
	return tsk_true;
}

static char* tdav_codec_vp8_sdp_att_get(const tmedia_codec_t* codec, const char* att_name)
{
#if 0
	if(tsk_striequals(att_name, "fmtp")){
		return tmedia_get_video_fmtp(TMEDIA_CODEC_VIDEO(codec)->pref_size);
	}
	else 
#endif
	if(tsk_striequals(att_name, "imageattr")){
		return tmedia_get_video_imageattr(TMEDIA_CODEC_VIDEO(codec)->pref_size, 
			TMEDIA_CODEC_VIDEO(codec)->in.width, TMEDIA_CODEC_VIDEO(codec)->in.height, TMEDIA_CODEC_VIDEO(codec)->out.width, TMEDIA_CODEC_VIDEO(codec)->out.height);
	}
	return tsk_null;
}

/* ============ VP8 object definition ================= */

/* constructor */
static tsk_object_t* tdav_codec_vp8_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_vp8_t *vp8 = self;
	if(vp8){
		/* init base: called by tmedia_codec_create() */
		/* init self */
		
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_vp8_dtor(tsk_object_t * self)
{ 
	tdav_codec_vp8_t *vp8 = self;
	if(vp8){
		/* deinit base */
		tmedia_codec_video_deinit(vp8);
		/* deinit self */
		if(vp8->encoder.rtp.ptr){
			TSK_FREE(vp8->encoder.rtp.ptr);
			vp8->encoder.rtp.size = 0;
		}
		if(vp8->encoder.initialized){
			vpx_codec_destroy(&vp8->encoder.context);
			vp8->encoder.initialized = tsk_false;
		}
		if(vp8->decoder.initialized){
			vpx_codec_destroy(&vp8->decoder.context);
			vp8->decoder.initialized = tsk_false;
		}
		if(vp8->decoder.accumulator){
			TSK_FREE(vp8->decoder.accumulator);
			vp8->decoder.accumulator_pos = 0;
		}
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_vp8_def_s = 
{
	sizeof(tdav_codec_vp8_t),
	tdav_codec_vp8_ctor, 
	tdav_codec_vp8_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_vp8_plugin_def_s = 
{
	&tdav_codec_vp8_def_s,

	tmedia_video,
	"VP8",
	"VP8 codec",
	TMEDIA_CODEC_FORMAT_VP8,
	tsk_true,
	90000, // rate
	
	/* audio */
	{ 0 },

	/* video (defaul width,height,fps) */
	{176, 144, 15},

	tdav_codec_vp8_set,
	tdav_codec_vp8_open,
	tdav_codec_vp8_close,
	tdav_codec_vp8_encode,
	tdav_codec_vp8_decode,
	tdav_codec_vp8_sdp_att_match,
	tdav_codec_vp8_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_vp8_plugin_def_t = &tdav_codec_vp8_plugin_def_s;

/* ============ Internal functions ================= */

int tdav_codec_vp8_open_encoder(tdav_codec_vp8_t* self)
{
	vpx_codec_err_t vpx_ret;
	vpx_enc_frame_flags_t enc_flags;

	if(self->encoder.initialized){
		TSK_DEBUG_ERROR("VP8 encoder already inialized");
		return -1;
	}

	if((vpx_ret = vpx_codec_enc_config_default(vp8_interface_enc, &self->encoder.cfg, 0)) != VPX_CODEC_OK){
		TSK_DEBUG_ERROR("vpx_codec_enc_config_default failed with error =%s", vpx_codec_err_to_string(vpx_ret));
		return -2;
	}
	self->encoder.cfg.g_timebase.num = 1;
	self->encoder.cfg.g_timebase.den = TMEDIA_CODEC_VIDEO(self)->out.fps;
	self->encoder.cfg.rc_target_bitrate = self->encoder.target_bitrate = (TMEDIA_CODEC_VIDEO(self)->out.width * TMEDIA_CODEC_VIDEO(self)->out.height * 256 / 320 / 240);
	self->encoder.cfg.rc_end_usage = VPX_CBR;
	self->encoder.cfg.g_w = (self->encoder.rotation == 90 || self->encoder.rotation == 270) ? TMEDIA_CODEC_VIDEO(self)->out.height : TMEDIA_CODEC_VIDEO(self)->out.width;
	self->encoder.cfg.g_h = (self->encoder.rotation == 90 || self->encoder.rotation == 270) ? TMEDIA_CODEC_VIDEO(self)->out.width : TMEDIA_CODEC_VIDEO(self)->out.height;
	self->encoder.cfg.kf_mode = VPX_KF_AUTO;
	self->encoder.cfg.kf_min_dist = self->encoder.cfg.kf_max_dist = (TDAV_VP8_GOP_SIZE_IN_SECONDS * TMEDIA_CODEC_VIDEO(self)->out.fps);
	self->encoder.cfg.g_error_resilient = 1;
	self->encoder.cfg.g_lag_in_frames = 0;
#if TDAV_UNDER_WINDOWS
	{
		SYSTEM_INFO SystemInfo;
		GetSystemInfo(&SystemInfo);
		self->encoder.cfg.g_threads = SystemInfo.dwNumberOfProcessors;
	}
#endif
	self->encoder.cfg.g_pass = VPX_RC_ONE_PASS;
	self->encoder.cfg.rc_min_quantizer = 0;//TSK_CLAMP(self->encoder.cfg.rc_min_quantizer, 10, self->encoder.cfg.rc_max_quantizer);
	self->encoder.cfg.rc_max_quantizer = 63;//TSK_CLAMP(self->encoder.cfg.rc_min_quantizer, 51, self->encoder.cfg.rc_max_quantizer);
	//self->encoder.cfg.rc_resize_allowed = 0;
	self->encoder.cfg.g_profile = 0;

	enc_flags = 0; //VPX_EFLAG_XXX

	if((vpx_ret = vpx_codec_enc_init(&self->encoder.context, vp8_interface_enc, &self->encoder.cfg, enc_flags)) != VPX_CODEC_OK){
		TSK_DEBUG_ERROR("vpx_codec_enc_init failed with error =%s", vpx_codec_err_to_string(vpx_ret));
		return -3;
	}
	self->encoder.pic_id = (rand() ^ rand()) % 0x7FFF;
	self->encoder.initialized = tsk_true;

	//vpx_codec_control(&self->encoder.context, VP8E_SET_CPUUSED, 0); 
	//vpx_codec_control(&self->encoder.context, VP8E_SET_SHARPNESS, 7);
	//vpx_codec_control(&self->encoder.context, VP8E_SET_ENABLEAUTOALTREF, 1);

	return 0;
}

int tdav_codec_vp8_open_decoder(tdav_codec_vp8_t* self)
{
	vpx_codec_err_t vpx_ret;
	vpx_codec_caps_t dec_caps;
	vpx_codec_flags_t dec_flags = 0;
	static vp8_postproc_cfg_t __pp = { VP8_DEBLOCK | VP8_DEMACROBLOCK, 4, 0};

	if(self->decoder.initialized){
		TSK_DEBUG_ERROR("VP8 decoder already initialized");
		return -1;
	}

	self->decoder.cfg.w = TMEDIA_CODEC_VIDEO(self)->out.width;
	self->decoder.cfg.h = TMEDIA_CODEC_VIDEO(self)->out.height;
#if TDAV_UNDER_WINDOWS
	{
		SYSTEM_INFO SystemInfo;
		GetSystemInfo(&SystemInfo);
		self->decoder.cfg.threads = SystemInfo.dwNumberOfProcessors;
	}
#endif

	dec_caps = vpx_codec_get_caps(&vpx_codec_vp8_dx_algo);
	if(dec_caps & VPX_CODEC_CAP_POSTPROC){
		dec_flags |= VPX_CODEC_USE_POSTPROC;
	}
	//--if(dec_caps & VPX_CODEC_CAP_ERROR_CONCEALMENT){
	//--	dec_flags |= VPX_CODEC_USE_ERROR_CONCEALMENT;
	//--}

	if((vpx_ret = vpx_codec_dec_init(&self->decoder.context, vp8_interface_dec, &self->decoder.cfg, dec_flags)) != VPX_CODEC_OK){
		TSK_DEBUG_ERROR("vpx_codec_dec_init failed with error =%s", vpx_codec_err_to_string(vpx_ret));
		return -4;
	}
	
	if((vpx_ret = vpx_codec_control(&self->decoder.context, VP8_SET_POSTPROC, &__pp))){
        TSK_DEBUG_WARN("vpx_codec_dec_init failed with error =%s", vpx_codec_err_to_string(vpx_ret));
	}
	self->decoder.initialized = tsk_true;

	return 0;
}

int tdav_codec_vp8_close_encoder(tdav_codec_vp8_t* self)
{
	if(self->encoder.initialized){
		vpx_codec_destroy(&self->encoder.context);
		self->encoder.initialized = tsk_false;
	}
	return 0;
}

int tdav_codec_vp8_close_decoder(tdav_codec_vp8_t* self)
{
	if(self->decoder.initialized){
		vpx_codec_destroy(&self->decoder.context);
		self->decoder.initialized = tsk_false;
	}

	return 0;
}

/* ============ VP8 RTP packetizer/depacketizer ================= */


static void tdav_codec_vp8_encap(tdav_codec_vp8_t* self, const vpx_codec_cx_pkt_t *pkt)
{
	tsk_bool_t non_ref, is_keyframe, part_start;
	uint8_t *frame_ptr;
	uint32_t part_size, part_ID, pkt_size, index;

	if(!self || !pkt || !pkt->data.frame.buf || !pkt->data.frame.sz){
		TSK_DEBUG_ERROR("Invalid parameter");
		return;
	}

	index = 0;
	frame_ptr = pkt->data.frame.buf ;
	pkt_size = pkt->data.frame.sz;
	non_ref = (pkt->data.frame.flags & VPX_FRAME_IS_DROPPABLE);
	is_keyframe = (pkt->data.frame.flags & VPX_FRAME_IS_KEY);

	// check P bit validity
	if((is_keyframe && (*frame_ptr & 0x01)) || (!is_keyframe && !(*frame_ptr & 0x01))){// 4.3. VP8 Payload Header
		TSK_DEBUG_ERROR("Invalid payload header");
		return;
	}

	// first partition (contains modes and motion vectors)
	part_ID = 0; // The first VP8 partition(containing modes and motion vectors) MUST be labeled with PartID = 0
	part_size = (frame_ptr[2] << 16) | (frame_ptr[1] << 8) | frame_ptr[0];
	part_size = (part_size >> 5) & 0x7FFFF;
	if(part_size > pkt_size){
		TSK_DEBUG_ERROR("part_size is > pkt_size(%u,%u)", part_size, pkt_size);
		return;
	}

	part_start = tsk_true;

#if 0 // The first partition could be as big as 10kb for HD 720p video frames => we have to split it
	tdav_codec_vp8_rtp_callback(self, &frame_ptr[index], part_size, part_ID, part_start, non_ref, (index + part_size)==pkt_size);
	index += part_size;
#else
	// first,first,....partitions (or fragment if part_size > TDAV_VP8_RTP_PAYLOAD_MAX_SIZE)
	while(index<part_size){
		uint32_t frag_size = TSK_MIN(TDAV_VP8_RTP_PAYLOAD_MAX_SIZE, (part_size - index));
		tdav_codec_vp8_rtp_callback(self, &frame_ptr[index], frag_size, part_ID, part_start, non_ref, tsk_false);
		part_start = tsk_false;
		index += frag_size;
	}
#endif

	// second,third,... partitions (or fragment if part_size > TDAV_VP8_RTP_PAYLOAD_MAX_SIZE)
	// FIXME: low FEC
	part_start = tsk_true;
	while(index<pkt_size){
		if(part_start){
			/* PartID SHOULD be incremented for each subsequent partition,
			  but MAY be kept at 0 for all packets.  PartID MUST NOT be larger
			  than 8.
			*/
			part_ID++;
		}
		part_size = TSK_MIN(TDAV_VP8_RTP_PAYLOAD_MAX_SIZE, (pkt_size - index));
		
		tdav_codec_vp8_rtp_callback(self, &frame_ptr[index], part_size, part_ID, part_start, non_ref, (index + part_size)==pkt_size);
		index += part_size;
		/*
		If more than one packet in an encoded frame contains the
			  same PartID, the S bit MUST NOT be set for any other packet than
			  the first packet with that PartID.
		*/
		part_start = tsk_false;
	}
}

static void tdav_codec_vp8_rtp_callback(tdav_codec_vp8_t *self, const void *data, tsk_size_t size, uint32_t partID, tsk_bool_t part_start, tsk_bool_t non_ref, tsk_bool_t last)
{
	tsk_size_t paydesc_and_hdr_size = TDAV_VP8_PAY_DESC_SIZE;
	tsk_bool_t has_hdr;
	/* draft-ietf-payload-vp8-04 - 4.2. VP8 Payload Descriptor
			 0 1 2 3 4 5 6 7
			+-+-+-+-+-+-+-+-+
			|X|R|N|S|PartID | (REQUIRED)
			+-+-+-+-+-+-+-+-+
	   X:   |I|L|T|K| RSV   | (OPTIONAL)
			+-+-+-+-+-+-+-+-+
	   I:   |M| PictureID   | (OPTIONAL)
			+-+-+-+-+-+-+-+-+
	   L:   |   TL0PICIDX   | (OPTIONAL)
			+-+-+-+-+-+-+-+-+
	   T/K: |TID|Y| KEYIDX  | (OPTIONAL)
			+-+-+-+-+-+-+-+-+
		
	  draft-ietf-payload-vp8-04 - 4.3. VP8 Payload Header
		  0 1 2 3 4 5 6 7
		 +-+-+-+-+-+-+-+-+
		 |Size0|H| VER |P|
		 +-+-+-+-+-+-+-+-+
		 |     Size1     |
		 +-+-+-+-+-+-+-+-+
		 |     Size2     |
		 +-+-+-+-+-+-+-+-+
		 | Bytes 4..N of |
		 | VP8 payload   |
		 :               :
		 +-+-+-+-+-+-+-+-+
		 | OPTIONAL RTP  |
		 | padding       |
		 :               :
		 +-+-+-+-+-+-+-+-+
	*/

	/*
		Note that the header is present only in packets which have the S bit equal to one and the
		PartID equal to zero in the payload descriptor.
	*/
	if((has_hdr = (part_start && partID == 0))){
		has_hdr = tsk_true;
		paydesc_and_hdr_size += 0; // encoded data already contains payload header?
	}

	if(!data || !size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return;
	}
	if(self->encoder.rtp.size < (size + paydesc_and_hdr_size)){
		if(!(self->encoder.rtp.ptr = tsk_realloc(self->encoder.rtp.ptr, (size + paydesc_and_hdr_size)))){
			TSK_DEBUG_ERROR("Failed to allocate new buffer");
			return;
		}
		self->encoder.rtp.size = (size + paydesc_and_hdr_size);
	}
	memcpy((self->encoder.rtp.ptr + paydesc_and_hdr_size), data, size);

	/* VP8 Payload Descriptor */
	// |X|R|N|S|PartID|
	self->encoder.rtp.ptr[0] = (partID & 0x0F) // PartID
		| ((part_start << 4) & 0x10)// S
		| ((non_ref << 5) & 0x20) // N
		// R = 0
		| (0x80) // X=1
		;
	// X:   |I|L|T|K| RSV   |
	self->encoder.rtp.ptr[1] = 0x80; // I = 1, L = 0, T = 0, K = 0, RSV = 0
	// I:   |M| PictureID   |
	self->encoder.rtp.ptr[2] = (0x80 | (self->encoder.pic_id >> 9)); // M = 1 (PictureID on 15 bits)
	self->encoder.rtp.ptr[3] = (self->encoder.pic_id & 0xFF);

	/* 4.2. VP8 Payload Header */
	if(has_hdr){
		// already part of the encoded stream
	}

	// Send data over the network
	if(TMEDIA_CODEC_VIDEO(self)->out.callback){
		TMEDIA_CODEC_VIDEO(self)->out.result.buffer.ptr = self->encoder.rtp.ptr;
		TMEDIA_CODEC_VIDEO(self)->out.result.buffer.size = (size + TDAV_VP8_PAY_DESC_SIZE);
		TMEDIA_CODEC_VIDEO(self)->out.result.duration = (3003* (30/TMEDIA_CODEC_VIDEO(self)->out.fps));
		TMEDIA_CODEC_VIDEO(self)->out.result.last_chunck = last;
		TMEDIA_CODEC_VIDEO(self)->out.callback(&TMEDIA_CODEC_VIDEO(self)->out.result);
	}
}

#endif /* HAVE_LIBVPX */
