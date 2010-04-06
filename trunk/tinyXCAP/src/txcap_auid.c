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
/**@file txcap_auid.h
 * @brief XCAP AUID manager.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyXCAP/txcap_auid.h"










//=================================================================================================
//	AUID object definition
//
static tsk_object_t* txcap_auid_create(tsk_object_t * self, va_list * app)
{
	txcap_auid_t *auid = self;
	if(auid){
		string->value = tsk_strdup(value);
	}
	return self;
}

static tsk_object_t* txcap_auid_destroy(tsk_object_t * self)
{ 
	txcap_auid_t *string = self;
	if(string){
		TSK_FREE(string->value);
	}

	return self;
}

static int txcap_auid_cmp(const tsk_object_t *_s1, const tsk_object_t *_s2)
{
	const txcap_auid_t *s1 = _s1;
	const txcap_auid_t *s2 = _s2;

	if(s1 && s2){
		return tsk_stricmp(s1->value, s2->value);
	}
	else if(!s1 && !s2) return 0;
	else return -1;
}

static const tsk_object_def_t txcap_auid_def_s = 
{
	sizeof(txcap_auid_t),
	txcap_auid_create, 
	txcap_auid_destroy,
	txcap_auid_cmp, 
};
const tsk_object_def_t *txcap_auid_def_t = &txcap_auid_def_s;

