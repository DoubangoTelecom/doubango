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

/**@file tsip_header_Event.h
 * @brief SIP header 'Event/o'as per RFC 3265.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_EVENT_H_
#define _TSIP_HEADER_EVENT_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

/**@def TSIP_HEADER_EVENT_CREATE
* Creates new sip 'Event' header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TSIP_HEADER_EVENT_VA_ARGS(package)		tsip_header_Event_def_t, (const char*)package
#define TSIP_HEADER_EVENT_CREATE(package)		tsk_object_new(TSIP_HEADER_EVENT_VA_ARGS(package))

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SIP header 'Event/o' .
/// @author	Mamadou
/// @date	12/3/2009
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

tsip_header_Event_t *tsip_header_Event_parse(const char *data, size_t size);

TINYSIP_GEXTERN const void *tsip_header_Event_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_EVENT_H_ */

