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
/**@file tsdp.h
 * @brief SDP (RFC 4566) implementations with both MMTel and PoC extensions.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSDP_TSDP_H
#define TINYSDP_TSDP_H

#include "tinysdp_config.h"

#include "tinySDP/tsdp_message.h"

TSDP_BEGIN_DECLS

#define TSDP_CTX_CREATE()	tsk_object_new(tsdp_ctx_def_t)

#define TSDP_LINE_S_VALUE_DEFAULT "-"	/* as per RFC 3264 subclause 5 */

#define TSDP_LINE_O_USERNAME_DEFAULT	"doubango"
#define TSDP_LINE_O_SESSION_VER_DEFAULT	2301
#define TSDP_LINE_O_SESSION_ID_DEFAULT	1983

typedef struct tsdp_ctx_s
{
	TSK_DECLARE_OBJECT;
	
	tsdp_caps_t* caps;
}
tsdp_ctx_t;

TINYSDP_API tsdp_message_t* tsdp_create_empty(const char* addr, tsk_bool_t ipv6);


TINYSDP_GEXTERN const void *tsdp_ctx_def_t;

TSDP_END_DECLS


#endif /* TINYSDP_TSDP_H */
