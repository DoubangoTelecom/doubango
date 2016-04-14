/*
* Copyright (C) 2014-2016 Mamadou DIOP.
*
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

/**@file tdav_codec_h264_cisco.cxx
 * @brief H.264 codec plugin using OpenH264 (https://github.com/cisco/openh264) v1.5.0 for encoding/decoding.
 */
#include "tinydav/codecs/h264/tdav_codec_h264_cisco.h"

#if HAVE_OPENH264

#include "tinydav/codecs/h264/tdav_codec_h264_common.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tinymedia/tmedia_codec.h"
#include "tinymedia/tmedia_params.h"
#include "tinymedia/tmedia_defaults.h"

#include "tsk_mutex.h"
#include "tsk_params.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

extern "C" {
#include <wels/codec_api.h>
#include <wels/codec_app_def.h>
}

#include <limits.h> /* INT_MAX */

typedef struct tdav_codec_h264_cisco_s {
    TDAV_DECLARE_CODEC_H264_COMMON;

    // Encoder
    struct {
        ISVCEncoder *pInst;
        SEncParamExt sEncParam;
        SSourcePicture sEncPic;
        void* buffer;
        int64_t frame_count;
        tsk_bool_t force_idr;
        int rotation;
        int neg_width;
        int neg_height;
        int neg_fps;
        tsk_mutex_handle_t* mutex;
    } encoder;

    // decoder
    struct {
        ISVCDecoder* pInst;
        void* accumulator;
        tsk_size_t accumulator_pos;
        tsk_size_t accumulator_size;
        uint16_t last_seq;
    } decoder;
}
tdav_codec_h264_cisco_t;

#if !defined(CISCO_H264_GOP_SIZE_IN_SECONDS)
#	define CISCO_H264_GOP_SIZE_IN_SECONDS		25
#endif

#define kResetRotationTrue tsk_true
#define kResetRotationFalse tsk_false

static int tdav_codec_h264_cisco_init(tdav_codec_h264_cisco_t* self, profile_idc_t profile);
static int tdav_codec_h264_cisco_deinit(tdav_codec_h264_cisco_t* self);
static int tdav_codec_h264_cisco_open_encoder(tdav_codec_h264_cisco_t* self);
static int tdav_codec_h264_cisco_close_encoder(tdav_codec_h264_cisco_t* self, tsk_bool_t reset_rotation);
static int tdav_codec_h264_cisco_open_decoder(tdav_codec_h264_cisco_t* self);
static int tdav_codec_h264_cisco_close_decoder(tdav_codec_h264_cisco_t* self);
static ELevelIdc tdav_codec_h264_cisco_convert_level(enum level_idc_e level);
static void tdav_codec_h264_cisco_debug_cb(void* context, int level, const char* message);

static void (*__tdav_codec_h264_cisco_debug_cb)(void* context, int level, const char* message) = tdav_codec_h264_cisco_debug_cb;

/* ============ H.264 Base/Main Profile X.X Plugin interface functions ================= */

