/* Copyright (C) 2013 Mamadou DIOP
* Copyright (C) 2013 Doubango Telecom <http://www.doubango.org>
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
*/
#include "plugin_cuda_config.h"
#include "plugin_cuda_utils.h"

#include "tinydav/codecs/h264/tdav_codec_h264_common.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tinymedia/tmedia_codec.h"
#include "tinymedia/tmedia_params.h"
#include "tinymedia/tmedia_defaults.h"

#include "tsk_mutex.h"
#include "tsk_params.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <unknwn.h>
#include <nvcuvid.h>
#include <cuviddec.h>
#include <NVEncoderAPI.h>
#include <NVEncodeDataTypes.h>
#include <d3d9.h>
#include <cudad3d9.h>
#include <cuda/types.h>
#include <cuda.h>
#include <Windows.h>

typedef struct cuda_codec_h264_s
{
	TDAV_DECLARE_CODEC_H264_COMMON;

	// Encoder
	struct{
		NVEncoder pInst;
		NVEncoderParams ctxParams;
		NVVE_CallbackParams clbParams;
		void* pBufferPtr;
		tsk_size_t nBufferSize;
		int64_t frame_count;
		tsk_bool_t force_idr;
		int32_t quality; // [1-31]
		int rotation;
		int neg_width;
		int neg_height;
		int neg_fps;
		int max_bitrate_bps;
		int32_t max_bw_kpbs;
		tsk_bool_t passthrough; // whether to bypass encoding
	} encoder;
	
	// decoder
	struct{
		CUvideodecoder pInst;
		CUVIDDECODECREATEINFO cuInfo;
		CUvideoparser cuParser;
		CUVIDPARSERPARAMS cuPaserParams;
		CUdevice cuDevice;
		IDirect3D9 *pD3D9;
		IDirect3DDevice9 *pD3D9Device;
		CUcontext cuContext;
		struct {
			void *pcuPtr; // MUST bee freed using cuMemFreeHost()
			tsk_size_t nSize;
			tsk_size_t nPitch;
			tsk_bool_t bAvail;
		} cuBuffer;
		void* accumulator;
		tsk_size_t accumulator_pos;
		tsk_size_t accumulator_size;
		uint16_t last_seq;
		tsk_bool_t passthrough; // whether to bypass decoding
		tsk_mutex_handle_t *phMutex;
	} decoder;
}
cuda_codec_h264_t;

#if !defined(PLUGIN_CUDA_H264_GOP_SIZE_IN_SECONDS)
#	define PLUGIN_CUDA_H264_GOP_SIZE_IN_SECONDS		25
#endif
#if !defined(PLUGIN_CUDA_H264_MAX_FRM_CNT)
#	define PLUGIN_CUDA_H264_MAX_FRM_CNT  2
#endif

static int cuda_codec_h264_init(cuda_codec_h264_t* self, profile_idc_t profile);
static int cuda_codec_h264_deinit(cuda_codec_h264_t* self);
static int cuda_codec_h264_open_encoder(cuda_codec_h264_t* self);
static int cuda_codec_h264_close_encoder(cuda_codec_h264_t* self);
static int cuda_codec_h264_open_decoder(cuda_codec_h264_t* self);
static int cuda_codec_h264_close_decoder(cuda_codec_h264_t* self);

static inline tsk_size_t _cuda_codec_h264_pict_layout(cuda_codec_h264_t* self, void**output, tsk_size_t *output_size);

static int CUDAAPI _NVCallback_HandleVideoSequence(void *pvUserData, CUVIDEOFORMAT *pFormat);
static int CUDAAPI _NVCallback_HandlePictureDecode(void *pvUserData, CUVIDPICPARAMS *pPicParams);
static int CUDAAPI _NVCallback_HandlePictureDisplay(void *pvUserData, CUVIDPARSERDISPINFO *pPicParams);
static unsigned char* CUDAAPI _NVCallback_HandleAcquireBitStream(int *pBufferSize, void *pUserdata);
static void CUDAAPI _NVCallback_HandleReleaseBitStream(int nBytesInBuffer, unsigned char *cb,void *pUserdata);
static void CUDAAPI _NVCallback_HandleOnBeginFrame(const NVVE_BeginFrameInfo *pbfi, void *pUserdata);
static void CUDAAPI _NVCallback_HandleOnEndFrame(const NVVE_EndFrameInfo *pefi, void *pUserdata);

/* ============ H.264 Base/Main Profile X.X Plugin interface functions ================= */

static int cuda_codec_h264_set(tmedia_codec_t* self, const tmedia_param_t* param)
{
	cuda_codec_h264_t* h264 = (cuda_codec_h264_t*)self;
	if(!self->opened){
		TSK_DEBUG_ERROR("Codec not opened");
		return -1;
	}
	if(param->value_type == tmedia_pvt_int32){
		if(tsk_striequals(param->key, "action")){
			tmedia_codec_action_t action = (tmedia_codec_action_t)TSK_TO_INT32((uint8_t*)param->value);
			switch(action){
				case tmedia_codec_action_encode_idr:
					{
						h264->encoder.force_idr = tsk_true;
						break;
					}
				case tmedia_codec_action_bw_down:
					{
						h264->encoder.quality = TSK_CLAMP(1, (h264->encoder.quality + 1), 31);
						break;
					}
				case tmedia_codec_action_bw_up:
					{
						h264->encoder.quality = TSK_CLAMP(1, (h264->encoder.quality - 1), 31);
						break;
					}
			}
			return 0;
		}
		else if(tsk_striequals(param->key, "bypass-encoding")){
			h264->encoder.passthrough = *((int32_t*)param->value) ? tsk_true : tsk_false;
			TSK_DEBUG_INFO("[H.264] bypass-encoding = %d", h264->encoder.passthrough);
			return 0;
		}
		else if(tsk_striequals(param->key, "bypass-decoding")){
			h264->decoder.passthrough = *((int32_t*)param->value) ? tsk_true : tsk_false;
			TSK_DEBUG_INFO("[H.264] bypass-decoding = %d", h264->decoder.passthrough);
			return 0;
		}
		else if(tsk_striequals(param->key, "rotation")){
			int rotation = *((int32_t*)param->value);
			if(h264->encoder.rotation != rotation){
				if(self->opened){
					int ret;
					h264->encoder.rotation = rotation;
					if((ret = cuda_codec_h264_close_encoder(h264))){
						return ret;
					}
					if((ret = cuda_codec_h264_open_encoder(h264))){
						return ret;
					}
				}
			}
			return 0;
		}
	}
	return -1;
}


