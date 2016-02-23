/*
 * Copyright (C) 2011-2015 Mamadou DIOP
 * Copyright (C) 2011-2015 Doubango Telecom <http://www.doubango.org>
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

/**@file tdav_video_jb.c
 * @brief Video Jitter Buffer
 */
#include "tinydav/video/jb/tdav_video_jb.h"
#include "tinydav/video/jb/tdav_video_frame.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tsk_time.h"
#include "tsk_memory.h"
#include "tsk_thread.h"
#include "tsk_condwait.h"
#include "tsk_debug.h"

#if TSK_UNDER_WINDOWS
#	include <windows.h>
#endif

#define TDAV_VIDEO_JB_DISABLE           0

// default frame rate
// the corret fps will be computed using the RTP timestamps
#define TDAV_VIDEO_JB_FPS		TDAV_VIDEO_JB_FPS_MAX
#define TDAV_VIDEO_JB_FPS_MIN	10
#define TDAV_VIDEO_JB_FPS_MAX	120
// Number of correct consecutive video frames to receive before computing the FPS
#define TDAV_VIDEO_JB_FPS_PROB	(TDAV_VIDEO_JB_FPS << 1)
// Maximum gap allowed (used to detect seqnum wrpping)
#define TDAV_VIDEO_JB_MAX_DROPOUT		0xFD9B

#define TDAV_VIDEO_JB_TAIL_MAX_LOG2		1
#if TDAV_UNDER_MOBILE /* to avoid too high memory usage */
#	define TDAV_VIDEO_JB_TAIL_MAX		(TDAV_VIDEO_JB_FPS_MIN << TDAV_VIDEO_JB_TAIL_MAX_LOG2)
#else
#	define TDAV_VIDEO_JB_TAIL_MAX		(TDAV_VIDEO_JB_FPS_MAX << TDAV_VIDEO_JB_TAIL_MAX_LOG2)
#endif

#define TDAV_VIDEO_JB_RATE				90 /* KHz */

#define TDAV_VIDEO_JB_LATENCY_MIN		2 /* Must be > 0 */
#define TDAV_VIDEO_JB_LATENCY_MAX		15 /* Default, will be updated using fps */

static int _tdav_video_jb_set_defaults(struct tdav_video_jb_s* self);
static const tdav_video_frame_t* _tdav_video_jb_get_frame(struct tdav_video_jb_s* self, uint32_t timestamp, uint8_t pt, tsk_bool_t *pt_matched);
static void* TSK_STDCALL _tdav_video_jb_decode_thread_func(void *arg);

typedef struct tdav_video_jb_s {
    TSK_DECLARE_OBJECT;

    tsk_bool_t started;
    int32_t fps;
    int32_t fps_prob;
    int32_t avg_duration;
    int32_t rate; // in Khz
    uint32_t last_timestamp;
    int32_t conseq_frame_drop;
    int32_t tail_max;
    tdav_video_frames_L_t *frames;
    int64_t frames_count;

    tsk_size_t latency_min;
    tsk_size_t latency_max;

    uint32_t decode_last_timestamp;
    int32_t decode_last_seq_num_with_mark; // -1 = unset
    uint64_t decode_last_time;
    tsk_thread_handle_t* decode_thread[1];
    tsk_condwait_handle_t* decode_thread_cond;

    uint16_t seq_nums[0xFF];
    tdav_video_jb_cb_f callback;
    const void* callback_data;

    // to avoid locking use different cb_data
    tdav_video_jb_cb_data_xt cb_data_rtp;
    tdav_video_jb_cb_data_xt cb_data_fdd;
    tdav_video_jb_cb_data_xt cb_data_any;

    struct {
        void* ptr;
        tsk_size_t size;
    } buffer;

    TSK_DECLARE_SAFEOBJ;
}
tdav_video_jb_t;


