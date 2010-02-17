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

/**@file thttp_url.h
 * @brief HTTP/HTTPS URL.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYHTTP_URL_H
#define TINYHTTP_URL_H

#include "tinyhttp_config.h"

#include "tsk_object.h"
#include "tsk_params.h"
#include "tsk_buffer.h"

THTTP_BEGIN_DECLS

/**@def THTTP_URL_CREATE
* Creates new http/https/tel url.  You must call @ref TSK_OBJECT_SAFE_FREE to free the url.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define THTTP_URL_CREATE(type)				tsk_object_new(thttp_url_def_t, (thttp_url_type_t) type)

#define THTTP_URL_IS_SECURE(url)		((url && url->type==url_https) ? 1 : 0)

typedef enum thttp_url_type_e
{
	url_unknown,
	url_http,
	url_https,
}
thttp_url_type_t;

typedef enum thttp_host_type_e
{
	host_unknown,
	host_hostname,
	host_ipv4,
	host_ipv6
}
thttp_host_type_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	thttp_url_t
///
/// @brief	HTTP/HTTPS URL.
///
/// @author	Mamadou
/// @date	12/6/2009
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct thttp_url_s
{
	TSK_DECLARE_OBJECT;

	thttp_url_type_t type;
	char *scheme;
	char *host; /**< Host name. Hostname or IPv4address or IPv6address. */
	thttp_host_type_t host_type; /**< IPv4 or IPv6 or domain name. */
	uint16_t port;
	char *password;
}
thttp_url_t;

TINYHTTP_API int thttp_url_serialize(const thttp_url_t *url, tsk_buffer_t *output);
TINYHTTP_API char* thttp_url_tostring(const thttp_url_t *url);
TINYHTTP_API thttp_url_t *thttp_url_clone(const thttp_url_t *url);

TINYHTTP_GEXTERN const void *thttp_url_def_t;

THTTP_END_DECLS

#endif /* TINYHTTP_URL_H */