static int tdav_codec_h264_cisco_set(tmedia_codec_t* self, const tmedia_param_t* param)
{
    tdav_codec_h264_cisco_t* h264 = (tdav_codec_h264_cisco_t*)self;
    tsk_bool_t reconf = tsk_false;
    if (param->value_type == tmedia_pvt_int32) {
        if (tsk_striequals(param->key, "action")) {
            tmedia_codec_action_t action = (tmedia_codec_action_t)TSK_TO_INT32((uint8_t*)param->value);
            switch (action) {
            case tmedia_codec_action_encode_idr: {
                TSK_DEBUG_INFO("OpenH264 force_idr action");
                h264->encoder.force_idr = tsk_true;
                return 0;
            }
            case tmedia_codec_action_bw_up:
            case tmedia_codec_action_bw_down: {
                int32_t rc_target_bitrate;
                int32_t bandwidth_max_upload_bps = TMEDIA_CODEC(h264)->bandwidth_max_upload == INT_MAX ? TMEDIA_CODEC(h264)->bandwidth_max_upload : (TMEDIA_CODEC(h264)->bandwidth_max_upload * 1024); // kbps -> bps
                if (action == tmedia_codec_action_bw_up) {
                    rc_target_bitrate = TSK_CLAMP(0, (int32_t)((h264->encoder.sEncParam.iTargetBitrate * 3) >> 1), bandwidth_max_upload_bps);
                }
                else {
                    rc_target_bitrate = TSK_CLAMP(0, (int32_t)((h264->encoder.sEncParam.iTargetBitrate << 1) / 3), bandwidth_max_upload_bps);
                }
                h264->encoder.sEncParam.iTargetBitrate = rc_target_bitrate;
                h264->encoder.sEncParam.iMaxBitrate = rc_target_bitrate;
                SSpatialLayerConfig* layer = &h264->encoder.sEncParam.sSpatialLayers[0];
                layer->iMaxSpatialBitrate = h264->encoder.sEncParam.iMaxBitrate;
                layer->iSpatialBitrate = h264->encoder.sEncParam.iTargetBitrate;
                reconf = tsk_true;
                TSK_DEBUG_INFO("OpenH264  new target bitrate = %d bps", rc_target_bitrate);
                break;
            }
            }
        }
        else if (tsk_striequals(param->key, "bw_kbps")) { // both up and down (from the SDP)
            int32_t max_bw_userdefine_kbps = tmedia_defaults_get_bandwidth_video_upload_max();
            int32_t max_bw_new_kbps = *((int32_t*)param->value), max_bitrate_bps;
            if (max_bw_userdefine_kbps > 0) {
                // do not use more than what the user defined in it's configuration
                TMEDIA_CODEC(h264)->bandwidth_max_upload = TSK_MIN(max_bw_new_kbps, max_bw_userdefine_kbps);
            }
            else {
                TMEDIA_CODEC(h264)->bandwidth_max_upload = max_bw_new_kbps;
            }
            max_bitrate_bps = TMEDIA_CODEC(h264)->bandwidth_max_upload << 10; // convert from kbps to bps
            TSK_DEBUG_INFO("OpenH264 codec: bandwidth-max-upload= %dbps, %dkbps", max_bitrate_bps, TMEDIA_CODEC(h264)->bandwidth_max_upload);
            h264->encoder.sEncParam.iTargetBitrate = max_bitrate_bps;
            h264->encoder.sEncParam.iMaxBitrate = max_bitrate_bps;
            SSpatialLayerConfig* layer = &h264->encoder.sEncParam.sSpatialLayers[0];
            layer->iMaxSpatialBitrate = h264->encoder.sEncParam.iMaxBitrate;
            layer->iSpatialBitrate = h264->encoder.sEncParam.iTargetBitrate;
            reconf = tsk_true;
        }
        else if (tsk_striequals(param->key, "bandwidth-max-upload")) {
            int32_t bw_max_upload_kbps = *((int32_t*)param->value);
            TSK_DEBUG_INFO("OpenH264 codec: bandwidth-max-upload= %d kbps", bw_max_upload_kbps);
            TMEDIA_CODEC(h264)->bandwidth_max_upload = bw_max_upload_kbps;
            reconf = tsk_true;
        }
        else if (tsk_striequals(param->key, "rotation")) {
            int rotation = *((int32_t*)param->value);
            if (h264->encoder.rotation != rotation) {
                h264->encoder.rotation = rotation;
                if (self->opened) {
                    int ret;
                    if ((ret = tdav_codec_h264_cisco_close_encoder(h264, kResetRotationFalse))) {
                        return ret;
                    }
                    if ((ret = tdav_codec_h264_cisco_open_encoder(h264))) {
                        return ret;
                    }
                }
            }
            return 0;
        }
		else if (tsk_striequals(param->key, "out-size")) {
			int ret;
			uint32_t new_size = *((uint32_t*)param->value);
			uint16_t new_width = (new_size & 0xFFFF);
			uint16_t new_height = (new_size >> 16) & 0xFFFF;
			if (self->opened) {
				// It's up to the caller to lock the codec or make sure no other code will code encode() function.
				// We must not call lock/unlock(encoder.mutex) here because close() will free the mutex
				// close encoder
                if ((ret = tdav_codec_h264_cisco_close_encoder(h264, kResetRotationFalse))) {
                    return ret;
                }
				// update size
				h264->encoder.neg_width = TMEDIA_CODEC_VIDEO(h264)->out.width = new_width;
				h264->encoder.neg_height = TMEDIA_CODEC_VIDEO(h264)->out.height = new_height;
				// re-open encoder
                if ((ret = tdav_codec_h264_cisco_open_encoder(h264))) {
                    return ret;
                }
            }
			else {
				// update size
				h264->encoder.neg_width = TMEDIA_CODEC_VIDEO(h264)->out.width = new_width;
				h264->encoder.neg_height = TMEDIA_CODEC_VIDEO(h264)->out.height = new_height;
			}
			return 0;
		}
    }

    if (reconf) {
        if (h264->encoder.pInst) {
            long err;
            // lock required because of https://code.google.com/p/doubango/issues/detail?id=422
            tsk_mutex_lock(h264->encoder.mutex);
            err = h264->encoder.pInst->InitializeExt(&h264->encoder.sEncParam);
            tsk_mutex_unlock(h264->encoder.mutex);
            if (err != cmResultSuccess) {
                TSK_DEBUG_ERROR("InitializeExt failed: %ld", err);
                return -1;
            }
        }
        return 0;
    }

    return -1;
}


static int tdav_codec_h264_cisco_open(tmedia_codec_t* self)
{
    int ret;
    tdav_codec_h264_cisco_t* h264 = (tdav_codec_h264_cisco_t*)self;

    if (!h264) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* the caller (base class) already checked that the codec is not opened */

    //	Encoder
    if ((ret = tdav_codec_h264_cisco_open_encoder(h264))) {
        return ret;
    }

    //	Decoder
    if ((ret = tdav_codec_h264_cisco_open_decoder(h264))) {
        return ret;
    }

    return 0;
}

