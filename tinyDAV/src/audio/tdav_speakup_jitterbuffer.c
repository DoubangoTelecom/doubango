/*
* Copyright (C) 2011 Mamadou Diop.
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

/**@file tdav_speakup_jitterbuffer.c
 * @brief Speakup Audio jitterbuffer Plugin
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>

 */
#include "tinydav/audio/tdav_speakup_jitterbuffer.h"

#if !(HAVE_SPEEX_DSP && HAVE_SPEEX_JB)

#include "tinyrtp/rtp/trtp_rtp_header.h"

#include "tsk_time.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <string.h>

#if TSK_UNDER_WINDOWS
#       include <Winsock2.h> // timeval
#elif defined(__SYMBIAN32__)
#       include <_timeval.h>
#else
#       include <sys/time.h>
#endif

#define TDAV_SPEAKUP_10MS						10
#define TDAV_SPEAKUP_10MS_FRAME_SIZE(self)		(((self)->rate * TDAV_SPEAKUP_10MS)/1000)
#define TDAV_SPEAKUP_PTIME_FRAME_SIZE(self)		(((self)->rate * (self)->framesize)/1000)

static int tdav_speakup_jitterbuffer_set(tmedia_jitterbuffer_t *self, const tmedia_param_t* param)
{
    TSK_DEBUG_ERROR("Not implemented");
    return -2;
}

static int tdav_speakup_jitterbuffer_open(tmedia_jitterbuffer_t* self, uint32_t frame_duration, uint32_t rate, uint32_t channels)
{
    tdav_speakup_jitterbuffer_t *jitterbuffer = (tdav_speakup_jitterbuffer_t *)self;
    if(!jitterbuffer->jbuffer) {
        if(!(jitterbuffer->jbuffer = jb_new())) {
            TSK_DEBUG_ERROR("Failed to create new buffer");
            return -1;
        }
        jitterbuffer->jcodec = JB_CODEC_OTHER;
    }
    jitterbuffer->ref_timestamp = 0;
    jitterbuffer->frame_duration = frame_duration;
    jitterbuffer->rate = rate;
    jitterbuffer->channels = channels;
    jitterbuffer->_10ms_size_bytes = 160 * (rate/8000);

    return 0;
}

static int tdav_speakup_jitterbuffer_tick(tmedia_jitterbuffer_t* self)
{
    return 0;
}

static int tdav_speakup_jitterbuffer_put(tmedia_jitterbuffer_t* self, void* data, tsk_size_t data_size, const tsk_object_t* proto_hdr)
{
    tdav_speakup_jitterbuffer_t *jitterbuffer = (tdav_speakup_jitterbuffer_t *)self;
    const trtp_rtp_header_t* rtp_hdr = (const trtp_rtp_header_t*)proto_hdr;
    int i;
    long now, ts;
    void* _10ms_buf;
    uint8_t* pdata;

    if(!self || !data || !data_size || !jitterbuffer->jbuffer || !rtp_hdr) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* synchronize the reference timestamp */
    if(!jitterbuffer->ref_timestamp) {
        uint64_t now = tsk_time_now();
        struct timeval tv;
        long ts = (rtp_hdr->timestamp/(jitterbuffer->rate/1000));
        //=> Do not use (see clock_gettime() on linux): tsk_gettimeofday(&tv, tsk_null);
        tv.tv_sec = (long)(now)/1000;
        tv.tv_usec = (long)(now - (tv.tv_sec*1000))*1000;

        tv.tv_sec -= (ts / jitterbuffer->rate);
        tv.tv_usec -= (ts % jitterbuffer->rate) * 125;
        if((tv.tv_usec -= (tv.tv_usec % (TDAV_SPEAKUP_10MS * 10000))) <0) {
            tv.tv_usec += 1000000;
            tv.tv_sec -= 1;
        }
        jitterbuffer->ref_timestamp = tsk_time_get_ms(&tv);

        switch(rtp_hdr->payload_type) {
        case 8: /*TMEDIA_CODEC_FORMAT_G711a*/
        case 0: /* TMEDIA_CODEC_FORMAT_G711u */
            jitterbuffer->jcodec = JB_CODEC_G711x;
            break;
        case 18: /* TMEDIA_CODEC_FORMAT_G729 */
            jitterbuffer->jcodec = JB_CODEC_G729A;
            break;
        case 3: /* TMEDIA_CODEC_FORMAT_GSM */
            jitterbuffer->jcodec = JB_CODEC_GSM_EFR;
            break;

        default:
            jitterbuffer->jcodec = JB_CODEC_OTHER;
            break;
        }
    }

    // split as several 10ms frames
    now = (long) (tsk_time_now()-jitterbuffer->ref_timestamp);
    ts = (long)(rtp_hdr->timestamp/(jitterbuffer->rate/1000));
    pdata = (uint8_t*)data;
    for(i=0; i<(int)(data_size/jitterbuffer->_10ms_size_bytes); i++) {
        if((_10ms_buf = tsk_calloc(jitterbuffer->_10ms_size_bytes, 1))) {
            memcpy(_10ms_buf, &pdata[i*jitterbuffer->_10ms_size_bytes], jitterbuffer->_10ms_size_bytes);
            jb_put(jitterbuffer->jbuffer, _10ms_buf, JB_TYPE_VOICE, TDAV_SPEAKUP_10MS, ts, now, jitterbuffer->jcodec);
            _10ms_buf = tsk_null;
        }
        ts += TDAV_SPEAKUP_10MS;
    }

    return 0;
}