static tsk_object_t* tdav_video_jb_ctor(tsk_object_t * self, va_list * app)
{
    tdav_video_jb_t *jb = self;
    if(jb) {
        if(!(jb->frames = tsk_list_create())) {
            TSK_DEBUG_ERROR("Failed to create list");
            return tsk_null;
        }
        if(!(jb->decode_thread_cond = tsk_condwait_create())) {
            TSK_DEBUG_ERROR("Failed to create condition var");
            return tsk_null;
        }
        jb->cb_data_fdd.type = tdav_video_jb_cb_data_type_fdd;
        jb->cb_data_rtp.type = tdav_video_jb_cb_data_type_rtp;

        tsk_safeobj_init(jb);
    }
    return self;
}
static tsk_object_t* tdav_video_jb_dtor(tsk_object_t * self)
{
    tdav_video_jb_t *jb = self;
    if(jb) {
        if(jb->started) {
            tdav_video_jb_stop(jb);
        }
        TSK_OBJECT_SAFE_FREE(jb->frames);
        if(jb->decode_thread_cond) {
            tsk_condwait_destroy(&jb->decode_thread_cond);
        }
        TSK_SAFE_FREE(jb->buffer.ptr);
        tsk_safeobj_deinit(jb);
    }

    return self;
}
static const tsk_object_def_t tdav_video_jb_def_s = {
    sizeof(tdav_video_jb_t),
    tdav_video_jb_ctor,
    tdav_video_jb_dtor,
    tsk_null,
};

tdav_video_jb_t* tdav_video_jb_create()
{
    tdav_video_jb_t* jb;

    if ((jb = tsk_object_new(&tdav_video_jb_def_s))) {
        if (_tdav_video_jb_set_defaults(jb) != 0) {
            TSK_OBJECT_SAFE_FREE(jb);
        }
    }
    return jb;
}

#define tdav_video_jb_reset_fps_prob(self) {\
(self)->fps_prob = TDAV_VIDEO_JB_FPS_PROB; \
(self)->last_timestamp = 0; \
(self)->avg_duration = 0; \
}
#define tdav_video_jb_reset_tail_min_prob(self) {\
(self)->tail_prob = TDAV_VIDEO_JB_TAIL_MIN_PROB; \
(self)->tail_min = TDAV_VIDEO_JB_TAIL_MIN_MAX; \
}

int tdav_video_jb_set_callback(tdav_video_jb_t* self, tdav_video_jb_cb_f callback, const void* usr_data)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->callback = callback;
    self->cb_data_any.usr_data = usr_data;
    self->cb_data_fdd.usr_data = usr_data;
    self->cb_data_rtp.usr_data = usr_data;
    return 0;
}

// Congestion quality metrics based
int tdav_video_jb_get_qcong(tdav_video_jb_t* self, float* q)
{
    float lm;
    if (!self || !q) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    lm = (float)self->latency_max;
    if (lm <= 0.f) { // must never happen...but used as a guard against div(0)
        *q = 1.f;
    }
    else {
        // when "frames_count" is > "latency_max" q is < 0 but it'll be clipped to 0.f
        *q = 1.f - (self->frames_count / lm);
    }
    // 0.0001f instead of zero which could be interpreted as "no data available"
    // 0.0001f encoded to 1-byte in RTCP-RR-JCNG will be coded as (0.0001f * 255.f) = zero
    *q = TSK_CLAMP(0.0001f, *q, 1.f);
    return 0;
}

int tdav_video_jb_start(tdav_video_jb_t* self)
{
    int ret = 0;
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(self->started) {
        return 0;
    }

    self->started = tsk_true;

    if(!self->decode_thread[0]) {
        ret = tsk_thread_create(&self->decode_thread[0], _tdav_video_jb_decode_thread_func, self);
        if(ret != 0 || !self->decode_thread[0]) {
            TSK_DEBUG_ERROR("Failed to create new thread");
        }
        ret = tsk_thread_set_priority(self->decode_thread[0], TSK_THREAD_PRIORITY_TIME_CRITICAL);
    }

    return ret;
}

