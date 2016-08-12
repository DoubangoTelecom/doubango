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

/**@file tsip_uri.c
 * @brief SIP/SIPS/TEL URI.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/tsip_uri.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_params.h"
#include "tsk_url.h"

#include <string.h>

/**@defgroup tsip_uri_group SIP/SIPS/TEL URI
*/


/**@ingroup tsip_uri_group
* Creates new empty SIP/SIPS/TEL URI. You should use @ref tsip_uri_parse() to create an URI from a buffer.
* @param type The type of the URI to create.
* @retval @ref tsip_uri_t* object if succeed and Null otherwise.
* @sa @ref tsip_uri_parse()
*/
tsip_uri_t* tsip_uri_create(tsip_uri_type_t type)
{
    return tsk_object_new(tsip_uri_def_t, type);
}

/* internal function used to serialize a SIP/SIPS/TEL URI */
int __tsip_uri_serialize(const tsip_uri_t *uri, tsk_bool_t with_params, tsk_buffer_t *output)
{
    tsk_istr_t port;

    if(uri->port) {
        tsk_itoa(uri->port, &port);
    }

    /* sip:alice:secretword@atlanta.com:65535 */
    tsk_buffer_append_2(output, "%s:%s%s%s%s%s%s%s%s%s",

                        uri->scheme ? uri->scheme : "sip", /* default scheme is sip: */

                        uri->user_name ? uri->user_name : "",

                        uri->password ? ":" : "",
                        uri->password ? uri->password : "",

                        uri->host ? (uri->user_name ? "@" : "") : "",
                            uri->host_type == host_ipv6 ? "[" : "",
                            uri->host ? uri->host : "",
                            uri->host_type == host_ipv6 ? "]" : "",

                            uri->port ? ":" : "",
                            uri->port ? port : ""
                           );

    /* Params */
    if(with_params && !TSK_LIST_IS_EMPTY(uri->params)) {
        tsk_buffer_append(output, ";", 1);
        tsk_params_tostring(uri->params, ';', output);
    }

    return 0;
}

/**@ingroup tsip_uri_group
* Serializes a SIP/SIPS/TEL URI to a string buffer.
* @param uri The URI to serialize.
* @param with_params Whether to serialize the parameters.
* @param quote Whether to add quotes("<" and ">").
* @param output Destination string buffer. Should be a valid buffer created using @a tsk_buffer_create() function.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tsip_uri_serialize(const tsip_uri_t *uri, tsk_bool_t with_params, tsk_bool_t quote, tsk_buffer_t *output)
{
    if(uri && output) {
        int ret = 0;
        if(quote) {
            if(uri->display_name) {
                tsk_buffer_append_2(output, "\"%s\"", uri->display_name);
            }

            tsk_buffer_append(output, "<", 1);
            ret = __tsip_uri_serialize(uri, with_params, output);
            tsk_buffer_append(output, ">", 1);
        }
        else {
            ret = __tsip_uri_serialize(uri, with_params, output);
        }
        return ret;
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
}

/**@ingroup tsip_uri_group
* Converts a SIP/SIPS/TEL URI object to a string.
* @param uri The URI to convert.
* @param with_params Whether to include the parameters.
* @param quote Whether to add quotes("<" and ">").
* @retval Pointer to a string if succeed and Null otherwise. It's up to the caller to free the returned string.
*/
char* tsip_uri_tostring(const tsip_uri_t *uri, tsk_bool_t with_params, tsk_bool_t quote)
{
    tsk_buffer_t *output = tsk_buffer_create_null();
    char* ret = 0;

    if(!tsip_uri_serialize(uri, with_params, quote, output)) {
        ret = tsk_strndup((const char*)output->data, output->size);
    }
    else {
        TSK_DEBUG_ERROR("Failed to serialize URI.");
    }

    TSK_OBJECT_SAFE_FREE(output);
    return ret;
}

/**@ingroup tsip_uri_group
* Clones a SIP/SIPS/TEL URI object.
* @param uri The URI to clone.
* @param with_params Whether to include the parameters.
* @param quote Whether to add quotes("<" and ">") and display name.
* @retval New URI object if succeed and Null otherwise.
*/
tsip_uri_t *tsip_uri_clone(const tsip_uri_t *uri, tsk_bool_t with_params, tsk_bool_t quote)
{
    tsip_uri_t *newuri = tsk_null;
	if (uri) {
		tsk_buffer_t *output = tsk_buffer_create_null();
		if((tsip_uri_serialize(uri, with_params, quote, output)) == 0) {
			if ((newuri = tsip_uri_parse(output->data, output->size))) {
				tsk_strupdate(&newuri->display_name, uri->display_name);
			}
		}
		TSK_OBJECT_SAFE_FREE(output);
	}

    return newuri;
}








//========================================================
//	SIP/SIPS/TEL URI object definition
//

static tsk_object_t* tsip_uri_ctor(tsk_object_t *self, va_list * app)
{
    tsip_uri_t *uri = self;
    if(uri) {
        uri->type = va_arg(*app, tsip_uri_type_t);
        uri->params = tsk_list_create(); /* Empty list. */
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new SIP/SIPS/TEL.");
    }
    return self;
}

static tsk_object_t* tsip_uri_dtor(tsk_object_t *self)
{
    tsip_uri_t *uri = self;
    if(uri) {
        TSK_FREE(uri->scheme);
        TSK_FREE(uri->host);
        TSK_FREE(uri->user_name);
        TSK_FREE(uri->password);
        TSK_FREE(uri->display_name);
        TSK_OBJECT_SAFE_FREE(uri->params);
    }
    else {
        TSK_DEBUG_ERROR("Null SIP/SIPS/TEL URI.");
    }

    return self;
}

