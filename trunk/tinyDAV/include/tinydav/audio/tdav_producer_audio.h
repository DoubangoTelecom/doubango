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

/**@file tdav_producer_audio.h
 * @brief Base class for all Audio producers.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYDAV_PRODUCER_AUDIO_H
#define TINYDAV_PRODUCER_AUDIO_H

#include "tinydav_config.h"

#include "tinymedia/tmedia_producer.h"

TDAV_BEGIN_DECLS

#define TDAV_BITS_PER_SAMPLE_DEFAULT 16

typedef struct tdav_producer_audio_s
{
	TMEDIA_DECLARE_PRODUCER;

	uint8_t channels;
	uint32_t rate;
	uint8_t bits_per_sample;
}
tdav_producer_audio_t;

int tdav_producer_audio_init(tdav_producer_audio_t* self);
int tdav_producer_audio_prepare(tdav_producer_audio_t *self);
int tdav_producer_audio_start(tdav_producer_audio_t *self);
int tdav_producer_audio_consume(tdav_producer_audio_t* self, const void* buffer, tsk_size_t size);
int tdav_producer_audio_pause(tdav_producer_audio_t *self);
int tdav_producer_audio_stop(tdav_producer_audio_t *self);
int tdav_producer_audio_deinit(tdav_producer_audio_t* self);

TDAV_END_DECLS

#endif /* TINYDAV_PRODUCER_AUDIO_H */

