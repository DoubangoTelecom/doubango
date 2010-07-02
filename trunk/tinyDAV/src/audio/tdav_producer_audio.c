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

/**@file tdav_producer_audio.c
 * @brief Base class for all Audio producers.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/audio/tdav_producer_audio.h"

#define TDAV_BITS_PER_SAMPLE_DEFAULT	16
#define TDAV_CHANNELS_DEFAULT			1
#define TDAV_RATE_DEFAULT				8000
#define TDAV_PTIME_DEFAULT				20

#include "tsk_debug.h"

/** Initialize Audio producer
* @param self The producer to initialize
*/
int tdav_producer_audio_init(tdav_producer_audio_t* self)
{
	int ret;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	/* base */
	if((ret = tmedia_producer_init(TMEDIA_PRODUCER(self)))){
		return ret;
	}

	/* self (should be update by prepare() by using the codec's info)*/
	self->bits_per_sample = TDAV_BITS_PER_SAMPLE_DEFAULT;
	self->channels = TDAV_CHANNELS_DEFAULT;
	self->rate = TDAV_RATE_DEFAULT;
	self->ptime = TDAV_PTIME_DEFAULT;

	return 0;
}

/**
* Generic function to compare two producers.
* @param producer1 The first producer to compare.
* @param producer2 The second producer to compare.
* @retval Returns an integral value indicating the relationship between the two producers:
* <0 : @a producer1 less than @a producer2.<br>
* 0  : @a producer1 identical to @a producer2.<br>
* >0 : @a producer1 greater than @a producer2.<br>
*/
int tdav_producer_audio_cmp(const tsk_object_t* producer1, const tsk_object_t* producer2)
{
	return (TDAV_PRODUCER_AUDIO(producer1) - TDAV_PRODUCER_AUDIO(producer2));
}

/** Deinitialize a producer
*/
int tdav_producer_audio_deinit(tdav_producer_audio_t* self)
{
	int ret;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* base */
	if((ret = tmedia_producer_deinit(TMEDIA_PRODUCER(self)))){
		return ret;
	}

	return ret;
}