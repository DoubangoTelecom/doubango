/*
 * Copyright (C) 2010-2015 Mamadou DIOP.
 * Copyright (C) 2015 Doubango Telecom.
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

#include "tinyhttp/thttp_proxy_node_plugin.h"
#include "tinyhttp/thttp_message.h"

#include "tinyhttp/parsers/thttp_parser_message.h"

#include "tinyhttp/auth/thttp_challenge.h"

#include "tinyhttp/headers/thttp_header_Authorization.h"
#include "tinyhttp/headers/thttp_header_WWW_Authenticate.h"
#include "tinyhttp/headers/thttp_header_Transfer_Encoding.h"
#include "tinyhttp/headers/thttp_header_Dummy.h"

#include "tnet_proxy_plugin.h"

#include "tsk_safeobj.h"
#include "tsk_buffer.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#define ONE_CRLF                "\r\n"
#define ONE_CRLF_LEN            2
#define TWO_CRLF                ONE_CRLF ONE_CRLF
#define TWO_CRLF_LEN            4
#define CONNECT_METHOD_NAME     "CONNECT"
#define END_OF_ADD_HEADERS()    tsk_null
#define BUILD_USING_PARSE       1

#if !defined(THTTP_PROXY_AUTH_CACHE)
#   define THTTP_PROXY_AUTH_CACHE         0
#endif

// Without closing CRLF
#define HTTP_CONNECT_FORMAT "CONNECT %s:%d HTTP/1.0\r\n" \
"User-Agent: Doubango Telecom\r\n"\
"Host: %s:%d\r\n" \
"Content-Length: 0\r\n"\
"Proxy-Connection: keep-Alive\r\n" \
"Pragma: no-cache\r\n"

typedef struct thttp_proxy_node_plugin_s {
    TNET_DECLARE_PROXY_NONE;

    struct {
        tsk_bool_t completed;
        tsk_bool_t started;
        void* pending_data_ptr;
        int pending_data_len;
        thttp_request_t *req_connect;
        tsk_buffer_t* buff;
        tsk_bool_t chanllenge_answered;
        thttp_challenge_t *challenge;
#if THTTP_PROXY_AUTH_CACHE
        struct thttp_proxy_auth_cache_s* proxy_auth_cache;
#endif
    } handshacking;

    TSK_DECLARE_SAFEOBJ;
}
thttp_proxy_node_plugin_t;

#if THTTP_PROXY_AUTH_CACHE

struct thttp_proxy_auth_cache_s;

static thttp_challenge_t* _thttp_proxy_auth_cache_challenge_get(struct thttp_proxy_auth_cache_s* self, const char* proxy_host, tnet_port_t proxy_port, const char* scheme, const char* realm, const char* nonce, const char* opaque, const char* algorithm, const char* qop);
static thttp_challenge_t* _thttp_proxy_auth_cache_challenge_get_2(struct thttp_proxy_auth_cache_s* self, const char* proxy_host, tnet_port_t proxy_port);
static thttp_header_t * _thttp_proxy_auth_cache_create_header_authorization(struct thttp_proxy_auth_cache_s* self, const char* proxy_host, tnet_port_t proxy_port, const char* username, const char* password, const char* uristring);
static struct thttp_proxy_auth_cache_s* _thttp_proxy_auth_cache_get_ref();
static int _thttp_proxy_auth_cache_lock(struct thttp_proxy_auth_cache_s* self);
static int _thttp_proxy_auth_cache_unlock(struct thttp_proxy_auth_cache_s* self);

#endif /* THTTP_PROXY_AUTH_CACHE */

static int _thttp_proxy_node_plugin_update_challenge(thttp_proxy_node_plugin_t *self, const thttp_response_t* response, tsk_bool_t answered);


static int _thttp_proxy_node_plugin_configure(tnet_proxy_node_t* self, ...)
{
    va_list ap;
    int ret = 0;
    thttp_proxy_node_plugin_t* node = (thttp_proxy_node_plugin_t*)self;
    // input parameters already checked by the caller

    va_start(ap, self);
    tsk_safeobj_lock(node);
    ret = tnet_proxy_node_configure_2(self, &ap);
    tsk_safeobj_unlock(node);
    va_end(ap);
    return ret;
}

