/*
* Copyright (C) 2010-2011 Mamadou Diop.
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

/**@file tmedia_qos.h
 * @brief RFC 3312 (Preconditions) implementation.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYMEDIA_QOS_H
#define TINYMEDIA_QOS_H

#include "tinymedia_config.h"

#include "tinysdp/tsdp_message.h"

#include "tsk_object.h"

TMEDIA_BEGIN_DECLS

#define TMEDIA_QOS_TLINE(self) ((tmedia_qos_tline_t*)(self))

/** List of all supported statues*/
typedef enum tmedia_qos_status_e
{
	//! current-status
	tmedia_qos_status_curr,
	//! desired-status
	tmedia_qos_status_des,
	//! confirm-status
	tmedia_qos_status_conf
}
tmedia_qos_status_t;

/** List of all supported precondition types */
typedef enum tmedia_qos_ptype_e
{
	tmedia_qos_ptype_qos
}
tmedia_qos_ptype_t;

/** List of all supported status types */
typedef enum tmedia_qos_stype_e
{
	tmedia_qos_stype_none,/* not part of the RFC */

	tmedia_qos_stype_segmented,
	tmedia_qos_stype_e2e,
}
tmedia_qos_stype_t;

/** strengths */
typedef enum tmedia_qos_strength_e
{
	/* do no change the order (none -> optional -> manadatory) */
	tmedia_qos_strength_none,	
	tmedia_qos_strength_failure,
	tmedia_qos_strength_unknown,
	tmedia_qos_strength_optional,
	tmedia_qos_strength_mandatory
}
tmedia_qos_strength_t;

/** directions */
typedef enum tmedia_qos_direction_e
{
	tmedia_qos_direction_none = 0x01,
	tmedia_qos_direction_send = (0x01 << 1),
	tmedia_qos_direction_recv = (0x01 << 2),
	tmedia_qos_direction_sendrecv = (tmedia_qos_direction_send | tmedia_qos_direction_recv)
}
tmedia_qos_direction_t;

/* QoS table-line */
typedef struct tmedia_qos_tline_s
{
	TSK_DECLARE_OBJECT;
	tmedia_qos_stype_t type;
}
tmedia_qos_tline_t;
#define TMEDIA_DECLARE_QOS_TLINE tmedia_qos_tline_t __tline__

TINYMEDIA_API tmedia_qos_tline_t* tmedia_qos_tline_create(tmedia_qos_stype_t type, tmedia_qos_strength_t strength);
TINYMEDIA_API tmedia_qos_stype_t tmedia_qos_get_type(const tsdp_header_M_t* m);
TINYMEDIA_API tmedia_qos_tline_t* tmedia_qos_tline_from_sdp(const tsdp_header_M_t* m);
TINYMEDIA_API int tmedia_qos_tline_to_sdp(const tmedia_qos_tline_t* self, tsdp_header_M_t* m);
TINYMEDIA_API int tmedia_qos_tline_set_ro(tmedia_qos_tline_t* self, const tmedia_qos_tline_t* ro);
TINYMEDIA_API tsk_bool_t tmedia_qos_tline_canresume(const tmedia_qos_tline_t* self);

/* QoS table-line for E2E type*/
typedef struct tmedia_qos_tline_e2e_s
{
	TMEDIA_DECLARE_QOS_TLINE;

	/* RFC 3312 - 5.1 Generating an offer

		Direction  Current  Desired Strength
		____________________________________
		send       no        mandatory
		recv       no        mandatory
	*/
	struct{
		unsigned current:1;
		unsigned confirm:1;
		tmedia_qos_strength_t strength;
	} send;

	struct{
		unsigned current:1;
		unsigned confirm:1;
		tmedia_qos_strength_t strength;
	} recv;
}
tmedia_qos_tline_e2e_t;

TINYMEDIA_API tmedia_qos_tline_e2e_t* tmedia_qos_tline_e2e_create(tmedia_qos_strength_t strength);
TINYMEDIA_API tmedia_qos_tline_e2e_t* tmedia_qos_tline_e2e_from_sdp(const tsdp_header_M_t* m);
TINYMEDIA_API int tmedia_qos_tline_e2e_to_sdp(const tmedia_qos_tline_e2e_t* self, tsdp_header_M_t* m);
TINYMEDIA_API int tmedia_qos_tline_e2e_set_ro(tmedia_qos_tline_e2e_t* self, const tmedia_qos_tline_e2e_t* ro);
TINYMEDIA_API tsk_bool_t tmedia_qos_tline_e2e_canresume(const tmedia_qos_tline_e2e_t* self);

/* QoS table-line for Segented type*/
typedef struct tmedia_qos_tline_segmented_s
{
	TMEDIA_DECLARE_QOS_TLINE;

	/* RFC 3312 - 5.1 Generating an offer
		Direction   Current  Desired Strength
		______________________________________
		local send     no           none
		local recv     no           none
		remote send    no         optional
		remote recv    no           none
	*/
	/* can be done in two lines but I prefer doing it like this (easier) */
	struct{
		unsigned current:1;
		unsigned confirm:1;
		tmedia_qos_strength_t strength;
	} local_send;

	struct{
		unsigned current:1;
		unsigned confirm:1;
		tmedia_qos_strength_t strength;
	} local_recv;

	struct{
		unsigned current:1;
		unsigned confirm:1;
		tmedia_qos_strength_t strength;
	} remote_send;

	struct{
		unsigned current:1;
		unsigned confirm:1;
		tmedia_qos_strength_t strength;
	} remote_recv;
}
tmedia_qos_tline_segmented_t;


TINYMEDIA_API tmedia_qos_tline_segmented_t* tmedia_qos_tline_segmented_create(tmedia_qos_strength_t strength);
TINYMEDIA_API tmedia_qos_tline_segmented_t* tmedia_qos_tline_segmented_from_sdp(const tsdp_header_M_t* m);
TINYMEDIA_API int tmedia_qos_tline_segmented_to_sdp(const tmedia_qos_tline_segmented_t* self, tsdp_header_M_t* m);
TINYMEDIA_API int tmedia_qos_tline_segmented_set_ro(tmedia_qos_tline_segmented_t* self, const tmedia_qos_tline_segmented_t* ro);
TINYMEDIA_API tsk_bool_t tmedia_qos_tline_segmented_canresume(const tmedia_qos_tline_segmented_t* self);

TINYMEDIA_GEXTERN const tsk_object_def_t *tmedia_qos_tline_segmented_def_t;
TINYMEDIA_GEXTERN const tsk_object_def_t *tmedia_qos_tline_e2e_def_t;

TMEDIA_END_DECLS

#endif /* TINYMEDIA_QOS_H */