static tsk_size_t tdav_speakup_jitterbuffer_get(tmedia_jitterbuffer_t* self, void* out_data, tsk_size_t out_size)
{
    tdav_speakup_jitterbuffer_t *jitterbuffer = (tdav_speakup_jitterbuffer_t *)self;
    int jret;

    int i, _10ms_count;
    long now;
    short* _10ms_buf = tsk_null;
    uint8_t* pout_data = (uint8_t*)out_data;

    if(!out_data || (out_size % jitterbuffer->_10ms_size_bytes)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    _10ms_count = (out_size/jitterbuffer->_10ms_size_bytes);
    now = (long) (tsk_time_now() - jitterbuffer->ref_timestamp);
    for(i=0; i<_10ms_count; i++) {

        jret = jb_get(jitterbuffer->jbuffer, (void**)&_10ms_buf, now, TDAV_SPEAKUP_10MS);
        switch(jret) {
        case JB_INTERP:
            TSK_DEBUG_INFO("JB_INTERP");
            jb_reset_all(jitterbuffer->jbuffer);
            memset(&pout_data[i*jitterbuffer->_10ms_size_bytes], 0, (_10ms_count*jitterbuffer->_10ms_size_bytes)-(i*jitterbuffer->_10ms_size_bytes));
            i = _10ms_count; // for exit
            break;
        case JB_OK:
        case JB_EMPTY:
        case JB_NOFRAME:
        case JB_NOJB: {
            if(_10ms_buf && (jret == JB_OK)) {
                /* copy data */
                memcpy(&pout_data[i*jitterbuffer->_10ms_size_bytes], _10ms_buf, jitterbuffer->_10ms_size_bytes);
            }
            else {
                /* copy silence */
                memset(&pout_data[i*jitterbuffer->_10ms_size_bytes], 0, jitterbuffer->_10ms_size_bytes);
            }
        }

        default:
            break;
        }
        TSK_FREE(_10ms_buf);
    }

    return (_10ms_count * jitterbuffer->_10ms_size_bytes);
}

static int tdav_speakup_jitterbuffer_reset(tmedia_jitterbuffer_t* self)
{
    tdav_speakup_jitterbuffer_t *jitterbuffer = (tdav_speakup_jitterbuffer_t *)self;
    if(jitterbuffer->jbuffer) {
        jb_reset_all(jitterbuffer->jbuffer);
        return 0;
    }
    else {
        TSK_DEBUG_ERROR("invalid parameter");
        return -1;
    }
}

static int tdav_speakup_jitterbuffer_close(tmedia_jitterbuffer_t* self)
{
    tdav_speakup_jitterbuffer_t *jitterbuffer = (tdav_speakup_jitterbuffer_t *)self;
    if(jitterbuffer->jbuffer) {
        jb_destroy(jitterbuffer->jbuffer);
        jitterbuffer->jbuffer = tsk_null;
    }
    return 0;
}



//
//	Speakup jitterbufferr Plugin definition
//

/* constructor */
static tsk_object_t* tdav_speakup_jitterbuffer_ctor(tsk_object_t * self, va_list * app)
{
    tdav_speakup_jitterbuffer_t *jitterbuffer = self;
    TSK_DEBUG_INFO("Create speekup jitter buffer");
    if(jitterbuffer) {
        /* init base */
        tmedia_jitterbuffer_init(TMEDIA_JITTER_BUFFER(jitterbuffer));
        /* init self */
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_speakup_jitterbuffer_dtor(tsk_object_t * self)
{
    tdav_speakup_jitterbuffer_t *jitterbuffer = self;
    if(jitterbuffer) {
        /* deinit base */
        tmedia_jitterbuffer_deinit(TMEDIA_JITTER_BUFFER(jitterbuffer));
        /* deinit self */
        if(jitterbuffer->jbuffer) {
            jb_destroy(jitterbuffer->jbuffer);
            jitterbuffer->jbuffer = tsk_null;
        }
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_speakup_jitterbuffer_def_s = {
    sizeof(tdav_speakup_jitterbuffer_t),
    tdav_speakup_jitterbuffer_ctor,
    tdav_speakup_jitterbuffer_dtor,
    tsk_null,
};
/* plugin definition*/
static const tmedia_jitterbuffer_plugin_def_t tdav_speakup_jitterbuffer_plugin_def_s = {
    &tdav_speakup_jitterbuffer_def_s,
    tmedia_audio,
    "Audio/video JitterBuffer based on Speakup",

    tdav_speakup_jitterbuffer_set,
    tdav_speakup_jitterbuffer_open,
    tdav_speakup_jitterbuffer_tick,
    tdav_speakup_jitterbuffer_put,
    tdav_speakup_jitterbuffer_get,
    tdav_speakup_jitterbuffer_reset,
    tdav_speakup_jitterbuffer_close,
};
const tmedia_jitterbuffer_plugin_def_t *tdav_speakup_jitterbuffer_plugin_def_t = &tdav_speakup_jitterbuffer_plugin_def_s;

#endif /* !(HAVE_SPEEX_DSP && HAVE_SPEEX_JB) */