static int _thttp_proxy_node_plugin_start_handshaking(tnet_proxy_node_t* self)
{
    thttp_proxy_node_plugin_t* node = (thttp_proxy_node_plugin_t*)self;
    int ret = 0;
#define kConnectContentData tsk_null
    // input parameters already checked by the caller

    if (tsk_strnullORempty(self->dst_host) || !self->dst_port) {
        TSK_DEBUG_ERROR("Invalid destination address for HTTP proxy node: %s:%d", self->dst_host, self->dst_port);
        return -1;
    }

    tsk_safeobj_lock(node);
    if (node->handshacking.started) {
        TSK_DEBUG_ERROR("handshaking already started");
        goto bail;
    }

    TSK_OBJECT_SAFE_FREE(node->handshacking.req_connect);
    TSK_FREE(node->handshacking.pending_data_ptr);
    node->handshacking.pending_data_len = 0;

    /* Build the HTTP(S) CONNECT Request */
#if BUILD_USING_PARSE
    {
        tsk_ragel_state_t state;
        thttp_header_t* auth_hdr = tsk_null;
        char* uristring = tsk_null;
        tsk_sprintf((char**)&node->handshacking.pending_data_ptr, HTTP_CONNECT_FORMAT, self->dst_host, self->dst_port, self->dst_host, self->dst_port);
#   if THTTP_PROXY_AUTH_CACHE
        tsk_sprintf(&uristring, "%s:%d", self->dst_host, self->dst_port);
        auth_hdr = _thttp_proxy_auth_cache_create_header_authorization(node->handshacking.proxy_auth_cache, self->proxy_host, self->proxy_port, self->login, self->password, uristring);
        TSK_FREE(uristring);
#   endif
        if (auth_hdr) {
            char *auth_hdr_string = tsk_null;
            if (auth_hdr && (auth_hdr_string = thttp_header_tostring(THTTP_HEADER(auth_hdr)))) {
                tsk_strcat((char**)&node->handshacking.pending_data_ptr, auth_hdr_string);
                TSK_FREE(auth_hdr_string);
            }
            TSK_OBJECT_SAFE_FREE(auth_hdr);
        }
        tsk_strcat((char**)&node->handshacking.pending_data_ptr, ONE_CRLF);

        node->handshacking.pending_data_len = (int)tsk_strlen(node->handshacking.pending_data_ptr);
        tsk_ragel_state_init(&state, node->handshacking.pending_data_ptr, node->handshacking.pending_data_len);
        if ((ret = thttp_message_parse(&state, &node->handshacking.req_connect, tsk_false/* do not extract the content */)) != 0) {
            TSK_DEBUG_ERROR("Failed to parse HTTP CONNECT message: %.*s", node->handshacking.pending_data_len, (const char*)node->handshacking.pending_data_ptr);
            TSK_FREE(node->handshacking.pending_data_ptr);
            node->handshacking.pending_data_len = 0;
            ret = -4;
            goto bail;
        }
    }
#else
    {
        thttp_url_t* url_connect = tsk_null;
        thttp_header_t* auth_hdr = tsk_null;
        char* uristring = tsk_null;
        if (!(url_connect = thttp_url_create(self->type ==  tnet_proxy_type_http ?  thttp_url_http : thttp_url_https))) {
            TSK_DEBUG_ERROR("Failed to create HTTP(S) URL");
            ret = -2;
            goto bail;
        }
        tsk_strupdate(&url_connect->host, self->dst_host);
        url_connect->port = self->dst_port;
        if (!(node->handshacking.req_connect = thttp_message_create(CONNECT_METHOD_NAME, url_connect))) {
            TSK_DEBUG_ERROR("Failed to create HTTP(S) message");
            TSK_OBJECT_SAFE_FREE(url_connect);
            ret = -3;
            goto bail;
        }
        TSK_OBJECT_SAFE_FREE(url_connect);
        ret = thttp_message_add_headers_2(node->handshacking.req_connect,
                                          THTTP_HEADER_DUMMY_VA_ARGS("User-Agent", "Doubango Telecom"),
                                          // Host header will be added by serialize()
                                          THTTP_HEADER_CONTENT_LENGTH_VA_ARGS(0),
                                          THTTP_HEADER_DUMMY_VA_ARGS("Proxy-Connection", "keep-Alive"),
                                          THTTP_HEADER_DUMMY_VA_ARGS("Connection", "keep-Alive"),
                                          THTTP_HEADER_DUMMY_VA_ARGS("Pragma", "no-cache"),
                                          END_OF_ADD_HEADERS()
                                         );
#   if THTTP_PROXY_AUTH_CACHE
        tsk_sprintf(&uristring, "%s:%d", self->dst_host, self->dst_port);
        auth_hdr = _thttp_proxy_auth_cache_create_header_authorization(node->handshacking.proxy_auth_cache, self->proxy_host, self->proxy_port, self->login, self->password, uristring);
        TSK_FREE(uristring);
#   endif
        if (auth_hdr) {
            thttp_message_add_header(node->handshacking.req_connect, auth_hdr);
            TSK_OBJECT_SAFE_FREE(auth_hdr);
        }
        if (ret != 0) {
            TSK_OBJECT_SAFE_FREE(node->handshacking.req_connect);
            TSK_DEBUG_ERROR("Failed to add HTTP(S) headers");
            goto bail;
        }
    }
#endif

    /* Parse the connect method */
    if (!node->handshacking.pending_data_ptr) { // see "BUILD_USING_PARSE"
        node->handshacking.pending_data_ptr = thttp_message_tostring(node->handshacking.req_connect);
        node->handshacking.pending_data_len = (int)tsk_strlen((const char*)node->handshacking.pending_data_ptr);
    }

    if (!(node->handshacking.started = (node->handshacking.pending_data_len > 0))) {
        TSK_DEBUG_ERROR("Failed to parse HTTP connect data");
        ret = -3;
        goto bail;
    }

bail:
    tsk_safeobj_unlock(node);
    return ret;
}

