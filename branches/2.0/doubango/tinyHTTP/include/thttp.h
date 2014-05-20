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
/**@file thttp.h
 * @brief HTTP (RFC 2616) and HTTP basic/digest authetication (RFC 2617) implementations.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYHTTP_THTTP_H
#define TINYHTTP_THTTP_H

#include "tinyhttp_config.h"

#include "tinyhttp/thttp_event.h"
#include "tinyhttp/thttp_session.h"

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
*	THTTP_STACK_SET_TLS_CERTS("C:\\tls\\ca.pki-crt.pem", "C:\\tls\\pub-crt.pem", "C:\\tls\\priv-key.pem"),
*	THTTP_STACK_SET_NULL());
* @endcode
*/

THTTP_BEGIN_DECLS

typedef enum thttp_stack_param_type_e
{
	thttp_pname_null = 0,
#define THTTP_STACK_SET_NULL()																thttp_pname_null

	/* Network */
	thttp_pname_local_ip,
	thttp_pname_local_port,
	thttp_pname_proxy,
#define THTTP_STACK_SET_LOCAL_IP(IP_STR)														thttp_pname_local_ip, (const char*)IP_STR
#define THTTP_STACK_SET_LOCAL_PORT(PORT_INT)													thttp_pname_local_port, (int)PORT_INT
#define THTTP_STACK_SET_PROXY(IP_STR, PORT_INT)													thttp_pname_proxy, (const char*)IP_STR, (int)PORT_INT

	/* Modes */
	thttp_pname_mode_client,
	thttp_pname_mode_server,
#define THTTP_STACK_SET_MODE_CLIENT()												thttp_pname_mode_client
#define THTTP_STACK_SET_MODE_SERVER()												thttp_pname_mode_server

	/* TLS */
	thttp_pname_tls_enabled,
#define THTTP_STACK_SET_TLS_ENABLED(ENABLED_BOOL)									thttp_pname_tls_enabled, (tsk_bool_t)ENABLED_BOOL
	thttp_pname_tls_certs_verify,
#define THTTP_STACK_SET_TLS_CERTS_VERIFY(CERTS_VERIFY_BOOL)							thttp_pname_tls_certs_verify, (tsk_bool_t)CERTS_VERIFY_BOOL
	thttp_pname_tls_certs,
#define THTTP_STACK_SET_TLS_CERTS(CA_FILE_STR, PUB_FILE_STR, PRIV_FILE_STR)			thttp_pname_tls_certs, (const char*)CA_FILE_STR, (const char*)PUB_FILE_STR, (const char*)PRIV_FILE_STR

	/* User Data */
	thttp_pname_userdata,
#define THTTP_STACK_SET_USERDATA(USERDATA_PTR)	thttp_pname_userdata, (const void*)USERDATA_PTR

}
thttp_stack_param_type_t;

typedef enum thttp_stack_mode_e
{
	thttp_stack_mode_none,
	thttp_stack_mode_client = (0x01 << 0),
	thttp_stack_mode_server = (0x01 << 1),
	thttp_stack_mode_hybrid = (thttp_stack_mode_client | thttp_stack_mode_server)
}
thttp_stack_mode_t;

/** HTTP/HTTPS Stack.
*/
typedef struct thttp_stack_s
{
	TSK_DECLARE_OBJECT;

	tsk_bool_t started;
	enum thttp_stack_mode_e mode;
	thttp_stack_callback_f callback;

	/* Network */
	char* local_ip;
	int local_port;
	char* proxy_ip;
	int proxy_port;
	tnet_transport_t *transport;

	/* TLS */
	struct {
		tsk_bool_t enabled;
		tsk_bool_t verify;
		char* ca;
		char* pbk;
		char* pvk;
	}tls;

	thttp_sessions_L_t* sessions;
	
	const void* userdata;

	TSK_DECLARE_SAFEOBJ;
}
thttp_stack_t;

TINYHTTP_API thttp_stack_handle_t *thttp_stack_create(thttp_stack_callback_f callback, ...);
TINYHTTP_API int thttp_stack_start(thttp_stack_handle_t *self);
TINYHTTP_API int thttp_stack_set(thttp_stack_handle_t *self, ...);
TINYHTTP_API const void* thttp_stack_get_userdata(thttp_stack_handle_t *self);
TINYHTTP_API int thttp_stack_stop(thttp_stack_handle_t *self);

TINYHTTP_GEXTERN const tsk_object_def_t *thttp_stack_def_t;

THTTP_END_DECLS

#endif /* TINYHTTP_THTTP_H */
