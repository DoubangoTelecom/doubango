/*
* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
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
* but WITHOUT ANY WArtpfbANTY; without even the implied wartpfbanty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
#include "tinyrtp/rtcp/trtp_rtcp_report_fb.h"
#include "tinyrtp/rtcp/trtp_rtcp_header.h"

#include "tnet_endianness.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <stdlib.h>
#include <string.h>

#define TRTP_RTCP_PACKET_FB_MIN_SIZE	(TRTP_RTCP_HEADER_SIZE + 4 + 4)

static int _trtp_rtcp_report_fb_deserialize(const void* data, tsk_size_t _size, trtp_rtcp_header_t** header, uint32_t* ssrc_sender, uint32_t* ssrc_media_src)
{
    const uint8_t* pdata = data;
    if(!data || !header || _size < TRTP_RTCP_PACKET_FB_MIN_SIZE || (_size & 0x03)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(!(*header = trtp_rtcp_header_deserialize(pdata, _size))) {
        TSK_DEBUG_ERROR("Failed to deserialize the header");
        return -3;
    }
    if((*header)->length_in_bytes < TRTP_RTCP_PACKET_FB_MIN_SIZE) {
        TSK_DEBUG_ERROR("Too short");
        return -4;
    }
    else if((*header)->length_in_bytes > _size) {
        TSK_DEBUG_ERROR("Too long");
        return -5;
    }

    *ssrc_sender = (uint32_t)tnet_ntohl_2(&pdata[4]);
    *ssrc_media_src = (uint32_t)tnet_ntohl_2(&pdata[8]);
    return 0;
}


static int _trtp_rtcp_report_fb_serialize_to(const trtp_rtcp_report_fb_t* self, void* data, tsk_size_t size)
{
    int ret;
    uint8_t* pdata = (uint8_t*)data;

    if(!self || !data || size < TRTP_RTCP_PACKET_FB_MIN_SIZE) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if((ret = trtp_rtcp_header_serialize_to(TRTP_RTCP_PACKET(self)->header, pdata, size))) {
        TSK_DEBUG_ERROR("Failed to serialize the header");
        return ret;
    }

    pdata[TRTP_RTCP_HEADER_SIZE] = self->ssrc_sender >> 24;
    pdata[TRTP_RTCP_HEADER_SIZE + 1] = (self->ssrc_sender >> 16) & 0xFF;
    pdata[TRTP_RTCP_HEADER_SIZE + 2] = (self->ssrc_sender >> 8) & 0xFF;
    pdata[TRTP_RTCP_HEADER_SIZE + 3] = (self->ssrc_sender & 0xFF);
    pdata[TRTP_RTCP_HEADER_SIZE + 4] = self->ssrc_media >> 24;
    pdata[TRTP_RTCP_HEADER_SIZE + 5] = (self->ssrc_media >> 16) & 0xFF;
    pdata[TRTP_RTCP_HEADER_SIZE + 6] = (self->ssrc_media >> 8) & 0xFF;
    pdata[TRTP_RTCP_HEADER_SIZE + 7] = (self->ssrc_media & 0xFF);

    return 0;
}



static tsk_object_t* trtp_rtcp_report_rtpfb_ctor(tsk_object_t * self, va_list * app)
{
    trtp_rtcp_report_rtpfb_t *rtpfb = self;
    if(rtpfb) {

    }
    return self;
}
static tsk_object_t* trtp_rtcp_report_rtpfb_dtor(tsk_object_t * self)
{
    trtp_rtcp_report_rtpfb_t *rtpfb = self;
    if(rtpfb) {
        // deinit self
        switch(rtpfb->fci_type) {
        case trtp_rtcp_rtpfb_fci_type_nack: {
            TSK_FREE(rtpfb->nack.pid);
            TSK_FREE(rtpfb->nack.blp);
            break;
        }
        case trtp_rtcp_rtpfb_fci_type_tmmbn: {
            TSK_FREE(rtpfb->tmmbn.ssrc);
            TSK_FREE(rtpfb->tmmbn.MxTBR_Exp);
            TSK_FREE(rtpfb->tmmbn.MxTBR_Mantissa);
            TSK_FREE(rtpfb->tmmbn.MeasuredOverhead);
            break;
        }
        }
        // deinit base
        trtp_rtcp_packet_deinit(TRTP_RTCP_PACKET(rtpfb));
    }

    return self;
}
static const tsk_object_def_t trtp_rtcp_report_rtpfb_def_s = {
    sizeof(trtp_rtcp_report_rtpfb_t),
    trtp_rtcp_report_rtpfb_ctor,
    trtp_rtcp_report_rtpfb_dtor,
    tsk_null,
};
const tsk_object_def_t *trtp_rtcp_report_rtpfb_def_t = &trtp_rtcp_report_rtpfb_def_s;

trtp_rtcp_report_rtpfb_t* trtp_rtcp_report_rtpfb_create_null()
{
    trtp_rtcp_report_rtpfb_t* rtpfb;
    if((rtpfb = (trtp_rtcp_report_rtpfb_t*)tsk_object_new(trtp_rtcp_report_rtpfb_def_t))) {
        trtp_rtcp_packet_init(TRTP_RTCP_PACKET(rtpfb), TRTP_RTCP_HEADER_VERSION_DEFAULT, 0, 0, trtp_rtcp_packet_type_rtpfb, TRTP_RTCP_PACKET_FB_MIN_SIZE);
    }
    return rtpfb;
}

trtp_rtcp_report_rtpfb_t* trtp_rtcp_report_rtpfb_create(trtp_rtcp_header_t* header)
{
    trtp_rtcp_report_rtpfb_t* rtpfb;
    if((rtpfb = (trtp_rtcp_report_rtpfb_t*)tsk_object_new(trtp_rtcp_report_rtpfb_def_t))) {
        TRTP_RTCP_PACKET(rtpfb)->header = tsk_object_ref(header);
    }
    return rtpfb;
}

trtp_rtcp_report_rtpfb_t* trtp_rtcp_report_rtpfb_create_2(trtp_rtcp_rtpfb_fci_type_t fci_type, uint32_t ssrc_sender, uint32_t ssrc_media_src)
{
    trtp_rtcp_report_rtpfb_t* rtpfb;
    if((rtpfb = trtp_rtcp_report_rtpfb_create_null())) {
        rtpfb->fci_type = TRTP_RTCP_PACKET(rtpfb)->header->rc = fci_type;
        TRTP_RTCP_REPORT_FB(rtpfb)->ssrc_sender = ssrc_sender;
        TRTP_RTCP_REPORT_FB(rtpfb)->ssrc_media = ssrc_media_src;
    }
    return rtpfb;
}

// seq_nums[n] must be in [seq_nums[0], seq_nums[0] + 16] and > seq_nums[n - 1]
trtp_rtcp_report_rtpfb_t* trtp_rtcp_report_rtpfb_create_nack(uint32_t ssrc_sender, uint32_t ssrc_media_src, const uint16_t* seq_nums, tsk_size_t count)
{
    trtp_rtcp_report_rtpfb_t* rtpfb;
    if(!seq_nums || !count) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
    if((rtpfb = trtp_rtcp_report_rtpfb_create_2(trtp_rtcp_rtpfb_fci_type_nack, ssrc_sender, ssrc_media_src))) {
        tsk_size_t i, j;
        rtpfb->nack.count = 1; // max = 16
        rtpfb->nack.blp = tsk_malloc(sizeof(uint16_t));
        rtpfb->nack.pid = tsk_malloc(sizeof(uint16_t));
        if(!rtpfb->nack.blp || !rtpfb->nack.pid) {
            TSK_OBJECT_SAFE_FREE(rtpfb);
            return tsk_null;
        }
        rtpfb->nack.pid[0] = seq_nums[0];
        rtpfb->nack.blp[0] = 0;
        for(i = 1; i <= 16 && i < count; ++i) {
            j = seq_nums[i] - rtpfb->nack.pid[0];
            rtpfb->nack.blp[0] |= (1 << (j - 1));
        }

        TRTP_RTCP_PACKET(rtpfb)->header->length_in_bytes += (uint32_t)(rtpfb->nack.count << 2);
        TRTP_RTCP_PACKET(rtpfb)->header->length_in_words_minus1 = ((TRTP_RTCP_PACKET(rtpfb)->header->length_in_bytes >> 2) - 1);
    }
    return rtpfb;
}


trtp_rtcp_report_rtpfb_t* trtp_rtcp_report_rtpfb_deserialize(const void* data, tsk_size_t _size)
{
    trtp_rtcp_report_rtpfb_t* rtpfb = tsk_null;
    trtp_rtcp_header_t* header = tsk_null;
    uint32_t ssrc_sender, ssrc_media_src;

    if(_trtp_rtcp_report_fb_deserialize(data, _size, &header, &ssrc_sender, &ssrc_media_src) == 0) {
        if((rtpfb = trtp_rtcp_report_rtpfb_create(header))) {
            const uint8_t* pdata = ((const uint8_t*)data) + TRTP_RTCP_PACKET_FB_MIN_SIZE;
            tsk_size_t size = (header->length_in_bytes - TRTP_RTCP_PACKET_FB_MIN_SIZE), i;

            TRTP_RTCP_REPORT_FB(rtpfb)->ssrc_sender = ssrc_sender;
            TRTP_RTCP_REPORT_FB(rtpfb)->ssrc_media = ssrc_media_src;

            switch(rtpfb->fci_type = (trtp_rtcp_rtpfb_fci_type_t)header->rc) {
            case trtp_rtcp_rtpfb_fci_type_nack: {
                if((rtpfb->nack.count = (size >> 2)) > 0) {
                    rtpfb->nack.pid = tsk_realloc(rtpfb->nack.pid, (rtpfb->nack.count * sizeof(uint16_t)));
                    rtpfb->nack.blp = tsk_realloc(rtpfb->nack.blp, (rtpfb->nack.count * sizeof(uint16_t)));
                    for(i = 0; i < rtpfb->nack.count; ++i) {
                        if(rtpfb->nack.pid) {
                            rtpfb->nack.pid[i] = tnet_ntohs_2(&pdata[0]);
                        }
                        if(rtpfb->nack.blp) {
                            rtpfb->nack.blp[i] = tnet_ntohs_2(&pdata[2]);
                        }
                        pdata += 4;
                    }
                }
                break;
            }
            case trtp_rtcp_rtpfb_fci_type_tmmbn: {
                TSK_DEBUG_INFO("TMMBN");
                if((rtpfb->tmmbn.count = (size >> 3)) > 0) {
                    uint32_t u32;
                    rtpfb->tmmbn.ssrc = tsk_realloc(rtpfb->tmmbn.ssrc, (rtpfb->tmmbn.count * sizeof(uint32_t)));
                    rtpfb->tmmbn.MxTBR_Exp = tsk_realloc(rtpfb->tmmbn.MxTBR_Exp, (rtpfb->tmmbn.count * sizeof(uint16_t)));
                    rtpfb->tmmbn.MxTBR_Mantissa = tsk_realloc(rtpfb->tmmbn.MxTBR_Mantissa, (rtpfb->tmmbn.count * sizeof(uint32_t)));
                    rtpfb->tmmbn.MeasuredOverhead = tsk_realloc(rtpfb->tmmbn.MeasuredOverhead, (rtpfb->tmmbn.count * sizeof(uint16_t)));
                    for(i = 0; i < rtpfb->tmmbn.count; ++i) {
                        if(rtpfb->tmmbn.ssrc) {
                            rtpfb->tmmbn.ssrc[i] = (uint32_t)tnet_ntohl_2(&pdata[0]);
                        }
                        u32 = (uint32_t)tnet_ntohl_2(&pdata[4]);
                        if(rtpfb->tmmbn.MxTBR_Exp) {
                            rtpfb->tmmbn.MxTBR_Exp[i] = (u32 >> 26);
                        }
                        if(rtpfb->tmmbn.MxTBR_Mantissa) {
                            rtpfb->tmmbn.MxTBR_Mantissa[i] = ((u32 >> 9) & 0x1FFFF);
                        }
                        if(rtpfb->tmmbn.MeasuredOverhead) {
                            rtpfb->tmmbn.MeasuredOverhead[i] = (u32 & 0x1FF);
                        }
                        pdata += 8;
                    }
                }
                break;
            }

            default: {
                TSK_DEBUG_ERROR("Unsupported Feedback message type %d", (int)rtpfb->fci_type);
                break;
            }
            }
        }
    }

    TSK_OBJECT_SAFE_FREE(header);
    return rtpfb;
}

int trtp_rtcp_report_rtpfb_serialize_to(const trtp_rtcp_report_rtpfb_t* self, void* data, tsk_size_t size)
{
    int ret;
    uint8_t* pdata = (uint8_t*)data;

    if(!self || !data || size < trtp_rtcp_report_rtpfb_get_size(self)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if((ret = _trtp_rtcp_report_fb_serialize_to(TRTP_RTCP_REPORT_FB(self), pdata, size))) {
        TSK_DEBUG_ERROR("Failed to serialize FB message");
        return ret;
    }
    pdata += TRTP_RTCP_PACKET_FB_MIN_SIZE;
    size -= TRTP_RTCP_PACKET_FB_MIN_SIZE;

    switch(self->fci_type) {
    case trtp_rtcp_rtpfb_fci_type_nack: {
        tsk_size_t i;
        for(i = 0; i < self->nack.count; ++i) {
            pdata[0] = self->nack.pid[i] >> 8;
            pdata[1] = (self->nack.pid[i] & 0xFF);
            pdata[2] = self->nack.blp[i] >> 8;
            pdata[3] = (self->nack.blp[i] & 0xFF);
            pdata += 4;
        }
        break;
    }
    default: {
        TSK_DEBUG_ERROR("Not implemented");
        return -2;
    }
    }
    return 0;
}

tsk_size_t trtp_rtcp_report_rtpfb_get_size(const trtp_rtcp_report_rtpfb_t* self)
{
    if(!self || !TRTP_RTCP_PACKET(self)->header) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }
    return TRTP_RTCP_PACKET(self)->header->length_in_bytes;
}









static tsk_object_t* trtp_rtcp_report_psfb_ctor(tsk_object_t * self, va_list * app)
{
    trtp_rtcp_report_psfb_t *psfb = self;
    if(psfb) {

    }
    return self;
}
static tsk_object_t* trtp_rtcp_report_psfb_dtor(tsk_object_t * self)
{
    trtp_rtcp_report_psfb_t *psfb = self;
    if(psfb) {
        // deinit self
        switch(psfb->fci_type) {
        case trtp_rtcp_psfb_fci_type_pli:
            break;
        case trtp_rtcp_psfb_fci_type_sli:
            TSK_FREE(psfb->sli.first);
            TSK_FREE(psfb->sli.number);
            TSK_FREE(psfb->sli.pic_id);
            break;
        case trtp_rtcp_psfb_fci_type_rpsi:
            TSK_FREE(psfb->rpsi.bytes);
            break;
        case trtp_rtcp_psfb_fci_type_fir:
            TSK_FREE(psfb->fir.ssrc);
            TSK_FREE(psfb->fir.seq_num);
            break;
        case trtp_rtcp_psfb_fci_type_afb:
            switch(psfb->afb.type) {
            case trtp_rtcp_psfb_afb_type_none:
                TSK_FREE(psfb->afb.none.bytes);
                break;
            case trtp_rtcp_psfb_afb_type_remb:
                TSK_FREE(psfb->afb.remb.ssrc_feedbacks);
                break;
            case trtp_rtcp_psfb_afb_type_jcng:
                TSK_FREE(psfb->afb.jcng.ssrc_feedbacks);
                break;
            }
            break;
        }
        // deinit base
        trtp_rtcp_packet_deinit(TRTP_RTCP_PACKET(psfb));
    }

    return self;
}
static const tsk_object_def_t trtp_rtcp_report_psfb_def_s = {
    sizeof(trtp_rtcp_report_psfb_t),
    trtp_rtcp_report_psfb_ctor,
    trtp_rtcp_report_psfb_dtor,
    tsk_null,
};
const tsk_object_def_t *trtp_rtcp_report_psfb_def_t = &trtp_rtcp_report_psfb_def_s;


trtp_rtcp_report_psfb_t* trtp_rtcp_report_psfb_create_null()
{
    trtp_rtcp_report_psfb_t* psfb;
    if ((psfb = (trtp_rtcp_report_psfb_t*)tsk_object_new(trtp_rtcp_report_psfb_def_t))) {
        trtp_rtcp_packet_init(TRTP_RTCP_PACKET(psfb), TRTP_RTCP_HEADER_VERSION_DEFAULT, 0, 0, trtp_rtcp_packet_type_psfb, TRTP_RTCP_PACKET_FB_MIN_SIZE);
    }
    return psfb;
}

trtp_rtcp_report_psfb_t* trtp_rtcp_report_psfb_create(trtp_rtcp_header_t* header)
{
    trtp_rtcp_report_psfb_t* psfb;
    if ((psfb = (trtp_rtcp_report_psfb_t*)tsk_object_new(trtp_rtcp_report_psfb_def_t))) {
        TRTP_RTCP_PACKET(psfb)->header = tsk_object_ref(header);
    }
    return psfb;
}

trtp_rtcp_report_psfb_t* trtp_rtcp_report_psfb_create_2(trtp_rtcp_psfb_fci_type_t fci_type, uint32_t ssrc_sender, uint32_t ssrc_media_src)
{
    trtp_rtcp_report_psfb_t* psfb;
    if ((psfb = trtp_rtcp_report_psfb_create_null())) {
        TRTP_RTCP_PACKET(psfb)->header->rc = psfb->fci_type = fci_type;
        TRTP_RTCP_REPORT_FB(psfb)->ssrc_sender = ssrc_sender;
        TRTP_RTCP_REPORT_FB(psfb)->ssrc_media = ssrc_media_src;
    }
    return psfb;
}

trtp_rtcp_report_psfb_t* trtp_rtcp_report_psfb_create_pli(uint32_t ssrc_sender, uint32_t ssrc_media_src)
{
    return trtp_rtcp_report_psfb_create_2(trtp_rtcp_psfb_fci_type_pli, ssrc_sender, ssrc_media_src);
}

trtp_rtcp_report_psfb_t* trtp_rtcp_report_psfb_create_fir(uint8_t seq_num, uint32_t ssrc_sender, uint32_t ssrc_media_src)
{
    trtp_rtcp_report_psfb_t* psfb;
    if ((psfb = trtp_rtcp_report_psfb_create_2(trtp_rtcp_psfb_fci_type_fir, ssrc_sender, ssrc_media_src))) {
        psfb->fir.ssrc = tsk_malloc(sizeof(uint32_t));
        psfb->fir.seq_num = tsk_malloc(sizeof(uint8_t));
        if (!psfb->fir.ssrc || !psfb->fir.seq_num) {
            TSK_OBJECT_SAFE_FREE(psfb);
            return tsk_null;
        }
        psfb->fir.count = 1;
        psfb->fir.seq_num[0] = seq_num;
        psfb->fir.ssrc[0] = ssrc_media_src;
        TRTP_RTCP_PACKET(psfb)->header->length_in_bytes += (uint32_t)(psfb->fir.count << 3);
        TRTP_RTCP_PACKET(psfb)->header->length_in_words_minus1 = ((TRTP_RTCP_PACKET(psfb)->header->length_in_bytes >> 2) - 1);
    }
    return psfb;
}

trtp_rtcp_report_psfb_t* trtp_rtcp_report_psfb_create_afb_remb(uint32_t ssrc_sender, const uint32_t* ssrc_media_src_list, uint32_t ssrc_media_src_list_count, uint32_t bitrate/*in bps*/)
{
    trtp_rtcp_report_psfb_t* psfb;
    // draft-alvestrand-rmcat-remb-02 2.2: SSRC media source always equal to zero
    if ((psfb = trtp_rtcp_report_psfb_create_2(trtp_rtcp_psfb_fci_type_afb, ssrc_sender, 0))) {
        static const uint32_t __max_mantissa = 131072;
        psfb->afb.type = trtp_rtcp_psfb_afb_type_remb;
        psfb->afb.remb.exp = 0;
        if (bitrate <= __max_mantissa) {
            psfb->afb.remb.mantissa = bitrate;
        }
        else {
            while (bitrate >= (__max_mantissa << psfb->afb.remb.exp) && psfb->afb.remb.exp < 63) {
                ++psfb->afb.remb.exp;
            }
            psfb->afb.remb.mantissa = (bitrate >> psfb->afb.remb.exp);
        }
        if (ssrc_media_src_list && ssrc_media_src_list_count > 0 && (psfb->afb.remb.ssrc_feedbacks = (uint32_t*)tsk_malloc(ssrc_media_src_list_count << 2))) {
            uint32_t i;
            psfb->afb.remb.num_ssrc = ssrc_media_src_list_count;
            for (i = 0; i < ssrc_media_src_list_count; ++i) {
                psfb->afb.remb.ssrc_feedbacks[i] = ssrc_media_src_list[i];
            }
        }
        TRTP_RTCP_PACKET(psfb)->header->length_in_bytes += 8; /*'R' 'E' 'M' 'B', Num SSRC, BR Exp, BR Mantissa */
        TRTP_RTCP_PACKET(psfb)->header->length_in_bytes += (psfb->afb.remb.num_ssrc << 2);
        TRTP_RTCP_PACKET(psfb)->header->length_in_words_minus1 = ((TRTP_RTCP_PACKET(psfb)->header->length_in_bytes >> 2) - 1);
    }
    return psfb;
}

