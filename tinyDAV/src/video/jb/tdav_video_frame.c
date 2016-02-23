/*
 * Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
 *
 * Contact: Mamadou Diop <diopmamadou(at)doubango(DOT)org>
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

/**@file tdav_video_frame.c
 * @brief Video Frame
 *
 * @author Mamadou Diop <diopmamadou(at)doubango(DOT)org>
 */
#include "tinydav/video/jb/tdav_video_frame.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

#include <string.h>
#include <stdlib.h>

static tsk_object_t* tdav_video_frame_ctor(tsk_object_t * self, va_list * app)
{
    tdav_video_frame_t *frame = self;
    if(frame) {
        if(!(frame->pkts = tsk_list_create())) {
            TSK_DEBUG_ERROR("Faile to list");
            return tsk_null;
        }
        tsk_safeobj_init(frame);
    }
    return self;
}
static tsk_object_t* tdav_video_frame_dtor(tsk_object_t * self)
{
    tdav_video_frame_t *frame = self;
    if(frame) {
        TSK_OBJECT_SAFE_FREE(frame->pkts);

        tsk_safeobj_deinit(frame);
    }

    return self;
}
static int tdav_video_frame_cmp(const tsk_object_t *_p1, const tsk_object_t *_p2)
{
    const tdav_video_frame_t *p1 = _p1;
    const tdav_video_frame_t *p2 = _p2;

    if(p1 && p2) {
        return (int)(p1->timestamp - p2->timestamp);
    }
    else if(!p1 && !p2) {
        return 0;
    }
    else {
        return -1;
    }
}
static const tsk_object_def_t tdav_video_frame_def_s = {
    sizeof(tdav_video_frame_t),
    tdav_video_frame_ctor,
    tdav_video_frame_dtor,
    tdav_video_frame_cmp,
};
const tsk_object_def_t *tdav_video_frame_def_t = &tdav_video_frame_def_s;


tdav_video_frame_t* tdav_video_frame_create(trtp_rtp_packet_t* rtp_pkt)
{
    tdav_video_frame_t* frame;
    if(!rtp_pkt || !rtp_pkt->header) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    if((frame = tsk_object_new(tdav_video_frame_def_t))) {
        rtp_pkt = tsk_object_ref(rtp_pkt);
        frame->payload_type = rtp_pkt->header->payload_type;
        frame->timestamp = rtp_pkt->header->timestamp;
        frame->highest_seq_num = rtp_pkt->header->seq_num;
        frame->ssrc = rtp_pkt->header->ssrc;
        tsk_list_push_ascending_data(frame->pkts, (void**)&rtp_pkt);
    }
    return frame;
}

int tdav_video_frame_put(tdav_video_frame_t* self, trtp_rtp_packet_t* rtp_pkt)
{
    if(!self || !rtp_pkt || !rtp_pkt->header) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(self->timestamp != rtp_pkt->header->timestamp) {
        TSK_DEBUG_ERROR("Timestamp mismatch");
        return -2;
    }
    if(self->payload_type != rtp_pkt->header->payload_type) {
        TSK_DEBUG_ERROR("Payload Type mismatch");
        return -2;
    }
#if 0
    if(self->ssrc != rtp_pkt->header->ssrc) {
        TSK_DEBUG_ERROR("SSRC mismatch");
        return -2;
    }
#endif

    rtp_pkt = tsk_object_ref(rtp_pkt);
    self->highest_seq_num = TSK_MAX(self->highest_seq_num, rtp_pkt->header->seq_num);
    tsk_list_lock(self->pkts);
    if (tdav_video_frame_find_by_seq_num(self, rtp_pkt->header->seq_num)) {
        TSK_DEBUG_INFO("JB: Packet with seq_num=%hu duplicated", rtp_pkt->header->seq_num);
    }
    else {
        tsk_list_push_ascending_data(self->pkts, (void**)&rtp_pkt);
    }
    tsk_list_unlock(self->pkts);

    return 0;
}

