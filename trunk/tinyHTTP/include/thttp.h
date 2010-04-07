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
/**@file thttp.h
 * @brief HTTP (RFC 2616) and HTTP basic/digest authetication (RFC 2617) implementations.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYHTTP_THTTP_H
#define TINYHTTP_THTTP_H

#include "tinyHTTP_config.h"

#include "tinyHTTP/thttp_event.h"
#include "tinyHTTP/thttp_session.h"

#include "tnet_transport.h"

/**@def THTTP_STACK_SET_NULL()
* Ends stack parameters. Must always be the last one.
*/

/**@def THTTP_STACK_SET_LOCAL_IP(STR)
* Sets local IP address to bind to. By default, the stack will bind to "0.0.0.0" or "::" depending on
* whether IPv4 is used or not (IPv6). 
* This is a helper macro for @ref thttp_stack_create and @ref thttp_stack_set.
* @param IP_STR The IP address (const char*).
*
* @code
* thttp_stack_create(callback, 
*	THTTP_STACK_SET_LOCAL_IP("192.168.0.15"),
*	THTTP_STACK_SET_NULL());
* @endcode
*
* @sa @ref THTTP_STACK_SET_LOCAL_PORT<br>@ref thttp_stack_create<br>@ref thttp_stack_set
*/
/**@def THTTP_STACK_SET_LOCAL_PORT(PORT_INT)
* Sets local Port to bind to. By default, the stack will bind to a random port.
* This is a helper macro for @ref thttp_stack_create and @ref thttp_stack_set.
* @param PORT_INT The Port (int32_t).
*
* @code
* thttp_stack_create(callback, 
*	THTTP_STACK_SET_LOCAL_PORT(1234),
*	THTTP_STACK_SET_NULL());
* @endcode
* @sa @ref THTTP_STACK_SET_LOCAL_IP<br>@ref thttp_stack_create<br>@ref thttp_stack_set
*/

/**@def THTTP_STACK_SET_TLS_CERTS(CA_FILE_STR, PUB_FILE_STR, PRIV_FILE_STR)
* Sets TLS certificates (Mutual Authentication). Not mandatory.
* This is a helper macro for @ref thttp_stack_create and @ref thttp_stack_set.
* @param CA_FILE_STR Path to the Certification Authority File.
* @param PUB_FILE_STR Path to the Public key file.
* @param PRIV_FILE_STR Path to the Private key file.
*
* @code
* thttp_stack_create(callback, 
*	THTTP_STACK_SET_TLS_CERTS("C:\\tls\\ca.pki-crt.pem", "C:\\tls\\pub-crt.pem", "C:\\tls\\pub-key.pem"),
*	THTTP_STACK_SET_NULL());
* @endcode
*/

THTTP_BEGIN_DECLS

typedef enum thttp_stack_param_type_e
{
	pname_null = tsk_null,
#define THTTP_STACK_SET_NULL()																pname_null

	/* Network */
	pname_local_ip,
	pname_local_port,
#define THTTP_STACK_SET_LOCAL_IP(IP_STR)														pname_local_ip, (const char*)IP_STR
#define THTTP_STACK_SET_LOCAL_PORT(PORT_INT)													pname_local_port, (int)PORT_INT

	/* TLS */
	pname_tls_certs,
#define THTTP_STACK_SET_TLS_CERTS(CA_FILE_STR, PUB_FILE_STR, PRIV_FILE_STR)			pname_tls_certs, (const char*)CA_FILE_STR, (const char*)PUB_FILE_STR, (const char*)PRIV_FILE_STR

}
thttp_stack_param_type_t;

/** HTTP/HTTPS Stack.
*/
typedef struct thttp_stack_s
{
	TSK_DECLARE_OBJECT;

	thttp_stack_callback callback;

	/* Network */
	char* local_ip;
	int local_port;
	tnet_transport_t *transport;

	/* TLS */
	struct {
		char* ca;
		char* pbk;
		char* pvk;
	}tls;

	thttp_sessions_L_t* sessions;
	
	TSK_DECLARE_SAFEOBJ;
}
thttp_stack_t;

/** Pointer to a valid HTTP/HTTPS stack object.
*/
typedef void thttp_stack_handle_t;

TINYHTTP_API thttp_stack_handle_t *thttp_stack_create(thttp_stack_callback callback, ...);
TINYHTTP_API int thttp_stack_start(thttp_stack_handle_t *self);
TINYHTTP_API int thttp_stack_set(thttp_stack_handle_t *self, ...);
TINYHTTP_API int thttp_stack_stop(thttp_stack_handle_t *self);

TINYHTTP_GEXTERN const tsk_object_def_t *thttp_stack_def_t;

THTTP_END_DECLS

#endif /* TINYHTTP_THTTP_H */
