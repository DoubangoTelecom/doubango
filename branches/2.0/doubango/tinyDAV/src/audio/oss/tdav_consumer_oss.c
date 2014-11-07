/* Copyright (C) 2014 Mamadou DIOP.
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
#include "tinydav/audio/oss/tdav_consumer_oss.h"

#if HAVE_LINUX_SOUNDCARD_H

#include "tsk_string.h"
#include "tsk_thread.h"
#include "tsk_memory.h"
#include "tsk_safeobj.h"
#include "tsk_debug.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/soundcard.h>

#define OSS_DEBUG_INFO(FMT, ...) TSK_DEBUG_INFO("[OSS Consumer] " FMT, ##__VA_ARGS__)
#define OSS_DEBUG_WARN(FMT, ...) TSK_DEBUG_WARN("[OSS Consumer] " FMT, ##__VA_ARGS__)
#define OSS_DEBUG_ERROR(FMT, ...) TSK_DEBUG_ERROR("[OSS Consumer] " FMT, ##__VA_ARGS__)
#define OSS_DEBUG_FATAL(FMT, ...) TSK_DEBUG_FATAL("[OSS Consumer] " FMT, ##__VA_ARGS__)

typedef struct tdav_consumer_oss_s
{
	TDAV_DECLARE_CONSUMER_AUDIO;

	tsk_bool_t b_started;
	tsk_bool_t b_prepared;
	
	int fd;
	
	TSK_DECLARE_SAFEOBJ;
}
tdav_consumer_oss_t;

/* ============ Media Consumer Interface ================= */
static int tdav_consumer_oss_set(tmedia_consumer_t* self, const tmedia_param_t* param)
{
	tdav_consumer_oss_t* p_oss = (tdav_consumer_oss_t*)self;
	int ret = 0;

	ret = tdav_consumer_audio_set(TDAV_CONSUMER_AUDIO(self), param);

	return ret;
}

static int tdav_consumer_oss_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
	int err = 0;
	tdav_consumer_oss_t* p_oss = (tdav_consumer_oss_t*)self;
	
	if (!p_oss || !codec) {
		OSS_DEBUG_ERROR("Invalid paramter");
		return -1;
	}

	tsk_safeobj_lock(p_oss);
	
	if (p_oss->b_started) {
		OSS_DEBUG_ERROR("You must stop the consumer first");
		err = -2;
		goto bail;
	}

	p_oss->b_prepared = tsk_true;

bail:
	tsk_safeobj_unlock(p_oss);
	return err;
}

static int tdav_consumer_oss_start(tmedia_consumer_t* self)
{
	int err = 0;
	tdav_consumer_oss_t* p_oss = (tdav_consumer_oss_t*)self;
	
	if (!p_oss) {
		OSS_DEBUG_ERROR("Invalid paramter");
		return -1;
	}

	tsk_safeobj_lock(p_oss);
	
	if (!p_oss->b_prepared) {
		OSS_DEBUG_ERROR("Not prepared");
		err = -2;
		goto bail;
	}
	if (p_oss->b_started) {
		OSS_DEBUG_WARN("Already started");
		goto bail;
	}

	p_oss->b_started = tsk_true;

bail:
	tsk_safeobj_unlock(p_oss);
	return err;
}

static int tdav_consumer_oss_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
	int err = 0;
	tdav_consumer_oss_t* p_oss = (tdav_consumer_oss_t*)self;
	
	if (!p_oss || !buffer || !size) {
		OSS_DEBUG_ERROR("Invalid paramter");
		return -1;
	}

	tsk_safeobj_lock(p_oss);
	
	if (!p_oss->b_started) {
		OSS_DEBUG_WARN("Not started");
		err = -2;
		goto bail;
	}

bail:
	tsk_safeobj_unlock(p_oss);
	return err;
}

static int tdav_consumer_oss_pause(tmedia_consumer_t* self)
{
	return 0;
}

static int tdav_consumer_oss_stop(tmedia_consumer_t* self)
{
	int err = 0;
	tdav_consumer_oss_t* p_oss = (tdav_consumer_oss_t*)self;
	
	if (!p_oss) {
		OSS_DEBUG_ERROR("Invalid paramter");
		return -1;
	}

	tsk_safeobj_lock(p_oss);
	
	p_oss->b_started = tsk_false;
	p_oss->b_prepared = tsk_false;

bail:
	tsk_safeobj_unlock(p_oss);
	return err;
}


//
//	Linux OSS consumer object definition
//
/* constructor */
static tsk_object_t* tdav_consumer_oss_ctor(tsk_object_t * self, va_list * app)
{
	tdav_consumer_oss_t *p_oss = self;
	if (p_oss) {
		/* init base */
		tdav_consumer_audio_init(TDAV_CONSUMER_AUDIO(p_oss));
		/* init self */

		p_oss->fd = -1;
		tsk_safeobj_init(p_oss);

		OSS_DEBUG_INFO("created");
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_consumer_oss_dtor(tsk_object_t * self)
{ 
	tdav_consumer_oss_t *p_oss = self;
	if (p_oss) {

		/* stop */
		if (p_oss->b_started) {
			tdav_consumer_oss_stop(self);
		}

		/* deinit base */
		tdav_consumer_audio_deinit(TDAV_CONSUMER_AUDIO(p_oss));
		/* deinit self */
		if (p_oss->fd > 0) {
			close(p_oss->fd);
			p_oss->fd = -1;
		}
		tsk_safeobj_deinit(p_oss);
		
		OSS_DEBUG_INFO("*** destroyed ***");
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_consumer_oss_def_s = 
{
	sizeof(tdav_consumer_oss_t),
	tdav_consumer_oss_ctor, 
	tdav_consumer_oss_dtor,
	tdav_consumer_audio_cmp, 
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t tdav_consumer_oss_plugin_def_s = 
{
	&tdav_consumer_oss_def_s,
	
	tmedia_audio,
	"Linux OSS consumer",
	
	tdav_consumer_oss_set,
	tdav_consumer_oss_prepare,
	tdav_consumer_oss_start,
	tdav_consumer_oss_consume,
	tdav_consumer_oss_pause,
	tdav_consumer_oss_stop
};
const tmedia_consumer_plugin_def_t *tdav_consumer_oss_plugin_def_t = &tdav_consumer_oss_plugin_def_s;

#endif /* HAVE_LINUX_SOUNDCARD_H */