static int _thttp_proxy_node_plugin_set_handshaking_data(tnet_proxy_node_t* self, const void* data_ptr, tsk_size_t data_size)
{
    int ret = 0, endOfheaders = -1;
    tsk_ragel_state_t state;
    thttp_message_t *message = tsk_null;
    tsk_bool_t have_all_content = tsk_false;
    thttp_proxy_node_plugin_t* node = (thttp_proxy_node_plugin_t*)self;

    // input parameters already checked by the caller

    TSK_DEBUG_INFO("HTTP(s) incoming proxy handshaking data:%.*s", (int)data_size, data_ptr);

    tsk_safeobj_lock(node);

    if (!node->handshacking.started) {
        TSK_DEBUG_ERROR("handshaking not started");
        ret = -3;
        goto bail;
    }

    if (!node->handshacking.buff) {
        if (!(node->handshacking.buff = tsk_buffer_create(data_ptr, data_size))) {
            ret = -2;
            goto bail;
        }
    }
    else {
        if ((ret = tsk_buffer_append(node->handshacking.buff, data_ptr, data_size)) != 0) {
            goto bail;
        }
    }

    /* Check if we have all HTTP headers. */
parse_buffer:
    if ((endOfheaders = tsk_strindexOf(TSK_BUFFER_DATA(node->handshacking.buff), TSK_BUFFER_SIZE(node->handshacking.buff), TWO_CRLF)) < 0) {
        TSK_DEBUG_INFO("No all HTTP headers in the TCP buffer.");
        goto bail;
    }

    /* If we are here this mean that we have all HTTP headers.
     *	==> Parse the HTTP message without the content.
     */
    tsk_ragel_state_init(&state, TSK_BUFFER_DATA(node->handshacking.buff), endOfheaders + TWO_CRLF_LEN);
    if ((ret = thttp_message_parse(&state, &message, tsk_false/* do not extract the content */)) == 0) {
        const thttp_header_Transfer_Encoding_t* transfer_Encoding;

        /* chunked? */
        if ((transfer_Encoding = (const thttp_header_Transfer_Encoding_t*)thttp_message_get_header(message, thttp_htype_Transfer_Encoding)) && tsk_striequals(transfer_Encoding->encoding, "chunked")) {
            const char* start = (const char*)(TSK_BUFFER_TO_U8(node->handshacking.buff) + (endOfheaders + TWO_CRLF_LEN));
            const char* end = (const char*)(TSK_BUFFER_TO_U8(node->handshacking.buff) + TSK_BUFFER_SIZE(node->handshacking.buff));
            int index;

            TSK_DEBUG_INFO("CHUNKED transfer.");
            while (start < end) {
                /* RFC 2616 - 19.4.6 Introduction of Transfer-Encoding */
                // read chunk-size, chunk-extension (if any) and CRLF
                tsk_size_t chunk_size = (tsk_size_t)tsk_atox(start);
                if ((index = tsk_strindexOf(start, (tsk_size_t)(end-start), ONE_CRLF)) >=0) {
                    start += index + ONE_CRLF_LEN;
                }
                else {
                    TSK_DEBUG_ERROR("Parsing chunked data has failed.");
                    break;
                }

                if (chunk_size == 0 && ((start + 2) <= end) && *start == '\r' && *(start+ 1) == '\n') {
                    int parsed_len = (int)(start - (const char*)(TSK_BUFFER_TO_U8(node->handshacking.buff))) + TWO_CRLF_LEN;
                    tsk_buffer_remove(node->handshacking.buff, 0, parsed_len);
                    have_all_content = tsk_true;
                    break;
                }

                thttp_message_append_content(message, start, chunk_size);
                start += chunk_size + ONE_CRLF_LEN;
            }
        }
        else {
            tsk_size_t clen = THTTP_MESSAGE_CONTENT_LENGTH(message); /* MUST have content-length header. */
            if (clen == 0) { /* No content */
                tsk_buffer_remove(node->handshacking.buff, 0, (endOfheaders + TWO_CRLF_LEN)); /* Remove HTTP headers and CRLF ==> must never happen */
                have_all_content = tsk_true;
            }
            else { /* There is a content */
                if ((endOfheaders + TWO_CRLF_LEN + clen) > TSK_BUFFER_SIZE(node->handshacking.buff)) { /* There is content but not all the content. */
                    TSK_DEBUG_INFO("No all HTTP content in the TCP buffer.");
                    goto bail;
                }
                else {
                    /* Add the content to the message. */
                    thttp_message_add_content(message, tsk_null, TSK_BUFFER_TO_U8(node->handshacking.buff) + endOfheaders + TWO_CRLF_LEN, clen);
                    /* Remove HTTP headers, CRLF and the content. */
                    tsk_buffer_remove(node->handshacking.buff, 0, (endOfheaders + TWO_CRLF_LEN + clen));
                    have_all_content = tsk_true;
                }
            }
        }
    }

    /* Alert the dialog (FSM) */
    if (message) {
        if (have_all_content) { /* only if we have all data */
            // CHECK MESSAGE HERE
            if (THTTP_MESSAGE_IS_RESPONSE(message)) {
                if (THTTP_RESPONSE_IS_1XX(message)) {
                    // nothing to do...ignore provisional responses
                }
                else if (THTTP_RESPONSE_IS_2XX(message)) {
                    node->handshacking.completed = tsk_true;
                    ret = 0;
                }
                else if (message->line.response.status_code == 407 || message->line.response.status_code == 401) {
                    ret = _thttp_proxy_node_plugin_update_challenge(node, (const thttp_response_t*)message, node->handshacking.chanllenge_answered);
                    if (ret != 0) {
                        TSK_DEBUG_ERROR("Breaking HTTP(s) handshaking process (failed to update challenges)");
                        goto bail;
                    }
                    // Add creadentials to the CONNECT request
                    if (node->handshacking.req_connect && node->handshacking.challenge) {
                        thttp_header_t* auth_hdr;
#if BUILD_USING_PARSE
                        char* auth_hdr_string = tsk_null, *uristring = tsk_null;
#endif /* BUILD_USING_PARSE */
                        TSK_FREE(node->handshacking.pending_data_ptr);
                        node->handshacking.pending_data_len = 0;
#if BUILD_USING_PARSE
                        node->handshacking.pending_data_len = tsk_sprintf((char**)&node->handshacking.pending_data_ptr, HTTP_CONNECT_FORMAT, self->dst_host, self->dst_port, self->dst_host, self->dst_port);
#endif /* BUILD_USING_PARSE */
                        tsk_sprintf(&uristring, "%s:%d", self->dst_host, self->dst_port);
#   if THTTP_PROXY_AUTH_CACHE
                        auth_hdr = _thttp_proxy_auth_cache_create_header_authorization(node->handshacking.proxy_auth_cache, self->proxy_host, self->proxy_port, self->login, self->password, uristring);
#   else
                        auth_hdr = thttp_challenge_create_header_authorization_2(node->handshacking.challenge, self->login, self->password, "CONNECT", uristring, kConnectContentData);
#   endif /* THTTP_PROXY_AUTH_CACHE */
                        if (auth_hdr) {
#if BUILD_USING_PARSE
                            if ((auth_hdr_string = thttp_header_tostring(THTTP_HEADER(auth_hdr)))) {
                                tsk_strcat((char**)&node->handshacking.pending_data_ptr, auth_hdr_string);
                                TSK_FREE(auth_hdr_string);
                            }
#else
                            thttp_message_add_header(node->handshacking.req_connect, auth_hdr);
#endif /* BUILD_USING_PARSE */
                            tsk_object_unref(auth_hdr), auth_hdr = tsk_null;
                        }
                        TSK_FREE(uristring);

#if BUILD_USING_PARSE
                        tsk_strcat((char**)&node->handshacking.pending_data_ptr, ONE_CRLF);
#endif /* BUILD_USING_PARSE */

                        // update pending handshaking data
                        if (!node->handshacking.pending_data_ptr && !(node->handshacking.pending_data_ptr = thttp_message_tostring(node->handshacking.req_connect))) {
                            TSK_DEBUG_ERROR("Breaking HTTP(s) handshaking process (failed to parse request)");
                            ret = -4;
                            goto bail;
                        }
                        node->handshacking.pending_data_len = (int)tsk_strlen((const char*)node->handshacking.pending_data_ptr);
                        node->handshacking.chanllenge_answered = (node->handshacking.pending_data_len > 0);
                    }
                }
                else {
                    // any other error must break the handshaking process
                    TSK_DEBUG_ERROR("Breaking HTTP(s) handshaking process (status code = %hu)", message->line.response.status_code);
                    ret = -5;
                    goto bail;
                }
            }
            else {
                TSK_DEBUG_ERROR("Incoming HTTP request not expected from the proxy");
                ret = -4;
                goto bail;
            }

            /* Parse next chunck */
            if (TSK_BUFFER_SIZE(node->handshacking.buff) > 0) {
                TSK_OBJECT_SAFE_FREE(message);
                goto parse_buffer;
            }
        }
    }

bail:
    tsk_safeobj_unlock(node);
    TSK_OBJECT_SAFE_FREE(message);
    return ret;
}

