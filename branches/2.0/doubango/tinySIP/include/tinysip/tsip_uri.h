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

/**@file tsip_uri.h
 * @brief SIP/SIPS/TEL URI.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_URI_H
#define TINYSIP_URI_H

#include "tinysip_config.h"

#include "tsk_object.h"
#include "tsk_params.h"
#include "tsk_buffer.h"

TSIP_BEGIN_DECLS

#define TSIP_URI_IS_SECURE(uri)		((uri && uri->type==uri_sips) ? tsk_true : tsk_false)

typedef enum tsip_uri_type_e
{
	uri_unknown,
	uri_sip,
	uri_sips,
	uri_tel
}
tsip_uri_type_t;

typedef enum tsip_host_type_e
{
	host_unknown,
	host_hostname,
	host_ipv4,
	host_ipv6
}
tsip_host_type_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP/SIPS/TEL URI.
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_uri_s
{
	TSK_DECLARE_OBJECT;

	tsip_uri_type_t type;
	char *scheme;
	char *host; /**< Host name. Hostname or IPv4address or IPv6address. */
	tsip_host_type_t host_type; /**< IPv4 or IPv6 or domain name. */
	uint16_t port;
	char *user_name;
	char *password;
	char *display_name;

	tsk_params_L_t *params; /**< list of @ref tsk_param_t elements containing all parameters. */
}
tsip_uri_t;

typedef tsk_list_t tsip_uris_L_t;

TINYSIP_API tsip_uri_t* tsip_uri_create(tsip_uri_type_t type);
TINYSIP_API int tsip_uri_serialize(const tsip_uri_t *uri, tsk_bool_t with_params, tsk_bool_t quote, tsk_buffer_t *output);
TINYSIP_API char* tsip_uri_tostring(const tsip_uri_t *uri, tsk_bool_t with_params, tsk_bool_t quote);
TINYSIP_API tsip_uri_t *tsip_uri_clone(const tsip_uri_t *uri, tsk_bool_t with_params, tsk_bool_t quote);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_uri_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_URI_H */

