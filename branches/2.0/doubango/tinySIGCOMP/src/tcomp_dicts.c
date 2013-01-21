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

/**@file tcomp_dicts.c
 * @brief SigComp Dictionaries
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#include "tcomp_dicts.h"
#include "tcomp_buffer.h"

#include "tcomp_rfc3485_dictionary_sip.h"
#include "tcomp_rfc5112_dictionary_presence.h"

#include "tsk_debug.h"

/**Presence-Specific Static Dictionary for Signaling Compression as per RFC 5112.
*/
tcomp_dictionary_t* tcomp_dicts_create_presence_dict()
{
	static const char *pres_dict_data = RFC5112_DICTIONARY_PRESENCE_VALUE;
	static tcomp_dictionary_t * pres_dict = tsk_null;

	if(!pres_dict){
		pres_dict = tcomp_state_create(
			RFC5112_DICTIONARY_PRESENCE_VALUE_LENGTH, 
			RFC5112_DICTIONARY_PRESENCE_STATE_ADDRESS, 
			RFC5112_DICTIONARY_PRESENCE_STATE_INSTRUCTION,
			RFC5112_DICTIONARY_PRESENCE_MINIMUM_ACCESS_LENGTH,
			65535);

		if(pres_dict){
			tcomp_buffer_referenceBuff(pres_dict->value, (uint8_t*)pres_dict_data, RFC5112_DICTIONARY_PRESENCE_VALUE_LENGTH);
			tcomp_state_makeValid(pres_dict);
			TSK_DEBUG_INFO("SigComp - Presence dict State id=");
			tcomp_buffer_print(pres_dict->identifier);
		}
		else{
			TSK_DEBUG_ERROR("Failed to create Presence dictionary.");
		}
	}

	return tsk_object_ref(pres_dict);
}

/**The Session Initiation Protocol (SIP) and Session Description Protocol (SDP) Static Dictionary for Signaling Compression as per RFC 3485.
*/
tcomp_dictionary_t* tcomp_dicts_create_sip_dict()
{
	
	static const char *sip_dict_data = RFC3485_DICTIONARY_SIP_VALUE;
	static tcomp_dictionary_t *sip_dict = tsk_null;

	if(!sip_dict){
		sip_dict = tcomp_state_create(
			RFC3485_DICTIONARY_SIP_VALUE_LENGTH, 
			RFC3485_DICTIONARY_SIP_STATE_ADDRESS, 
			RFC3485_DICTIONARY_SIP_STATE_INSTRUCTION,
			RFC3485_DICTIONARY_SIP_MINIMUM_ACCESS_LENGTH,
			65535);
		
		if(sip_dict){
			tcomp_buffer_referenceBuff(sip_dict->value, (uint8_t*)sip_dict_data, RFC3485_DICTIONARY_SIP_VALUE_LENGTH);
			tcomp_state_makeValid(sip_dict);
			TSK_DEBUG_INFO("SigComp - SIP dict State id=");
			tcomp_buffer_print(sip_dict->identifier);
		}
		else{
			TSK_DEBUG_ERROR("Failed to create SIP/SDP dictionary.");
		}
	}

	return tsk_object_ref(sip_dict);
}