int tsip_uri_strcmp(const char* s1, const char* s2, tsk_bool_t case_sensitive)
{
    if(s1 && s2) {
        tsk_bool_t s1_is_encoded = tsk_false;
        tsk_bool_t s2_is_encoded = tsk_false;
        char* s1_decoded = (char*)s1;
        char* s2_decoded = (char*)s2;
        int ret;

        if(tsk_strcontains(s1, tsk_strlen(s1), "%")) {
            s1_is_encoded = 1;
            s1_decoded = tsk_url_decode(s1);
        }
        if(tsk_strcontains(s2, tsk_strlen(s2), "%")) {
            s2_is_encoded = 1;
            s2_decoded = tsk_url_decode(s2);
        }

        ret = case_sensitive ? tsk_strcmp(s1_decoded, s2_decoded) : tsk_stricmp(s1_decoded, s2_decoded);
        if(s1_is_encoded) {
            TSK_FREE(s1_decoded);
        }
        if(s2_is_encoded) {
            TSK_FREE(s2_decoded);
        }
        return ret;
    }
    return case_sensitive ? tsk_strcmp(s1, s2) : tsk_stricmp(s1, s2);
}
#define tsip_uri_strequals(s1, s2) !tsip_uri_strcmp(s1, s2, tsk_true)
#define tsip_uri_striequals(s1, s2) !tsip_uri_strcmp(s1, s2, tsk_false)

// FIXME: tel uris are compared as per RFC 3966 section 4
static int tsip_uri_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
    if(obj1 && obj2) {
        const tsip_uri_t* uri1 = obj1;
        const tsip_uri_t* uri2 = obj2;
        const tsk_param_t* param1;
        const tsk_param_t* param2;
        const tsk_list_item_t *item;

        /* RFC 3261 - 19.1.4 URI Comparison

        	Comparison of the userinfo of SIP and SIPS URIs is case-sensitive.  This includes userinfo containing passwords or
        	formatted as telephone-subscribers.  Comparison of all other components of the URI is case-insensitive unless explicitly
        	defined otherwise.

        	An IP address that is the result of a DNS lookup of a host name does not match that host name.

        	For two URIs to be equal, the user, password, host, and port components must match.

        	A URI omitting the user component will not match a URI that includes one.  A URI omitting the password component will not
        	match a URI that includes one.

        	userinfo	= 	( user   /   telephone-subscriber )   [ ":" password ]   "@"
        */
        if(!tsk_strequals(uri1->scheme, uri2->scheme) ||
                !tsip_uri_strequals(uri1->user_name, uri2->user_name) ||
                !tsip_uri_strequals(uri1->host, uri2->host) ||
                !tsk_strequals(uri1->password, uri2->password) ||
                uri1->port != uri2->port) {
            return -2;
        }

        /* Is there parameters */
        if((!uri1->params && !uri2->params) || (TSK_LIST_IS_EMPTY(uri1->params) && TSK_LIST_IS_EMPTY(uri2->params))) {
            return 0;
        }

        /*	RFC 3261 - 19.1.4 URI Comparison

        	A URI omitting any component with a default value will not match a URI explicitly containing that component with its
        	default value.  For instance, a URI omitting the optional port component will not match a URI explicitly declaring port 5060.
        	The same is true for the transport-parameter, ttl-parameter, user-parameter, and method components.

        	-  A user, ttl, or method uri-parameter appearing in only one URI never matches, even if it contains the default value.
        	-  A URI that includes an maddr parameter will not match a URI that contains no maddr parameter.
        */
#define TSIP_URI_CMP_PARAMETER(pname) \
		param1 = tsk_params_get_param_by_name(uri1->params, pname);\
		param2 = tsk_params_get_param_by_name(uri2->params, pname);\
		if((param1 || param2) && ((param1 && !param2) || (!param1 && param2) || (!tsip_uri_striequals(param1->value, param2->value)))){\
			return -3;\
		}
        TSIP_URI_CMP_PARAMETER("transport");
        TSIP_URI_CMP_PARAMETER("ttl");
        TSIP_URI_CMP_PARAMETER("user");
        TSIP_URI_CMP_PARAMETER("method");
        TSIP_URI_CMP_PARAMETER("maddr");

        /*	RFC 3261 - 19.1.4 URI Comparison

        	URI uri-parameter components are compared as follows:

        	1 -  Any uri-parameter appearing in both URIs must match.
        	2 -  All other uri-parameters appearing in only one URI are ignored when comparing the URIs.

        	o  URI header components are never ignored.  Any present header component MUST be present in both URIs and match for the URIs
        	to match.  The matching rules are defined for each header field in Section 20.
        */
        tsk_list_foreach(item, uri1->params) {
            param1 = item->data;
            if((param2 = tsk_params_get_param_by_name(uri2->params, param1->name))) {
                if(!tsip_uri_striequals(param1->value, param2->value)) {
                    return -4;
                }
            }
        }
        tsk_list_foreach(item, uri2->params) {
            param2 = item->data;
            if((param1 = tsk_params_get_param_by_name(uri1->params, param2->name))) {
                if(!tsk_striequals(param1->value, param2->value)) {
                    return -4;
                }
            }
        }

        return 0;
    }
    else {
        return (!obj1 && !obj2) ? 0 : -1;
    }
}

static const tsk_object_def_t tsip_uri_def_s = {
    sizeof(tsip_uri_t),
    tsip_uri_ctor,
    tsip_uri_dtor,
    tsip_uri_cmp
};
const tsk_object_def_t *tsip_uri_def_t = &tsip_uri_def_s;