static int tdav_codec_h264_cisco_close(tmedia_codec_t* self)
{
    tdav_codec_h264_cisco_t* h264 = (tdav_codec_h264_cisco_t*)self;

    if (!h264) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* the caller (base class) already checked that the codec is opened */

    //	Encoder
    tdav_codec_h264_cisco_close_encoder(h264, kResetRotationTrue);

    //	Decoder
    tdav_codec_h264_cisco_close_decoder(h264);

    return 0;
}

static tsk_size_t tdav_codec_h264_cisco_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
    long err;
    tsk_bool_t send_idr, send_hdr;
    tsk_size_t in_xsize;
    SFrameBSInfo bsInfo;

    tdav_codec_h264_cisco_t* h264 = (tdav_codec_h264_cisco_t*)self;
    tdav_codec_h264_common_t* common = (tdav_codec_h264_common_t*)self;

    if (!self || !in_data || !in_size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    if (!self->opened || !h264->encoder.pInst) {
        TSK_DEBUG_ERROR("Encoder not opened or not ready");
        return 0;
    }

    in_xsize = (h264->encoder.sEncPic.iPicHeight * h264->encoder.sEncPic.iPicWidth * 3) >> 1;
    if (in_xsize != in_size) {
        /* guard */
        TSK_DEBUG_ERROR("Invalid size: %u<>%u", in_xsize, in_size);
        return 0;
    }

    // send IDR for:
    //	- the first frame
    //  - remote peer requested an IDR
    //	- every second within the first 4seconds
    send_idr = (
                   h264->encoder.frame_count++ == 0
                   || h264 ->encoder.force_idr
                   //|| ( (h264->encoder.frame_count < (int)TMEDIA_CODEC_VIDEO(h264)->out.fps * 4) && ((h264->encoder.frame_count % TMEDIA_CODEC_VIDEO(h264)->out.fps)==0) )
               );

    // send SPS and PPS headers for:
    //  - IDR frames (not required but it's the easiest way to deal with pkt loss)
    //  - every 5 seconds after the first 4seconds
    send_hdr = (
                   send_idr
                   //|| ( (h264->encoder.frame_count % (TMEDIA_CODEC_VIDEO(h264)->out.fps * 5))==0 )
               );

    if (send_idr) {
        TSK_DEBUG_INFO("OpenH264 call ForceIntraFrame");
        if ((err = h264->encoder.pInst->ForceIntraFrame(true)) != cmResultSuccess) {
            TSK_DEBUG_WARN("OpenH264 ForceIntraFrame(%d) failed: %ld", send_idr, err);
        }
    }
    if (send_hdr) {
#if 0 // Not needed
        memset(&bsInfo, 0, sizeof(bsInfo));
        tsk_mutex_lock(h264->encoder.mutex);
        if ((err = h264->encoder.pInst->EncodeParameterSets(&bsInfo)) != cmResultSuccess) {
            TSK_DEBUG_WARN("OpenH264 EncodeParameterSets(%d) failed: %ld", send_idr, err);
        }
        else {
            for (int iLayerNum = 0; iLayerNum < bsInfo.iLayerNum; ++iLayerNum) {
                unsigned char* pBsBuf = bsInfo.sLayerInfo[iLayerNum].pBsBuf;
                int iNalLengthInByte = 0, _iNalLengthInByte;
                for (int iNalCount = 0; iNalCount < bsInfo.sLayerInfo[iLayerNum].iNalCount; ++iNalCount) {
                    if ((_iNalLengthInByte = bsInfo.sLayerInfo[iLayerNum].pNalLengthInByte[iNalCount]) > 0) {
                        iNalLengthInByte += _iNalLengthInByte;
                    }
                }
                if (iNalLengthInByte > 0) {
                    tdav_codec_h264_rtp_encap(TDAV_CODEC_H264_COMMON(h264), pBsBuf, (tsk_size_t)iNalLengthInByte);
                }
            }
        }
        tsk_mutex_unlock(h264->encoder.mutex);
#endif
    }

    h264->encoder.sEncPic.pData[0] = ((unsigned char*)in_data);
    h264->encoder.sEncPic.pData[1] = h264->encoder.sEncPic.pData[0] + (h264->encoder.sEncPic.iPicHeight * h264->encoder.sEncPic.iPicWidth);
    h264->encoder.sEncPic.pData[2] = h264->encoder.sEncPic.pData[1] + ((h264->encoder.sEncPic.iPicHeight * h264->encoder.sEncPic.iPicWidth) >> 2);
    // h264->encoder.sEncPic.uiTimeStamp = rand();

    memset(&bsInfo, 0, sizeof(bsInfo));

    tsk_mutex_lock(h264->encoder.mutex);
    if ((err = h264->encoder.pInst->EncodeFrame(&h264->encoder.sEncPic, &bsInfo)) != cmResultSuccess) {
        TSK_DEBUG_ERROR("OpenH264 setting EncodeFrame() failed: %ld", err);
        tsk_mutex_unlock(h264->encoder.mutex);
        return 0;
    }

    // Memory held by bsInfo is freed when "InitializeExt()" is called this is why the unlock is after reading the output stream
    if (bsInfo.eFrameType != videoFrameTypeInvalid) {
        for (int iLayerNum = 0; iLayerNum < bsInfo.iLayerNum; ++iLayerNum) {
            unsigned char* pBsBuf = bsInfo.sLayerInfo[iLayerNum].pBsBuf;
            int iNalLengthInByte = 0, _iNalLengthInByte;
            for (int iNalCount = 0; iNalCount < bsInfo.sLayerInfo[iLayerNum].iNalCount; ++iNalCount) {
                if ((_iNalLengthInByte = bsInfo.sLayerInfo[iLayerNum].pNalLengthInByte[iNalCount]) > 0) {
                    iNalLengthInByte += _iNalLengthInByte;
                }
            }
            if (iNalLengthInByte > 0) {
                tdav_codec_h264_rtp_encap(TDAV_CODEC_H264_COMMON(h264), pBsBuf, (tsk_size_t)iNalLengthInByte);
            }
        }
    }
    tsk_mutex_unlock(h264->encoder.mutex);

    h264 ->encoder.force_idr = tsk_false; // reset

    return 0;
}