static int cuda_codec_h264_open(tmedia_codec_t* self)
{
	int ret;
	cuda_codec_h264_t* h264 = (cuda_codec_h264_t*)self;

	if(!h264){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	/* the caller (base class) already checked that the codec is not opened */

	//	Encoder
	if((ret = cuda_codec_h264_open_encoder(h264))){
		return ret;
	}

	//	Decoder
	if((ret = cuda_codec_h264_open_decoder(h264))){
		return ret;
	}

	return 0;
}

static int cuda_codec_h264_close(tmedia_codec_t* self)
{
	cuda_codec_h264_t* h264 = (cuda_codec_h264_t*)self;

	if(!h264){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* the caller (base class) alreasy checked that the codec is opened */

	//	Encoder
	cuda_codec_h264_close_encoder(h264);

	//	Decoder
	cuda_codec_h264_close_decoder(h264);

	return 0;
}

static tsk_size_t cuda_codec_h264_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
	int ret = 0;
	NVVE_EncodeFrameParams efparams;
	tsk_bool_t send_idr, send_hdr;
	unsigned long flags = 0;

	cuda_codec_h264_t* h264 = (cuda_codec_h264_t*)self;
	tdav_codec_h264_common_t* common = (tdav_codec_h264_common_t*)self;

	if(!self || !in_data || !in_size)
	{
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	if(h264->encoder.passthrough) {
		tdav_codec_h264_rtp_encap(common, (const uint8_t*)in_data, in_size);
		return 0;
	}

	if((h264->encoder.ctxParams.iOutputSize[1] * h264->encoder.ctxParams.iOutputSize[0] * 3) >> 1 != in_size)
	{
		/* guard */
		TSK_DEBUG_ERROR("Invalid size");
		return 0;
	}

	if(!self->opened || !h264->encoder.pInst /*|| !h264->encoder.pInst->IsReady()*/)
	{
		TSK_DEBUG_ERROR("Encoder not opened or not ready");
		return 0;
	}

	if(h264->encoder.passthrough)
	{
		tdav_codec_h264_rtp_encap(TDAV_CODEC_H264_COMMON(h264), (const uint8_t*)in_data, in_size);
		return 0;
	}

	HRESULT hr = S_OK;

	efparams.Width = h264->encoder.ctxParams.iOutputSize[0];
    efparams.Height = h264->encoder.ctxParams.iOutputSize[1];  
	efparams.Pitch = (h264->encoder.ctxParams.nDeviceMemPitch ? h264->encoder.ctxParams.nDeviceMemPitch : h264->encoder.ctxParams.iOutputSize[0]);
    efparams.PictureStruc = (NVVE_PicStruct)h264->encoder.ctxParams.iPictureType; 
    efparams.SurfFmt = (NVVE_SurfaceFormat)h264->encoder.ctxParams.iSurfaceFormat;
	efparams.progressiveFrame = (h264->encoder.ctxParams.iSurfaceFormat == 3) ? 1 : 0;
    efparams.repeatFirstField = 0;
	efparams.topfieldfirst = (h264->encoder.ctxParams.iSurfaceFormat == 1) ? 1 : 0;
	efparams.picBuf = (unsigned char *)in_data;
	efparams.bLast = 0;

	// send IDR for:
	//	- the first frame
	//  - remote peer requested an IDR
	//	- every second within the first 4seconds
	send_idr = (
		h264->encoder.frame_count++ == 0
		|| h264 ->encoder.force_idr
		|| ( (h264->encoder.frame_count < h264->encoder.neg_fps * 4) && ((h264->encoder.frame_count % h264->encoder.neg_fps)==0) )
	   );

	if(send_idr)
	{
		flags |= 0x04;  // FORCE IDR
	}

	// send SPS and PPS headers for:
	//  - IDR frames (not required but it's the easiest way to deal with pkt loss)
	//  - every 5 seconds after the first 4seconds
	send_hdr = (
		send_idr
		|| ( (h264->encoder.frame_count % (h264->encoder.neg_fps * 5))==0 )
		);
	if(send_hdr)
	{
		if(h264->encoder.ctxParams.iDisableSPSPPS)
		{
			unsigned char SPSPPSBuff[1024];
			int SPSPPSBuffSize = sizeof(SPSPPSBuff);
			hr = NVGetSPSPPS(h264->encoder.pInst, SPSPPSBuff, SPSPPSBuffSize, &SPSPPSBuffSize);
			if(SUCCEEDED(hr))
			{
				int size = 0;
				while(size < SPSPPSBuffSize - 2)
				{
					int16_t next_size = ((int16_t)SPSPPSBuff[size])<<1 | ((int16_t)SPSPPSBuff[size + 1]);
					tdav_codec_h264_rtp_encap(common, &SPSPPSBuff[size + 2], next_size);
					size += next_size + 2;
				}
			}
			else
			{
				TSK_DEBUG_ERROR("NVGetSPSPPS failed with error code = %08x", hr)
			}
		}
	}

	// Encode data
	CHECK_HR(hr = NVEncodeFrame(h264->encoder.pInst, &efparams, flags, NULL));

	// reset
	h264->encoder.force_idr = tsk_false;

bail:
	return 0;
}

static tsk_size_t cuda_codec_h264_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
	cuda_codec_h264_t* h264 = (cuda_codec_h264_t*)self;
	const trtp_rtp_header_t* rtp_hdr = (const trtp_rtp_header_t*)proto_hdr;
	
	const uint8_t* pay_ptr = tsk_null;
	tsk_size_t pay_size = 0;
	int ret;
	tsk_bool_t append_scp;
	tsk_bool_t sps_or_pps;
	tsk_size_t retsize = 0, size_to_copy = 0;
	static const tsk_size_t xmax_size = (3840 * 2160 * 3) >> 3; // >>3 instead of >>1 (not an error)
	static tsk_size_t start_code_prefix_size = sizeof(H264_START_CODE_PREFIX);

	if(!h264 || !in_data || !in_size || !out_data)
	{
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	if(!self->opened || !h264->encoder.pInst)
	{
		TSK_DEBUG_ERROR("Decoder not opened or not ready");
		return 0;
	}
	
	HRESULT hr = S_OK;

	/* Packet lost? */
	if((h264->decoder.last_seq + 1) != rtp_hdr->seq_num && h264->decoder.last_seq)
	{
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
	if(*((uint8_t*)in_data) & 0x80)
	{
		TSK_DEBUG_WARN("F=1");
		/* reset accumulator */
		h264->decoder.accumulator = 0;
		return 0;
	}

	/* get payload */
	if((ret = tdav_codec_h264_get_pay(in_data, in_size, (const void**)&pay_ptr, &pay_size, &append_scp)) || !pay_ptr || !pay_size)
	{
		TSK_DEBUG_ERROR("Depayloader failed to get H.264 content");
		return 0;
	}
	//append_scp = tsk_true;
	size_to_copy = pay_size + (append_scp ? start_code_prefix_size : 0);
	// whether it's SPS or PPS (append_scp is false for subsequent FUA chuncks)
	sps_or_pps = append_scp && pay_ptr && ((pay_ptr[0] & 0x1F) == 7 || (pay_ptr[0] & 0x1F) == 8);
	
	// start-accumulator
	if(!h264->decoder.accumulator)
	{
		if(size_to_copy > xmax_size)
		{
			TSK_DEBUG_ERROR("%u too big to contain valid encoded data. xmax_size=%u", size_to_copy, xmax_size);
			return 0;
		}
		if(!(h264->decoder.accumulator = tsk_calloc(size_to_copy, sizeof(uint8_t))))
		{
			TSK_DEBUG_ERROR("Failed to allocated new buffer");
			return 0;
		}
		h264->decoder.accumulator_size = size_to_copy;
	}
	if((h264->decoder.accumulator_pos + size_to_copy) >= xmax_size)
	{
		TSK_DEBUG_ERROR("BufferOverflow");
		h264->decoder.accumulator_pos = 0;
		return 0;
	}
	if((h264->decoder.accumulator_pos + size_to_copy) > h264->decoder.accumulator_size)
	{
		if(!(h264->decoder.accumulator = tsk_realloc(h264->decoder.accumulator, (h264->decoder.accumulator_pos + size_to_copy))))
		{
			TSK_DEBUG_ERROR("Failed to reallocated new buffer");
			h264->decoder.accumulator_pos = 0;
			h264->decoder.accumulator_size = 0;
			return 0;
		}
		h264->decoder.accumulator_size = (h264->decoder.accumulator_pos + size_to_copy);
	}

	if(append_scp)
	{
		memcpy(&((uint8_t*)h264->decoder.accumulator)[h264->decoder.accumulator_pos], H264_START_CODE_PREFIX, start_code_prefix_size);
		h264->decoder.accumulator_pos += start_code_prefix_size;
	}
	memcpy(&((uint8_t*)h264->decoder.accumulator)[h264->decoder.accumulator_pos], pay_ptr, pay_size);
	h264->decoder.accumulator_pos += pay_size;
	// end-accumulator


	if(sps_or_pps)
	{
		// http://libav-users.943685.n4.nabble.com/Decode-H264-streams-how-to-fill-AVCodecContext-from-SPS-PPS-td2484472.html
		// SPS and PPS should be bundled with IDR
		TSK_DEBUG_INFO("Receiving SPS or PPS ...to be tied to an IDR");
	}
	else if(rtp_hdr->marker)
	{
		if(h264->decoder.passthrough)
		{
			if(*out_max_size < h264->decoder.accumulator_pos)
			{
				if((*out_data = tsk_realloc(*out_data, h264->decoder.accumulator_pos)))
				{
					*out_max_size = h264->decoder.accumulator_pos;
				}
				else
				{
					*out_max_size = 0;
					return 0;
				}
			}
			memcpy(*out_data, h264->decoder.accumulator, h264->decoder.accumulator_pos);
			retsize = h264->decoder.accumulator_pos;
		}
		else
		{ 
			// !h264->decoder.passthrough
			CUVIDSOURCEDATAPACKET pkt;
			CUresult cuResult;
			pkt.flags = 0;
			pkt.payload_size = (unsigned long) h264->decoder.accumulator_pos;
			pkt.payload = (unsigned char *)h264->decoder.accumulator;
			pkt.timestamp = 0;

			// reset accumulator
			h264->decoder.accumulator_pos = 0;
			cuResult = cuvidParseVideoData(h264->decoder.cuParser, &pkt);
			if(cuResult != CUDA_SUCCESS)
			{
				TSK_DEBUG_ERROR("cuvidParseVideoData() failed with error code = %d", (int)cuResult);
				CHECK_HR(hr = E_FAIL);
			}
			
			if(h264->decoder.cuBuffer.bAvail)
			{
				h264->decoder.cuBuffer.bAvail = tsk_false;
				if((retsize = _cuda_codec_h264_pict_layout(h264, out_data, out_max_size)) == 0)
				{
					TSK_DEBUG_ERROR("_cuda_codec_h264_pict_layout failed");
					CHECK_HR(hr = E_FAIL);
				}
			}
		}// else(!h264->decoder.passthrough)
	} // else if(rtp_hdr->marker)

bail:
	if(FAILED(hr))
	{
		TSK_DEBUG_INFO("Failed to decode the buffer with error code =%d, size=%u, append=%s", ret, h264->decoder.accumulator_pos, append_scp ? "yes" : "no");
		if(TMEDIA_CODEC_VIDEO(self)->in.callback)
		{
			TMEDIA_CODEC_VIDEO(self)->in.result.type = tmedia_video_decode_result_type_error;
			TMEDIA_CODEC_VIDEO(self)->in.result.proto_hdr = proto_hdr;
			TMEDIA_CODEC_VIDEO(self)->in.callback(&TMEDIA_CODEC_VIDEO(self)->in.result);
		}
	}
	return retsize;
}

static tsk_bool_t cuda_codec_h264_sdp_att_match(const tmedia_codec_t* self, const char* att_name, const char* att_value)
{
	return tdav_codec_h264_common_sdp_att_match((tdav_codec_h264_common_t*)self, att_name, att_value);
}

static char* cuda_codec_h264_sdp_att_get(const tmedia_codec_t* self, const char* att_name)
{
	char* att = tdav_codec_h264_common_sdp_att_get((const tdav_codec_h264_common_t*)self, att_name);
	if(att && tsk_striequals(att_name, "fmtp")) {
		tsk_strcat(&att, "; impl=CUDA");
	}
	return att;
}




/* ============ H.264 Base Profile Plugin interface ================= */

/* constructor */
static tsk_object_t* cuda_codec_h264_base_ctor(tsk_object_t * self, va_list * app)
{
	cuda_codec_h264_t *h264 = (cuda_codec_h264_t*)self;
	if(h264){
		/* init base: called by tmedia_codec_create() */
		/* init self */
		if(cuda_codec_h264_init(h264, profile_idc_baseline) != 0){
			return tsk_null;
		}
	}
	return self;
}
/* destructor */
static tsk_object_t* cuda_codec_h264_base_dtor(tsk_object_t * self)
{ 
	cuda_codec_h264_t *h264 = (cuda_codec_h264_t*)self;
	if(h264){
		/* deinit base */
		tdav_codec_h264_common_deinit(TDAV_CODEC_H264_COMMON(self));
		/* deinit self */
		cuda_codec_h264_deinit(h264);
	}

	return self;
}
/* object definition */
static const tsk_object_def_t cuda_codec_h264_base_def_s = 
{
	sizeof(cuda_codec_h264_t),
	cuda_codec_h264_base_ctor, 
	cuda_codec_h264_base_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t cuda_codec_h264_base_plugin_def_s = 
{
	&cuda_codec_h264_base_def_s,

	tmedia_video,
	tmedia_codec_id_h264_bp,
	"H264",
	"H264 Base Profile (NVIDIA CUDA)",
	TMEDIA_CODEC_FORMAT_H264_BP,
	tsk_true,
	90000, // rate
	
	/* audio */
	{ 0 },

	/* video (width, height, fps) */
	{176, 144, 0}, // fps is @deprecated

	cuda_codec_h264_set,
	cuda_codec_h264_open,
	cuda_codec_h264_close,
	cuda_codec_h264_encode,
	cuda_codec_h264_decode,
	cuda_codec_h264_sdp_att_match,
	cuda_codec_h264_sdp_att_get
};
const tmedia_codec_plugin_def_t *cuda_codec_h264_base_plugin_def_t = &cuda_codec_h264_base_plugin_def_s;

/* ============ H.264 Main Profile Plugin interface ================= */

/* constructor */
static tsk_object_t* cuda_codec_h264_main_ctor(tsk_object_t * self, va_list * app)
{
	cuda_codec_h264_t *h264 = (cuda_codec_h264_t*)self;
	if(h264){
		/* init base: called by tmedia_codec_create() */
		/* init self */
		if(cuda_codec_h264_init(h264, profile_idc_main) != 0){
			return tsk_null;
		}
	}
	return self;
}
/* destructor */
static tsk_object_t* cuda_codec_h264_main_dtor(tsk_object_t * self)
{ 
	cuda_codec_h264_t *h264 = (cuda_codec_h264_t*)self;
	if(h264){
		/* deinit base */
		tdav_codec_h264_common_deinit(TDAV_CODEC_H264_COMMON(self));
		/* deinit self */
		cuda_codec_h264_deinit(h264);
		
	}

	return self;
}
/* object definition */
static const tsk_object_def_t cuda_codec_h264_main_def_s = 
{
	sizeof(cuda_codec_h264_t),
	cuda_codec_h264_main_ctor, 
	cuda_codec_h264_main_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t cuda_codec_h264_main_plugin_def_s = 
{
	&cuda_codec_h264_main_def_s,

	tmedia_video,
	tmedia_codec_id_h264_mp,
	"H264",
	"H264 Main Profile (NVIDIA CUDA)",
	TMEDIA_CODEC_FORMAT_H264_MP,
	tsk_true,
	90000, // rate
	
	/* audio */
	{ 0 },

	/* video (width, height, fps)*/
	{176, 144, 0},// fps is @deprecated

	cuda_codec_h264_set,
	cuda_codec_h264_open,
	cuda_codec_h264_close,
	cuda_codec_h264_encode,
	cuda_codec_h264_decode,
	cuda_codec_h264_sdp_att_match,
	cuda_codec_h264_sdp_att_get
};
const tmedia_codec_plugin_def_t *cuda_codec_h264_main_plugin_def_t = &cuda_codec_h264_main_plugin_def_s;





/* ============ Common To all H264 codecs ================= */

int cuda_codec_h264_open_encoder(cuda_codec_h264_t* self)
{
	HRESULT hr = S_OK;
	int32_t max_bw_kpbs;
	int bestGPU = 0, gpuPerf = 0;
	static int low_latency = 1;
	tdav_codec_h264_common_t* common = (tdav_codec_h264_common_t*)self;

	if(self->encoder.pInst)
	{
		TSK_DEBUG_ERROR("Encoder already initialized");
#if defined(E_ILLEGAL_METHOD_CALL)
		CHECK_HR(hr = E_ILLEGAL_METHOD_CALL);
#else
		CHECK_HR(hr = 0x8000000EL);
#endif
	}

	memset(&self->encoder.clbParams, 0, sizeof(self->encoder.clbParams));
	memset(&self->encoder.ctxParams, 0, sizeof(self->encoder.ctxParams));

	// create encoder
	CHECK_HR(hr = NVCreateEncoder(&self->encoder.pInst));
	CHECK_HR(hr = NVSetCodec(self->encoder.pInst, NV_CODEC_TYPE_H264));
	CHECK_HR(hr = NVSetDefaultParam(self->encoder.pInst));

	CHECK_HR(hr = NVGetParamValue(self->encoder.pInst, NVVE_GET_GPU_COUNT, &self->encoder.ctxParams.GPU_count));
	{
		int temp = 0, deviceCount;
		for (deviceCount=0; deviceCount < self->encoder.ctxParams.GPU_count; deviceCount++)
		{
			NVVE_GPUAttributes GPUAttributes = {0};

			GPUAttributes.iGpuOrdinal = deviceCount;
			hr = NVGetParamValue(self->encoder.pInst, NVVE_GET_GPU_ATTRIBUTES,  &GPUAttributes);
			if(FAILED(hr))
			{
				TSK_DEBUG_ERROR("NVGetParamValue(NVVE_GET_GPU_ATTRIBUTES) failed with error code = %08x", hr);
				continue;
			}

			temp = GPUAttributes.iClockRate * GPUAttributes.iMultiProcessorCount;
			temp = temp * CudaUtils::ConvertSMVer2Cores(GPUAttributes.iMajor, GPUAttributes.iMinor);

			if(temp > gpuPerf)
			{
				gpuPerf = temp;
				bestGPU = deviceCount;
			}
		}
	}
	
	self->encoder.neg_width = (self->encoder.rotation == 90 || self->encoder.rotation == 270) ? TMEDIA_CODEC_VIDEO(self)->out.height : TMEDIA_CODEC_VIDEO(self)->out.width;
	self->encoder.neg_height = (self->encoder.rotation == 90 || self->encoder.rotation == 270) ? TMEDIA_CODEC_VIDEO(self)->out.width : TMEDIA_CODEC_VIDEO(self)->out.height;
	self->encoder.neg_fps = TMEDIA_CODEC_VIDEO(self)->out.fps;
	max_bw_kpbs = TSK_CLAMP(
		0,
		tmedia_get_video_bandwidth_kbps_2(self->encoder.neg_width, self->encoder.neg_height, self->encoder.neg_fps), 
		self->encoder.max_bw_kpbs
	);
	self->encoder.max_bitrate_bps = (max_bw_kpbs * 1024);

	TSK_DEBUG_INFO("[H.264 CUDA Encoder] neg_width=%d, neg_height=%d, neg_fps=%d, max_bitrate_bps=%d",
		self->encoder.neg_width,
		self->encoder.neg_height,
		self->encoder.neg_fps,
		self->encoder.max_bitrate_bps
		);

	self->encoder.ctxParams.iForcedGPU = bestGPU;
	self->encoder.ctxParams.iInputSize[0] = self->encoder.neg_width;
	self->encoder.ctxParams.iInputSize[1] = self->encoder.neg_height;
	self->encoder.ctxParams.iOutputSize[0] = self->encoder.neg_width;
	self->encoder.ctxParams.iOutputSize[1] = self->encoder.neg_height;
	self->encoder.ctxParams.GPUOffloadLevel= NVVE_GPU_OFFLOAD_ALL;
	self->encoder.ctxParams.iSurfaceFormat = (int)IYUV;
	self->encoder.ctxParams.iPictureType   = (int)FRAME_PICTURE;
	self->encoder.ctxParams.Fieldmode = MODE_FRAME;
	self->encoder.ctxParams.Presets = (NVVE_PRESETS_TARGET)-1;//Should be iPod, Zune ...
	// self->encoder.ctxParams.iP_Interval = 1;
	self->encoder.ctxParams.iAspectRatio[0] = 1;
	self->encoder.ctxParams.iAspectRatio[1] = 1;
	self->encoder.ctxParams.iAspectRatio[2] = 0;
	self->encoder.ctxParams.iIDR_Period = (self->encoder.neg_fps * PLUGIN_CUDA_H264_GOP_SIZE_IN_SECONDS);
	self->encoder.ctxParams.iUseDeviceMem = 0;
	self->encoder.ctxParams.iDynamicGOP = 0;
	self->encoder.ctxParams.RCType = RC_CBR;
	self->encoder.ctxParams.iAvgBitrate = self->encoder.max_bitrate_bps;
	self->encoder.ctxParams.iPeakBitrate = self->encoder.max_bitrate_bps;
	self->encoder.ctxParams.iQP_Level_Intra = 25;
	self->encoder.ctxParams.iQP_Level_InterP = 28;
	self->encoder.ctxParams.iQP_Level_InterB = 31;
	self->encoder.ctxParams.iFrameRate[0] = self->encoder.neg_fps;
	self->encoder.ctxParams.iFrameRate[1] = 1;
	self->encoder.ctxParams.iDeblockMode = 1;
	self->encoder.ctxParams.iForceIntra = 0;
	self->encoder.ctxParams.iForceIDR = 0;
	self->encoder.ctxParams.iClearStat = 0;
	self->encoder.ctxParams.DIMode = DI_MEDIAN;
	self->encoder.ctxParams.iDisableSPSPPS = 1; // Do not include SPS/PPS frames
	self->encoder.ctxParams.iNaluFramingType = 0; // StartCodes
	self->encoder.ctxParams.iMultiGPU = 1;
	switch(common->profile)
	{
		case profile_idc_baseline:
			{
				self->encoder.ctxParams.iDisableCabac = 1;
				self->encoder.ctxParams.iProfileLevel = 0xff42; // 0xff -> autoselect level
				break;
			}
		case profile_idc_main:
			{
				self->encoder.ctxParams.iDisableCabac = 0;
				self->encoder.ctxParams.iProfileLevel = 0xff4d; // 0xff -> autoselect level
				break;
			}
		default:
			{
				CHECK_HR(hr = E_NOTIMPL);
				break;
			}
	}

	//
	// Allocate memory
	//
	self->encoder.nBufferSize = (self->encoder.ctxParams.iOutputSize[1] * self->encoder.ctxParams.iOutputSize[0] * 3) >> 4;
	if(!self->encoder.pBufferPtr && !(self->encoder.pBufferPtr = tsk_realloc(self->encoder.pBufferPtr, self->encoder.nBufferSize)))
	{
		self->encoder.nBufferSize = 0;
		CHECK_HR(hr = E_OUTOFMEMORY);
	}

	//
	// Set parameters
	//
	hr = NVSetParamValue(self->encoder.pInst, NVVE_FORCE_GPU_SELECTION, &self->encoder.ctxParams.iForcedGPU);
	if(FAILED(hr))
	{
		TSK_DEBUG_WARN("NVSetParamValue(NVVE_FORCE_GPU_SELECTION) failed with error code = %08x", hr);
	}
	CHECK_HR(hr = NVSetParamValue(self->encoder.pInst, NVVE_DEVICE_MEMORY_INPUT, &(self->encoder.ctxParams.iUseDeviceMem)));
	hr = NVSetParamValue(self->encoder.pInst,NVVE_OUT_SIZE,		&(self->encoder.ctxParams.iOutputSize)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_OUT_SIZE) failed with error code = %08x", hr); }
	hr = NVSetParamValue(self->encoder.pInst,NVVE_IN_SIZE,		&(self->encoder.ctxParams.iInputSize)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_IN_SIZE) failed with error code = %08x", hr); }
    hr = NVSetParamValue(self->encoder.pInst,NVVE_MULTI_GPU,	&(self->encoder.ctxParams.iMultiGPU)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_MULTI_GPU) failed with error code = %08x", hr); }
	hr = NVSetParamValue(self->encoder.pInst,NVVE_ASPECT_RATIO,	&(self->encoder.ctxParams.iAspectRatio));if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_ASPECT_RATIO) failed with error code = %08x", hr); }
    hr = NVSetParamValue(self->encoder.pInst,NVVE_FIELD_ENC_MODE,	&(self->encoder.ctxParams.Fieldmode)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_FIELD_ENC_MODE) failed with error code = %08x", hr); }
    hr = NVSetParamValue(self->encoder.pInst,NVVE_P_INTERVAL,		&(self->encoder.ctxParams.iP_Interval)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_P_INTERVAL) failed with error code = %08x", hr); }
    hr = NVSetParamValue(self->encoder.pInst,NVVE_IDR_PERIOD,		&(self->encoder.ctxParams.iIDR_Period)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_IDR_PERIOD) failed with error code = %08x", hr); }
    hr = NVSetParamValue(self->encoder.pInst,NVVE_DYNAMIC_GOP,	&(self->encoder.ctxParams.iDynamicGOP)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_DYNAMIC_GOP) failed with error code = %08x", hr); }
    hr = NVSetParamValue(self->encoder.pInst,NVVE_RC_TYPE,		&(self->encoder.ctxParams.RCType)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_RC_TYPE) failed with error code = %08x", hr); }
    hr = NVSetParamValue(self->encoder.pInst,NVVE_AVG_BITRATE,	&(self->encoder.ctxParams.iAvgBitrate)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_AVG_BITRATE) failed with error code = %08x", hr); }
    hr = NVSetParamValue(self->encoder.pInst,NVVE_PEAK_BITRATE,	&(self->encoder.ctxParams.iPeakBitrate)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_PEAK_BITRATE) failed with error code = %08x", hr); }
    hr = NVSetParamValue(self->encoder.pInst,NVVE_QP_LEVEL_INTRA,	&(self->encoder.ctxParams.iQP_Level_Intra)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_OUT_SIZE) failed with error code = %08x", hr); }
    hr = NVSetParamValue(self->encoder.pInst,NVVE_QP_LEVEL_INTER_P,&(self->encoder.ctxParams.iQP_Level_InterP)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_QP_LEVEL_INTER_P) failed with error code = %08x", hr); }
    hr = NVSetParamValue(self->encoder.pInst,NVVE_QP_LEVEL_INTER_B,&(self->encoder.ctxParams.iQP_Level_InterB)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_QP_LEVEL_INTER_B) failed with error code = %08x", hr); }
    hr = NVSetParamValue(self->encoder.pInst,NVVE_FRAME_RATE,		&(self->encoder.ctxParams.iFrameRate)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_FRAME_RATE) failed with error code = %08x", hr); }
    hr = NVSetParamValue(self->encoder.pInst,NVVE_DEBLOCK_MODE,	&(self->encoder.ctxParams.iDeblockMode)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_DEBLOCK_MODE) failed with error code = %08x", hr); }
    hr = NVSetParamValue(self->encoder.pInst,NVVE_PROFILE_LEVEL,	&(self->encoder.ctxParams.iProfileLevel)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_PROFILE_LEVEL) failed with error code = %08x", hr); }
    hr = NVSetParamValue(self->encoder.pInst,NVVE_FORCE_INTRA,	&(self->encoder.ctxParams.iForceIntra)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_FORCE_INTRA) failed with error code = %08x", hr); }
    hr = NVSetParamValue(self->encoder.pInst,NVVE_FORCE_IDR,		&(self->encoder.ctxParams.iForceIDR)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_FORCE_IDR) failed with error code = %08x", hr); }
    hr = NVSetParamValue(self->encoder.pInst,NVVE_CLEAR_STAT,		&(self->encoder.ctxParams.iClearStat)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_CLEAR_STAT) failed with error code = %08x", hr); }
    hr = NVSetParamValue(self->encoder.pInst,NVVE_SET_DEINTERLACE,&(self->encoder.ctxParams.DIMode)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_SET_DEINTERLACE) failed with error code = %08x", hr); }
    if (self->encoder.ctxParams.Presets != -1)
	{
        hr = NVSetParamValue(self->encoder.pInst,NVVE_PRESETS,	&(self->encoder.ctxParams.Presets)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_PRESETS) failed with error code = %08x", hr); }
	}
    hr = NVSetParamValue(self->encoder.pInst,NVVE_DISABLE_CABAC,	&(self->encoder.ctxParams.iDisableCabac)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_DISABLE_CABAC) failed with error code = %08x", hr); }
    hr = NVSetParamValue(self->encoder.pInst,NVVE_CONFIGURE_NALU_FRAMING_TYPE, &(self->encoder.ctxParams.iNaluFramingType)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_CONFIGURE_NALU_FRAMING_TYPE) failed with error code = %08x", hr); }
    hr = NVSetParamValue(self->encoder.pInst,NVVE_DISABLE_SPS_PPS,&(self->encoder.ctxParams.iDisableSPSPPS)); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_DISABLE_SPS_PPS) failed with error code = %08x", hr); }
	hr = NVSetParamValue(self->encoder.pInst,NVVE_LOW_LATENCY,&low_latency); if (hr!=S_OK) { TSK_DEBUG_WARN("NVSetParamValue(NVVE_LOW_LATENCY) failed with error code = %08x", hr); }

	self->encoder.clbParams.pfnacquirebitstream = _NVCallback_HandleAcquireBitStream;
	self->encoder.clbParams.pfnonbeginframe     = _NVCallback_HandleOnBeginFrame;
	self->encoder.clbParams.pfnonendframe       = _NVCallback_HandleOnEndFrame;
	self->encoder.clbParams.pfnreleasebitstream = _NVCallback_HandleReleaseBitStream;
	NVRegisterCB(self->encoder.pInst, self->encoder.clbParams, self);
	
	
	CHECK_HR(hr = NVCreateHWEncoder(self->encoder.pInst));
	
bail:
	return SUCCEEDED(hr) ? 0 : -1;
}

int cuda_codec_h264_close_encoder(cuda_codec_h264_t* self)
{
	if(self)
	{
		if(self->encoder.pInst)
		{
			NVDestroyEncoder(self->encoder.pInst);
			self->encoder.pInst = NULL;
		}
		if(self->encoder.pBufferPtr)
		{
			TSK_FREE(self->encoder.pBufferPtr);
			self->encoder.nBufferSize = 0;
		}
		self->encoder.frame_count = 0;
	}

	return 0;
}

int cuda_codec_h264_open_decoder(cuda_codec_h264_t* self)
{
	HRESULT hr = S_OK;
	tdav_codec_h264_common_t* common = (tdav_codec_h264_common_t*)self;
	int i, adapterCount;
	CUresult cuResult;
	D3DPRESENT_PARAMETERS d3dpp;

	if(self->decoder.pInst || self->decoder.cuDevice || self->decoder.cuContext || self->decoder.pD3D9 || self->decoder.pD3D9Device) 
	{
		TSK_DEBUG_ERROR("Decoder already initialized");
#if defined(E_ILLEGAL_METHOD_CALL)
		CHECK_HR(hr = E_ILLEGAL_METHOD_CALL);
#else
		CHECK_HR(hr = 0x8000000EL);
#endif
	}

	TSK_DEBUG_INFO("[H.264 MF Decoder] neg_width=%d, neg_height=%d, neg_fps=%d",
		TMEDIA_CODEC_VIDEO(self)->in.width,
		TMEDIA_CODEC_VIDEO(self)->in.height,
		TMEDIA_CODEC_VIDEO(self)->in.fps
		);

	memset(&self->decoder.cuInfo, 0, sizeof(self->decoder.cuInfo));
	self->decoder.cuInfo.ulCreationFlags = cudaVideoCreate_PreferCUDA;
	self->decoder.cuInfo.CodecType = cudaVideoCodec_H264;
	self->decoder.cuInfo.ulWidth = TMEDIA_CODEC_VIDEO(self)->in.width;
	self->decoder.cuInfo.ulTargetWidth = TMEDIA_CODEC_VIDEO(self)->in.width;
	self->decoder.cuInfo.ulHeight = TMEDIA_CODEC_VIDEO(self)->in.height;
	self->decoder.cuInfo.ulTargetHeight = TMEDIA_CODEC_VIDEO(self)->in.height;
	self->decoder.cuInfo.ulNumDecodeSurfaces = PLUGIN_CUDA_H264_MAX_FRM_CNT;
	self->decoder.cuInfo.ulNumOutputSurfaces = 1;
	self->decoder.cuInfo.ChromaFormat = cudaVideoChromaFormat_420;
	self->decoder.cuInfo.OutputFormat = cudaVideoSurfaceFormat_NV12;
	self->decoder.cuInfo.DeinterlaceMode = cudaVideoDeinterlaceMode_Adaptive;

	self->decoder.cuDevice = CudaUtils::GetMaxGflopsDeviceId();

#if _DEBUG || DEBUG
	{
		int major, minor;
		size_t totalGlobalMem;
		char deviceName[256];
		cuDeviceComputeCapability(&major, &minor, self->decoder.cuDevice);
		cuDeviceGetName(deviceName, sizeof(deviceName), self->decoder.cuDevice);
		TSK_DEBUG_INFO("[CUDA H.264 decoder] Using GPU Device %d: %s has SM %d.%d compute capability", self->decoder.cuDevice, deviceName, major, minor);
 
		/*cutilDrvSafeCallNoSync(*/cuDeviceTotalMem(&totalGlobalMem, self->decoder.cuDevice)/*)*/;
		TSK_DEBUG_INFO("[CUDA H.264 decoder] Total amount of global memory in GPU device: %4.4f MB", (float)totalGlobalMem/(1024*1024));
	}
#endif

	// create Direct3D instance
	self->decoder.pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if(!self->decoder.pD3D9)
	{
		CHECK_HR(hr = E_OUTOFMEMORY);
	}
	adapterCount = self->decoder.pD3D9->GetAdapterCount();
	for(i = 0; i < adapterCount; ++i)
	{
		ZeroMemory(&d3dpp, sizeof(d3dpp));
        d3dpp.Windowed = TRUE;
        d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
        d3dpp.BackBufferWidth = self->decoder.cuInfo.ulTargetWidth;
        d3dpp.BackBufferHeight = self->decoder.cuInfo.ulTargetHeight;
        d3dpp.BackBufferCount = 1;
        d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
        d3dpp.Flags = D3DPRESENTFLAG_VIDEO;
        hr = self->decoder.pD3D9->CreateDevice(i,
                                  D3DDEVTYPE_HAL,
                                  GetDesktopWindow(),
                                  D3DCREATE_FPU_PRESERVE | D3DCREATE_MULTITHREADED | D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                  &d3dpp,
                                  &self->decoder.pD3D9Device);
        if(hr == S_OK)
		{
            cuResult = cuD3D9CtxCreate(&self->decoder.cuContext, &self->decoder.cuDevice, 0, self->decoder.pD3D9Device);
			if(cuResult == CUDA_SUCCESS)
			{
                break;
			}
			SafeRelease(&self->decoder.pD3D9Device);
			if(self->decoder.cuContext)
			{
				cuCtxDestroy(self->decoder.cuContext);
				self->decoder.cuContext = NULL;
			}
        }
	}

	if(!self->decoder.pD3D9Device)
	{
		TSK_DEBUG_ERROR("Failed to create D3D9 device");
		CHECK_HR(hr = E_FAIL);
	}


	memset(&self->decoder.cuPaserParams, 0, sizeof(self->decoder.cuPaserParams));
	self->decoder.cuPaserParams.CodecType = cudaVideoCodec_H264;
    self->decoder.cuPaserParams.ulMaxNumDecodeSurfaces = PLUGIN_CUDA_H264_MAX_FRM_CNT;
    self->decoder.cuPaserParams.pUserData = self;
    self->decoder.cuPaserParams.pfnSequenceCallback = _NVCallback_HandleVideoSequence;
    self->decoder.cuPaserParams.pfnDecodePicture = _NVCallback_HandlePictureDecode;
    self->decoder.cuPaserParams.pfnDisplayPicture = _NVCallback_HandlePictureDisplay;
    cuResult = cuvidCreateVideoParser(&self->decoder.cuParser, &self->decoder.cuPaserParams);
	if(cuResult != CUDA_SUCCESS)
	{
		TSK_DEBUG_ERROR("cuvidCreateVideoParser(0) failed with error code = %d", (int)cuResult);
		CHECK_HR(hr = E_FAIL);
	}

	cuResult = cuvidCreateDecoder(&self->decoder.pInst, &self->decoder.cuInfo);
	if(CUDA_SUCCESS != cuResult)
	{
		TSK_DEBUG_ERROR("cuvidCreateDecoder failed with error code=%d", (int)cuResult);
		CHECK_HR(hr = E_FAIL);
	}

	if(!self->decoder.phMutex && !(self->decoder.phMutex = tsk_mutex_create()))
	{
		TSK_DEBUG_ERROR("Failed to create mutex");
		CHECK_HR(hr = E_FAIL);
	}
	
bail:
	return SUCCEEDED(hr) ? 0 : -1;
}

int cuda_codec_h264_close_decoder(cuda_codec_h264_t* self)
{
	if(self)
	{
		if(self->decoder.pInst)
		{
			cuvidDestroyDecoder(self->decoder.pInst);
			self->decoder.pInst = NULL;
		}
		if(self->decoder.cuContext)
		{
			cuCtxDestroy(self->decoder.cuContext);
			self->decoder.cuContext = NULL;
		}
		SafeRelease(&self->decoder.pD3D9Device);
		SafeRelease(&self->decoder.pD3D9);
		if(self->decoder.cuParser)
		{
			cuvidDestroyVideoParser(self->decoder.cuParser);
			self->decoder.cuParser = NULL;
		}
		{/* cuBuffer.XXX */
			if(self->decoder.cuBuffer.pcuPtr)
			{
				cuMemFreeHost(self->decoder.cuBuffer.pcuPtr);
				self->decoder.cuBuffer.pcuPtr = NULL;
			}
			self->decoder.cuBuffer.nSize = self->decoder.cuBuffer.nPitch = 0;
			self->decoder.cuBuffer.bAvail = tsk_false;
		}
		
		if(self->decoder.phMutex)
		{
			tsk_mutex_destroy(&self->decoder.phMutex);
		}
		
		TSK_FREE(self->decoder.accumulator);
		self->decoder.accumulator_pos = 0;
	}

	return 0;
}

int cuda_codec_h264_init(cuda_codec_h264_t* self, profile_idc_t profile)
{
	int ret = 0;
	level_idc_t level;
	tdav_codec_h264_common_t* common = (tdav_codec_h264_common_t*)self;

	if(!self)
	{
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	CudaUtils::Startup();

	if((ret = tdav_codec_h264_common_init(common)))
	{
		TSK_DEBUG_ERROR("cuda_codec_h264_common_init() faile with error code=%d", ret);
		return ret;
	}

	if((ret = tdav_codec_h264_common_level_from_size(TMEDIA_CODEC_VIDEO(self)->out.width, TMEDIA_CODEC_VIDEO(self)->out.height, &level)))
	{
		TSK_DEBUG_ERROR("Failed to find level for size=[%u, %u]", TMEDIA_CODEC_VIDEO(self)->out.width, TMEDIA_CODEC_VIDEO(self)->out.height);
		return ret;
	}

	(self)->encoder.max_bw_kpbs = tmedia_defaults_get_bandwidth_video_upload_max();
	common->pack_mode = H264_PACKETIZATION_MODE;
	common->profile = profile;
	common->level = level;
	TMEDIA_CODEC_VIDEO(self)->in.max_mbps = TMEDIA_CODEC_VIDEO(self)->out.max_mbps = H264_MAX_MBPS*1000;
	TMEDIA_CODEC_VIDEO(self)->in.max_br = TMEDIA_CODEC_VIDEO(self)->out.max_br = H264_MAX_BR*1000;
	
	TMEDIA_CODEC_VIDEO(self)->in.chroma = tmedia_chroma_nv12; // decoder
	TMEDIA_CODEC_VIDEO(self)->out.chroma = tmedia_chroma_yuv420p; // encoder

	self->encoder.quality = 1;
	
	return ret;
}

int cuda_codec_h264_deinit(cuda_codec_h264_t* self)
{
	if(!self)
	{
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	cuda_codec_h264_close((tmedia_codec_t*)self);

	return 0;
}

static inline tsk_size_t _cuda_codec_h264_pict_layout(cuda_codec_h264_t* self, void**output, tsk_size_t *output_size)
{
	if(self && self->decoder.cuBuffer.pcuPtr && self->decoder.cuBuffer.nSize)
	{
		const unsigned int w = TMEDIA_CODEC_VIDEO(self)->in.width;
		const unsigned int w_div_2 = (w >> 1);
		const unsigned int h = TMEDIA_CODEC_VIDEO(self)->in.height;
		const unsigned int h_div_2 = (h >> 1);
		const unsigned int pitch = self->decoder.cuBuffer.nPitch;
		const unsigned int pitch_div_2 = (pitch >> 1);
		const tsk_size_t xsize = (w * h * 3) >> 1;
		// resize if too short
		if(*output_size < xsize)
		{
			if((*output = tsk_realloc(*output, xsize)))
			{
				*output_size = xsize;
			}
			else
			{
				*output_size = 0;
				return 0;
			}
		}
		

		register unsigned int y;
        const unsigned char *p = (const unsigned char *)self->decoder.cuBuffer.pcuPtr, *q = p + (h * pitch);
        register unsigned char *i = (unsigned char *)*output, *j = i + (h * w);

        for (y = 0; y < h; y++)
		{
			 // luma
            memcpy(i, p, w);
			i += w;
			p += pitch;

			 // chroma
			memcpy(j, &q[(y&1) ? w_div_2 : 0], w_div_2);
			j += w_div_2;
			if(y&1)
			{
				q += pitch;
			}
        }
		
		return xsize;
	}
	return 0;
}



static int CUDAAPI _NVCallback_HandleVideoSequence(void *pvUserData, CUVIDEOFORMAT *pFormat)
{
	cuda_codec_h264_t* h264 = (cuda_codec_h264_t*)pvUserData;
	CUresult cuResult;

	if(!h264 || !pFormat)
	{
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;//error
	}

	tsk_mutex_lock(h264->decoder.phMutex);

	int ret = 1;
	// http://corecodec.com/products/coreavc/guide
	// CROP 1088 to 1080
	// H.264 encoded video size is always a multiple of 16, and sequences that are 1080 pixels high are encoded as 1088 padded at the bottom. 
	// Also H.264 specifications provides a set of cropping parameters to signal that parts of the encoded picture are not important and should not be displayed. 
	// Some H.264 encoders fail to specify cropping parameters when encoding 1080 video.
	int newWidth = pFormat->coded_width;//pFormat->display_area.right - pFormat->display_area.left;
	int newHeight = pFormat->coded_height;//pFormat->display_area.bottom - pFormat->display_area.top;

	if(newWidth != TMEDIA_CODEC_VIDEO(h264)->in.width || pFormat->coded_height != newHeight)
	{
		TSK_DEBUG_INFO("[H.264 CUDA decoder] display area = left:%d, right:%d, bottom:%d, top:%d",
				pFormat->display_area.left,
				pFormat->display_area.right,
				pFormat->display_area.bottom,
				pFormat->display_area.top
			);

		h264->decoder.cuInfo.ulWidth = newWidth;
		h264->decoder.cuInfo.ulTargetWidth = newWidth;
		h264->decoder.cuInfo.ulHeight = newHeight;
		h264->decoder.cuInfo.ulTargetHeight = newHeight;

		CUresult cuResult = cuCtxPushCurrent(h264->decoder.cuContext);
		if(cuResult != CUDA_SUCCESS)
		{
			TSK_DEBUG_ERROR("cuCtxPushCurrent failed with error code=%d", (int)cuResult);
			ret = 0; //error
			goto bail;
		}

		if(h264->decoder.pInst)
		{
			cuvidDestroyDecoder(h264->decoder.pInst);
			h264->decoder.pInst = NULL;
		}
		cuResult = cuvidCreateDecoder(&h264->decoder.pInst, &h264->decoder.cuInfo);
		if(CUDA_SUCCESS != cuResult)
		{
			TSK_DEBUG_ERROR("cuvidCreateDecoder failed with error code=%d", (int)cuResult);
			ret = 0; //error
			goto bail;
		}
		else
		{
			TMEDIA_CODEC_VIDEO(h264)->in.width = /*pFormat->coded_width*/newWidth;
			TMEDIA_CODEC_VIDEO(h264)->in.height = /*pFormat->coded_height*/newHeight;
			ret = 1; //success
		}
	}
bail:
	cuResult = cuCtxPopCurrent(NULL);
	tsk_mutex_unlock(h264->decoder.phMutex);
	return ret;//success
}

static int CUDAAPI _NVCallback_HandlePictureDecode(void *pvUserData, CUVIDPICPARAMS *pPicParams)
{
	cuda_codec_h264_t* h264 = (cuda_codec_h264_t*)pvUserData;
	if(!h264 || !pPicParams)
	{
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;//error
	}

	tsk_mutex_lock(h264->decoder.phMutex);
	CUresult cuResult = cuvidDecodePicture(h264->decoder.pInst, pPicParams);
	tsk_mutex_unlock(h264->decoder.phMutex);

    if(cuResult != CUDA_SUCCESS)
	{
		TSK_DEBUG_ERROR("cuvidDecodePicture failed with error code= %d", cuResult);
		return 0;//error
    }

	return 1;//success
}

static int CUDAAPI _NVCallback_HandlePictureDisplay(void *pvUserData, CUVIDPARSERDISPINFO *pPicParams)
{
	cuda_codec_h264_t* h264 = (cuda_codec_h264_t*)pvUserData;
	CUVIDPROCPARAMS vpp = {0};
	CUdeviceptr devPtr;
	CUresult cuResult;
	tsk_size_t nv12_size;
	tsk_bool_t mapped = tsk_false;
	int ret = 1;//success
	
	if(!h264 || !pPicParams)
	{
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;//error
	}

	cuResult = cuCtxPushCurrent(h264->decoder.cuContext);
	if(cuResult != CUDA_SUCCESS)
	{
		TSK_DEBUG_ERROR("cuCtxPushCurrent failed with error code = %d", (int)cuResult);
		ret = 0;//error
		goto bail;
	}
	
	vpp.progressive_frame = pPicParams->progressive_frame;
	vpp.top_field_first = pPicParams->top_field_first;
	cuResult = cuvidMapVideoFrame(h264->decoder.pInst, pPicParams->picture_index, &devPtr, &h264->decoder.cuBuffer.nPitch, &vpp);
	
	if(cuResult != CUDA_SUCCESS)
	{
		TSK_DEBUG_ERROR("cuvidMapVideoFrame failed with error code = %d", (int)cuResult);
		ret = 0;//error
		goto bail;
	}
	mapped = tsk_true;
	nv12_size = ((h264->decoder.cuBuffer.nPitch * TMEDIA_CODEC_VIDEO(h264)->in.height) * 3) >> 1;
	if ((!h264->decoder.cuBuffer.pcuPtr) || (nv12_size > h264->decoder.cuBuffer.nSize))
	{
		h264->decoder.cuBuffer.nSize = 0;
		if (h264->decoder.cuBuffer.pcuPtr)
		{
			cuResult = cuMemFreeHost(h264->decoder.cuBuffer.pcuPtr);
			h264->decoder.cuBuffer.pcuPtr = NULL;
		}
		cuResult = cuMemAllocHost((void**)&h264->decoder.cuBuffer.pcuPtr, nv12_size);
		if (cuResult != CUDA_SUCCESS)
		{
			TSK_DEBUG_ERROR("cuMemAllocHost failed to allocate %d bytes (error code=%d)", nv12_size, (int)cuResult);
			h264->decoder.cuBuffer.pcuPtr = tsk_null;
			h264->decoder.cuBuffer.nSize = 0;
			ret = 0;//error
		}
		else
		{
			h264->decoder.cuBuffer.nSize = nv12_size;
		}
	}
	if(h264->decoder.cuBuffer.pcuPtr)
	{
		cuResult = cuMemcpyDtoH(h264->decoder.cuBuffer.pcuPtr, devPtr, nv12_size);
	}

bail:
	if(mapped)
	{
		cuResult = cuvidUnmapVideoFrame(h264->decoder.pInst, devPtr);
	}
	cuResult = cuCtxPopCurrent(NULL);
	
	h264->decoder.cuBuffer.bAvail = (ret == 1);
	return ret;
}

static unsigned char* CUDAAPI _NVCallback_HandleAcquireBitStream(int *pBufferSize, void *pUserdata)
{
	cuda_codec_h264_t* h264 = (cuda_codec_h264_t*)pUserdata;
	if(!h264 || !pBufferSize)
	{
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
	
	*pBufferSize = (int)h264->encoder.nBufferSize;
	return (unsigned char*)h264->encoder.pBufferPtr;
}

static void CUDAAPI _NVCallback_HandleReleaseBitStream(int nBytesInBuffer, unsigned char *cb, void *pUserdata)
{
	tdav_codec_h264_common_t* common = (tdav_codec_h264_common_t*)pUserdata;
	if(!common || !cb || !nBytesInBuffer)
	{
		TSK_DEBUG_ERROR("Invalid parameter");
		return;
	}
	tdav_codec_h264_rtp_encap(common, (const uint8_t*)cb, (tsk_size_t)nBytesInBuffer);
}

static void CUDAAPI _NVCallback_HandleOnBeginFrame(const NVVE_BeginFrameInfo *pbfi, void *pUserdata)
{
    return;
}

static void CUDAAPI _NVCallback_HandleOnEndFrame(const NVVE_EndFrameInfo *pefi, void *pUserdata)
{
    return;
}