const trtp_rtp_packet_t* tdav_video_frame_find_by_seq_num(const tdav_video_frame_t* self, uint16_t seq_num)
{
    const tsk_list_item_t *item;
    const trtp_rtp_packet_t* pkt;
    const trtp_rtp_packet_t* ret;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    ret = tsk_null;

    tsk_list_lock(self->pkts);
    tsk_list_foreach(item, self->pkts) {
        if(!(pkt = item->data) || !pkt->header) {
            continue;
        }
        if(pkt->header->seq_num == seq_num) {
            ret = pkt;
            break;
        }
    }
    tsk_list_unlock(self->pkts);

    return ret;
}

// @buffer_ptr pointer to the destination buffer
// @buffer_size the actual buffer size. Could be enlarged if too small to fit
// @retval number of copied bytes
tsk_size_t tdav_video_frame_write(struct tdav_video_frame_s* self, void** buffer_ptr, tsk_size_t* buffer_size)
{
    const tsk_list_item_t *item;
    const trtp_rtp_packet_t* pkt;
    tsk_size_t ret_size = 0;
    int32_t last_seq_num = -1; // guard against duplicated packets

    if(!self || !buffer_ptr || !buffer_size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    tsk_list_lock(self->pkts);
    tsk_list_foreach(item, self->pkts) {
        if(!(pkt = item->data) || !pkt->payload.size || !pkt->header || pkt->header->seq_num == last_seq_num) {
            continue;
        }
        if((ret_size + pkt->payload.size) > *buffer_size) {
            if(!(*buffer_ptr = tsk_realloc(*buffer_ptr, (ret_size + pkt->payload.size)))) {
                TSK_DEBUG_ERROR("Failed to resize the buffer");
                *buffer_size = 0;
                goto bail;
            }
            *buffer_size = (ret_size + pkt->payload.size);
        }
        memcpy(&((uint8_t*)*buffer_ptr)[ret_size], (pkt->payload.data ? pkt->payload.data : pkt->payload.data_const), pkt->payload.size);
        ret_size += pkt->payload.size;
        last_seq_num = pkt->header->seq_num;
    }

bail:
    tsk_list_unlock(self->pkts);

    return ret_size;
}


/**
 Checks if the frame is complete (no gap/loss) or not.
 IMPORTANT: This function assume that the RTP packets use the marker bit to signal end of sequences.
 *@param self The frame with all rtp packets to check
 *@param last_seq_num_with_mark The last seq num value of the packet with the mark bit set. Use negative value to ignore.
 *@param missing_seq_num A missing seq num if any. This value is set only if the function returns False.
 *@return True if the frame is complete and False otherwise. If False is returned then, missing_seq_num is set.
 */
tsk_bool_t tdav_video_frame_is_complete(const tdav_video_frame_t* self, int32_t last_seq_num_with_mark, int32_t* missing_seq_num_start, int32_t* missing_seq_num_count)
{
    const trtp_rtp_packet_t* pkt;
    const tsk_list_item_t *item;
    uint16_t i;
    tsk_bool_t is_complete = tsk_false;

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_false;
    }

    i = 0;
    tsk_list_lock(self->pkts);
    tsk_list_foreach (item, self->pkts) {
        if (!(pkt = item->data)) {
            continue;
        }
        if (last_seq_num_with_mark >= 0 && pkt->header->seq_num != (last_seq_num_with_mark + ++i)) {
            if (missing_seq_num_start) {
                *missing_seq_num_start = (last_seq_num_with_mark + i);
            }
            if (missing_seq_num_count) {
                *missing_seq_num_count = pkt->header->seq_num - (*missing_seq_num_start);
            }
            break;
        }
        if (item == self->pkts->tail) {
            if(!(is_complete = (pkt->header->marker))) {
                if (missing_seq_num_start) {
                    *missing_seq_num_start = (pkt->header->seq_num + 1);
                }
                if (missing_seq_num_count) {
                    *missing_seq_num_count = 1;
                }
            }
        }
    }
    tsk_list_unlock(self->pkts);

    return is_complete;
}