static tsk_size_t tdav_codec_h264_cisco_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
    tdav_codec_h264_cisco_t* h264 = (tdav_codec_h264_cisco_t*)self;
    const trtp_rtp_header_t* rtp_hdr = (const trtp_rtp_header_t*)proto_hdr;

    const uint8_t* pay_ptr = tsk_null;
    tsk_size_t pay_size = 0;
    int ret;
    long err = cmResultSuccess;
    tsk_bool_t append_scp, end_of_unit, got_picture_ptr = tsk_false;
    tsk_bool_t sps_or_pps;
    tsk_size_t retsize = 0, size_to_copy = 0;
    static const tsk_size_t xmax_size = (3840 * 2160 * 3) >> 3; // >>3 instead of >>1 (not an error)
    static tsk_size_t start_code_prefix_size = sizeof(H264_START_CODE_PREFIX);

    if (!h264 || !in_data || !in_size || !out_data) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    if (!self->opened || !h264->encoder.pInst) {
        TSK_DEBUG_ERROR("Decoder not opened or not ready");
        return 0;
    }

    /* Packet lost? */
    if ((h264->decoder.last_seq + 1) != rtp_hdr->seq_num && h264->decoder.last_seq) {
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
    if (*((uint8_t*)in_data) & 0x80) {
        TSK_DEBUG_WARN("F=1");
        /* reset accumulator */
        h264->decoder.accumulator_pos = 0;
        return 0;
    }

    /* get payload */
    if ((ret = tdav_codec_h264_get_pay(in_data, in_size, (const void**)&pay_ptr, &pay_size, &append_scp, &end_of_unit)) || !pay_ptr || !pay_size) {
        TSK_DEBUG_ERROR("Depayloader failed to get H.264 content");
        return 0;
    }
    //append_scp = tsk_true;
    size_to_copy = pay_size + (append_scp ? start_code_prefix_size : 0);
    // whether it's SPS or PPS (append_scp is false for subsequent FUA chuncks)
    sps_or_pps = append_scp && pay_ptr && ((pay_ptr[0] & 0x1F) == 7 || (pay_ptr[0] & 0x1F) == 8);

    // start-accumulator
    if (!h264->decoder.accumulator) {
        if (size_to_copy > xmax_size) {
            TSK_DEBUG_ERROR("%u too big to contain valid encoded data. xmax_size=%u", size_to_copy, xmax_size);
            return 0;
        }
        if (!(h264->decoder.accumulator = tsk_calloc(size_to_copy, sizeof(uint8_t)))) {
            TSK_DEBUG_ERROR("Failed to allocated new buffer");
            return 0;
        }
        h264->decoder.accumulator_size = size_to_copy;
    }
    if ((h264->decoder.accumulator_pos + size_to_copy) >= xmax_size) {
        TSK_DEBUG_ERROR("BufferOverflow");
        h264->decoder.accumulator_pos = 0;
        return 0;
    }
    if ((h264->decoder.accumulator_pos + size_to_copy) > h264->decoder.accumulator_size) {
        if (!(h264->decoder.accumulator = tsk_realloc(h264->decoder.accumulator, (h264->decoder.accumulator_pos + size_to_copy)))) {
            TSK_DEBUG_ERROR("Failed to reallocated new buffer");
            h264->decoder.accumulator_pos = 0;
            h264->decoder.accumulator_size = 0;
            return 0;
        }
        h264->decoder.accumulator_size = (h264->decoder.accumulator_pos + size_to_copy);
    }

    if (append_scp) {
        memcpy(&((uint8_t*)h264->decoder.accumulator)[h264->decoder.accumulator_pos], H264_START_CODE_PREFIX, start_code_prefix_size);
        h264->decoder.accumulator_pos += start_code_prefix_size;
    }
    memcpy(&((uint8_t*)h264->decoder.accumulator)[h264->decoder.accumulator_pos], pay_ptr, pay_size);
    h264->decoder.accumulator_pos += pay_size;
    // end-accumulator

    if (/*rtp_hdr->marker*/end_of_unit) {
        /* decode the picture */
        unsigned char* out_ptr[3] = { NULL };
        int out_stride[2] = {0}, out_width = 0, out_height = 0;
        tsk_size_t out_xsize;

        // Decode a Unit
        err = h264->decoder.pInst->DecodeFrame(
                  (const unsigned char*)h264->decoder.accumulator, h264->decoder.accumulator_pos,
                  out_ptr, out_stride, out_width, out_height);

        if (err != cmResultSuccess) {
            if (0 && err == dsDataErrorConcealed) {
                TSK_DEBUG_INFO("OpenH264: Data error concealed");
                err = cmResultSuccess;
            }
            else {
                TSK_DEBUG_WARN("OpenH264: DecodeFrame failed: %ld", err);
                goto bail;
            }
        }
        // Do we have a complete frame?
        if (!(got_picture_ptr = ((out_ptr[0] && out_ptr[1] && out_ptr[2]) && (out_stride[0] && out_stride[1]) && out_width && out_height))) {
            goto bail;
        }
        out_xsize = (out_width * out_height * 3) >> 1; // I420
        /* IDR ? */
        if (((pay_ptr[0] & 0x1F) == 0x05) && TMEDIA_CODEC_VIDEO(self)->in.callback) {
            TSK_DEBUG_INFO("Decoded H.264 IDR");
            TMEDIA_CODEC_VIDEO(self)->in.result.type = tmedia_video_decode_result_type_idr;
            TMEDIA_CODEC_VIDEO(self)->in.result.proto_hdr = proto_hdr;
            TMEDIA_CODEC_VIDEO(self)->in.callback(&TMEDIA_CODEC_VIDEO(self)->in.result);
        }
        /* fill out */
        if (*out_max_size < out_xsize) {
            if ((*out_data = tsk_realloc(*out_data, out_xsize))) {
                *out_max_size = out_xsize;
            }
            else {
                *out_max_size = 0;
                return 0;
            }
        }
        TMEDIA_CODEC_VIDEO(h264)->in.width = out_width;
        TMEDIA_CODEC_VIDEO(h264)->in.height = out_height;
        /* layout picture */
        {
            int plane, y, stride;
            retsize = 0;
            for (plane=0; plane < 3; plane++) {
                unsigned char *buf = out_ptr[plane];
                stride = out_stride[plane ? 1 : 0];
                for (y=0; y<out_height >> (plane ? 1 : 0); y++) {
                    unsigned int w_count = out_width >> (plane ? 1 : 0);
                    if ((ret + w_count) > *out_max_size) {
                        TSK_DEBUG_ERROR("BufferOverflow");
                        ret = 0;
                        goto bail;
                    }
                    memcpy(((uint8_t*)*out_data) + retsize, buf, w_count);
                    retsize += w_count;
                    buf += stride;
                }
            }
        }
    } // else if(rtp_hdr->marker)

bail:
    /* end of frame */
    if (got_picture_ptr) {
        int32_t endOfStream = 1;
        err = h264->decoder.pInst->SetOption(DECODER_OPTION_END_OF_STREAM, (void*)&endOfStream);
        if (err != cmResultSuccess) {
            TSK_DEBUG_WARN("OpenH264 setting DECODER_OPTION_END_OF_STREAM failed: %ld", err);
            goto bail;
        }
    }
    if (/*rtp_hdr->marker*/end_of_unit) {
        /* reset accumulator */
        h264->decoder.accumulator_pos = 0;
    }
    if (err != cmResultSuccess) {
        TSK_DEBUG_INFO("Failed to decode the buffer with error code =%ld, size=%u, append=%s", err, h264->decoder.accumulator_pos, append_scp ? "yes" : "no");
        if (TMEDIA_CODEC_VIDEO(self)->in.callback) {
            TMEDIA_CODEC_VIDEO(self)->in.result.type = tmedia_video_decode_result_type_error;
            TMEDIA_CODEC_VIDEO(self)->in.result.proto_hdr = proto_hdr;
            TMEDIA_CODEC_VIDEO(self)->in.callback(&TMEDIA_CODEC_VIDEO(self)->in.result);
        }
    }
    return retsize;
}

