/*
* Copyright (C) 2012-2015 Doubango Telecom <http://www.doubango.org>
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

/**@file tdav_codec_ulpfec.c
 * @brief Forward Error Correction (FEC) implementation as per RFC 5109
 */
#include "tinydav/codecs/fec/tdav_codec_ulpfec.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#define TDAV_FEC_PKT_HDR_SIZE	10

typedef struct tdav_codec_ulpfec_s {
    TMEDIA_DECLARE_CODEC_VIDEO;

    struct {
        struct tdav_fec_pkt_s* pkt;
    } encoder;
}
tdav_codec_ulpfec_t;

//
//	FEC LEVEL
//
typedef struct tdav_fec_level_s {
    TSK_DECLARE_OBJECT;

    struct { // 7.4. FEC Level Header for FEC Packets
        uint16_t length;
        uint64_t mask;
        tsk_size_t mask_size; // in bits
    } hdr;
    struct {
        uint8_t* ptr;
        tsk_size_t size;
    } payload;
} tdav_fec_level_t;
typedef tsk_list_t tdav_fec_levels_L_t;
static tsk_object_t* tdav_fec_level_ctor(tsk_object_t * self, va_list * app)
{
    tdav_fec_level_t *level = self;
    if (level) {
        level->hdr.mask_size = 16; // L=0
    }
    return self;
}
static tsk_object_t* tdav_fec_level_dtor(tsk_object_t * self)
{
    tdav_fec_level_t *level = self;
    if (level) {
        TSK_FREE(level->payload.ptr);
    }

    return self;
}
static const tsk_object_def_t tdav_fec_level_def_s = {
    sizeof(tdav_fec_level_t),
    tdav_fec_level_ctor,
    tdav_fec_level_dtor,
    tsk_null,
};
const tsk_object_def_t *tdav_fec_level_def_t = &tdav_fec_level_def_s;


//
//	FEC PACKET
//
typedef struct tdav_fec_pkt_s {
    TSK_DECLARE_OBJECT;

    struct { // RFC 5109 - 7.3. FEC Header for FEC Packets
        unsigned E : 1;
        unsigned L : 1;
        unsigned P : 1;
        unsigned X : 1;
        unsigned CC : 4;
        unsigned M : 1;
        unsigned PT : 7;
        struct {
            uint16_t value;
            unsigned set : 1;
        } SN_base;
        uint32_t TS;
        uint16_t length;
    } hdr;

    tdav_fec_levels_L_t* levels;
}
tdav_fec_pkt_t;
static tsk_object_t* tdav_fec_pkt_ctor(tsk_object_t * self, va_list * app)
{
    tdav_fec_pkt_t *pkt = self;
    if (pkt) {
        if (!(pkt->levels = tsk_list_create())) {
            TSK_DEBUG_ERROR("Failed to create levels");
            return tsk_null;
        }
    }
    return self;
}
static tsk_object_t* tdav_fec_pkt_dtor(tsk_object_t * self)
{
    tdav_fec_pkt_t *pkt = self;
    if (pkt) {
        TSK_OBJECT_SAFE_FREE(pkt->levels);
    }

    return self;
}
static int tdav_fec_pkt_cmp(const tsk_object_t *_p1, const tsk_object_t *_p2)
{
    const tdav_fec_pkt_t *p1 = _p1;
    const tdav_fec_pkt_t *p2 = _p2;

    if (p1 && p2) {
        return (int)(p1->hdr.SN_base.value - p2->hdr.SN_base.value);
    }
    else if (!p1 && !p2) {
        return 0;
    }
    else {
        return -1;
    }
}
static const tsk_object_def_t tdav_fec_pkt_def_s = {
    sizeof(tdav_fec_pkt_t),
    tdav_fec_pkt_ctor,
    tdav_fec_pkt_dtor,
    tdav_fec_pkt_cmp,
};
const tsk_object_def_t *tdav_fec_pkt_def_t = &tdav_fec_pkt_def_s;


tsk_size_t tdav_codec_ulpfec_guess_serialbuff_size(const tdav_codec_ulpfec_t* self)
{
    tsk_size_t size = TDAV_FEC_PKT_HDR_SIZE;
    tsk_list_item_t *item;
    tdav_fec_level_t* level;

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    tsk_list_foreach(item, self->encoder.pkt->levels) {
        if (!(level = item->data)) {
            continue;
        }
        size += 2 /* Protection length */ + (level->hdr.mask_size >> 3) + level->hdr.length;
    }

    return size;
}

