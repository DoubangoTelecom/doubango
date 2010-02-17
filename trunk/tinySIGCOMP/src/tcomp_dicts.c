/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tcomp_dicts.h"
#include "tcomp_buffer.h"

#include "tcomp_rfc3485_dictionary_sip.h"
#include "tcomp_rfc5112_dictionary_presence.h"

#include "tsk_debug.h"

/**@defgroup tcomp_dicts_group SIGCOMP dictionaries.
*/

/**@ingroup tcomp_dicts_group
*/
tcomp_dictionary_t* tcomp_dicts_create_presence_dict()
{
	static const char *pres_dict_data = RFC5112_DICTIONARY_PRESENCE_VALUE;
	static tcomp_dictionary_t * pres_dict = 0;

	if(!pres_dict)
	{
		pres_dict = TCOMP_STATE_CREATE(
			RFC5112_DICTIONARY_PRESENCE_VALUE_LENGTH, 
			RFC5112_DICTIONARY_PRESENCE_STATE_ADDRESS, 
			RFC5112_DICTIONARY_PRESENCE_STATE_INSTRUCTION,
			RFC5112_DICTIONARY_PRESENCE_MINIMUM_ACCESS_LENGTH,
			65535);

		if(pres_dict)
		{
			tcomp_buffer_referenceBuff(pres_dict->value, (uint8_t*)pres_dict_data, RFC5112_DICTIONARY_PRESENCE_VALUE_LENGTH);
			tcomp_state_makeValid(pres_dict);
		}
		else
		{
			TSK_DEBUG_ERROR("Failed to create Presence dictionary.");
		}
	}

	return pres_dict;
}

/**@ingroup tcomp_dicts_group
*/
tcomp_dictionary_t* tcomp_dicts_create_sip_dict()
{
	
	static const char *sip_dict_data = RFC3485_DICTIONARY_SIP_VALUE;
	static tcomp_dictionary_t *sip_dict = 0;

	if(!sip_dict)
	{
		sip_dict = TCOMP_STATE_CREATE(
			RFC3485_DICTIONARY_SIP_VALUE_LENGTH, 
			RFC3485_DICTIONARY_SIP_STATE_ADDRESS, 
			RFC3485_DICTIONARY_SIP_STATE_INSTRUCTION,
			RFC3485_DICTIONARY_SIP_MINIMUM_ACCESS_LENGTH,
			65535);
		
		if(sip_dict)
		{
			tcomp_buffer_referenceBuff(sip_dict->value, (uint8_t*)sip_dict_data, RFC3485_DICTIONARY_SIP_VALUE_LENGTH);
			tcomp_state_makeValid(sip_dict);
		}
		else
		{
			TSK_DEBUG_ERROR("Failed to create SIP/SDP dictionary.");
		}
	}

	return sip_dict;
}