static int _thttp_proxy_node_plugin_get_handshaking_pending_data(tnet_proxy_node_t* self, void** data_pptr, tsk_size_t* data_psize)
{
    thttp_proxy_node_plugin_t* node = (thttp_proxy_node_plugin_t*)self;
    int ret = -1;

    // input parameters already checked by the caller
    tsk_safeobj_lock(node);

    if (node->handshacking.pending_data_ptr && node->handshacking.pending_data_len > 0) {
        if ((*data_pptr = tsk_realloc(*data_pptr, node->handshacking.pending_data_len))) {
            memcpy(*data_pptr, node->handshacking.pending_data_ptr, (tsk_size_t) node->handshacking.pending_data_len);
            *data_psize = (tsk_size_t) node->handshacking.pending_data_len;
            ret = 0;
        }
        // reset the pending data. Up to the caller to hold the returned data and send it as many as required (e.g. when using unreliable transport)
        TSK_FREE(node->handshacking.pending_data_ptr);
        node->handshacking.pending_data_len = 0;
    }

    tsk_safeobj_unlock(node);
    return ret;
}

static int _thttp_proxy_node_plugin_get_handshaking_completed(tnet_proxy_node_t* self, tsk_bool_t* completed)
{
    thttp_proxy_node_plugin_t* node = (thttp_proxy_node_plugin_t*)self;

    tsk_safeobj_lock(node);
    *completed = node->handshacking.completed;
    tsk_safeobj_unlock(node);

    return 0;
}


