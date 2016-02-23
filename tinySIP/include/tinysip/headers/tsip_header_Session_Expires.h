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

/**@file tsip_header_Session_Expires.h
 * @brief SIP header 'Session-Expires'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_SESSION_EXPIRES_H_
#define _TSIP_HEADER_SESSION_EXPIRES_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

#define TSIP_HEADER_SESSION_EXPIRES_VA_ARGS(delta_seconds, refresher_uas)		tsip_header_Session_Expires_def_t, (int64_t)delta_seconds, (tsk_bool_t)refresher_uas

#define TSIP_SESSION_EXPIRES_DEFAULT_VALUE					1800

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'Session-Expires'.
///
/// @par ABNF: Session-Expires	=  	 ( "Session-Expires" / "x" ) HCOLON delta-seconds *( SEMI (se-params )
/// se-params	= 	refresher-param / generic-param
/// refresher-param	= 	"refresher" EQUAL ("uas" / "uac")
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Session_Expires_s {
    TSIP_DECLARE_HEADER;

    int64_t delta_seconds;
    tsk_bool_t refresher_uas;
}
tsip_header_Session_Expires_t;

TINYSIP_API tsip_header_Session_Expires_t* tsip_header_Session_Expires_create(int64_t delta_seconds, tsk_bool_t refresher_uas);

TINYSIP_API tsip_header_Session_Expires_t *tsip_header_Session_Expires_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_Session_Expires_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_SESSION_EXPIRES_H_ */

