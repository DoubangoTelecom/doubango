/*
 * Copyright (C) 2010-2011 Mamadou Diop.
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
#ifndef TDAV_AUDIO_UNIT_H
#define TDAV_AUDIO_UNIT_H

#include "tinydav_config.h"

#if HAVE_COREAUDIO_AUDIO_UNIT

#include <AudioUnit/AudioUnit.h>
#include <AudioToolbox/AudioToolbox.h>
#include "tsk_object.h"

TDAV_BEGIN_DECLS


#if TARGET_OS_MAC
#	if AUDIO_UNIT_VERSION < 1060
#		define AudioComponent Component
#		define AudioComponentInstance ComponentInstance
#		define AudioComponentDescription ComponentDescription
#		define AudioComponentFindNext FindNextComponent
#		define AudioComponentInstanceNew OpenAComponent
#		define AudioComponentInstanceDispose CloseComponent
#	endif
#endif /* TARGET_OS_MAC */

typedef void* tdav_audiounit_handle_t;

tdav_audiounit_handle_t* tdav_audiounit_handle_create(uint64_t session_id);
AudioComponentInstance tdav_audiounit_handle_get_instance(tdav_audiounit_handle_t* self);
int tdav_audiounit_handle_signal_consumer_ready(tdav_audiounit_handle_t* self);
int tdav_audiounit_handle_signal_producer_ready(tdav_audiounit_handle_t* self);
int tdav_audiounit_handle_start(tdav_audiounit_handle_t* self);
uint32_t tdav_audiounit_handle_get_frame_duration(tdav_audiounit_handle_t* self);
int tdav_audiounit_handle_configure(tdav_audiounit_handle_t* self, tsk_bool_t consumer, uint32_t ptime, AudioStreamBasicDescription* audioFormat);
int tdav_audiounit_handle_mute(tdav_audiounit_handle_t* self, tsk_bool_t mute);
int tdav_audiounit_handle_interrupt(tdav_audiounit_handle_t* self, tsk_bool_t interrupt);
int tdav_audiounit_handle_stop(tdav_audiounit_handle_t* self);
int tdav_audiounit_handle_destroy(tdav_audiounit_handle_t** self);

TDAV_END_DECLS

#endif /* HAVE_COREAUDIO_AUDIO_UNIT */

#endif /* TDAV_AUDIO_UNIT_H */