static int _thttp_proxy_node_plugin_update_challenge(thttp_proxy_node_plugin_t *self, const thttp_response_t* response, tsk_bool_t answered)
{
#if THTTP_PROXY_AUTH_CACHE
    tnet_proxy_node_t *base = (tnet_proxy_node_t *)self;
#endif
    int ret = 0;
    const thttp_header_Proxy_Authenticate_t *Proxy_Authenticate;

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
#define kIsProxyYes tsk_true

    tsk_safeobj_lock(self);
#if THTTP_PROXY_AUTH_CACHE
    _thttp_proxy_auth_cache_lock(self->handshacking.proxy_auth_cache);
#endif

    /* RFC 2617 - Digest Operation

     *	(A) The client response to a WWW-Authenticate challenge for a protection
     space starts an authentication session with that protection space.
     The authentication session lasts until the client receives another
     WWW-Authenticate challenge from any server in the protection space.

     (B) The server may return a 401 response with a new nonce value, causing the client
     to retry the request; by specifying stale=TRUE with this response,
     the server tells the client to retry with the new nonce, but without
     prompting for a new username and password.
     */
    /* RFC 2617 - 1.2 Access Authentication Framework
     The realm directive (case-insensitive) is required for all authentication schemes that issue a challenge.
     */
    Proxy_Authenticate = (const thttp_header_Proxy_Authenticate_t*)thttp_message_get_headerAt(response, thttp_htype_Proxy_Authenticate, 0);
    if (Proxy_Authenticate) {
        if (self->handshacking.challenge) {
            if (tsk_striequals(self->handshacking.challenge->realm, Proxy_Authenticate->realm) && (Proxy_Authenticate->stale || !answered)) {
#if THTTP_PROXY_AUTH_CACHE
                TSK_OBJECT_SAFE_FREE(self->handshacking.challenge);
                self->handshacking.challenge = _thttp_proxy_auth_cache_challenge_get(self->handshacking.proxy_auth_cache,
                                               base->proxy_host,
                                               base->proxy_port,
                                               Proxy_Authenticate->scheme,
                                               Proxy_Authenticate->realm,
                                               Proxy_Authenticate->nonce,
                                               Proxy_Authenticate->opaque,
                                               Proxy_Authenticate->algorithm,
                                               Proxy_Authenticate->qop);
#else
                ret = thttp_challenge_update(self->handshacking.challenge,
                                             Proxy_Authenticate->scheme,
                                             Proxy_Authenticate->realm,
                                             Proxy_Authenticate->nonce,
                                             Proxy_Authenticate->opaque,
                                             Proxy_Authenticate->algorithm,
                                             Proxy_Authenticate->qop);
#endif
                if (ret != 0) {
                    goto bail;
                }
            }
            else {
                TSK_DEBUG_ERROR("Authentication failed");
                ret = -1;
                goto bail;
            }
        }
        else {
#if THTTP_PROXY_AUTH_CACHE
            self->handshacking.challenge = _thttp_proxy_auth_cache_challenge_get(self->handshacking.proxy_auth_cache,
                                           base->proxy_host,
                                           base->proxy_port,
                                           Proxy_Authenticate->scheme,
                                           Proxy_Authenticate->realm,
                                           Proxy_Authenticate->nonce,
                                           Proxy_Authenticate->opaque,
                                           Proxy_Authenticate->algorithm,
                                           Proxy_Authenticate->qop);
#else
            self->handshacking.challenge = thttp_challenge_create(kIsProxyYes,
                                           Proxy_Authenticate->scheme,
                                           Proxy_Authenticate->realm,
                                           Proxy_Authenticate->nonce,
                                           Proxy_Authenticate->opaque,
                                           Proxy_Authenticate->algorithm,
                                           Proxy_Authenticate->qop);
#endif /* THTTP_PROXY_AUTH_CACHE */
            if (!self->handshacking.challenge) {
                ret = -1;
                goto bail;
            }
        }
    }
    else  {
        TSK_DEBUG_ERROR("Proxy-Authenticate header is missing");
        ret = -1;
        goto bail;
    }

bail:
#if THTTP_PROXY_AUTH_CACHE
    _thttp_proxy_auth_cache_unlock(self->handshacking.proxy_auth_cache);
#endif
    tsk_safeobj_unlock(self);
    return ret;

}


