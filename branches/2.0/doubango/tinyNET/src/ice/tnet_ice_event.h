/*
* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

#ifndef TNET_ICE_EVENT_H
#define TNET_ICE_EVENT_H

#include "tinynet_config.h"

#include "tsk_object.h"

TNET_BEGIN_DECLS

typedef enum tnet_ice_event_type_e
{
	// Public events
	tnet_ice_event_type_started,
	tnet_ice_event_type_start_failed,
	tnet_ice_event_type_stopped,
	tnet_ice_event_type_stop_failed,
	tnet_ice_event_type_gathering_host_candidates_failed,
	tnet_ice_event_type_gathering_host_candidates_succeed,
	tnet_ice_event_type_gathering_reflexive_candidates_failed,
	tnet_ice_event_type_gathering_reflexive_candidates_succeed,
	tnet_ice_event_type_gathering_relay_candidates_failed,
	tnet_ice_event_type_gathering_relay_candidates_succeed,
	tnet_ice_event_type_gathering_completed,
	tnet_ice_event_type_conncheck_succeed,
	tnet_ice_event_type_conncheck_failed,
	tnet_ice_event_type_cancelled,
	tnet_ice_event_type_turn_connection_broken,

	// Private events
	tnet_ice_event_type_action
}
tnet_ice_event_type_t;

typedef struct tnet_ice_event_s
{
	TSK_DECLARE_OBJECT;

	tnet_ice_event_type_t type;
	char* phrase;
	struct tnet_ice_action_s* action; 
	const struct tnet_ice_ctx_s* ctx;

	const void* userdata;
}
tnet_ice_event_t;

TINYNET_GEXTERN const tsk_object_def_t *tnet_ice_event_def_t;

tnet_ice_event_t* tnet_ice_event_create(const struct tnet_ice_ctx_s* ctx, tnet_ice_event_type_t type, const char* phrase, const void* userdata);
int tnet_ice_event_set_action(tnet_ice_event_t* self, struct tnet_ice_action_s* action);

TNET_END_DECLS

#endif /* TNET_ICE_EVENT_H */