trtp_rtcp_report_psfb_t* trtp_rtcp_report_psfb_create_afb_jcng(uint32_t ssrc_sender, const uint32_t* ssrc_media_src_list, uint32_t ssrc_media_src_list_count, float jcng_q/*in quality metric*/)
{
    trtp_rtcp_report_psfb_t* psfb;
    // SSRC media source always equal to zero
    if ((psfb = trtp_rtcp_report_psfb_create_2(trtp_rtcp_psfb_fci_type_afb, ssrc_sender, 0))) {
        psfb->afb.type = trtp_rtcp_psfb_afb_type_jcng;
        psfb->afb.jcng.q = (uint8_t)(jcng_q * 255.f);
        if (ssrc_media_src_list && ssrc_media_src_list_count > 0 && (psfb->afb.jcng.ssrc_feedbacks = (uint32_t*)tsk_malloc(ssrc_media_src_list_count << 2))) {
            uint32_t i;
            psfb->afb.jcng.num_ssrc = ssrc_media_src_list_count;
            for (i = 0; i < ssrc_media_src_list_count; ++i) {
                psfb->afb.jcng.ssrc_feedbacks[i] = ssrc_media_src_list[i];
            }
        }
        TRTP_RTCP_PACKET(psfb)->header->length_in_bytes += 8; /*'J' 'C' 'N' 'G', Num SSRC, Q, Reserverd */
        TRTP_RTCP_PACKET(psfb)->header->length_in_bytes += (psfb->afb.jcng.num_ssrc << 2);
        TRTP_RTCP_PACKET(psfb)->header->length_in_words_minus1 = ((TRTP_RTCP_PACKET(psfb)->header->length_in_bytes >> 2) - 1);
    }
    return psfb;
}