/* constructor */
static tsk_object_t* thttp_proxy_node_plugin_ctor(tsk_object_t * self, va_list * app)
{
    thttp_proxy_node_plugin_t *node = self;
    if (node) {
        /* init base */
        tnet_proxy_node_init(TNET_PROXY_NODE(node));
        /* init self */
#if THTTP_PROXY_AUTH_CACHE
        node->handshacking.proxy_auth_cache = _thttp_proxy_auth_cache_get_ref();
#endif
        tsk_safeobj_init(node);
        TSK_DEBUG_INFO("Create HTTP(s) proxy node");
    }
    return self;
}
/* destructor */
static tsk_object_t* thttp_proxy_node_plugin_dtor(tsk_object_t * self)
{
    thttp_proxy_node_plugin_t *node = self;
    if (node) {
        /* deinit base */
        tnet_proxy_node_deinit(TNET_PROXY_NODE(node));
        /* deinit self */
        TSK_OBJECT_SAFE_FREE(node->handshacking.buff);
        TSK_FREE(node->handshacking.pending_data_ptr);
        TSK_OBJECT_SAFE_FREE(node->handshacking.req_connect);
        TSK_OBJECT_SAFE_FREE(node->handshacking.challenge);
#if THTTP_PROXY_AUTH_CACHE
        TSK_OBJECT_SAFE_FREE(node->handshacking.proxy_auth_cache);
#endif

        tsk_safeobj_deinit(node);

        TSK_DEBUG_INFO("*** HTTP proxy node destroyed ***");
    }
    return self;
}

/* object definition */
static const tsk_object_def_t thttp_proxy_node_def_s = {
    sizeof(thttp_proxy_node_plugin_t),
    thttp_proxy_node_plugin_ctor,
    thttp_proxy_node_plugin_dtor,
    tsk_null,
};
/* plugin definition*/
static const struct tnet_proxy_node_plugin_def_s thttp_proxy_node_plugin_def_s = {
    &thttp_proxy_node_def_s,

    tnet_proxy_type_http | tnet_proxy_type_https,

    "HTTP(s) proxy node plugin",

    _thttp_proxy_node_plugin_configure,
    _thttp_proxy_node_plugin_start_handshaking,
    _thttp_proxy_node_plugin_set_handshaking_data,
    _thttp_proxy_node_plugin_get_handshaking_pending_data,
    _thttp_proxy_node_plugin_get_handshaking_completed
};
const struct tnet_proxy_node_plugin_def_s *thttp_proxy_node_plugin_def_t = &thttp_proxy_node_plugin_def_s;



#if THTTP_PROXY_AUTH_CACHE

extern const tsk_object_def_t* thttp_proxy_auth_cache_def_t;
extern const tsk_object_def_t* thttp_proxy_auth_def_t;

typedef struct thttp_proxy_auth_s {
    TSK_DECLARE_OBJECT;

    char* hostname;
    tnet_port_t port;
    struct {
        char* scheme;
        char* realm;
        char* nonce;
        char* opaque;
        char* algorithm;
        char* qop;
        unsigned nc;
        tsk_md5string_t cnonce;
        tsk_bool_t is_active;
    } challenge;
}
thttp_proxy_auth_t;
typedef tsk_list_t thttp_proxy_auths_L_t;

typedef struct thttp_proxy_auth_cache_s {
    TSK_DECLARE_OBJECT;

    thttp_proxy_auths_L_t* auths_list;
    TSK_DECLARE_SAFEOBJ;
}
thttp_proxy_auth_cache_t;

