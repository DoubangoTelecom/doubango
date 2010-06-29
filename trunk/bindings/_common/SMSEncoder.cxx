/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@doubango.org>
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
#include "SMSEncoder.h"

#include "tinysip.h" /* SIP/IMS */
#include "tinysms.h" /* Binary SMS API*/

tsk_bool_t is_valid_telnum(const tsip_uri_t* uri);

SMSData* SMSEncoder::encode(const char *smsc, const char *destination, const char *asscii)
{
	SMSData* encodedData = tsk_null;
	
	


	return encodedData;
}


tsk_bool_t is_valid_telnum(const tsip_uri_t* uri)
{
	tsk_size_t i;
	tsk_size_t len;

	if(!uri || tsk_strnullORempty(uri->user_name)){
		return tsk_false;
	}

	for(i = 0, len = tsk_strlen(uri->user_name); i<len; i++){
		if(uri->user_name[i] != '+' && !isdigit(uri->user_name[i])){
			return tsk_false;
		}
	}
	return tsk_true;
}
