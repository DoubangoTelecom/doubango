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
#include "internals/mf_codec.h"
#include "internals/mf_utils.h"

#include "tinydav/codecs/h264/tdav_codec_h264_common.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tinymedia/tmedia_codec.h"
#include "tinymedia/tmedia_params.h"
#include "tinymedia/tmedia_defaults.h"

#include "tsk_params.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

typedef struct mf_codec_h264_s {
    TDAV_DECLARE_CODEC_H264_COMMON;

    // Encoder
    struct {
        MFCodecVideoH264* pInst;
        void* buffer;
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
    struct {
        MFCodecVideoH264* pInst;
        void* accumulator;
        tsk_size_t accumulator_pos;
        tsk_size_t accumulator_size;
        uint16_t last_seq;
        tsk_bool_t passthrough; // whether to bypass decoding
    } decoder;
}
mf_codec_h264_t;

#if !defined(PLUGIN_MF_H264_GOP_SIZE_IN_SECONDS)
#	define PLUGIN_MF_H264_GOP_SIZE_IN_SECONDS		25
#endif

static int mf_codec_h264_init(mf_codec_h264_t* self, profile_idc_t profile);
static int mf_codec_h264_deinit(mf_codec_h264_t* self);
static int mf_codec_h264_open_encoder(mf_codec_h264_t* self);
static int mf_codec_h264_close_encoder(mf_codec_h264_t* self);
static int mf_codec_h264_open_decoder(mf_codec_h264_t* self);
static int mf_codec_h264_close_decoder(mf_codec_h264_t* self);

/* ============ H.264 Base/Main Profile X.X Plugin interface functions ================= */

static int mf_codec_h264_set(tmedia_codec_t* self, const tmedia_param_t* param)
{
    mf_codec_h264_t* h264 = (mf_codec_h264_t*)self;
    if(!self->opened) {
        TSK_DEBUG_ERROR("Codec not opened");
        return -1;
    }
    if(param->value_type == tmedia_pvt_int32) {
        if(tsk_striequals(param->key, "action")) {
            tmedia_codec_action_t action = (tmedia_codec_action_t)TSK_TO_INT32((uint8_t*)param->value);
            switch(action) {
            case tmedia_codec_action_encode_idr: {
                h264->encoder.force_idr = tsk_true;
                break;
            }
            case tmedia_codec_action_bw_down: {
                h264->encoder.quality = TSK_CLAMP(1, (h264->encoder.quality + 1), 31);
                break;
            }
            case tmedia_codec_action_bw_up: {
                h264->encoder.quality = TSK_CLAMP(1, (h264->encoder.quality - 1), 31);
                break;
            }
            }
            return 0;
        }
        else if(tsk_striequals(param->key, "bypass-encoding")) {
            h264->encoder.passthrough = *((int32_t*)param->value) ? tsk_true : tsk_false;
            h264->encoder.pInst->setBundled(h264->encoder.passthrough);
            TSK_DEBUG_INFO("[H.264] bypass-encoding = %d", h264->encoder.passthrough);
            return 0;
        }
        else if(tsk_striequals(param->key, "bypass-decoding")) {
            h264->decoder.passthrough = *((int32_t*)param->value) ? tsk_true : tsk_false;
            h264->decoder.pInst->setBundled(h264->decoder.passthrough);
            TSK_DEBUG_INFO("[H.264] bypass-decoding = %d", h264->decoder.passthrough);
            return 0;
        }
        else if(tsk_striequals(param->key, "rotation")) {
            int rotation = *((int32_t*)param->value);
            if(h264->encoder.rotation != rotation) {
                if(self->opened) {
                    int ret;
                    h264->encoder.rotation = rotation;
                    if((ret = mf_codec_h264_close_encoder(h264))) {
                        return ret;
                    }
                    if((ret = mf_codec_h264_open_encoder(h264))) {
                        return ret;
                    }
                }
            }
            return 0;
        }
    }
    return -1;
}


static int mf_codec_h264_open(tmedia_codec_t* self)
{
    int ret;
    mf_codec_h264_t* h264 = (mf_codec_h264_t*)self;

    if(!h264) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* the caller (base class) already checked that the codec is not opened */

    //	Encoder
    if((ret = mf_codec_h264_open_encoder(h264))) {
        return ret;
    }

    //	Decoder
    if((ret = mf_codec_h264_open_decoder(h264))) {
        return ret;
    }

    return 0;
}

static int mf_codec_h264_close(tmedia_codec_t* self)
{
    mf_codec_h264_t* h264 = (mf_codec_h264_t*)self;

    if(!h264) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* the caller (base class) alreasy checked that the codec is opened */

    //	Encoder
    mf_codec_h264_close_encoder(h264);

    //	Decoder
    mf_codec_h264_close_decoder(h264);

    return 0;
}

static tsk_size_t mf_codec_h264_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
    int ret = 0;
    tsk_bool_t send_idr, send_hdr;

    mf_codec_h264_t* h264 = (mf_codec_h264_t*)self;
    tdav_codec_h264_common_t* common = (tdav_codec_h264_common_t*)self;

    if(!self || !in_data || !in_size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    if(!self->opened || !h264->encoder.pInst || !h264->encoder.pInst->IsReady()) {
        TSK_DEBUG_ERROR("Encoder not opened or not ready");
        return 0;
    }


    HRESULT hr = S_OK;
    IMFSample *pSampleOut = NULL;
    IMFMediaBuffer* pBufferOut = NULL;

    // send IDR for:
    //	- the first frame
    //  - remote peer requested an IDR
    //	- every second within the first 4seconds
    send_idr = (
                   h264->encoder.frame_count++ == 0
                   || h264 ->encoder.force_idr
                   || ( (h264->encoder.frame_count < h264->encoder.neg_fps * 4) && ((h264->encoder.frame_count % h264->encoder.neg_fps)==0) )
               );

    if(send_idr) {
        CHECK_HR(hr = h264->encoder.pInst->RequestKeyFrame());
    }

    // send SPS and PPS headers for:
    //  - IDR frames (not required but it's the easiest way to deal with pkt loss)
    //  - every 5 seconds after the first 4seconds
    send_hdr = (
                   send_idr
                   || ( (h264->encoder.frame_count % (h264->encoder.neg_fps * 5))==0 )
               );
    if(send_hdr) {
        //FIXME: MF_MT_MPEG_SEQUENCE_HEADER
        // tdav_codec_h264_rtp_encap(TDAV_CODEC_H264_COMMON(h264), h264->encoder.context->extradata, (tsk_size_t)h264->encoder.context->extradata_size);
    }

    if (h264->encoder.passthrough) {
        tdav_codec_h264_rtp_encap(common, (const uint8_t*)in_data, in_size);
        return 0;
    }

    // Encode data
    CHECK_HR(hr = h264->encoder.pInst->Process(in_data, (UINT32)in_size, &pSampleOut));
    if(pSampleOut) {
        CHECK_HR(hr = pSampleOut->GetBufferByIndex(0, &pBufferOut));

        BYTE* pBufferPtr = NULL;
        DWORD dwDataLength = 0;
        CHECK_HR(hr = pBufferOut->GetCurrentLength(&dwDataLength));
        if(dwDataLength > 0) {
            CHECK_HR(hr = pBufferOut->Lock(&pBufferPtr, NULL, NULL));
            tdav_codec_h264_rtp_encap(common, (const uint8_t*)pBufferPtr, (tsk_size_t)dwDataLength);
            CHECK_HR(hr = pBufferOut->Unlock());
        }
    }

    // reset
    h264->encoder.force_idr = tsk_false;

bail:
    SafeRelease(&pSampleOut);
    SafeRelease(&pBufferOut);
    return 0;
}

static tsk_size_t mf_codec_h264_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
    mf_codec_h264_t* h264 = (mf_codec_h264_t*)self;
    const trtp_rtp_header_t* rtp_hdr = (const trtp_rtp_header_t*)proto_hdr;

    const uint8_t* pay_ptr = tsk_null;
    tsk_size_t pay_size = 0;
    int ret;
    tsk_bool_t append_scp, end_of_unit;
    tsk_bool_t sps_or_pps;
    tsk_size_t retsize = 0, size_to_copy = 0;
    static const tsk_size_t xmax_size = (3840 * 2160 * 3) >> 3; // >>3 instead of >>1 (not an error)
    static tsk_size_t start_code_prefix_size = sizeof(H264_START_CODE_PREFIX);

    if(!h264 || !in_data || !in_size || !out_data) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    if(!self->opened || !h264->encoder.pInst || !h264->decoder.pInst->IsReady()) {
        TSK_DEBUG_ERROR("Decoder not opened or not ready");
        return 0;
    }

    HRESULT hr = S_OK;
    IMFSample *pSampleOut = NULL;
    IMFMediaBuffer* pBufferOut = NULL;

    /* Packet lost? */
    if((h264->decoder.last_seq + 1) != rtp_hdr->seq_num && h264->decoder.last_seq) {
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
        if(!(h264->decoder.accumulator = tsk_realloc(h264->decoder.accumulator, (h264->decoder.accumulator_pos + size_to_copy)))) {
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

    /*if(sps_or_pps){
    	// http://libav-users.943685.n4.nabble.com/Decode-H264-streams-how-to-fill-AVCodecContext-from-SPS-PPS-td2484472.html
    	// SPS and PPS should be bundled with IDR
    	TSK_DEBUG_INFO("Receiving SPS or PPS ...to be tied to an IDR");
    }
    else */if (rtp_hdr->marker) {
        if (h264->decoder.passthrough) {
            if (*out_max_size < h264->decoder.accumulator_pos) {
                if ((*out_data = tsk_realloc(*out_data, h264->decoder.accumulator_pos))) {
                    *out_max_size = h264->decoder.accumulator_pos;
                }
                else {
                    *out_max_size = 0;
                    return 0;
                }
            }
            memcpy(*out_data, h264->decoder.accumulator, h264->decoder.accumulator_pos);
            retsize = h264->decoder.accumulator_pos;
        }
        else { // !h264->decoder.passthrough
            /* decode the picture */
            CHECK_HR(hr = h264->decoder.pInst->Process(h264->decoder.accumulator, (UINT32)h264->decoder.accumulator_pos, &pSampleOut));
            if (pSampleOut) {
                CHECK_HR(hr = pSampleOut->GetBufferByIndex(0, &pBufferOut));

                BYTE* pBufferPtr = NULL;
                DWORD dwDataLength = 0;
                CHECK_HR(hr = pBufferOut->GetCurrentLength(&dwDataLength));
                if (dwDataLength > 0) {
                    CHECK_HR(hr = pBufferOut->Lock(&pBufferPtr, NULL, NULL));
                    {
                        /* IDR ? */
                        if(((pay_ptr[0] & 0x1F) == 0x05) && TMEDIA_CODEC_VIDEO(self)->in.callback) {
                            TSK_DEBUG_INFO("Decoded H.264 IDR");
                            TMEDIA_CODEC_VIDEO(self)->in.result.type = tmedia_video_decode_result_type_idr;
                            TMEDIA_CODEC_VIDEO(self)->in.result.proto_hdr = proto_hdr;
                            TMEDIA_CODEC_VIDEO(self)->in.callback(&TMEDIA_CODEC_VIDEO(self)->in.result);
                        }
                        /* fill out */
                        if(*out_max_size < dwDataLength) {
                            if((*out_data = tsk_realloc(*out_data, dwDataLength))) {
                                *out_max_size = dwDataLength;
                            }
                            else {
                                *out_max_size = 0;
                                return 0;
                            }
                        }
                        retsize = (tsk_size_t)dwDataLength;
                        TMEDIA_CODEC_VIDEO(h264)->in.width = h264->decoder.pInst->GetWidth();
                        TMEDIA_CODEC_VIDEO(h264)->in.height = h264->decoder.pInst->GetHeight();
                        memcpy(*out_data, pBufferPtr, retsize);
                    }
                    CHECK_HR(hr = pBufferOut->Unlock());
                }
            }
        }// else(!h264->decoder.passthrough)
    } // else if(rtp_hdr->marker)

bail:
    if (rtp_hdr->marker) {
        h264->decoder.accumulator_pos = 0;
    }
    if (FAILED(hr) /*|| (!pSampleOut && rtp_hdr->marker)*/) {
        TSK_DEBUG_INFO("Failed to decode the buffer with error code =%d, size=%u, append=%s", ret, h264->decoder.accumulator_pos, append_scp ? "yes" : "no");
        if(TMEDIA_CODEC_VIDEO(self)->in.callback) {
            TMEDIA_CODEC_VIDEO(self)->in.result.type = tmedia_video_decode_result_type_error;
            TMEDIA_CODEC_VIDEO(self)->in.result.proto_hdr = proto_hdr;
            TMEDIA_CODEC_VIDEO(self)->in.callback(&TMEDIA_CODEC_VIDEO(self)->in.result);
        }
    }
    SafeRelease(&pSampleOut);
    SafeRelease(&pBufferOut);
    return retsize;
}

static tsk_bool_t mf_codec_h264_sdp_att_match(const tmedia_codec_t* self, const char* att_name, const char* att_value)
{
    return tdav_codec_h264_common_sdp_att_match((tdav_codec_h264_common_t*)self, att_name, att_value);
}

static char* mf_codec_h264_sdp_att_get(const tmedia_codec_t* self, const char* att_name)
{
    char* att = tdav_codec_h264_common_sdp_att_get((const tdav_codec_h264_common_t*)self, att_name);
    if(att && tsk_striequals(att_name, "fmtp")) {
        tsk_strcat(&att, "; impl=MF");
    }
    return att;
}




/* ============ H.264 Base Profile Plugin interface ================= */

/* constructor */
static tsk_object_t* mf_codec_h264_base_ctor(tsk_object_t * self, va_list * app)
{
    mf_codec_h264_t *h264 = (mf_codec_h264_t*)self;
    if(h264) {
        /* init base: called by tmedia_codec_create() */
        /* init self */
        if(mf_codec_h264_init(h264, profile_idc_baseline) != 0) {
            return tsk_null;
        }
    }
    return self;
}
/* destructor */
static tsk_object_t* mf_codec_h264_base_dtor(tsk_object_t * self)
{
    mf_codec_h264_t *h264 = (mf_codec_h264_t*)self;
    if(h264) {
        /* deinit base */
        tdav_codec_h264_common_deinit(TDAV_CODEC_H264_COMMON(self));
        /* deinit self */
        mf_codec_h264_deinit(h264);
    }

    return self;
}
/* object definition */
static const tsk_object_def_t mf_codec_h264_base_def_s = {
    sizeof(mf_codec_h264_t),
    mf_codec_h264_base_ctor,
    mf_codec_h264_base_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t mf_codec_h264_base_plugin_def_s = {
    &mf_codec_h264_base_def_s,

    tmedia_video,
    tmedia_codec_id_h264_bp,
    "H264",
    "H264 Base Profile (Media Foundation)",
    TMEDIA_CODEC_FORMAT_H264_BP,
    tsk_true,
    90000, // rate

    /* audio */
    { 0 },

    /* video (width, height, fps) */
    {176, 144, 0}, // fps is @deprecated

    mf_codec_h264_set,
    mf_codec_h264_open,
    mf_codec_h264_close,
    mf_codec_h264_encode,
    mf_codec_h264_decode,
    mf_codec_h264_sdp_att_match,
    mf_codec_h264_sdp_att_get
};
const tmedia_codec_plugin_def_t *mf_codec_h264_base_plugin_def_t = &mf_codec_h264_base_plugin_def_s;

/* ============ H.264 Main Profile Plugin interface ================= */

/* constructor */
static tsk_object_t* mf_codec_h264_main_ctor(tsk_object_t * self, va_list * app)
{
    mf_codec_h264_t *h264 = (mf_codec_h264_t*)self;
    if(h264) {
        /* init base: called by tmedia_codec_create() */
        /* init self */
        if(mf_codec_h264_init(h264, profile_idc_main) != 0) {
            return tsk_null;
        }
    }
    return self;
}
/* destructor */
static tsk_object_t* mf_codec_h264_main_dtor(tsk_object_t * self)
{
    mf_codec_h264_t *h264 = (mf_codec_h264_t*)self;
    if(h264) {
        /* deinit base */
        tdav_codec_h264_common_deinit(TDAV_CODEC_H264_COMMON(self));
        /* deinit self */
        mf_codec_h264_deinit(h264);

    }

    return self;
}
/* object definition */
static const tsk_object_def_t mf_codec_h264_main_def_s = {
    sizeof(mf_codec_h264_t),
    mf_codec_h264_main_ctor,
    mf_codec_h264_main_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t mf_codec_h264_main_plugin_def_s = {
    &mf_codec_h264_main_def_s,

    tmedia_video,
    tmedia_codec_id_h264_mp,
    "H264",
    "H264 Main Profile (Media Foundation)",
    TMEDIA_CODEC_FORMAT_H264_MP,
    tsk_true,
    90000, // rate

    /* audio */
    { 0 },

    /* video (width, height, fps)*/
    {176, 144, 0},// fps is @deprecated

    mf_codec_h264_set,
    mf_codec_h264_open,
    mf_codec_h264_close,
    mf_codec_h264_encode,
    mf_codec_h264_decode,
    mf_codec_h264_sdp_att_match,
    mf_codec_h264_sdp_att_get
};
const tmedia_codec_plugin_def_t *mf_codec_h264_main_plugin_def_t = &mf_codec_h264_main_plugin_def_s;



/* ============ Common To all H264 codecs ================= */

int mf_codec_h264_open_encoder(mf_codec_h264_t* self)
{
    HRESULT hr = S_OK;
    int32_t max_bw_kpbs;
    tdav_codec_h264_common_t* common = (tdav_codec_h264_common_t*)self;

    if(self->encoder.pInst) {
        TSK_DEBUG_ERROR("Encoder already initialized");
#if defined(E_ILLEGAL_METHOD_CALL)
        CHECK_HR(hr = E_ILLEGAL_METHOD_CALL);
#else
        CHECK_HR(hr = 0x8000000EL);
#endif
    }

    // create encoder
    if(!(self->encoder.pInst = (common->profile == profile_idc_baseline) ? MFCodecVideoH264::CreateCodecH264Base(MFCodecType_Encoder) : MFCodecVideoH264::CreateCodecH264Main(MFCodecType_Encoder))) {
        TSK_DEBUG_ERROR("Failed to find H.264 encoder");
        CHECK_HR(hr = E_OUTOFMEMORY);
    }

    //self->encoder.context->pix_fmt		= PIX_FMT_YUV420P;
    //self->encoder.context->time_base.num  = 1;
    //self->encoder.context->time_base.den  = TMEDIA_CODEC_VIDEO(self)->out.fps;
    self->encoder.neg_width = (self->encoder.rotation == 90 || self->encoder.rotation == 270) ? TMEDIA_CODEC_VIDEO(self)->out.height : TMEDIA_CODEC_VIDEO(self)->out.width;
    self->encoder.neg_height = (self->encoder.rotation == 90 || self->encoder.rotation == 270) ? TMEDIA_CODEC_VIDEO(self)->out.width : TMEDIA_CODEC_VIDEO(self)->out.height;
    self->encoder.neg_fps = TMEDIA_CODEC_VIDEO(self)->out.fps;
    max_bw_kpbs = TSK_CLAMP(
                      0,
                      tmedia_get_video_bandwidth_kbps_2(self->encoder.neg_width, self->encoder.neg_height, self->encoder.neg_fps),
                      self->encoder.max_bw_kpbs
                  );
    self->encoder.max_bitrate_bps = (max_bw_kpbs * 1024);

    TSK_DEBUG_INFO("[H.264 MF Encoder] neg_width=%d, neg_height=%d, neg_fps=%d, max_bitrate_bps=%d",
                   self->encoder.neg_width,
                   self->encoder.neg_height,
                   self->encoder.neg_fps,
                   self->encoder.max_bitrate_bps
                  );

    CHECK_HR(hr = self->encoder.pInst->Initialize(
                      self->encoder.neg_fps,
                      self->encoder.neg_width,
                      self->encoder.neg_height,
                      self->encoder.max_bitrate_bps));

    CHECK_HR(hr = self->encoder.pInst->SetGOPSize(self->encoder.neg_fps * PLUGIN_MF_H264_GOP_SIZE_IN_SECONDS));
    CHECK_HR(hr = self->encoder.pInst->SetSliceMaxSizeInBytes((H264_RTP_PAYLOAD_SIZE - 100)));
bail:
    return SUCCEEDED(hr) ? 0 : -1;
}

int mf_codec_h264_close_encoder(mf_codec_h264_t* self)
{
    if(self) {
        SafeRelease(&self->encoder.pInst);
        if(self->encoder.buffer) {
            TSK_FREE(self->encoder.buffer);
        }
        self->encoder.frame_count = 0;
    }

    return 0;
}

int mf_codec_h264_open_decoder(mf_codec_h264_t* self)
{
    HRESULT hr = S_OK;
    tdav_codec_h264_common_t* common = (tdav_codec_h264_common_t*)self;

    if(self->decoder.pInst) {
        TSK_DEBUG_ERROR("Decoder already initialized");
#if defined(E_ILLEGAL_METHOD_CALL)
        CHECK_HR(hr = E_ILLEGAL_METHOD_CALL);
#else
        CHECK_HR(hr = 0x8000000EL);
#endif
    }

    // create decoder
    if(!(self->decoder.pInst = (common->profile == profile_idc_baseline) ? MFCodecVideoH264::CreateCodecH264Base(MFCodecType_Decoder) : MFCodecVideoH264::CreateCodecH264Main(MFCodecType_Decoder))) {
        TSK_DEBUG_ERROR("Failed to find H.264 encoder");
        CHECK_HR(hr = E_OUTOFMEMORY);
    }

    TSK_DEBUG_INFO("[H.264 MF Decoder] neg_width=%d, neg_height=%d, neg_fps=%d",
                   TMEDIA_CODEC_VIDEO(self)->in.width,
                   TMEDIA_CODEC_VIDEO(self)->in.height,
                   TMEDIA_CODEC_VIDEO(self)->in.fps
                  );

    CHECK_HR(hr = self->decoder.pInst->Initialize(
                      TMEDIA_CODEC_VIDEO(self)->in.fps,
                      TMEDIA_CODEC_VIDEO(self)->in.width,
                      TMEDIA_CODEC_VIDEO(self)->in.height));

bail:
    return SUCCEEDED(hr) ? 0 : -1;
}

int mf_codec_h264_close_decoder(mf_codec_h264_t* self)
{
    if(self) {
        SafeRelease(&self->decoder.pInst);
        TSK_FREE(self->decoder.accumulator);
        self->decoder.accumulator_pos = 0;
    }

    return 0;
}

int mf_codec_h264_init(mf_codec_h264_t* self, profile_idc_t profile)
{
    int ret = 0;
    level_idc_t level;
    tdav_codec_h264_common_t* common = (tdav_codec_h264_common_t*)self;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if((ret = tdav_codec_h264_common_init(common))) {
        TSK_DEBUG_ERROR("mf_codec_h264_common_init() faile with error code=%d", ret);
        return ret;
    }

    if((ret = tdav_codec_h264_common_level_from_size(TMEDIA_CODEC_VIDEO(self)->out.width, TMEDIA_CODEC_VIDEO(self)->out.height, &level))) {
        TSK_DEBUG_ERROR("Failed to find level for size=[%u, %u]", TMEDIA_CODEC_VIDEO(self)->out.width, TMEDIA_CODEC_VIDEO(self)->out.height);
        return ret;
    }

    (self)->encoder.max_bw_kpbs = tmedia_defaults_get_bandwidth_video_upload_max();
    if (MFUtils::IsLowLatencyH264SupportsMaxSliceSize()) {
        common->pack_mode_local = H264_PACKETIZATION_MODE;
    }
    else {
        common->pack_mode_local = Non_Interleaved_Mode;
    }
    common->profile = profile;
    common->level = level;
    TMEDIA_CODEC_VIDEO(self)->in.max_mbps = TMEDIA_CODEC_VIDEO(self)->out.max_mbps = H264_MAX_MBPS*1000;
    TMEDIA_CODEC_VIDEO(self)->in.max_br = TMEDIA_CODEC_VIDEO(self)->out.max_br = H264_MAX_BR*1000;

    TMEDIA_CODEC_VIDEO(self)->in.chroma = tmedia_chroma_nv12;
    TMEDIA_CODEC_VIDEO(self)->out.chroma = tmedia_chroma_nv12;

    self->encoder.quality = 1;

    return ret;
}

int mf_codec_h264_deinit(mf_codec_h264_t* self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    mf_codec_h264_close((tmedia_codec_t*)self);

    return 0;
}