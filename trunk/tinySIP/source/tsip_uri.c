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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_uri.c
 * @brief SIP/SIPS/TEL URI.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/tsip_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_uri_group
*/













//========================================================
//	SIP/SIPS/TEL URI object definition
//

/**@ingroup tsip_uri_group
*/
static void* tsip_uri_create(void *self, va_list * app)
{
	tsip_uri_t *uri = self;
	if(uri)
	{
		uri->type = va_arg(*app, tsip_uri_type_t);
		uri->params = TSK_LIST_CREATE(); /* Empty list. */
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new SIP/SIPS/TEL.");
	}
	return self;
}

/**@ingroup tsip_uri_group
*/
static void* tsip_uri_destroy(void *self)
{
	tsip_uri_t *uri = self;
	if(uri)
	{
		TSK_FREE(uri->scheme);
		TSK_FREE(uri->host);
		TSK_FREE(uri->user_name);
		TSK_FREE(uri->password);
		TSK_FREE(uri->display_name);
		TSK_LIST_SAFE_FREE(uri->params);
	}
	else TSK_DEBUG_ERROR("Null SIP/SIPS/TEL URI.");

	return self;
}

static const tsk_object_def_t tsip_uri_def_s = 
{
	sizeof(tsip_uri_t),
	tsip_uri_create,
	tsip_uri_destroy,
	0
};
const void *tsip_uri_def_t = &tsip_uri_def_s;