int tdav_video_jb_put(tdav_video_jb_t* self, trtp_rtp_packet_t* rtp_pkt)
{
#if TDAV_VIDEO_JB_DISABLE
    self->cb_data_rtp.rtp.pkt = rtp_pkt;
    self->callback(&self->cb_data_rtp);
#else
    const tdav_video_frame_t* old_frame;
    tsk_bool_t pt_matched = tsk_false, is_frame_late_or_dup = tsk_false, is_restarted = tsk_false;
    uint16_t* seq_num;

    if(!self || !rtp_pkt || !rtp_pkt->header) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(!self->started) {
        TSK_DEBUG_INFO("Video jitter buffer not started");
        return 0;
    }

    seq_num = &self->seq_nums[rtp_pkt->header->payload_type];

    tsk_safeobj_lock(self);

    //TSK_DEBUG_INFO("receive seqnum=%u", rtp_pkt->header->seq_num);

    if(self->decode_last_timestamp && (self->decode_last_timestamp > rtp_pkt->header->timestamp)) {
        if((self->decode_last_timestamp - rtp_pkt->header->timestamp) < TDAV_VIDEO_JB_MAX_DROPOUT) {
            TSK_DEBUG_INFO("--------Frame already Decoded [seqnum=%u]------------", rtp_pkt->header->seq_num);
            tsk_safeobj_unlock(self);
            return 0;
        }
    }

    old_frame = _tdav_video_jb_get_frame(self, rtp_pkt->header->timestamp, rtp_pkt->header->payload_type, &pt_matched);

    if((*seq_num && *seq_num != 0xFFFF) && (*seq_num + 1) != rtp_pkt->header->seq_num) {
        int32_t diff = ((int32_t)rtp_pkt->header->seq_num - (int32_t)*seq_num);
        tsk_bool_t is_frame_loss = (diff > 0);
        is_restarted = (TSK_ABS(diff) > TDAV_VIDEO_JB_MAX_DROPOUT);
        is_frame_late_or_dup = !is_frame_loss;
        tdav_video_jb_reset_fps_prob(self);
        TSK_DEBUG_INFO("Packet %s (from JB) [%hu - %hu]", is_frame_loss ? "loss" : "late/duplicated/nack", *seq_num, rtp_pkt->header->seq_num);

        if(is_frame_loss && !is_restarted) {
            if(self->callback) {
                self->cb_data_any.type = tdav_video_jb_cb_data_type_fl;
                self->cb_data_any.ssrc = rtp_pkt->header->ssrc;
                self->cb_data_any.fl.seq_num = (*seq_num + 1);
                self->cb_data_any.fl.count = diff - 1;
                self->callback(&self->cb_data_any);
            }
        }
    }

    if(!old_frame) {
        tdav_video_frame_t* new_frame;
        if(pt_matched) {
            // if we have a frame with the same payload type but without this timestamp this means that we moved to a new frame
            // this happens if the frame is waiting to be decoded or the marker is lost
        }
        if((new_frame = tdav_video_frame_create(rtp_pkt))) {
            // compute avg frame duration
            if(self->last_timestamp && self->last_timestamp < rtp_pkt->header->timestamp) {
                uint32_t duration = (rtp_pkt->header->timestamp - self->last_timestamp)/self->rate;
                self->avg_duration = self->avg_duration ? ((self->avg_duration + duration) >> 1) : duration;
                --self->fps_prob;
            }
            self->last_timestamp = rtp_pkt->header->timestamp;

            tsk_list_lock(self->frames);
            if(self->frames_count >= self->tail_max) {
                if(++self->conseq_frame_drop >= self->tail_max) {
                    TSK_DEBUG_ERROR("Too many frames dropped and fps=%d", self->fps);
                    tsk_list_clear_items(self->frames);
                    self->conseq_frame_drop = 0;
                    self->frames_count = 1;
                    if(self->callback) {
                        self->cb_data_any.type = tdav_video_jb_cb_data_type_tmfr;
                        self->cb_data_any.ssrc = rtp_pkt->header->ssrc;
                        self->callback(&self->cb_data_any);
                    }
                }
                else {
                    TSK_DEBUG_INFO("Dropping video frame because frames_count(%lld)>=tail_max(%d)", self->frames_count, self->tail_max);
                    tsk_list_remove_first_item(self->frames);
                }
                tdav_video_jb_reset_fps_prob(self);
            }
            else {
                ++self->frames_count;
            }
            tsk_list_push_ascending_data(self->frames, (void**)&new_frame);
            tsk_list_unlock(self->frames);
        }
        if(self->fps_prob <= 0 && self->avg_duration) {
            // compute FPS using timestamp values
            int32_t fps_new = (1000 / self->avg_duration);
            int32_t fps_old = self->fps;
            self->fps = TSK_CLAMP(TDAV_VIDEO_JB_FPS_MIN, fps_new, TDAV_VIDEO_JB_FPS_MAX);
            self->tail_max = (self->fps << TDAV_VIDEO_JB_TAIL_MAX_LOG2); // maximum delay = 2 seconds
            self->latency_max = self->fps; // maximum = 1 second
            TSK_DEBUG_INFO("According to rtp-timestamps ...FPS = %d (clipped to %d) tail_max=%d, latency_max=%u", fps_new, self->fps, self->tail_max, (unsigned)self->latency_max);
            tdav_video_jb_reset_fps_prob(self);
            if(self->callback && (fps_old != self->fps)) {
                self->cb_data_any.type = tdav_video_jb_cb_data_type_fps_changed;
                self->cb_data_any.ssrc = rtp_pkt->header->ssrc;
                self->cb_data_any.fps.new = self->fps; // clipped value
                self->cb_data_any.fps.old = fps_old;
                self->callback(&self->cb_data_any);
            }
        }
    }
    else {
        tdav_video_frame_put((tdav_video_frame_t*)old_frame, rtp_pkt);
    }

    tsk_safeobj_unlock(self);

    if(!is_frame_late_or_dup || is_restarted) {
        *seq_num = rtp_pkt->header->seq_num;
    }
#endif

    return 0;
}