static const thttp_proxy_auth_t* _thttp_proxy_auth_find(thttp_proxy_auths_L_t* list, const char* hostname, tnet_port_t port);
static const thttp_proxy_auth_t* _thttp_proxy_auth_add(thttp_proxy_auths_L_t* list, const char* hostname, tnet_port_t port);
static int _thttp_proxy_auth_set_nc(thttp_proxy_auth_t* self, unsigned nc);

static thttp_proxy_auth_cache_t* _thttp_proxy_auth_cache_get_ref()
{
    static thttp_proxy_auth_cache_t* instance = tsk_null;
    if (!instance) {
        instance = tsk_object_new(thttp_proxy_auth_cache_def_t);
        return instance; // to have refcount = 1
    }
    return (thttp_proxy_auth_cache_t*)tsk_object_ref(instance);
}

static int _thttp_proxy_auth_cache_lock(thttp_proxy_auth_cache_t* self)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter")
        return -1;
    }
    tsk_safeobj_lock(self);
    return 0;
}

static int _thttp_proxy_auth_cache_unlock(thttp_proxy_auth_cache_t* self)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter")
        return -1;
    }
    tsk_safeobj_unlock(self);
    return 0;
}

static thttp_challenge_t* _thttp_proxy_auth_cache_challenge_get(thttp_proxy_auth_cache_t* self, const char* proxy_host, tnet_port_t proxy_port, const char* scheme, const char* realm, const char* nonce, const char* opaque, const char* algorithm, const char* qop)
{
    if (self) {
        const thttp_proxy_auth_t* proxy_auth = _thttp_proxy_auth_add(self->auths_list, proxy_host, proxy_port);
        if (proxy_auth) {
            if (!proxy_auth->challenge.is_active || !tsk_striequals((proxy_auth)->challenge.nonce, nonce)) {
                tsk_istr_t istr;
                TSK_DEBUG_INFO("HTTP proxy auth cache (%s:%d) reset with nonce = %s", proxy_host, proxy_port, nonce);
                tsk_strupdate(&((thttp_proxy_auth_t*)proxy_auth)->challenge.scheme, scheme);
                tsk_strupdate(&((thttp_proxy_auth_t*)proxy_auth)->challenge.realm, realm);
                tsk_strupdate(&((thttp_proxy_auth_t*)proxy_auth)->challenge.nonce, nonce);
                tsk_strupdate(&((thttp_proxy_auth_t*)proxy_auth)->challenge.opaque, opaque);
                tsk_strupdate(&((thttp_proxy_auth_t*)proxy_auth)->challenge.algorithm, algorithm);
                tsk_strupdate(&((thttp_proxy_auth_t*)proxy_auth)->challenge.qop, qop);
                ((thttp_proxy_auth_t*)proxy_auth)->challenge.nc = 1;
                tsk_strrandom(&istr);
                tsk_md5compute(istr, tsk_strlen(istr), &((thttp_proxy_auth_t*)proxy_auth)->challenge.cnonce);

                ((thttp_proxy_auth_t*)proxy_auth)->challenge.is_active = tsk_true;
            }
        }
#define kIsProxyYes tsk_true
        return thttp_challenge_create(kIsProxyYes, scheme, realm, nonce, opaque, algorithm, qop);
    }
    return tsk_null;
}

static thttp_challenge_t* _thttp_proxy_auth_cache_challenge_get_2(thttp_proxy_auth_cache_t* self, const char* proxy_host, tnet_port_t proxy_port)
{
    if (self) {
        const thttp_proxy_auth_t* proxy_auth = _thttp_proxy_auth_find(self->auths_list, proxy_host, proxy_port);
        if (proxy_auth && proxy_auth->challenge.is_active) {
#define kIsProxyYes tsk_true
            return thttp_challenge_create(kIsProxyYes, proxy_auth->challenge.scheme, proxy_auth->challenge.realm, proxy_auth->challenge.nonce, proxy_auth->challenge.opaque, proxy_auth->challenge.algorithm, proxy_auth->challenge.qop);
        }
    }
    return tsk_null;
}

