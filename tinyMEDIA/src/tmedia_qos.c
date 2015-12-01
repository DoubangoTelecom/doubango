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

/**@file tmedia_qos.c
 * @brief RFC 3312 (Preconditions) implementation.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinymedia/tmedia_qos.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

/* ========================= Internal ==================================*/

static const char* tmedia_qos_strength_tostring(tmedia_qos_strength_t strength)
{
	switch(strength){
		case tmedia_qos_strength_none:
			return "none";
		case tmedia_qos_strength_mandatory:
			return "mandatory";
		case tmedia_qos_strength_optional:
			return "optional";
		case tmedia_qos_strength_failure:
			return "failure";
		case tmedia_qos_strength_unknown:
		default:
			return "unknown";
	}
}

static tmedia_qos_strength_t tmedia_qos_strength_fromstring(const char* strength)
{
	if(tsk_strequals(strength, "none")){
		return tmedia_qos_strength_none;
	}
	else if(tsk_strequals(strength, "mandatory")){
		return tmedia_qos_strength_mandatory;
	}
	else if(tsk_strequals(strength, "optional")){
		return tmedia_qos_strength_optional;
	}
	else if(tsk_strequals(strength, "failure")){
		return tmedia_qos_strength_failure;
	}
	else{
		return tmedia_qos_strength_unknown;
	}
}

static tmedia_qos_direction_t tmedia_qos_direction_fromstring(const char* direction)
{
	if(tsk_strequals(direction, "send")){
		return tmedia_qos_direction_send;
	}
	else if(tsk_strequals(direction, "recv")){
		return tmedia_qos_direction_recv;
	}
	else if(tsk_strequals(direction, "sendrecv")){
		return tmedia_qos_direction_sendrecv;
	}
	else{
		return tmedia_qos_direction_none;
	}
}

/* ========================= Common ==================================*/

tmedia_qos_tline_t* tmedia_qos_tline_create(tmedia_qos_stype_t type, tmedia_qos_strength_t strength)
{
	switch(type){
		case tmedia_qos_stype_e2e:
			return (tmedia_qos_tline_t*)tmedia_qos_tline_e2e_create(strength);
		case tmedia_qos_stype_segmented:
			return (tmedia_qos_tline_t*)tmedia_qos_tline_segmented_create(strength);
        default: break;
	}
	return tsk_null;
}

tmedia_qos_stype_t tmedia_qos_get_type(const tsdp_header_M_t* m)
{
	const tsdp_header_A_t* A;
	char s0[10];

	if(!m){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tmedia_qos_stype_none;
	}

	if((A = tsdp_header_M_findA(m, "curr"))){
		if(sscanf(A->value, "qos %s %*s", s0) != EOF){
			return tsk_strequals(s0, "e2e") ? tmedia_qos_stype_e2e : tmedia_qos_stype_segmented;
		}
		else{
			TSK_DEBUG_ERROR("Failed to parse a=%s:%s", A->field, A->value);
		}
	}
	
	return tmedia_qos_stype_none;
}