int tdav_video_jb_stop(tdav_video_jb_t* self)
{
    int ret;
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(!self->started) {
        return 0;
    }

    TSK_DEBUG_INFO("tdav_video_jb_stop()");

    self->started = tsk_false;

    ret = tsk_condwait_broadcast(self->decode_thread_cond);

    if (self->decode_thread[0]) {
        ret = tsk_thread_join(&self->decode_thread[0]);
    }

    // clear pending frames
    tsk_list_lock(self->frames);
    tsk_list_clear_items(self->frames);
    self->frames_count = 0;
    tsk_list_unlock(self->frames);

    // reset default values to make sure next start will be called with right defaults
    // do not call this function in start to avoid overriding values defined between prepare() and start()
    _tdav_video_jb_set_defaults(self);

    return ret;
}

static int _tdav_video_jb_set_defaults(struct tdav_video_jb_s* self)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    self->fps = TDAV_VIDEO_JB_FPS;
    self->fps_prob = TDAV_VIDEO_JB_FPS_PROB;
    self->tail_max = TDAV_VIDEO_JB_TAIL_MAX;
    self->avg_duration = 0;
    self->rate = TDAV_VIDEO_JB_RATE;
    self->conseq_frame_drop = 0;
    self->frames_count = 0;
    self->decode_last_timestamp = 0;
    self->decode_last_seq_num_with_mark = -1;
    self->decode_last_time = 0;

    self->latency_min = TDAV_VIDEO_JB_LATENCY_MIN;
    self->latency_max = TDAV_VIDEO_JB_LATENCY_MAX;

    return 0;
}

static const tdav_video_frame_t* _tdav_video_jb_get_frame(tdav_video_jb_t* self, uint32_t timestamp, uint8_t pt, tsk_bool_t *pt_matched)
{
    const tdav_video_frame_t* ret = tsk_null;
    const tsk_list_item_t *item;

    *pt_matched =tsk_false;

    tsk_list_lock(self->frames);
    tsk_list_foreach(item, self->frames) {
        if(TDAV_VIDEO_FRAME(item->data)->payload_type == pt) {
            if(!(*pt_matched)) {
                *pt_matched = tsk_true;
            }
            if(TDAV_VIDEO_FRAME(item->data)->timestamp == timestamp) {
                ret = item->data;
                break;
            }
        }

    }
    tsk_list_unlock(self->frames);

    return ret;
}