trtp_rtcp_report_psfb_t* trtp_rtcp_report_psfb_deserialize(const void* data, tsk_size_t _size)
{
    trtp_rtcp_report_psfb_t* psfb = tsk_null;
    trtp_rtcp_header_t* header = tsk_null;
    uint32_t ssrc_sender, ssrc_media_src;

    if(_trtp_rtcp_report_fb_deserialize(data, _size, &header, &ssrc_sender, &ssrc_media_src) == 0) {
        if((psfb = trtp_rtcp_report_psfb_create(header))) {
            const uint8_t* pdata = ((const uint8_t*)data) + TRTP_RTCP_PACKET_FB_MIN_SIZE;
            tsk_size_t size = (header->length_in_bytes - TRTP_RTCP_PACKET_FB_MIN_SIZE);

            TRTP_RTCP_REPORT_FB(psfb)->ssrc_sender = ssrc_sender;
            TRTP_RTCP_REPORT_FB(psfb)->ssrc_media = ssrc_media_src;

            switch((psfb->fci_type = header->rc)/* FMT for RTCP-FB messages */) {
            case trtp_rtcp_psfb_fci_type_pli: {
                // No FCI in PLI
                // TSK_DEBUG_INFO("PLI");
                break;
            }
            case trtp_rtcp_psfb_fci_type_sli: {
                tsk_size_t sli_count = (size >> 2), i;
                uint32_t u32;
                if(sli_count == 0) {
                    TSK_DEBUG_ERROR("Too short");
                    goto bail;
                }
                psfb->sli.first = tsk_realloc(psfb->sli.first, (sli_count * sizeof(uint16_t)));
                psfb->sli.number = tsk_realloc(psfb->sli.number, (sli_count * sizeof(uint16_t)));
                psfb->sli.pic_id = tsk_realloc(psfb->sli.pic_id, (sli_count * sizeof(uint16_t)));
                for(i = 0; i < sli_count; ++i) {
                    u32 = (uint32_t)tnet_ntohl_2(&pdata[i >> 2]);
                    if(psfb->sli.first) {
                        psfb->sli.first[i] = (u32 >> 19);
                    }
                    if(psfb->sli.number) {
                        psfb->sli.number[i] = (u32 >> 6) & 0x1FFF;
                    }
                    if(psfb->sli.pic_id) {
                        psfb->sli.pic_id[i] = u32 & 0x3F;
                    }
                }

                break;
            }
            case trtp_rtcp_psfb_fci_type_rpsi: {
                uint16_t u16;
                if(size < 2) {
                    TSK_DEBUG_ERROR("Too short");
                    goto bail;
                }
                u16 = tnet_ntohs_2(&pdata[0]);
                psfb->rpsi.pb = (u16 >> 8);
                psfb->rpsi.pt = (u16 & 0x7F);
                if((psfb->rpsi.bytes = tsk_calloc((size - 2), sizeof(uint8_t)))) {
                    memcpy(psfb->rpsi.bytes, &pdata[2], (size - 2));
                }
                break;
            }
            case trtp_rtcp_psfb_fci_type_fir: {
                tsk_size_t fir_count = (size >> 3), i;
                if(fir_count == 0) {
                    TSK_DEBUG_ERROR("Too short");
                    goto bail;
                }
                psfb->fir.count = fir_count;
                psfb->fir.ssrc = tsk_realloc(psfb->fir.seq_num, (fir_count * sizeof(uint32_t)));
                psfb->fir.seq_num = tsk_realloc(psfb->fir.seq_num, (fir_count * sizeof(uint8_t)));
                for(i = 0; i < fir_count; ++i) {
                    if(psfb->fir.ssrc) {
                        psfb->fir.ssrc[i] = (uint32_t)tnet_ntohl_2(&pdata[0]);
                    }
                    if(psfb->fir.seq_num) {
                        psfb->fir.seq_num[i] = pdata[4];
                    }
                    pdata+=8;
                }
                break;
            }
            case trtp_rtcp_psfb_fci_type_afb: {
                if(size > 0) {
                    psfb->afb.type = trtp_rtcp_psfb_afb_type_none;
                    // REMB (http://tools.ietf.org/html/draft-alvestrand-rmcat-remb-02) or jitter buffer congestion estimation message?
                    if (size > 4 && tsk_strniequals(pdata, "REMB", 4)) {
                        uint32_t _u32;
                        if (size < 8) { // REMB, Num SSRC, BR Exp, BR Mantissa
                            TSK_DEBUG_ERROR("Too short");
                            goto bail;
                        }
                        psfb->afb.type = trtp_rtcp_psfb_afb_type_remb;
                        _u32 = (uint32_t)tnet_ntohl_2(&pdata[4]);
                        psfb->afb.remb.num_ssrc = ((_u32 >> 24) & 0xFF);
                        if ((psfb->afb.remb.num_ssrc << 2) != (size - 8)) {
                            TSK_DEBUG_ERROR("Invalid size");
                            psfb->afb.remb.num_ssrc = 0;
                            goto bail;
                        }
                        psfb->afb.remb.exp = ((_u32 >> 18) & 0x3F);
                        psfb->afb.remb.mantissa = (_u32 & 0x3FFFF);
                        if ((psfb->afb.remb.ssrc_feedbacks = tsk_malloc(psfb->afb.remb.num_ssrc << 2))) {
                            for (_u32 = 0; _u32 < psfb->afb.remb.num_ssrc; ++_u32) {
                                psfb->afb.remb.ssrc_feedbacks[_u32] = (uint32_t)tnet_ntohl_2(&pdata[8 + (_u32 << 2)]);
                            }
                        }
                    }
                    else if (size > 4 && tsk_strniequals(pdata, "JCNG", 4)) {
                        uint32_t _u32;
                        if (size < 8) { // JCNG, Num SSRC, Q, Reserved
                            TSK_DEBUG_ERROR("Too short");
                            goto bail;
                        }
                        psfb->afb.type = trtp_rtcp_psfb_afb_type_jcng;
                        _u32 = (uint32_t)tnet_ntohl_2(&pdata[4]);
                        psfb->afb.jcng.num_ssrc = ((_u32 >> 24) & 0xFF);
                        if ((psfb->afb.jcng.num_ssrc << 2) != (size - 8)) {
                            TSK_DEBUG_ERROR("Invalid size");
                            psfb->afb.jcng.num_ssrc = 0;
                            goto bail;
                        }
                        psfb->afb.jcng.q = ((_u32 >> 16) & 0xFF);
                        if ((psfb->afb.jcng.ssrc_feedbacks = tsk_malloc(psfb->afb.jcng.num_ssrc << 2))) {
                            for (_u32 = 0; _u32 < psfb->afb.jcng.num_ssrc; ++_u32) {
                                psfb->afb.jcng.ssrc_feedbacks[_u32] = (uint32_t)tnet_ntohl_2(&pdata[8 + (_u32 << 2)]);
                            }
                        }
                    }
                    else {
                        if ((psfb->afb.none.bytes = tsk_calloc(size, sizeof(uint8_t)))) {
                            memcpy(psfb->afb.none.bytes, &pdata[0], size);
                        }
                    }
                }
                break;
            }
            default: {
                TSK_DEBUG_ERROR("%d not a valid FCI", psfb->fci_type);
                goto bail;
            }
            }
        }
    }

bail:
    TSK_OBJECT_SAFE_FREE(header);
    return psfb;
}