int tdav_codec_ulpfec_enc_reset(tdav_codec_ulpfec_t* self)
{
    tsk_list_item_t *item;
    tdav_fec_level_t* level;

    if (!self || !self->encoder.pkt) {
        TSK_DEBUG_ERROR("invalid parameter");
        return -1;
    }

    // reset packet
    memset(&self->encoder.pkt->hdr, 0, sizeof(self->encoder.pkt->hdr));

    // reset levels
    tsk_list_foreach(item, self->encoder.pkt->levels) {
        if ((level = item->data)) {
            memset(&level->hdr, 0, sizeof(level->hdr));
            if (level->payload.ptr) {
                memset(level->payload.ptr, 0, level->payload.size);
            }
        }
    }
    return 0;
}

int tdav_codec_ulpfec_enc_protect(tdav_codec_ulpfec_t* self, const trtp_rtp_packet_t* rtp_packet)
{
    if (!self || !self->encoder.pkt || !rtp_packet || !rtp_packet->header) {
        TSK_DEBUG_ERROR("invalid parameter");
        return -1;
    }

    // Packet
    self->encoder.pkt->hdr.P ^= rtp_packet->header->padding;
    self->encoder.pkt->hdr.X ^= rtp_packet->header->extension;
    self->encoder.pkt->hdr.CC ^= rtp_packet->header->csrc_count;
    self->encoder.pkt->hdr.M ^= rtp_packet->header->marker;
    self->encoder.pkt->hdr.PT ^= rtp_packet->header->payload_type;
    if (!self->encoder.pkt->hdr.SN_base.set) {
        self->encoder.pkt->hdr.SN_base.value = rtp_packet->header->seq_num;
        self->encoder.pkt->hdr.SN_base.set = 1;
    }
    else {
        self->encoder.pkt->hdr.SN_base.value = TSK_MIN(self->encoder.pkt->hdr.SN_base.value, rtp_packet->header->seq_num);
    }
    self->encoder.pkt->hdr.TS ^= rtp_packet->header->timestamp;
    self->encoder.pkt->hdr.length ^= (trtp_rtp_packet_guess_serialbuff_size(rtp_packet) - TRTP_RTP_HEADER_MIN_SIZE);

    // Level
    // For now, always single-level protection
    {
        tdav_fec_level_t* level0 = TSK_LIST_FIRST_DATA(self->encoder.pkt->levels);
        const uint8_t* rtp_payload = (const uint8_t*)(rtp_packet->payload.data_const ? rtp_packet->payload.data_const : rtp_packet->payload.data);
        tsk_size_t i;
        if (!level0) {
            tdav_fec_level_t* _level0;
            if (!(_level0 = tsk_object_new(tdav_fec_level_def_t))) {
                TSK_DEBUG_ERROR("Failed to create level");
                return -2;
            }
            level0 = _level0;
            tsk_list_push_back_data(self->encoder.pkt->levels, (void**)&_level0);
        }
        if (level0->payload.size < rtp_packet->payload.size) {
            if (!(level0->payload.ptr = tsk_realloc(level0->payload.ptr, rtp_packet->payload.size))) {
                TSK_DEBUG_ERROR("Failed to realloc size %d", rtp_packet->payload.size);
                level0->payload.size = 0;
                return -3;
            }
            level0->payload.size = rtp_packet->payload.size;
        }
        for (i = 0; i < rtp_packet->payload.size; ++i) {
            level0->payload.ptr[i] ^= rtp_payload[i];
        }
        level0->hdr.mask_size = self->encoder.pkt->hdr.L ? 48 : 16;
        level0->hdr.mask |= (uint64_t)((uint64_t)1 << (level0->hdr.mask_size - (rtp_packet->header->seq_num - self->encoder.pkt->hdr.SN_base.value)));
        level0->hdr.length = (uint16_t)(TSK_MAX(level0->hdr.length, rtp_packet->payload.size));
    }

    return 0;
}