static void* TSK_STDCALL _tdav_video_jb_decode_thread_func(void *arg)
{
    tdav_video_jb_t* jb = (tdav_video_jb_t*)arg;
    //uint64_t delay;
    int32_t missing_seq_num_start = 0, prev_missing_seq_num_start = 0;
    int32_t missing_seq_num_count = 0, prev_lasted_missing_seq_num_count = 0;
    const tdav_video_frame_t* frame;
    uint64_t next_decode_duration = 0, now, _now, latency = 0;
    //uint64_t x_decode_duration = (1000 / jb->fps); // expected
    //uint64_t x_decode_time = tsk_time_now();//expected
    tsk_bool_t postpone, cleaning_delay = tsk_false;
#if 0
    static const uint64_t __toomuch_delay_to_be_valid = 10000; // guard against systems with buggy "tsk_time_now()" -Won't say Windows ...but :)-
#endif

    jb->decode_last_seq_num_with_mark = -1; // -1 -> unset
    jb->decode_last_time = tsk_time_now();

    (void)(now);
    //(void)(delay);

    TSK_DEBUG_INFO("Video jitter buffer thread - ENTER");

    while(jb->started) {
        now = tsk_time_now();
        if (next_decode_duration > 0) {
            tsk_condwait_timedwait(jb->decode_thread_cond, next_decode_duration);
        }

        if(!jb->started) {
            break;
        }

        // TSK_DEBUG_INFO("Frames count = %d", jb->frames_count);

        // the second condition (jb->frames_count > 0 && latency >= jb->latency_max) is required to make sure we'll process the pending pkts even if the remote party stops sending frames. GE issue: device stops sending frames when it enters in "frame freeze" mode which means #"latency_min" frames won't be displayed.
        if (jb->frames_count >= (int64_t)jb->latency_min || (jb->frames_count > 0 && latency >= jb->latency_max)) {
            tsk_list_item_t *item = tsk_null;
            postpone = tsk_false;
            latency = 0;

            tsk_safeobj_lock(jb); // against get_frame()
            tsk_list_lock(jb->frames); // against put()

            // is it still acceptable to wait for missing packets?
            if (jb->frames_count < (int64_t)jb->latency_max) {
                frame = (const tdav_video_frame_t*)jb->frames->head->data;
                if (!tdav_video_frame_is_complete(frame, jb->decode_last_seq_num_with_mark, &missing_seq_num_start, &missing_seq_num_count)) {
                    TSK_DEBUG_INFO("Time to decode frame...but some RTP packets are missing (missing_seq_num_start=%d, missing_seq_num_count=%d, last_seq_num_with_mark=%d). Postpone :(", missing_seq_num_start, missing_seq_num_count, jb->decode_last_seq_num_with_mark);
                    // signal to the session that a sequence number is missing (will send a NACK)
                    // the missing seqnum has been already requested in jb_put() and here we request it again only ONE time
                    if (jb->callback && frame) {
                        if(prev_missing_seq_num_start != missing_seq_num_start || prev_lasted_missing_seq_num_count != missing_seq_num_count) { // guard to request it only once
                            jb->cb_data_any.type = tdav_video_jb_cb_data_type_fl;
                            jb->cb_data_any.ssrc = frame->ssrc;
                            jb->cb_data_any.fl.seq_num = prev_missing_seq_num_start = missing_seq_num_start;
                            jb->cb_data_any.fl.count = prev_lasted_missing_seq_num_count = missing_seq_num_count;
                            jb->callback(&jb->cb_data_any);
                        }
                        postpone = tsk_true;
                    }
                }
            }
            else {
                TSK_DEBUG_INFO("frames_count(%lld)>=latency_max(%u)...decoding video frame even if pkts are missing :(", jb->frames_count, (unsigned)jb->latency_max);
                jb->decode_last_seq_num_with_mark = -1; // unset()
                // postpone is equal to "tsk_false" which means the pending frame will be displayed in all cases
            }
            if (!postpone) {
                if ((item = tsk_list_pop_first_item(jb->frames))) { // always true (jb->frames_count > 0)
                    --jb->frames_count;
                    // Update the latest decoded timestamp here while we have the lock on the frames
                    jb->decode_last_timestamp = ((const tdav_video_frame_t*)item->data)->timestamp;
                }
            }
            tsk_list_unlock(jb->frames);
            tsk_safeobj_unlock(jb);

            if (item) {
                if(jb->callback) {
                    trtp_rtp_packet_t* pkt;
                    const tsk_list_item_t* _item = item; // save memory address as "tsk_list_foreach() will change it for each loop"
                    int32_t last_seq_num = -1; // guard against duplicated packets
                    frame = _item->data;
                    tsk_list_foreach(_item, frame->pkts) {
                        if(!(pkt = _item->data) || !pkt->payload.size || !pkt->header || pkt->header->seq_num == last_seq_num || !jb->started) {
                            TSK_DEBUG_ERROR("Skipping invalid rtp packet (do not decode!)");
                            continue;
                        }
                        jb->cb_data_rtp.rtp.pkt = pkt;
                        jb->callback(&jb->cb_data_rtp);
                        if(pkt->header->marker) {
                            jb->decode_last_seq_num_with_mark = pkt->header->seq_num;
                        }
                    }
                }

                TSK_OBJECT_SAFE_FREE(item);
            }
        }
        else {
            if (jb->frames_count > 0) { // there are pending frames but we cannot display them yet -> increase latency
                latency++;
            }
        }

#if 1
        if (cleaning_delay || jb->frames_count > (int64_t)jb->latency_max) {
            //x_decode_time = now;
            next_decode_duration = 0;
            cleaning_delay = ((jb->frames_count << 1) > (int64_t)jb->latency_max); // cleanup up2 half
        }
        else {
            next_decode_duration = (1000 / jb->fps);
            _now = tsk_time_now();
            if (_now > now) {
                if ((_now - now) > next_decode_duration) {
                    next_decode_duration = 0;
                }
                else {
                    next_decode_duration -= (_now - now);
                }
            }
            //delay = ( (now > x_decode_time) ? (now - x_decode_time) : (x_decode_duration >> 1)/* do not use zero to avoid endless loop when there is no frame to display */ );
            //next_decode_duration = (delay > x_decode_duration) ? 0 : (x_decode_duration - delay);
            //x_decode_duration = (1000 / jb->fps);
            //x_decode_time += x_decode_duration;
        }
        //delay = /*(now - x_decode_time);*/(now > x_decode_time) ? (now - x_decode_time) : ( (jb->frames_count >= jb->latency_max) ? 0 : (x_decode_duration >> 1) )/* do not use zero to avoid endless loop when there is no frame to display */;
        // delay = (jb->frames_count > jb->latency_max) ? 0 : ( (now > x_decode_time) ? (now - x_decode_time) : (x_decode_duration >> 1)/* do not use zero to avoid endless loop when there is no frame to display */ );
        // comparison used as guard against time wrapping
        /*if(delay > __toomuch_delay_to_be_valid){
         TSK_DEBUG_INFO("Too much delay (%llu) in video jb. Reseting...", delay);
         x_decode_time = now;
         next_decode_duration = 0;
         }
         else*/{
            //next_decode_duration = (delay > x_decode_duration) ? 0 : (x_decode_duration - delay);
            //x_decode_duration = (1000 / jb->fps);
            //x_decode_time += x_decode_duration;
        }


        //TSK_DEBUG_INFO("next_decode_timeout=%llu, delay = %llu", next_decode_duration, delay);
#else
        next_decode_duration = (1000 / jb->fps);
#endif
    }

    TSK_DEBUG_INFO("Video jitter buffer thread - EXIT");

    return tsk_null;
}