int trtp_rtcp_report_psfb_serialize_to(const trtp_rtcp_report_psfb_t* self, void* data, tsk_size_t size)
{
    int ret;
    uint8_t* pdata = (uint8_t*)data;

    if(!self || !data || size < trtp_rtcp_report_psfb_get_size(self)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if((ret = _trtp_rtcp_report_fb_serialize_to(TRTP_RTCP_REPORT_FB(self), pdata, size))) {
        TSK_DEBUG_ERROR("Failed to serialize FB message");
        return ret;
    }
    pdata += TRTP_RTCP_PACKET_FB_MIN_SIZE;
    size -= TRTP_RTCP_PACKET_FB_MIN_SIZE;

    switch(self->fci_type) {
    case trtp_rtcp_psfb_fci_type_pli: {
        // No FCI in PLI
        break;
    }
    case trtp_rtcp_psfb_fci_type_fir: {
        tsk_size_t i;
        for(i = 0; i < self->fir.count; ++i) {
            pdata[0] = self->fir.ssrc[i] >> 24;
            pdata[1] = (self->fir.ssrc[i] >> 16) & 0xFF;
            pdata[2] = (self->fir.ssrc[i] >> 8) & 0xFF;
            pdata[3] = (self->fir.ssrc[i] & 0xFF);
            pdata[4] = self->fir.seq_num[i];
            pdata += 8; // SSRC (4), Seq nr(1), Reserved(3)
        }
        break;
    }
    case trtp_rtcp_psfb_fci_type_afb: {
        if (self->afb.type == trtp_rtcp_psfb_afb_type_remb) {
            tsk_size_t i;
            // 'R' 'E' 'M' 'B'
            pdata[0] = 'R', pdata[1] = 'E', pdata[2] = 'M', pdata[3] = 'B';
            // |  Num SSRC     | BR Exp    |  BR Mantissa
            pdata[4] = self->afb.remb.num_ssrc; // 8bits
            pdata[5] = (self->afb.remb.exp << 2) & 0xFC;// 6bits
            // 18bits
            pdata[5] |= (self->afb.remb.mantissa >> 16) & 0x3;
            pdata[6] = (self->afb.remb.mantissa >> 8) & 0xFF;
            pdata[7] = (self->afb.remb.mantissa & 0xFF);
            if (self->afb.remb.ssrc_feedbacks) {
                for (i = 0; i < self->afb.remb.num_ssrc; ++i) {
                    pdata[8 + (i<<2)] = self->afb.remb.ssrc_feedbacks[i] >> 24;
                    pdata[8 + (i<<2) + 1] = (self->afb.remb.ssrc_feedbacks[i] >> 16) & 0xFF;
                    pdata[8 + (i<<2) + 2] = (self->afb.remb.ssrc_feedbacks[i] >> 8) & 0xFF;
                    pdata[8 + (i<<2) + 3] = (self->afb.remb.ssrc_feedbacks[i] & 0xFF);
                }
            }
        }
        else if (self->afb.type == trtp_rtcp_psfb_afb_type_jcng) {
            tsk_size_t i;
            // 'J' 'C' 'N' 'G'
            pdata[0] = 'J', pdata[1] = 'C', pdata[2] = 'N', pdata[3] = 'G';
            // |  Num SSRC     |    Q    |  Reserved
            pdata[4] = self->afb.jcng.num_ssrc; // 8bits
            pdata[5] = self->afb.jcng.q;// 8bits
            // 16bits, reserved, zeros
            pdata[6] = 0x00;
            pdata[7] = 0x00;
            if (self->afb.jcng.ssrc_feedbacks) {
                for (i = 0; i < self->afb.jcng.num_ssrc; ++i) {
                    pdata[8 + (i<<2)] = self->afb.jcng.ssrc_feedbacks[i] >> 24;
                    pdata[8 + (i<<2) + 1] = (self->afb.jcng.ssrc_feedbacks[i] >> 16) & 0xFF;
                    pdata[8 + (i<<2) + 2] = (self->afb.jcng.ssrc_feedbacks[i] >> 8) & 0xFF;
                    pdata[8 + (i<<2) + 3] = (self->afb.jcng.ssrc_feedbacks[i] & 0xFF);
                }
            }
        }
        else {
            TSK_DEBUG_ERROR("Not implemented yet");
            return -1;
        }
        break;
    }
    default: {
        TSK_DEBUG_ERROR("Not implemented yet");
        return -1;
    }
    }

    return ret;
}

tsk_size_t trtp_rtcp_report_psfb_get_size(const trtp_rtcp_report_psfb_t* self)
{
    if(!self || !TRTP_RTCP_PACKET(self)->header) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }
    return TRTP_RTCP_PACKET(self)->header->length_in_bytes;
}