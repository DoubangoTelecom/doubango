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

/**@file tsip_header_Event.h
 * @brief SIP header 'Event/o'as per RFC 3265.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_EVENT_H_
#define _TSIP_HEADER_EVENT_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

#define TSIP_HEADER_EVENT_VA_ARGS(package)		tsip_header_Event_def_t, (const char*)package

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'Event/o' .
///
/// @par ABNF: Event / o
///	Event	= 	( "Event" / "o" ) HCOLON event-type *( SEMI event-param )
///	event-type	= 	event-package *( "." event-template )
///	event-package	= 	token-nodot
///	event-template	= 	token-nodot
///	token-nodot	= 	1*( alphanum / "-" / "!" / "%" / "*" / "_" / "+" / "`" / "'" / "~" )
///	event-param	= 	generic-param / ( "id" EQUAL token ) / call-ident / from-tag / to-tag / with-sessd
///	call-ident	= 	"call-id" EQUAL ( token / DQUOTE callid DQUOTE )
///	with-sessd	= 	"include-session-description"
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Event_s
{	
	TSIP_DECLARE_HEADER;

	char *package;
}
tsip_header_Event_t;

TINYSIP_API tsip_header_Event_t* tsip_header_Event_create(const char* package);

TINYSIP_API tsip_header_Event_t *tsip_header_Event_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_Event_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_EVENT_H_ */

