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

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tsk_memory.h"
#include "tsk_time.h"
#include "tsk_debug.h"

#define TDAV_VP8_PAY_DESC_SIZE				1 /* |X|R|N|S|PartID| */
#define TDAV_SYSTEM_CORES_COUNT				1
#define TDAV_VP8_GOP_SIZE_IN_SECONDS		2
#define TDAV_VP8_RTP_PAYLOAD_MAX_SIZE		950

#define vp8_interface_enc (vpx_codec_vp8_cx())
#define vp8_interface_dec (vpx_codec_vp8_dx())

static void tdav_codec_vp8_encap(tdav_codec_vp8_t* self, const vpx_codec_cx_pkt_t *pkt);
static void tdav_codec_vp8_rtp_callback(tdav_codec_vp8_t *self, const void *data, tsk_size_t size, uint32_t partID, tsk_bool_t part_start, tsk_bool_t non_ref, tsk_bool_t last);

/* ============ VP8 Plugin interface ================= */

static int tdav_codec_vp8_open(tmedia_codec_t* self)
{
	tdav_codec_vp8_t* vp8 = (tdav_codec_vp8_t*)self;

	vpx_codec_enc_cfg_t enc_cfg;
	vpx_codec_dec_cfg_t dec_cfg;
	vpx_enc_frame_flags_t enc_flags;
	vpx_codec_flags_t dec_flags = 0;
	vpx_codec_err_t vpx_ret;

	if(!vp8){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* the caller (base class) already checked that the codec is not opened */

	//
	//	Encoder
	//
	if((vpx_ret = vpx_codec_enc_config_default(vp8_interface_enc, &enc_cfg, 0)) != VPX_CODEC_OK){
		TSK_DEBUG_ERROR("vpx_codec_enc_config_default failed with error =%s", vpx_codec_err_to_string(vpx_ret));
		return -2;
	}
	enc_cfg.g_timebase.num = 1;
	enc_cfg.g_timebase.den = TMEDIA_CODEC_VIDEO(vp8)->out.fps;
	enc_cfg.rc_target_bitrate = TMEDIA_CODEC_VIDEO(vp8)->out.width * TMEDIA_CODEC_VIDEO(vp8)->out.height * enc_cfg.rc_target_bitrate
                        / enc_cfg.g_w / enc_cfg.g_h;
	enc_cfg.g_w = TMEDIA_CODEC_VIDEO(vp8)->out.width;
	enc_cfg.g_h = TMEDIA_CODEC_VIDEO(vp8)->out.height;
	enc_cfg.g_error_resilient = VPX_ERROR_RESILIENT_DEFAULT;
	enc_cfg.g_lag_in_frames = 0;
	enc_cfg.g_threads = TDAV_SYSTEM_CORES_COUNT;

	enc_flags = 0; //VPX_EFLAG_XXX

	if((vpx_ret = vpx_codec_enc_init(&vp8->encoder.context, vp8_interface_enc, &enc_cfg, enc_flags)) != VPX_CODEC_OK){
		TSK_DEBUG_ERROR("vpx_codec_enc_init failed with error =%s", vpx_codec_err_to_string(vpx_ret));
		return -3;
	}
	vp8->encoder.pic_id = (rand() ^ rand()) % 0x7FFF;
	vp8->encoder.gop_size = TDAV_VP8_GOP_SIZE_IN_SECONDS * TMEDIA_CODEC_VIDEO(vp8)->out.fps;
	vp8->encoder.initialized = tsk_true;

	//
	//	Decoder
	//
	dec_cfg.w = TMEDIA_CODEC_VIDEO(vp8)->out.width;
	dec_cfg.h = TMEDIA_CODEC_VIDEO(vp8)->out.height;
	dec_cfg.threads = TDAV_SYSTEM_CORES_COUNT;
	if((vpx_ret = vpx_codec_dec_init(&vp8->decoder.context, vp8_interface_dec, &dec_cfg, dec_flags)) != VPX_CODEC_OK){
		TSK_DEBUG_ERROR("vpx_codec_dec_init failed with error =%s", vpx_codec_err_to_string(vpx_ret));
		return -4;
	}
	vp8->decoder.initialized = tsk_true;
	
	
	
	
	return 0;
}

static int tdav_codec_vp8_close(tmedia_codec_t* self)
{
	tdav_codec_vp8_t* vp8 = (tdav_codec_vp8_t*)self;

	if(!vp8){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	if(vp8->encoder.initialized){
		vpx_codec_destroy(&vp8->encoder.context);
		vp8->encoder.initialized = tsk_false;
	}
	if(vp8->decoder.initialized){
		vpx_codec_destroy(&vp8->decoder.context);
		vp8->decoder.initialized = tsk_false;
	}
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

	// flip
	if(self->video.flip.encoded){
		vpx_img_flip(&image);
	}

	// encode data
	++vp8->encoder.pts;
	if(vp8->encoder.frame_count == 0 || (vp8->encoder.frame_count % vp8->encoder.gop_size) == 0){
		flags |= VPX_EFLAG_FORCE_KF;
	}
	if((vpx_ret = vpx_codec_encode(&vp8->encoder.context, &image, vp8->encoder.pts, 1, flags, VPX_DL_REALTIME)) != VPX_CODEC_OK){
		TSK_DEBUG_ERROR("vpx_codec_encode failed with error =%s", vpx_codec_err_to_string(vpx_ret));
		vpx_img_free(&image);
		return 0;
	}

	++vp8->encoder.frame_count;

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
	tsk_size_t xmax_size, ret = 0;
	uint8_t X, R, N, S, PartID; // |X|R|N|S|PartID|

	if(!self || !in_data || in_size<1 || !out_data || !vp8->decoder.initialized){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	
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
	++pdata, --in_size;
	// check "OPTIONAL" headers
	if(X){
		tsk_size_t size_to_skip = 0;
		// X: |I|L|T|  RSV-A  |
		if(*pdata & 0x80){//I
			++size_to_skip;// PictureID, not used yet
		}
		if(*pdata & 0x40){//L
			++size_to_skip;//TL0PICIDX, not used yet
		}
		if(*pdata & 0x20){//T
			++size_to_skip;//TID and RSV-B, not used yet
		}
		if(in_size<=size_to_skip){
			TSK_DEBUG_ERROR("Too short");
			goto bail;
		}
	}

	// Packet lost?
	if(vp8->decoder.last_seq != (rtp_hdr->seq_num - 1) && vp8->decoder.last_seq){
		if(vp8->decoder.last_seq == rtp_hdr->seq_num){
			// Could happen on some stupid emulators
			TSK_DEBUG_INFO("Packet duplicated, seq_num=%d", rtp_hdr->seq_num);
			goto bail;
		}
		TSK_DEBUG_INFO("Packet lost, seq_num=%d", rtp_hdr->seq_num);
	}
	vp8->decoder.last_seq = rtp_hdr->seq_num;

	xmax_size = (TMEDIA_CODEC_VIDEO(vp8)->in.width * TMEDIA_CODEC_VIDEO(vp8)->in.height * 3) >> 1;

	// start-accumulator
	if(!vp8->decoder.accumulator){
		if(in_size > xmax_size){
			TSK_DEBUG_ERROR("%u too big to contain valid encoded data. xmax_size=%u", in_size, xmax_size);
			goto bail;
		}
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
		vpx_codec_iter_t  iter = tsk_null;
		vpx_codec_err_t vpx_ret = vpx_codec_decode(&vp8->decoder.context, vp8->decoder.accumulator, vp8->decoder.accumulator_pos, tsk_null, 0);
		// in all cases: reset accumulator
		vp8->decoder.accumulator_pos = 0;
		if(vpx_ret != VPX_CODEC_OK){
			TSK_DEBUG_ERROR("vpx_codec_decode failed with error =%s", vpx_codec_err_to_string(vpx_ret));
			goto bail;
		}
		// update sizes
		TMEDIA_CODEC_VIDEO(vp8)->in.width = vp8->decoder.context.config.dec->w;
		TMEDIA_CODEC_VIDEO(vp8)->in.height = vp8->decoder.context.config.dec->h;
		xmax_size = (TMEDIA_CODEC_VIDEO(vp8)->in.width * TMEDIA_CODEC_VIDEO(vp8)->in.height * 3) >> 1;
		// allocate destination buffer
		if(*out_max_size <xmax_size){
			if(!(*out_data = tsk_realloc(*out_data, xmax_size))){
				TSK_DEBUG_ERROR("Failed to allocate new buffer");
				vp8->decoder.accumulator_pos = 0;
				*out_max_size = 0;
				goto bail;
			}
			*out_max_size = xmax_size;
		}
		// copy decoded data
		ret = 0;
		while((img = vpx_codec_get_frame(&vp8->decoder.context, &iter))){
			unsigned int plane, y;
			// flip
			if(self->video.flip.decoded){
				vpx_img_flip(img);
			}
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

	vp8->decoder.last_PartID = PartID;
	vp8->decoder.last_S = S;
	vp8->decoder.last_N = N;
	return ret;
}

static tsk_bool_t tdav_codec_vp8_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{
	unsigned width, height, fps;

	if(tmedia_parse_video_fmtp(fmtp, codec->bl, &width, &height, &fps)){
		TSK_DEBUG_ERROR("Failed to match fmtp=%s", fmtp);
		return tsk_false;
	}

	TMEDIA_CODEC_VIDEO(codec)->in.width = TMEDIA_CODEC_VIDEO(codec)->out.width = width;
	TMEDIA_CODEC_VIDEO(codec)->in.height = TMEDIA_CODEC_VIDEO(codec)->out.height = height;
	TMEDIA_CODEC_VIDEO(codec)->in.fps = TMEDIA_CODEC_VIDEO(codec)->out.fps = fps;

	return tsk_true;
}

static char* tdav_codec_vp8_fmtp_get(const tmedia_codec_t* self)
{
	switch(self->bl){
		case tmedia_bl_low:
		default:
			return tsk_strdup("QCIF=2;SQCIF=2");
			break;
		case tmedia_bl_medium:
		case tmedia_bl_hight:
		case tmedia_bl_unrestricted:
			return tsk_strdup("CIF=2;QCIF=2;SQCIF=2");
			break;
	}
}

static int tdav_codec_vp8_fmtp_set(tmedia_codec_t* self, const char* fmtp)
{
	TSK_DEBUG_INFO("remote fmtp=%s", fmtp);
	return 0;
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
	tsk_false,
	90000, // rate
	
	/* audio */
	{ 0 },

	/* video (defaul width,height,fps) */
	{176, 144, 15},

	tdav_codec_vp8_open,
	tdav_codec_vp8_close,
	tdav_codec_vp8_encode,
	tdav_codec_vp8_decode,
	tdav_codec_vp8_fmtp_match,
	tdav_codec_vp8_fmtp_get,
	tdav_codec_vp8_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_vp8_plugin_def_t = &tdav_codec_vp8_plugin_def_s;

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
	if((is_keyframe && (*frame_ptr & 0x01)) || (!is_keyframe && !(*frame_ptr & 0x01))){// 4.2. VP8 Payload Header
		TSK_DEBUG_ERROR("Invalid payload header");
		return;
	}

	// first partition (contains modes and motion vectors)
	part_ID = 0; // The first VP8 partition(containing modes and motion vectors) MUST be labeled with PartID = 0
	part_start = tsk_true;
	part_size = (frame_ptr[2] << 16) | (frame_ptr[1] << 8) | frame_ptr[0];
	part_size = (part_size >> 5) & 0x7FFFF;
	if(part_size > pkt_size){
		TSK_DEBUG_ERROR("part_size is > pkt_size(%u,%u)", part_size, pkt_size);
		return;
	}
	tdav_codec_vp8_rtp_callback(self, &frame_ptr[index], part_size, part_ID, part_start, non_ref, (index + part_size)==pkt_size);
	index += part_size;

	// second,third,... partitions (or fragment if part_size > TDAV_VP8_RTP_PAYLOAD_MAX_SIZE)
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
	/* draft-ietf-payload-vp8-01 - 4.1. VP8 Payload Descriptor
		   0 1 2 3 4 5 6 7
		  +-+-+-+-+-+-+-+-+
		  |X|R|N|S|PartID | (REQUIRED)
		  +-+-+-+-+-+-+-+-+
	   X: |I|L|T|  RSV-A  | (OPTIONAL)
		  +-+-+-+-+-+-+-+-+
	   I: |   PictureID   | (OPTIONAL)
		  +-+-+-+-+-+-+-+-+
	   L: |   TL0PICIDX   | (OPTIONAL)
		  +-+-+-+-+-+-+-+-+
	   T: | TID |  RSV-B  | (OPTIONAL)
		  +-+-+-+-+-+-+-+-+
		
	  draft-ietf-payload-vp8-01 - 4.2. VP8 Payload Header
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
	// X and R are equal to zero
	self->encoder.rtp.ptr[0] = (partID & 0x0F) // PartID
		| (non_ref << 5) & 0x20 // N
		| (part_start << 4) &0x10 // S
		;

	/* 4.2. VP8 Payload Header */
	if(has_hdr){
		// already part of the encoded stream
	}

	// Send data over the network
	if(TMEDIA_CODEC_VIDEO(self)->callback){
		TMEDIA_CODEC_VIDEO(self)->callback(TMEDIA_CODEC_VIDEO(self)->callback_data, self->encoder.rtp.ptr, (size + TDAV_VP8_PAY_DESC_SIZE), (3003* (30/TMEDIA_CODEC_VIDEO(self)->out.fps)), last);
	}
}

#endif /* HAVE_LIBVPX */
