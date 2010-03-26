/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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
/**@file thttp.h
 * @brief HTTP (RFC 2616) and HTTP basic/digest authetication (RFC 2617) implementations.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYHTTP_THTTP_H
#define TINYHTTP_THTTP_H

#include "tinyhttp_config.h"

#include "tinyHTTP/thttp_event.h"
#include "tinyHTTP/thttp_operation.h"

THTTP_BEGIN_DECLS

#define THTTP_MAX_CONTENT_SIZE 0xFFFF

typedef enum thttp_stack_param_type_e
{
	/* Identity */
	pname_usr,
	pname_proxy,
	pname_useragent,
#define THTTP_STACK_SET_USER(USRNAME_STR, PASSWORD_STR)										pname_usr, (const char*)USRNAME_STR, (const char*)PASSWORD_STR
#define THTTP_STACK_SET_PROXY(HOST_STR, PORT_INT, USRNAME_STR, PASSWORD_STR)				pname_proxy, (const char*)HOST_STR, (int)PORT_INT, (const char*)USRNAME_STR, (const char*)PASSWORD_STR
#define THTTP_STACK_SET_USER_AGENT(UA_STR)													pname_useragent, (const char*)UA_STR

	/* Network */
	pname_local_ip,
	pname_local_port,
#define THTTP_STACK_SET_LOCAL_IP(STR)														pname_local_ip, (const char*)STR
#define THTTP_STACK_SET_LOCAL_PORT(INT)														pname_local_port, (int)INT


	pname_null,
#define THTTP_STACK_SET_NULL()																pname_null
}
thttp_stack_param_type_t;

typedef void thttp_stack_handle_t;

TINYHTTP_API int thttp_global_init();
TINYHTTP_API int thttp_global_deinit();


TINYHTTP_API thttp_stack_handle_t *thttp_stack_create(thttp_stack_callback callback, ...);
TINYHTTP_API int thttp_stack_start(thttp_stack_handle_t *self);
TINYHTTP_API int thttp_stack_set(thttp_stack_handle_t *self, ...);
TINYHTTP_API int thttp_stack_stop(thttp_stack_handle_t *self);

int thttp_stack_send(thttp_stack_handle_t *self, thttp_operation_handle_t* op, const struct thttp_message_s* message);
int thttp_stack_push_op(thttp_stack_handle_t *self, thttp_operation_handle_t* op);
int thttp_stack_pop_op(thttp_stack_handle_t *self, thttp_operation_handle_t* op);

TINYHTTP_GEXTERN const void *thttp_stack_def_t;

THTTP_END_DECLS

#endif /* TINYHTTP_THTTP_H */