tsk_size_t tdav_codec_ulpfec_enc_serialize(const tdav_codec_ulpfec_t* self, void** out_data, tsk_size_t* out_max_size)
{
    uint8_t* pdata;
    tsk_size_t xsize;
    int32_t i;
    tsk_list_item_t* item;
    tdav_fec_level_t* level;

    if (!self || !self->encoder.pkt || !out_data) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }
    xsize = tdav_codec_ulpfec_guess_serialbuff_size(self);

    if (*out_max_size < xsize) {
        if (!(*out_data = tsk_realloc(*out_data, xsize))) {
            TSK_DEBUG_ERROR("Failed to reallocate buffer with size =%d", xsize);
            *out_max_size = 0;
            return 0;
        }
        *out_max_size = xsize;
    }
    pdata = (uint8_t*)*out_data;

    // E(1), L(1), P(1), X(1), CC(4)
    pdata[0] =
        (self->encoder.pkt->hdr.E << 7) |
        (self->encoder.pkt->hdr.L << 6) |
        (self->encoder.pkt->hdr.P << 5) |
        (self->encoder.pkt->hdr.X << 4) |
        (self->encoder.pkt->hdr.CC & 0x0F);
    // M(1), PT(7)
    pdata[1] = (self->encoder.pkt->hdr.M << 7) | (self->encoder.pkt->hdr.PT & 0x7F);
    // SN base (16)
    pdata[2] = (self->encoder.pkt->hdr.SN_base.value >> 8);
    pdata[3] = (self->encoder.pkt->hdr.SN_base.value & 0xFF);
    // TS (32)
    pdata[4] = self->encoder.pkt->hdr.TS >> 24;
    pdata[5] = (self->encoder.pkt->hdr.TS >> 16) & 0xFF;
    pdata[6] = (self->encoder.pkt->hdr.TS >> 8) & 0xFF;
    pdata[7] = (self->encoder.pkt->hdr.TS & 0xFF);
    // Length (16)
    pdata[8] = (self->encoder.pkt->hdr.length >> 8);
    pdata[9] = (self->encoder.pkt->hdr.length & 0xFF);

    pdata += 10;

    tsk_list_foreach(item, self->encoder.pkt->levels) {
        if (!(level = item->data)) {
            continue;
        }
        // Protection length (16)
        pdata[0] = (level->hdr.length >> 8);
        pdata[1] = (level->hdr.length & 0xFF);
        pdata += 2;
        // mask (16 or 48)
        for (i = (int32_t)(level->hdr.mask_size - 8); i >= 0; i -= 8) {
            *pdata = ((level->hdr.mask >> i) & 0xFF);
            ++pdata;
        }
        // payload
        memcpy(pdata, level->payload.ptr, level->hdr.length);
    }

    return xsize;
}



static int tdav_codec_ulpfec_open(tmedia_codec_t* self)
{
    return 0;
}

static int tdav_codec_ulpfec_close(tmedia_codec_t* self)
{
    return 0;
}

static tsk_size_t tdav_codec_ulpfec_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
    TSK_DEBUG_ERROR("Not expected to be called");
    return 0;
}

static tsk_size_t tdav_codec_ulpfec_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
    TSK_DEBUG_ERROR("Not expected to be called");
    return 0;
}

static tsk_bool_t tdav_codec_ulpfec_sdp_att_match(const tmedia_codec_t* codec, const char* att_name, const char* att_value)
{
    return tsk_true;
}

static char* tdav_codec_ulpfec_sdp_att_get(const tmedia_codec_t* self, const char* att_name)
{
    return tsk_null;
}


/* ============ ULPFEC object definition ================= */

/* constructor */
static tsk_object_t* tdav_codec_ulpfec_ctor(tsk_object_t * self, va_list * app)
{
    tdav_codec_ulpfec_t *ulpfec = self;
    if (ulpfec) {
        /* init base: called by tmedia_codec_create() */
        /* init self */
        if (!(ulpfec->encoder.pkt = tsk_object_new(tdav_fec_pkt_def_t))) {
            TSK_DEBUG_ERROR("Failed to create FEC packet");
            return tsk_null;
        }
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_codec_ulpfec_dtor(tsk_object_t * self)
{
    tdav_codec_ulpfec_t *ulpfec = self;
    if (ulpfec) {
        /* deinit base */
        tmedia_codec_video_deinit(ulpfec);
        /* deinit self */
        TSK_OBJECT_SAFE_FREE(ulpfec->encoder.pkt);
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_ulpfec_def_s = {
    sizeof(tdav_codec_ulpfec_t),
    tdav_codec_ulpfec_ctor,
    tdav_codec_ulpfec_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_ulpfec_plugin_def_s = {
    &tdav_codec_ulpfec_def_s,

    tmedia_video,
    tmedia_codec_id_none, // fake codec
    "ulpfec",
    "ulpfec codec",
    TMEDIA_CODEC_FORMAT_ULPFEC,
    tsk_true,
    90000, // rate

    /* audio */
    { 0 },

    /* video (defaul width,height,fps) */
    { 176, 144, 15 },

    tsk_null, // set()
    tdav_codec_ulpfec_open,
    tdav_codec_ulpfec_close,
    tdav_codec_ulpfec_encode,
    tdav_codec_ulpfec_decode,
    tdav_codec_ulpfec_sdp_att_match,
    tdav_codec_ulpfec_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_ulpfec_plugin_def_t = &tdav_codec_ulpfec_plugin_def_s;