static thttp_header_t * _thttp_proxy_auth_cache_create_header_authorization(struct thttp_proxy_auth_cache_s* self, const char* proxy_host, tnet_port_t proxy_port, const char* username, const char* password, const char* uristring)
{
    thttp_challenge_t* challenge = tsk_null;
    thttp_header_t * header = tsk_null;

    if (!self || !uristring) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
    challenge = _thttp_proxy_auth_cache_challenge_get_2(self, proxy_host, proxy_port);
    if (challenge) {
#define kNullContentData tsk_null
        const thttp_proxy_auth_t* proxy_auth = _thttp_proxy_auth_find(self->auths_list, proxy_host, proxy_port);
        if (!proxy_auth) {
            TSK_DEBUG_ERROR("Cannot find cached info for proxy (%s: %d)", proxy_host, proxy_port);
        }
        else {
            challenge->nc = proxy_auth->challenge.nc;
            memcpy(challenge->cnonce, proxy_auth->challenge.cnonce, sizeof(tsk_md5string_t));
            header = thttp_challenge_create_header_authorization_2(challenge, username, password, "CONNECT", uristring, kNullContentData);
            if (header) {
                _thttp_proxy_auth_set_nc((thttp_proxy_auth_t*)proxy_auth, challenge->nc);
            }
        }
    }
    TSK_OBJECT_SAFE_FREE(challenge);
    return header;
}

static int _thttp_proxy_auth_set_nc(thttp_proxy_auth_t* self, unsigned nc)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->challenge.nc = nc;
    return 0;
}

static const thttp_proxy_auth_t* _thttp_proxy_auth_add(thttp_proxy_auths_L_t* list, const char* hostname, tnet_port_t port)
{
    const thttp_proxy_auth_t* old = _thttp_proxy_auth_find(list, hostname, port);
    if (!old) {
        thttp_proxy_auth_t* new = tsk_object_new(thttp_proxy_auth_def_t);
        if (new) {
            const thttp_proxy_auth_t* newptr = new; // saveptr
            new->hostname = tsk_strdup(hostname);
            new->port = port;
            tsk_list_push_back_data(list, (void**)&new);
            TSK_OBJECT_SAFE_FREE(new);
            return newptr;
        }
    }
    return old;
}

static const thttp_proxy_auth_t* _thttp_proxy_auth_find(thttp_proxy_auths_L_t* list, const char* hostname, tnet_port_t port)
{
    if (list) {
        const tsk_list_item_t* item;
        tsk_list_foreach(item, list) {
            const thttp_proxy_auth_t* auth = (thttp_proxy_auth_t*)item->data;
            if (auth->port == port && tsk_striequals(auth->hostname, hostname)) {
                return auth;
            }
        }
    }
    return tsk_null;
}

static const tsk_bool_t _thttp_proxy_auth_exists(thttp_proxy_auths_L_t* list, const char* hostname, tnet_port_t port)
{
    return _thttp_proxy_auth_find(list, hostname, port) ? tsk_true : tsk_false;
}

static tsk_object_t* _thttp_proxy_auth_cache_ctor(tsk_object_t * self, va_list * app)
{
    thttp_proxy_auth_cache_t *cache = (thttp_proxy_auth_cache_t *)self;
    if (cache) {
        if (!(cache->auths_list = tsk_list_create())) {
            return tsk_null;
        }
        tsk_safeobj_init(cache);
    }
    return self;
}
static tsk_object_t* _thttp_proxy_auth_cache_dtor(tsk_object_t * self)
{
    thttp_proxy_auth_cache_t *cache = (thttp_proxy_auth_cache_t *)self;
    if (cache) {
        TSK_OBJECT_SAFE_FREE(cache->auths_list);
        tsk_safeobj_deinit(cache);
    }

    return self;
}
static const tsk_object_def_t thttp_proxy_auth_cache_def_s = {
    sizeof(thttp_proxy_auth_cache_t),
    _thttp_proxy_auth_cache_ctor,
    _thttp_proxy_auth_cache_dtor,
    tsk_null,
};
const tsk_object_def_t* thttp_proxy_auth_cache_def_t = &thttp_proxy_auth_cache_def_s;





static tsk_object_t* _thttp_proxy_auth_ctor(tsk_object_t * self, va_list * app)
{
    thttp_proxy_auth_t *auth = (thttp_proxy_auth_t *)self;
    if (auth) {

    }
    return self;
}
static tsk_object_t* _thttp_proxy_auth_dtor(tsk_object_t * self)
{
    thttp_proxy_auth_t *auth = (thttp_proxy_auth_t *)self;
    if (auth) {
        TSK_FREE(auth->challenge.scheme);
        TSK_FREE(auth->challenge.realm);
        TSK_FREE(auth->challenge.nonce);
        TSK_FREE(auth->challenge.opaque);
        TSK_FREE(auth->challenge.algorithm);
        TSK_FREE(auth->challenge.qop);
        TSK_FREE(auth->hostname);
    }

    return self;
}
static const tsk_object_def_t thttp_proxy_auth_def_s = {
    sizeof(thttp_proxy_auth_t),
    _thttp_proxy_auth_ctor,
    _thttp_proxy_auth_dtor,
    tsk_null,
};
const tsk_object_def_t* thttp_proxy_auth_def_t = &thttp_proxy_auth_def_s;


#endif /* HTTP_AUTH_CACHE */