tmedia_qos_tline_t* tmedia_qos_tline_from_sdp(const tsdp_header_M_t* m)
{
	if(!m){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	switch(tmedia_qos_get_type(m)){
		case tmedia_qos_stype_e2e:
			return (tmedia_qos_tline_t*)tmedia_qos_tline_e2e_from_sdp(m);
		case tmedia_qos_stype_segmented:
			return (tmedia_qos_tline_t*)tmedia_qos_tline_segmented_from_sdp(m);
		default:
			return tsk_null;
	}
}

int tmedia_qos_tline_to_sdp(const tmedia_qos_tline_t* self, tsdp_header_M_t* m)
{
	if(!self || !m){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	switch(self->type){
		case tmedia_qos_stype_e2e:
			return tmedia_qos_tline_e2e_to_sdp((tmedia_qos_tline_e2e_t*)self, m);
		case tmedia_qos_stype_segmented:
			return tmedia_qos_tline_segmented_to_sdp((tmedia_qos_tline_segmented_t*)self, m);
		default:
			TSK_DEBUG_ERROR("Invalid type");
			return -2;
	}
}

int tmedia_qos_tline_set_ro(tmedia_qos_tline_t* self, const tmedia_qos_tline_t* ro)
{
	if(!self || !ro){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	switch(self->type){
		case tmedia_qos_stype_e2e:
			return tmedia_qos_tline_e2e_set_ro((tmedia_qos_tline_e2e_t*)self, (const tmedia_qos_tline_e2e_t*)ro);
		case tmedia_qos_stype_segmented:
			return tmedia_qos_tline_segmented_set_ro((tmedia_qos_tline_segmented_t*)self, (const tmedia_qos_tline_segmented_t*)ro);
		default:
			TSK_DEBUG_ERROR("Invalid type");
			return -2;
	}
}

tsk_bool_t tmedia_qos_tline_canresume(const tmedia_qos_tline_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_true;
	}
	switch(self->type){
		case tmedia_qos_stype_segmented:
			return tmedia_qos_tline_segmented_canresume((const tmedia_qos_tline_segmented_t*)self); 
		case tmedia_qos_stype_e2e:
			return tmedia_qos_tline_e2e_canresume((const tmedia_qos_tline_e2e_t*)self);
		default:
			return tsk_true;
	}
}

/* ========================= E2E ==================================*/

tmedia_qos_tline_e2e_t* tmedia_qos_tline_e2e_create(tmedia_qos_strength_t strength)
{
	return tsk_object_new(tmedia_qos_tline_e2e_def_t, strength);
}

tmedia_qos_tline_e2e_t* tmedia_qos_tline_e2e_from_sdp(const tsdp_header_M_t* m)
{
	tmedia_qos_tline_e2e_t* e2e = tsk_null;
	const tsdp_header_A_t* A;
	tsk_size_t i;

	char s0[10], s1[10];

	if(!m){
		TSK_DEBUG_ERROR("invalid parameter");
		return tsk_null;
	}
	
	/*  Example
		a=curr:qos e2e none
		a=des:qos mandatory e2e sendrecv
		a=conf:qos e2e recv
	*/
	
	e2e = tmedia_qos_tline_e2e_create(tmedia_qos_strength_unknown);

	/* curr */
	for(i = 0; (A = tsdp_header_M_findA_at(m, "curr", i)); i++){
		if(sscanf(A->value, "qos e2e %10s", s0) != EOF){
			tmedia_qos_direction_t dir = tmedia_qos_direction_fromstring(s0);
			switch(dir){
				case tmedia_qos_direction_send:
					e2e->send.current = tsk_true;
					break;
				case tmedia_qos_direction_recv:
					e2e->recv.current = tsk_true;
					break;
				case tmedia_qos_direction_sendrecv:
					e2e->send.current = tsk_true;
					e2e->recv.current = tsk_true;
					break;
				case tmedia_qos_direction_none:
					e2e->send.current = tsk_false;
					e2e->recv.current = tsk_false;
					break;
				default:
					break;
			}
		}
		else{
			TSK_DEBUG_ERROR("Failed to parse a=%s:%s", A->field, A->value);
		}
	}

	/* des */
	for(i = 0; (A = tsdp_header_M_findA_at(m, "des", i)); i++){
		if(sscanf(A->value, "qos %10s e2e %10s", s0, s1) != EOF){
			tmedia_qos_strength_t strength = tmedia_qos_strength_fromstring(s0);
			tmedia_qos_direction_t dir = tmedia_qos_direction_fromstring(s1);
			switch(dir){
				case tmedia_qos_direction_send:
					e2e->send.strength = strength;
					break;
				case tmedia_qos_direction_recv:
					e2e->recv.strength = strength;
					break;
				case tmedia_qos_direction_sendrecv:
					e2e->send.strength = strength;
					e2e->recv.strength = strength;
					break;
				default:
					break;
			}
		}
		else{
			TSK_DEBUG_ERROR("Failed to parse a=%s:%s", A->field, A->value);
		}
	}
	
	/* conf */
	for(i = 0; (A = tsdp_header_M_findA_at(m, "conf", i)); i++){
		if(sscanf(A->value, "qos e2e %10s", s0) != EOF){
			tmedia_qos_direction_t dir = tmedia_qos_direction_fromstring(s0);
			switch(dir){
				case tmedia_qos_direction_send:
					e2e->send.confirm = tsk_true;
					break;
				case tmedia_qos_direction_recv:
					e2e->recv.confirm = tsk_true;
					break;
				case tmedia_qos_direction_sendrecv:
					e2e->send.confirm = tsk_true;
					e2e->recv.confirm = tsk_true;
					break;
				default:
					break;
			}
		}
		else{
			TSK_DEBUG_ERROR("Failed to parse a=%s:%s", A->field, A->value);
		}
	}

	return e2e;
}

int tmedia_qos_tline_e2e_to_sdp(const tmedia_qos_tline_e2e_t* self, tsdp_header_M_t* m)
{
   /* RFC 3312 - 5.1.1 SDP encoding

	   For the end-to-end status type, the user agent MUST generate one
	   current status line with the tag "e2e" for the media stream.  If the
	   strength-tags for both directions are equal (e.g., both "mandatory")
	   in the transaction status table, the user agent MUST add one desired
	   status line with the tag "sendrecv".  If both tags are different, the
	   user agent MUST include two desired status lines, one with the tag
	   "send" and the other with the tag "recv".
	*/
	char* temp = tsk_null;

	if(!self || !m){
		TSK_DEBUG_ERROR("invalid parameter");
		return -1;
	}
	/*  Example
		a=curr:qos e2e none
		a=des:qos mandatory e2e sendrecv
	*/

	
	/* curr */
	tsk_sprintf(&temp, "qos e2e %s", (self->recv.current && self->send.current) ? "sendrecv" : (self->recv.current ? "recv" : (self->send.current ? "send" : "none")));
	tsdp_header_M_add_headers(m,
		TSDP_HEADER_A_VA_ARGS("curr", temp),
		tsk_null);
	TSK_FREE(temp);
	
	/* des */
	if(self->recv.strength == self->send.strength){
		/* sendrecv */
		tsk_sprintf(&temp, "qos %s e2e sendrecv", tmedia_qos_strength_tostring(self->recv.strength));
		tsdp_header_M_add_headers(m,
			TSDP_HEADER_A_VA_ARGS("des", temp),
			tsk_null);
		TSK_FREE(temp);
	}
	else{
		/* send */
		tsk_sprintf(&temp, "qos %s e2e send", tmedia_qos_strength_tostring(self->send.strength));
		tsdp_header_M_add_headers(m,
			TSDP_HEADER_A_VA_ARGS("des", temp),
			tsk_null);
		TSK_FREE(temp);

		/* recv */
		tsk_sprintf(&temp, "qos %s e2e recv", tmedia_qos_strength_tostring(self->recv.strength));
		tsdp_header_M_add_headers(m,
			TSDP_HEADER_A_VA_ARGS("des", temp),
			tsk_null);
		TSK_FREE(temp);
	}

	/* conf (should not request confirm on "send" direction)*/
	if(self->recv.confirm){
		tsdp_header_M_add_headers(m,
			TSDP_HEADER_A_VA_ARGS("conf", "qos e2e recv"),
			tsk_null);
	}
	return 0;
}

int tmedia_qos_tline_e2e_set_ro(tmedia_qos_tline_e2e_t* self, const tmedia_qos_tline_e2e_t* ro)
{
	if(!self || !ro){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* We were the offerer
	* Remote asked for confirmation in its "recv" direction? 
	* "recv" direction for remote is our "send" direction
	* As we don't support RSVP (under the way), confirm immediatly.
	* "send" direction should not requested for confirmation
	*/
	if(ro->recv.confirm){
		self->send.current = tsk_true;
		goto bail;
	}
	if(ro->send.current){
		self->recv.confirm = tsk_false; /* remote confirmed */
		self->recv.current = tsk_true; /* because ro confirmed */
		self->send.current = tsk_true; /* beacuse we don't support RSVP */
		goto bail;
	}

	/* We are the answerer
	* As we don't support RSVP (under the way):
	* ==> request confirmation for "recv" direction if equal to "none" (not reserved)
	* =>
	*/
	if(!self->recv.current){
		self->recv.confirm = tsk_true;
		goto bail;
	}

bail:
	/* all other cases: success */
	return 0;
}

tsk_bool_t tmedia_qos_tline_e2e_canresume(const tmedia_qos_tline_e2e_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_true;
	}

	/*  Example
		a=curr:qos e2e none
		a=des:qos mandatory e2e sendrecv

		Or

		a=curr:qos e2e send
		a=des:qos mandatory e2e recv
		a=des:qos optional e2e send
	*/

	/* only "mandatory" strength should force the application to continue nego. */
	if(self->recv.strength == tmedia_qos_strength_mandatory && !self->recv.current){
		return tsk_false;
	}
	/*else */if(self->send.strength == tmedia_qos_strength_mandatory && !self->send.current){
		return tsk_false;
	}
	
	/* "optinal" and "none" strengths */
	return tsk_true;
}

//
//	E2E QoS line object definition
//
static tsk_object_t* tmedia_qos_tline_e2e_ctor(tsk_object_t * self, va_list * app)
{
	tmedia_qos_tline_e2e_t *e2e = self;
	if(e2e){
		e2e->recv.strength = e2e->send.strength = va_arg(*app, tmedia_qos_strength_t);
		TMEDIA_QOS_TLINE(e2e)->type = tmedia_qos_stype_e2e;
	}
	return self;
}

static tsk_object_t* tmedia_qos_tline_e2e_dtor(tsk_object_t * self)
{ 
	tmedia_qos_tline_e2e_t *e2e = self;
	if(e2e){
	}

	return self;
}

static const tsk_object_def_t tmedia_qos_tline_e2e_def_s = 
{
	sizeof(tmedia_qos_tline_e2e_t),
	tmedia_qos_tline_e2e_ctor, 
	tmedia_qos_tline_e2e_dtor,
	tsk_null, 
};
const tsk_object_def_t *tmedia_qos_tline_e2e_def_t = &tmedia_qos_tline_e2e_def_s;










/* ========================= Segmented ==================================*/

tmedia_qos_tline_segmented_t* tmedia_qos_tline_segmented_create(tmedia_qos_strength_t strength)
{
	return tsk_object_new(tmedia_qos_tline_segmented_def_t, strength);
}

tmedia_qos_tline_segmented_t* tmedia_qos_tline_segmented_from_sdp(const tsdp_header_M_t* m)
{
	tmedia_qos_tline_segmented_t* segmented = tsk_null;
	const tsdp_header_A_t* A;
	tsk_size_t i;

	char s0[10], s1[10], s2[10];

	if(!m){
		TSK_DEBUG_ERROR("invalid parameter");
		return tsk_null;
	}
	
	/*  Example
		a=curr:qos local none
		a=curr:qos remote none
		a=des:qos optional remote send
		a=des:qos none remote recv
		a=des:qos none local sendrecv
		a=conf:qos local recv
	*/
	segmented = tmedia_qos_tline_segmented_create(tmedia_qos_strength_unknown);

	/* curr */
	for(i = 0; (A = tsdp_header_M_findA_at(m, "curr", i)); i++){
		if(sscanf(A->value, "qos %10s %10s", s0, s1) != EOF){
			/* For segmented, s0 should be equal to "local" or "remote" */
			tmedia_qos_direction_t dir = tmedia_qos_direction_fromstring(s1);
			if(tsk_strequals(s0, "local")){
				/* local */
				switch(dir){
					case tmedia_qos_direction_send:
						segmented->local_send.current = tsk_true;
						break;
					case tmedia_qos_direction_recv:
						segmented->local_recv.current = tsk_true;
						break;
					case tmedia_qos_direction_sendrecv:
						segmented->local_send.current = tsk_true;
						segmented->local_recv.current = tsk_true;
						break;
					case tmedia_qos_direction_none:
						segmented->local_send.current = tsk_false;
						segmented->local_recv.current = tsk_false;
						break;
					default:
						break;
				}
			}
			else{
				/* remote */
				switch(dir){
					case tmedia_qos_direction_send:
						segmented->remote_send.current = tsk_true;
						break;
					case tmedia_qos_direction_recv:
						segmented->remote_recv.current = tsk_true;
						break;
					case tmedia_qos_direction_sendrecv:
						segmented->remote_send.current = tsk_true;
						segmented->remote_recv.current = tsk_true;
						break;
					case tmedia_qos_direction_none:
						segmented->remote_send.current = tsk_false;
						segmented->remote_recv.current = tsk_false;
						break;
					default:
						break;
				}
			}
		}
		else{
			TSK_DEBUG_ERROR("Failed to parse a=%s:%s", A->field, A->value);
		}
	}

	/* des */
	for(i = 0; (A = tsdp_header_M_findA_at(m, "des", i)); i++){
		if(sscanf(A->value, "qos %10s %10s %10s", s0, s1, s2) != EOF){
			/* For segmented, s1 should be equal to "local" or "remote" */
			tmedia_qos_strength_t strength = tmedia_qos_strength_fromstring(s0);
			tmedia_qos_direction_t dir = tmedia_qos_direction_fromstring(s2);
			if(tsk_strequals(s1, "local")){
				/* local */
				switch(dir){
					case tmedia_qos_direction_send:
						segmented->local_send.strength = strength;
						break;
					case tmedia_qos_direction_recv:
						segmented->local_recv.strength = strength;
						break;
					case tmedia_qos_direction_sendrecv:
						segmented->local_send.strength = strength;
						segmented->local_recv.strength = strength;
						break;
					default:
						break;
				}
			}
			else{
				/* remote */
				switch(dir){
					case tmedia_qos_direction_send:
						segmented->remote_send.strength = strength;
						break;
					case tmedia_qos_direction_recv:
						segmented->remote_recv.strength = strength;
						break;
					case tmedia_qos_direction_sendrecv:
						segmented->remote_send.strength = strength;
						segmented->remote_recv.strength = strength;
						break;
					default:
						break;
				}
			}
		}
		else{
			TSK_DEBUG_ERROR("Failed to parse a=%s:%s", A->field, A->value);
		}
	}

	/* conf */
	for(i = 0; (A = tsdp_header_M_findA_at(m, "conf", i)); i++){
		if(sscanf(A->value, "qos %10s %10s", s0, s1) != EOF){
			/* For segmented, s0 should be equal to "local" or "remote" */
			tmedia_qos_direction_t dir = tmedia_qos_direction_fromstring(s1);
			if(tsk_strequals(s0, "local")){
				/* local */
				switch(dir){
					case tmedia_qos_direction_send:
						segmented->local_send.confirm = tsk_true;
						break;
					case tmedia_qos_direction_recv:
						segmented->local_recv.confirm = tsk_true;
						break;
					case tmedia_qos_direction_sendrecv:
						segmented->local_send.confirm = tsk_true;
						segmented->local_recv.confirm = tsk_true;
						break;
					default:
						break;
				}
			}
			else{
				/* remote */
				switch(dir){
					case tmedia_qos_direction_send:
						segmented->remote_send.confirm = tsk_true;
						break;
					case tmedia_qos_direction_recv:
						segmented->remote_recv.confirm = tsk_true;
						break;
					case tmedia_qos_direction_sendrecv:
						segmented->remote_send.confirm = tsk_true;
						segmented->remote_recv.confirm = tsk_true;
						break;
					default:
						break;
				}
			}
		}
		else{
			TSK_DEBUG_ERROR("Failed to parse a=%s:%s", A->field, A->value);
		}
	}

	return segmented;
}

int tmedia_qos_tline_segmented_to_sdp(const tmedia_qos_tline_segmented_t* self, tsdp_header_M_t* m)
{
	/*	RFC 3312 - 5.1.1 SDP encoding
		
		For the segmented status type, the user agent MUST generate two
		current status lines: one with the tag "local" and the other with the
		tag "remote".  The user agent MUST add one or two desired status
		lines per segment (i.e., local and remote).  If, for a particular
		segment (local or remote), the tags for both directions in the
		transaction status table are equal (e.g., both "mandatory"), the user
		agent MUST add one desired status line with the tag "sendrecv".  If
		both tags are different, the user agent MUST include two desired
		status lines, one with the tag "send" and the other with the tag "recv".
	*/
	char* temp = tsk_null;

	if(!self || !m){
		TSK_DEBUG_ERROR("invalid parameter");
		return -1;
	}

	/*  Example
		a=curr:qos local none
		a=curr:qos remote none
		a=des:qos optional remote send
		a=des:qos none remote recv
		a=des:qos none local sendrecv
	*/

	/* curr:local */
	tsk_sprintf(&temp, "qos local %s", (self->local_recv.current && self->local_send.current) ? "sendrecv" : (self->local_recv.current ? "recv" : (self->local_send.current ? "send" : "none")));
		tsdp_header_M_add_headers(m,
			TSDP_HEADER_A_VA_ARGS("curr", temp),
			tsk_null);
		TSK_FREE(temp);

	/* curr:remote */
	tsk_sprintf(&temp, "qos remote %s", (self->remote_recv.current && self->remote_send.current) ? "sendrecv" : (self->remote_recv.current ? "recv" : (self->remote_send.current ? "send" : "none")));
	tsdp_header_M_add_headers(m,
		TSDP_HEADER_A_VA_ARGS("curr", temp),
		tsk_null);
	TSK_FREE(temp);

	
	/* des:local */
	if(self->local_recv.strength == self->local_send.strength){
		/* sendrecv */
		tsk_sprintf(&temp, "qos %s local sendrecv", tmedia_qos_strength_tostring(self->local_send.strength));
		tsdp_header_M_add_headers(m,
			TSDP_HEADER_A_VA_ARGS("des", temp),
			tsk_null);
		TSK_FREE(temp);
	}
	else{
		/* send */
		tsk_sprintf(&temp, "qos %s local send", tmedia_qos_strength_tostring(self->local_send.strength));
		tsdp_header_M_add_headers(m,
			TSDP_HEADER_A_VA_ARGS("des", temp),
			tsk_null);
		TSK_FREE(temp);

		/* recv */
		tsk_sprintf(&temp, "qos %s local recv", tmedia_qos_strength_tostring(self->local_recv.strength));
		tsdp_header_M_add_headers(m,
			TSDP_HEADER_A_VA_ARGS("des", temp),
			tsk_null);
		TSK_FREE(temp);
	}
	

	/* des:remote */
	if(self->remote_recv.strength == self->remote_send.strength){
		/* sendrecv */
		tsk_sprintf(&temp, "qos %s remote sendrecv", tmedia_qos_strength_tostring(self->remote_send.strength));
		tsdp_header_M_add_headers(m,
			TSDP_HEADER_A_VA_ARGS("des", temp),
			tsk_null);
		TSK_FREE(temp);
	}
	else{
		/* send */
		tsk_sprintf(&temp, "qos %s remote send", tmedia_qos_strength_tostring(self->remote_send.strength));
		tsdp_header_M_add_headers(m,
			TSDP_HEADER_A_VA_ARGS("des", temp),
			tsk_null);
		TSK_FREE(temp);

		/* recv */
		tsk_sprintf(&temp, "qos %s remote recv", tmedia_qos_strength_tostring(self->remote_recv.strength));
		tsdp_header_M_add_headers(m,
			TSDP_HEADER_A_VA_ARGS("des", temp),
			tsk_null);
		TSK_FREE(temp);
	}

	/* conf */
	if(self->remote_recv.confirm || self->remote_send.confirm){
		tsk_sprintf(&temp, "qos remote %s", (self->remote_recv.confirm && self->remote_send.confirm) ? "sendrecv" : (self->remote_recv.confirm ? "recv" : (self->remote_send.confirm ? "send" : "none")));
		tsdp_header_M_add_headers(m,
			TSDP_HEADER_A_VA_ARGS("conf", temp),
			tsk_null);
		TSK_FREE(temp);
	}

	return 0;
}

int tmedia_qos_tline_segmented_set_ro(tmedia_qos_tline_segmented_t* self, const tmedia_qos_tline_segmented_t* ro)
{
	if(!self || !ro){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	//////////////
	if(!ro->local_recv.current && !ro->remote_recv.confirm){
		/* request confirmation */
		self->remote_recv.confirm = tsk_true;
	}
	else{
		self->remote_recv.confirm = tsk_false;
		self->local_recv.current = tsk_true;
	}
	if(!ro->local_send.current && !ro->remote_send.confirm){
		/* request confirmation */
		self->remote_send.confirm = tsk_true;
	}
	else{
		self->remote_send.confirm = tsk_false;
		self->local_send.current = tsk_true;
	}

	//////////////
	if(ro->remote_recv.confirm){
		self->local_recv.current = tsk_true;
	}
	if(ro->remote_send.confirm){
		self->local_send.current = tsk_true;
	}

	//////////////
	if(ro->local_recv.current){
		self->remote_recv.current = tsk_true;
	}
	if(ro->local_send.current){
		self->remote_send.current = tsk_true;
	}

	return 0;
}

tsk_bool_t tmedia_qos_tline_segmented_canresume(const tmedia_qos_tline_segmented_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_true;
	}

	/* == Strength is mandatory == */
	if(self->local_recv.strength == tmedia_qos_strength_mandatory && !self->local_recv.current){
		return tsk_false;
	}
	
	if(self->local_send.strength == tmedia_qos_strength_mandatory && !self->local_send.current){
		return tsk_false;
	}

	if(self->remote_recv.strength == tmedia_qos_strength_mandatory && !self->remote_recv.current){
		return tsk_false;
	}

	if(self->remote_send.strength == tmedia_qos_strength_mandatory && !self->remote_send.current){
		return tsk_false;
	}

	/* "optinal" and "none" strengths */
	return tsk_true;
}

//
//	Segmented QoS line object definition
//
static tsk_object_t* tmedia_qos_tline_segmented_ctor(tsk_object_t * self, va_list * app)
{
	tmedia_qos_tline_segmented_t *segmented = self;
	if(segmented){
		segmented->local_recv.strength = segmented->local_send.strength
			= segmented->remote_recv.strength = segmented->remote_send.strength = va_arg(*app, tmedia_qos_strength_t);
		TMEDIA_QOS_TLINE(segmented)->type = tmedia_qos_stype_segmented;
	}
	return self;
}

static tsk_object_t* tmedia_qos_tline_segmented_dtor(tsk_object_t * self)
{ 
	tmedia_qos_tline_segmented_t *segmented = self;
	if(segmented){
	}

	return self;
}

static const tsk_object_def_t tmedia_qos_tline_segmented_def_s = 
{
	sizeof(tmedia_qos_tline_segmented_t),
	tmedia_qos_tline_segmented_ctor, 
	tmedia_qos_tline_segmented_dtor,
	tsk_null, 
};
const tsk_object_def_t *tmedia_qos_tline_segmented_def_t = &tmedia_qos_tline_segmented_def_s;