/*
* Copyright (C) 2009 Mamadou Diop.
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

/**@file tdav_consumer_dsound.c
 * @brief Microsoft DirectSound consumer.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/audio/directsound/tdav_consumer_dsound.h"






/* ============ Media Consumer Interface ================= */
int tdav_consumer_dsound_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
	return 0;
}

int tdav_consumer_dsound_start(tmedia_consumer_t* self)
{
	return 0;
}

int tdav_consumer_dsound_consume(tmedia_consumer_t* self, void** buffer, tsk_size_t size)
{
	return 0;
}

int tdav_consumer_dsound_pause(tmedia_consumer_t* self)
{
	return 0;
}

int tdav_consumer_dsound_stop(tmedia_consumer_t* self)
{
	return 0;
}


//
//	WaveAPI consumer object definition
//
/* constructor */
static tsk_object_t* tdav_consumer_dsound_ctor(tsk_object_t * self, va_list * app)
{
	tdav_consumer_dsound_t *consumer = self;
	if(consumer){
		/* init base */
		tdav_consumer_audio_init(TDAV_CONSUMER_AUDIO(consumer));
		/* init self */
		
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_consumer_dsound_dtor(tsk_object_t * self)
{ 
	tdav_consumer_dsound_t *consumer = self;
	if(consumer){
		/* stop */
		if(consumer->started){
			tdav_consumer_dsound_stop(self);
		}

		/* deinit base */
		tdav_consumer_audio_deinit(TDAV_CONSUMER_AUDIO(consumer));
		/* deinit self */
		
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_consumer_dsound_def_s = 
{
	sizeof(tdav_consumer_dsound_t),
	tdav_consumer_dsound_ctor, 
	tdav_consumer_dsound_dtor,
	tdav_consumer_audio_cmp, 
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t tmedia_consumer_dsound_plugin_def_s = 
{
	&tdav_consumer_dsound_def_s,
	
	tmedia_audio,
	"Microsoft DirectSound consumer",
	
	tdav_consumer_dsound_prepare,
	tdav_consumer_dsound_start,
	tdav_consumer_dsound_consume,
	tdav_consumer_dsound_pause,
	tdav_consumer_dsound_stop
};
const tmedia_consumer_plugin_def_t *tmedia_consumer_dsound_plugin_def_t = &tmedia_consumer_dsound_plugin_def_s;