static tsk_bool_t tdav_codec_h264_cisco_sdp_att_match(const tmedia_codec_t* self, const char* att_name, const char* att_value)
{
    return tdav_codec_h264_common_sdp_att_match((tdav_codec_h264_common_t*)self, att_name, att_value);
}

static char* tdav_codec_h264_cisco_sdp_att_get(const tmedia_codec_t* self, const char* att_name)
{
    char* att = tdav_codec_h264_common_sdp_att_get((const tdav_codec_h264_common_t*)self, att_name);
    if (att && tsk_striequals(att_name, "fmtp")) {
        tsk_strcat(&att, "; impl=openh264");
    }
    return att;
}

/* ============ H.264 Base Profile Plugin interface ================= */

/* constructor */
static tsk_object_t* tdav_codec_h264_cisco_base_ctor(tsk_object_t * self, va_list * app)
{
    tdav_codec_h264_cisco_t *h264 = (tdav_codec_h264_cisco_t*)self;
    if (h264) {
        /* init base: called by tmedia_codec_create() */
        /* init self */
        if (tdav_codec_h264_cisco_init(h264, profile_idc_baseline) != 0) {
            return tsk_null;
        }
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_codec_h264_cisco_base_dtor(tsk_object_t * self)
{
    tdav_codec_h264_cisco_t *h264 = (tdav_codec_h264_cisco_t*)self;
    if (h264) {
        /* deinit base */
        tdav_codec_h264_common_deinit(TDAV_CODEC_H264_COMMON(self));
        /* deinit self */
        tdav_codec_h264_cisco_deinit(h264);
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_h264_cisco_base_def_s = {
    sizeof(tdav_codec_h264_cisco_t),
    tdav_codec_h264_cisco_base_ctor,
    tdav_codec_h264_cisco_base_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_h264_cisco_base_plugin_def_s = {
    &tdav_codec_h264_cisco_base_def_s,

    tmedia_video,
    tmedia_codec_id_h264_bp,
    "H264",
    "H264 Base Profile (OpenH264)",
    TMEDIA_CODEC_FORMAT_H264_BP,
    tsk_true,
    90000, // rate

    /* audio */
    { 0 },

    /* video (width, height, fps) */
    {176, 144, 0}, // fps is @deprecated

    tdav_codec_h264_cisco_set,
    tdav_codec_h264_cisco_open,
    tdav_codec_h264_cisco_close,
    tdav_codec_h264_cisco_encode,
    tdav_codec_h264_cisco_decode,
    tdav_codec_h264_cisco_sdp_att_match,
    tdav_codec_h264_cisco_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_h264_cisco_base_plugin_def_t = &tdav_codec_h264_cisco_base_plugin_def_s;

/* ============ Common To all H264 profiles ================= */

static int tdav_codec_h264_cisco_open_encoder(tdav_codec_h264_cisco_t* self)
{
    int ret = -1, max_bitrate_bps;
    long err;
    SSpatialLayerConfig* layer;

    int32_t max_bw_kpbs;
    tdav_codec_h264_common_t* common = (tdav_codec_h264_common_t*)self;

    if (self->encoder.pInst) {
        TSK_DEBUG_ERROR("Encoder already initialized");
        goto bail;
    }

    // create encoder
    if ((err = WelsCreateSVCEncoder(&self->encoder.pInst)) != cmResultSuccess) {
        TSK_DEBUG_ERROR("Failed to create ancoder: %ld", err);
        goto bail;
    }

    self->encoder.pInst->SetOption(ENCODER_OPTION_TRACE_CALLBACK_CONTEXT, self);
    self->encoder.pInst->SetOption(ENCODER_OPTION_TRACE_CALLBACK, &__tdav_codec_h264_cisco_debug_cb);

    if ((err = self->encoder.pInst->GetDefaultParams(&self->encoder.sEncParam)) != cmResultSuccess) {
        TSK_DEBUG_ERROR("GetDefaultParams failed: %ld", err);
        goto bail;
    }

    self->encoder.neg_width = (self->encoder.rotation == 90 || self->encoder.rotation == 270) ? TMEDIA_CODEC_VIDEO(self)->out.height : TMEDIA_CODEC_VIDEO(self)->out.width;
    self->encoder.neg_height = (self->encoder.rotation == 90 || self->encoder.rotation == 270) ? TMEDIA_CODEC_VIDEO(self)->out.width : TMEDIA_CODEC_VIDEO(self)->out.height;
    self->encoder.neg_fps = TMEDIA_CODEC_VIDEO(self)->out.fps;
    max_bw_kpbs = TSK_CLAMP(
                      1,
                      tmedia_get_video_bandwidth_kbps_2(self->encoder.neg_width, self->encoder.neg_height, self->encoder.neg_fps),
                      TMEDIA_CODEC(self)->bandwidth_max_upload
                  );
    max_bitrate_bps = (max_bw_kpbs * 1024);

    TSK_DEBUG_INFO("[H.264 OpenH264 Encoder] neg_width=%d, neg_height=%d, neg_fps=%d, max_bitrate_bps=%d",
                   self->encoder.neg_width,
                   self->encoder.neg_height,
                   self->encoder.neg_fps,
                   max_bitrate_bps
                  );

    self->encoder.sEncParam.iSpatialLayerNum = 1;
    self->encoder.sEncParam.iTemporalLayerNum = 1;
    self->encoder.sEncParam.uiIntraPeriod = (self->encoder.neg_fps * CISCO_H264_GOP_SIZE_IN_SECONDS);
    self->encoder.sEncParam.iUsageType = CAMERA_VIDEO_REAL_TIME; // TODO: use "SCREEN_CONTENT_REAL_TIME" screencast
    self->encoder.sEncParam.iPicWidth = self->encoder.neg_width;
    self->encoder.sEncParam.iPicHeight = self->encoder.neg_height;
    self->encoder.sEncParam.iTargetBitrate = max_bitrate_bps;
    self->encoder.sEncParam.iMaxBitrate = max_bitrate_bps;
    self->encoder.sEncParam.fMaxFrameRate = (float)self->encoder.neg_fps;
    self->encoder.sEncParam.uiMaxNalSize = H264_RTP_PAYLOAD_SIZE;
    self->encoder.sEncParam.eSpsPpsIdStrategy = INCREASING_ID;
    self->encoder.sEncParam.bEnableFrameCroppingFlag = true;

    layer = &self->encoder.sEncParam.sSpatialLayers[0];
    layer->uiProfileIdc	= PRO_BASELINE;
#if BUILD_TYPE_TCH
    layer->uiLevelIdc = tdav_codec_h264_cisco_convert_level(common->level);
#else
    layer->uiLevelIdc = LEVEL_UNKNOWN; // auto-detect
#endif
    layer->fFrameRate = self->encoder.sEncParam.fMaxFrameRate;
    layer->iMaxSpatialBitrate = self->encoder.sEncParam.iMaxBitrate;
    layer->iSpatialBitrate = self->encoder.sEncParam.iTargetBitrate;
    layer->iVideoWidth = self->encoder.sEncParam.iPicWidth;
    layer->iVideoHeight = self->encoder.sEncParam.iPicHeight;
    layer->sSliceCfg.uiSliceMode = SM_DYN_SLICE;
    layer->sSliceCfg.sSliceArgument.uiSliceSizeConstraint = H264_RTP_PAYLOAD_SIZE;
    layer->sSliceCfg.sSliceArgument.uiSliceNum = 1;
    //layer->sSliceCfg.sSliceArgument.uiSliceMbNum[0] = 960;

    if ((err = self->encoder.pInst->InitializeExt(&self->encoder.sEncParam)) != cmResultSuccess) {
        TSK_DEBUG_ERROR("InitializeExt failed: %ld", err);
        goto bail;
    }

    self->encoder.sEncPic.iColorFormat = videoFormatI420;
    self->encoder.sEncPic.iPicWidth = self->encoder.sEncParam.iPicWidth;
    self->encoder.sEncPic.iPicHeight = self->encoder.sEncParam.iPicHeight;
    self->encoder.sEncPic.iStride[0] = self->encoder.sEncPic.iPicWidth;
    self->encoder.sEncPic.iStride[1] = self->encoder.sEncPic.iStride[0] >> 1;
    self->encoder.sEncPic.iStride[2] = self->encoder.sEncPic.iStride[1];

    // Create encoder mutex
    if (!self->encoder.mutex && !(self->encoder.mutex = tsk_mutex_create())) {
        TSK_DEBUG_ERROR("Failed to create mutex for the encoder");
        goto bail;
    }

    self->encoder.frame_count = 0;

    ret = 0;

bail:
    return ret;
}

static int tdav_codec_h264_cisco_close_encoder(tdav_codec_h264_cisco_t* self, tsk_bool_t reset_rotation)
{
    if (self) {
        if (self->encoder.pInst) {
            self->encoder.pInst->Uninitialize();
            WelsDestroySVCEncoder(self->encoder.pInst);
            self->encoder.pInst = NULL;
        }
        if (self->encoder.buffer) {
            TSK_FREE(self->encoder.buffer);
        }
        if (self->encoder.mutex) {
            tsk_mutex_destroy(&self->encoder.mutex);
        }
        self->encoder.frame_count = 0;
        if (reset_rotation) {
            self->encoder.rotation = 0; // reset rotation
        }
    }
    return 0;
}

int tdav_codec_h264_cisco_open_decoder(tdav_codec_h264_cisco_t* self)
{
    int ret = -1;
    long err;
    tdav_codec_h264_common_t* common = (tdav_codec_h264_common_t*)self;
    SDecodingParam sDecParam = { 0 };

    if (self->decoder.pInst) {
        TSK_DEBUG_ERROR("Decoder already initialized");
        goto bail;
    }

    // create decoder
    if ((err = WelsCreateDecoder(&self->decoder.pInst)) != cmResultSuccess) {
        TSK_DEBUG_ERROR("Failed to create decoder: %ld", err);
        goto bail;
    }
    self->decoder.pInst->SetOption(DECODER_OPTION_TRACE_CALLBACK_CONTEXT, self);
    self->decoder.pInst->SetOption(DECODER_OPTION_TRACE_CALLBACK, &__tdav_codec_h264_cisco_debug_cb);

    // initialize decoder
    sDecParam.eOutputColorFormat = videoFormatI420;
    sDecParam.sVideoProperty.eVideoBsType = VIDEO_BITSTREAM_AVC;
   
    if ((err = self->decoder.pInst->Initialize(&sDecParam)) != cmResultSuccess) {
        TSK_DEBUG_ERROR("Failed to initialize decoder: %ld", err);
        goto bail;
    }
    self->decoder.last_seq = 0;
    TSK_DEBUG_INFO("[OpenH264 Decoder] neg_width=%d, neg_height=%d, neg_fps=%d",
                   TMEDIA_CODEC_VIDEO(self)->in.width,
                   TMEDIA_CODEC_VIDEO(self)->in.height,
                   TMEDIA_CODEC_VIDEO(self)->in.fps
                  );
    ret = 0;

bail:
    return ret;
}

static int tdav_codec_h264_cisco_close_decoder(tdav_codec_h264_cisco_t* self)
{
    if (self) {
        if (self->decoder.pInst) {
            self->decoder.pInst->Uninitialize();
            WelsDestroyDecoder(self->decoder.pInst);
            self->decoder.pInst = NULL;
        }
        TSK_FREE(self->decoder.accumulator);
        self->decoder.accumulator_pos = 0;
    }

    return 0;
}

static ELevelIdc tdav_codec_h264_cisco_convert_level(enum level_idc_e level)
{
    switch(level) {
    case level_idc_1_0:
            return LEVEL_1_0;
    case level_idc_1_b:
        return LEVEL_1_B;
    case level_idc_1_1:
        return LEVEL_1_1;
    case level_idc_1_2:
        return LEVEL_1_2;
    case level_idc_1_3:
        return LEVEL_1_3;
    case level_idc_2_0:
        return LEVEL_2_0;
    case level_idc_2_1:
        return LEVEL_2_1;
    case level_idc_2_2:
        return LEVEL_2_2;
    case level_idc_3_0:
        return LEVEL_3_0;
    case level_idc_3_1:
        return LEVEL_3_1;
    case level_idc_3_2:
        return LEVEL_3_2;
    case level_idc_4_0:
        return LEVEL_4_0;
    case level_idc_4_1:
        return LEVEL_4_1;
    case level_idc_4_2:
        return LEVEL_4_2;
    case level_idc_5_0:
        return LEVEL_5_0;
    case level_idc_5_1:
        return LEVEL_5_1;
    case level_idc_5_2:
        return LEVEL_2_2;
    default:
        return LEVEL_UNKNOWN;
    }
}

static void tdav_codec_h264_cisco_debug_cb(void* context, int level, const char* message)
{
    switch (level) {
    case WELS_LOG_ERROR:
    case WELS_LOG_QUIET:
        TSK_DEBUG_ERROR("OpenH264: level=%d, message=%s", level, message);
        break;
    case WELS_LOG_WARNING:
        TSK_DEBUG_WARN("OpenH264: level=%d, message=%s", level, message);
        break;
    default:
        TSK_DEBUG_INFO("OpenH264: level=%d, message=%s", level, message);
        break;
    }
}

static int tdav_codec_h264_cisco_init(tdav_codec_h264_cisco_t* self, profile_idc_t profile)
{
    int ret = -1;
    level_idc_t level;
    tdav_codec_h264_common_t* common = (tdav_codec_h264_common_t*)self;

    if (!self || profile != profile_idc_baseline) {
        TSK_DEBUG_ERROR("Invalid parameter");
        goto bail;
    }

    if ((ret = tdav_codec_h264_common_init(common))) {
        TSK_DEBUG_ERROR("tdav_codec_h264_cisco_common_init() faile with error code=%d", ret);
        goto bail;
    }

    if ((ret = tdav_codec_h264_common_level_from_size(TMEDIA_CODEC_VIDEO(self)->out.width, TMEDIA_CODEC_VIDEO(self)->out.height, &level))) {
        TSK_DEBUG_ERROR("Failed to find level for size=[%u, %u]", TMEDIA_CODEC_VIDEO(self)->out.width, TMEDIA_CODEC_VIDEO(self)->out.height);
        goto bail;
    }

    common->pack_mode_local = H264_PACKETIZATION_MODE;
    common->profile = profile;
    common->level = level;
    // A.2.1.1 Constrained Baseline profile
    // Conformance of a bitstream to the Constrained Baseline profile is indicated by profile_idc being equal to 66 with
    // constraint_set1_flag being equal to 1.
    common->profile_iop = 0xe0; // "constraint_set0_flag=1 and constraint_set1_flag=1" -> Constrained Baseline profile
    TMEDIA_CODEC_VIDEO(self)->in.max_mbps = TMEDIA_CODEC_VIDEO(self)->out.max_mbps = H264_MAX_MBPS*1000;
    TMEDIA_CODEC_VIDEO(self)->in.max_br = TMEDIA_CODEC_VIDEO(self)->out.max_br = H264_MAX_BR*1000;

    TMEDIA_CODEC_VIDEO(self)->in.chroma = tmedia_chroma_yuv420p;
    TMEDIA_CODEC_VIDEO(self)->out.chroma = tmedia_chroma_yuv420p;

    ret = 0;

bail:
    return ret;
}

static int tdav_codec_h264_cisco_deinit(tdav_codec_h264_cisco_t* self)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tdav_codec_h264_cisco_close((tmedia_codec_t*)self);

    return 0;
}

#endif /* HAVE_OPENH264 */
