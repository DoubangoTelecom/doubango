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
#include "tinydav/audio/alsa/tdav_consumer_alsa.h"

#if HAVE_ALSA_ASOUNDLIB_H


#include "tsk_thread.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/soundcard.h>

typedef struct tdav_consumer_alsa_s
{
	TDAV_DECLARE_CONSUMER_AUDIO;

	tsk_bool_t b_started;
}
tdav_consumer_alsa_t;

/* ============ Media Consumer Interface ================= */
static int tdav_consumer_alsa_set(tmedia_consumer_t* self, const tmedia_param_t* param)
{
	tdav_consumer_alsa_t* p_alsa = (tdav_consumer_alsa_t*)self;
	int ret = 0;

	ret = tdav_consumer_audio_set(TDAV_CONSUMER_AUDIO(self), param);

	return ret;
}

static int tdav_consumer_alsa_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
	return 0;
}

static int tdav_consumer_alsa_start(tmedia_consumer_t* self)
{
	return 0;
}

static int tdav_consumer_alsa_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
	return 0;
}

static int tdav_consumer_alsa_pause(tmedia_consumer_t* self)
{
	return 0;
}

static int tdav_consumer_alsa_stop(tmedia_consumer_t* self)
{

	return 0;
}


//
//	ALSA consumer object definition
//
/* constructor */
static tsk_object_t* tdav_consumer_alsa_ctor(tsk_object_t * self, va_list * app)
{
	tdav_consumer_alsa_t *consumer = self;
	if (consumer) {
		/* init base */
		tdav_consumer_audio_init(TDAV_CONSUMER_AUDIO(consumer));
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_consumer_alsa_dtor(tsk_object_t * self)
{ 
	tdav_consumer_alsa_t *consumer = self;
	if (consumer) {
		tsk_size_t i;

		/* stop */
		if (consumer->b_started) {
			tdav_consumer_alsa_stop(self);
		}

		/* deinit base */
		tdav_consumer_audio_deinit(TDAV_CONSUMER_AUDIO(consumer));
		/* deinit self */
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_consumer_alsa_def_s = 
{
	sizeof(tdav_consumer_alsa_t),
	tdav_consumer_alsa_ctor, 
	tdav_consumer_alsa_dtor,
	tdav_consumer_audio_cmp, 
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t tdav_consumer_alsa_plugin_def_s = 
{
	&tdav_consumer_alsa_def_s,
	
	tmedia_audio,
	"Linux ALSA consumer",
	
	tdav_consumer_alsa_set,
	tdav_consumer_alsa_prepare,
	tdav_consumer_alsa_start,
	tdav_consumer_alsa_consume,
	tdav_consumer_alsa_pause,
	tdav_consumer_alsa_stop
};
const tmedia_consumer_plugin_def_t *tdav_consumer_alsa_plugin_def_t = &tdav_consumer_alsa_plugin_def_s;

#endif /* #if HAVE_ALSA_ASOUNDLIB_H */
