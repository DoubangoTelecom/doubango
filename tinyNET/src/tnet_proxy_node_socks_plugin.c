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

// http://en.wikipedia.org/wiki/SOCKS
// SOCKS Protocol Version 5: http://tools.ietf.org/html/rfc1928
// Username/Password Authentication for SOCKS V5: https://www.ietf.org/rfc/rfc1929.txt
// GSS-API Authentication Method for SOCKS Version 5: https://tools.ietf.org/html/rfc1961
// SSPI/Kerberos Interoperability with GSSAPI: https://msdn.microsoft.com/en-us/library/ms995352.aspx
// Mozilla issue: https://bugzilla.mozilla.org/show_bug.cgi?id=122752

#include "tnet_proxy_node_socks_plugin.h"
#include "tnet_proxy_plugin.h"
#include "tnet_socket.h"
#include "tnet_utils.h"
#include "tnet_endianness.h"

#include "tsk_safeobj.h"
#include "tsk_buffer.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#if TNET_UNDER_APPLE
#   if !defined(USING_CFSTREAM)
#       define USING_CFSTREAM 0 // Works with HTTP streams only :(
#   endif /* USING_CFSTREAM */
#endif /* TNET_UNDER_APPLE */

#if USING_CFSTREAM
#   import <CFNetwork/CFNetwork.h>
#   import <SystemConfiguration/SystemConfiguration.h>
#endif /* TNET_UNDER_APPLE */

#if HAVE_GSSAPI_H
#   if TNET_UNDER_APPLE
#       include <GSS/gssapi.h>
#   else
#       include <gssapi.h>
#   endif
#endif /* HAVE_GSSAPI_H */

#define kSocks4StatusGranted 0x5a

#if !defined(TNET_SOCKS5_HAVE_AUTH_NONE)
#   define TNET_SOCKS5_HAVE_AUTH_NONE 1
#endif /* TNET_SOCKS5_HAVE_AUTH_NONE */

#if !defined(TNET_SOCKS5_HAVE_AUTH_USRPWD)
#   define TNET_SOCKS5_HAVE_AUTH_USRPWD 1
#endif /* TNET_SOCKS5_HAVE_AUTH_USRPWD */

#if !defined(TNET_SOCKS5_HAVE_AUTH_GSSAPI)
#   if HAVE_GSSAPI_H // TODO: Add Microsoft SSPI
#       define TNET_SOCKS5_HAVE_AUTH_GSSAPI 0
#   endif /* HAVE_GSSAPI_H */
#endif /* TNET_SOCKS5_HAVE_AUTH_GSSAPI */

#if !TNET_SOCKS5_HAVE_AUTH_NONE && !TNET_SOCKS5_HAVE_AUTH_USRPWD && !TNET_SOCKS5_HAVE_AUTH_GSSAPI
#   error "At least one Socks5 authentication method must be enabled"
#endif

#define kSocks5AuthMethodNone   0x00 // NO AUTHENTICATION REQUIRED
#define kSocks5AuthMethodGSSAPI 0x01 // GSSAPI
#define kSocks5AuthMethodUsrPwd 0x02 // USERNAME/PASSWORD

typedef int socks5_auth_method_t;

typedef enum socks5_state_e {
    socks5_state_none,
    socks5_state_greeting,
    socks5_state_auth_req,
    socks5_state_conn_req,
    socks5_state_conn_accept,

    socks5_state_error
}
socks5_state_t;

typedef struct tnet_proxy_node_socks_plugin_s {
    TNET_DECLARE_PROXY_NONE;

    struct {
        tsk_bool_t completed;
        tsk_bool_t started;
        tsk_buffer_t* buff;
        uint8_t* pending_data_ptr;
        tsk_size_t pending_data_len;

        socks5_state_t socks5_state;
        socks5_auth_method_t socks5_auth_method;
#if TNET_SOCKS5_HAVE_AUTH_GSSAPI
        struct {
            gss_ctx_id_t ctx;
            OM_uint32 status_minor;
            OM_uint32 status_major;
            gss_name_t server_name;
            tsk_bool_t init_sec_complete;
        } gss;
#endif /* TNET_SOCKS5_HAVE_AUTH_GSSAPI */
    } handshacking;

    TSK_DECLARE_SAFEOBJ;
}
tnet_proxy_node_socks_plugin_t;

static const char* __socks5_state_to_string(socks5_state_t state);
static const char* __socks5_method_to_string(socks5_auth_method_t method);
#if TNET_SOCKS5_HAVE_AUTH_GSSAPI
static int __socks5_gss_import_name(tnet_proxy_node_t* self);
static int __socks5_gss_init_sec_context(tnet_proxy_node_t* self);
static void __socks_gss_print_error(const char* info, OM_uint32 status_major, OM_uint32 status_minor);
#endif /* TNET_SOCKS5_HAVE_AUTH_GSSAPI */

static int _tnet_proxy_node_socks_plugin_configure(tnet_proxy_node_t* self, ...)
{
    tnet_proxy_node_socks_plugin_t* node = (tnet_proxy_node_socks_plugin_t*)self;
    va_list ap;
    int ret = 0;

    // input parameters already checked by the caller

    tsk_safeobj_lock(node);

    // extract dst_host, dst_port, proxy_host, proxy_port, ipv6_enabled, ...
    va_start(ap, self);
    ret = tnet_proxy_node_configure_2(self, &ap);
    va_end(ap);

    tsk_safeobj_unlock(node);

    return 0;
}

static int _tnet_proxy_node_socks_plugin_start_handshaking(tnet_proxy_node_t* self)
{
    tnet_proxy_node_socks_plugin_t* node = (tnet_proxy_node_socks_plugin_t*)self;
    int ret = 0;

    // input parameters already checked by the caller

    tsk_safeobj_lock(node);
    if (node->handshacking.started) {
        TSK_DEBUG_ERROR("handshaking already started");
        goto bail;
    }
    if (tsk_strnullORempty(self->dst_host) || !self->dst_port) {
        TSK_DEBUG_ERROR("Invalid proxy host and/or port for socks server %s:%hu", self->proxy_host, self->proxy_port);
        ret = -2;
        goto bail;
    }

    // reset pending data
    TSK_FREE(node->handshacking.pending_data_ptr);
    node->handshacking.pending_data_len = 0;

#if USING_CFSTREAM
    if (tsk_strnullORempty(self->proxy_host) || !self->proxy_port) {
        TSK_DEBUG_ERROR("Invalid proxy host and/or port for socks server %s:%hu", self->proxy_host, self->proxy_port);
        ret = -2;
        goto bail;
    }
    if (!self->cf_read_stream || !self->cf_write_stream) {
        TSK_DEBUG_ERROR("Invalid CFStreams: read=%d write=%d", !!self->cf_read_stream, !!self->cf_write_stream);
        ret = -3;
        goto bail;
    }

    CFStringRef cfstrHost = CFStringCreateWithCStringNoCopy(kCFAllocatorDefault, self->proxy_host, kCFStringEncodingUTF8, NULL);
    int intPort = (int)self->proxy_port;
    CFNumberRef cfintPort = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &intPort);
    CFDictionaryRef proxyDict = CFNetworkCopySystemProxySettings();
    CFMutableDictionaryRef socksConfig = CFDictionaryCreateMutableCopy(kCFAllocatorDefault, 0, proxyDict);
    CFDictionarySetValue(socksConfig, kCFStreamPropertySOCKSProxyHost, cfstrHost);
    CFDictionarySetValue(socksConfig, kCFStreamPropertySOCKSProxyPort, cfintPort);
    CFDictionarySetValue(socksConfig, kCFStreamPropertySOCKSVersion, self->type == tnet_proxy_type_socks4 ? kCFStreamSocketSOCKSVersion4 : kCFStreamSocketSOCKSVersion5);
    if (!tsk_strnullORempty(self->login)) {
        CFStringRef cfstrLogin = CFStringCreateWithCStringNoCopy(kCFAllocatorDefault, self->login, kCFStringEncodingUTF8, NULL);
        CFDictionarySetValue(socksConfig, kCFStreamPropertySOCKSUser, cfstrLogin);
        CFRelease(cfstrLogin);
    }
    if (!tsk_strnullORempty(self->password)) {
        CFStringRef cfstrPassword = CFStringCreateWithCStringNoCopy(kCFAllocatorDefault, self->password, kCFStringEncodingUTF8, NULL);
        CFDictionarySetValue(socksConfig, kCFStreamPropertySOCKSPassword, cfstrPassword);
        CFRelease(cfstrPassword);
    }

    if (!CFReadStreamSetProperty(self->cf_read_stream, kCFStreamPropertySOCKSProxy, socksConfig)) {
        CFStreamError error = CFReadStreamGetError(self->cf_read_stream);

        TSK_DEBUG_INFO("CFReadStreamSetProperty(kCFStreamPropertySOCKSProxy) failed code=%d, domain=%ld", (int)error.error, error.domain);
        ret = -4;
    }
    else if (!CFWriteStreamSetProperty(self->cf_write_stream, kCFStreamPropertySOCKSProxy, socksConfig)) {
        CFStreamError error = CFWriteStreamGetError(self->cf_write_stream);
        TSK_DEBUG_INFO("CFWriteStreamSetProperty(kCFStreamPropertySOCKSProxy)  code=%d, domain=%ld", (int)error.error, error.domain);
        ret = -5;
    }

    CFRelease(cfstrHost);
    CFRelease(cfintPort);
    CFRelease(socksConfig);
    CFRelease(proxyDict);

    node->handshacking.started = (ret == 0);
    node->handshacking.completed = node->handshacking.started; // no handshaking data to send, up to the system
#else
    {
        tsk_size_t size_to_reserve, userid_len = tsk_strlen(self->login), domain_len = 0;
        if (self->type == tnet_proxy_type_socks4 || self->type == tnet_proxy_type_socks4a) {
            size_to_reserve = 1 /* version number */
                              + 1 /* command code */
                              + 2 /* network byte order port number */
                              + 4 /* network byte order IP address */
                              + userid_len + 1 /* the user ID string, variable length, terminated with a null (0x00) */
                              ;
            if (self->type == tnet_proxy_type_socks4a) {
                domain_len = tsk_strlen(self->dst_host);
                size_to_reserve +=  domain_len + 1/* the domain name of the host we want to contact, variable length, terminated with a null (0x00) */
                                    ;
            }
        }
        else { // SOCKS5
            if (node->handshacking.socks5_state != socks5_state_none) {
                TSK_DEBUG_ERROR("Socks5 handshaking state mut start at none");
                ret = -6;
                goto bail;
            }
            // Greeting
            size_to_reserve = 1 /* version number */
                              + 1 /* number of authentication methods supported */
#if TNET_SOCKS5_HAVE_AUTH_NONE
                              + 1
#endif
#if TNET_SOCKS5_HAVE_AUTH_USRPWD
                              + 1
#endif
#if TNET_SOCKS5_HAVE_AUTH_GSSAPI
                              + 1
#endif
                              ;
        }
        node->handshacking.pending_data_ptr = (uint8_t*)tsk_realloc(node->handshacking.pending_data_ptr, size_to_reserve);
        if (!node->handshacking.pending_data_ptr) {
            TSK_DEBUG_ERROR("Failed to allocate buffer with size = %u", (unsigned)size_to_reserve);
            node->handshacking.pending_data_len = 0;
            ret = -5;
            goto bail;
        }
        node->handshacking.pending_data_len = size_to_reserve;

        if (self->type == tnet_proxy_type_socks4 || self->type == tnet_proxy_type_socks4a) {
            node->handshacking.pending_data_ptr[0] = 0x04; // version number
            node->handshacking.pending_data_ptr[1] = 0x01; // establish a TCP/IP stream connection (caller aldready check we're dealing with Streams)
            node->handshacking.pending_data_ptr[2] = (self->dst_port >> 8) & 0xFF; // port, first byte
            node->handshacking.pending_data_ptr[3] = self->dst_port & 0xFF; // port, second byte
            if (self->type == tnet_proxy_type_socks4) {
                struct sockaddr_storage addr;
                // network byte order IPv4 address (SOCKS4 doesn't support hostnames)
                if ((ret = tnet_sockaddr_init(self->dst_host, self->dst_port, self->socket.type, &addr)) != 0 || addr.ss_family != AF_INET) {
                    TSK_DEBUG_ERROR("tnet_sockaddr_init(%s, %d, %d) failed", self->dst_host, self->dst_port, self->socket.type);
                    ret = -6;
                    goto bail;
                }
                memcpy(&node->handshacking.pending_data_ptr[4], (void*)&((const struct sockaddr_in*)&addr)->sin_addr.s_addr, 4);
            }
            else {
                // deliberate invalid IP address, 4 bytes, first three must be 0x00 and the last one must not be 0x00
                node->handshacking.pending_data_ptr[4] = 0x00;
                node->handshacking.pending_data_ptr[5] = 0x00;
                node->handshacking.pending_data_ptr[6] = 0x00;
                node->handshacking.pending_data_ptr[7] = 0x83; // non-zeo byte
            }
            // the user ID string, variable length, terminated with a null (0x00)
            if (userid_len > 0) {
                memcpy(&node->handshacking.pending_data_ptr[8], self->login, userid_len);
            }
            node->handshacking.pending_data_ptr[8 + userid_len] = 0x00;

            // the domain name of the host we want to contact, variable length, terminated with a null (0x00)
            if (self->type == tnet_proxy_type_socks4a) {
                if (domain_len > 0) {
                    memcpy(&node->handshacking.pending_data_ptr[8 + userid_len + 1], self->dst_host, domain_len);
                }
                node->handshacking.pending_data_ptr[8 + userid_len + 1 + domain_len] = 0x00;
            }
        }
        else { // SOCKS5
            uint8_t* auths_ptr = &node->handshacking.pending_data_ptr[2];
            node->handshacking.pending_data_ptr[0] = 0x05; // version number
            node->handshacking.pending_data_ptr[1] = 0x00; // number of authentication methods supported
#if TNET_SOCKS5_HAVE_AUTH_NONE
            node->handshacking.pending_data_ptr[1] += 1;
#endif
#if TNET_SOCKS5_HAVE_AUTH_USRPWD
            node->handshacking.pending_data_ptr[1] += 1;
#endif
#if TNET_SOCKS5_HAVE_AUTH_GSSAPI
            node->handshacking.pending_data_ptr[1] += 1;
#endif
#if TNET_SOCKS5_HAVE_AUTH_GSSAPI
            *auths_ptr++ = kSocks5AuthMethodGSSAPI;
#endif
#if TNET_SOCKS5_HAVE_AUTH_USRPWD
            *auths_ptr++ = kSocks5AuthMethodUsrPwd;
#endif
#if TNET_SOCKS5_HAVE_AUTH_NONE
            *auths_ptr++ = kSocks5AuthMethodNone;
#endif
            // change state from none to greeting
            node->handshacking.socks5_state = socks5_state_greeting;
        }
        node->handshacking.started = tsk_true;
    }
#endif


bail:
    tsk_safeobj_unlock(node);
    return ret;
}

static int _tnet_proxy_node_socks_plugin_set_handshaking_data(tnet_proxy_node_t* self, const void* data_ptr, tsk_size_t data_size)
{
    tnet_proxy_node_socks_plugin_t* node = (tnet_proxy_node_socks_plugin_t*)self;
    int ret = 0;

    // input parameters already checked by the caller

    tsk_safeobj_lock(node);

    if (!node->handshacking.started) {
        TSK_DEBUG_ERROR("handshaking not started");
        ret = -3;
        goto bail;
    }

#if USING_CFSTREAM
    TSK_DEBUG_ERROR("Up to CFStreams to handle handshaking");
    ret = -2;
    goto bail;
#else
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
    if (self->type == tnet_proxy_type_socks4 || self->type == tnet_proxy_type_socks4a) {
        static const tsk_size_t min_bytes_count = 8;
        const uint8_t* buff = (const uint8_t*)node->handshacking.buff->data;
        uint8_t status;
        if (node->handshacking.buff->size < min_bytes_count) {
            TSK_DEBUG_INFO("Not enough data in the SOCKS4/4a handshaking buffer yet");
            goto bail;
        }
        status = buff[1];
        TSK_DEBUG_INFO("SOCKS4/4a status = 0x%x", status);
        if (status != kSocks4StatusGranted) { // status == granted ?
            TSK_DEBUG_ERROR("SOCKS response from the server has status equal to 0x%x", status);
            ret = -3;
            goto bail;
        }
        // remove already parsed data
        tsk_buffer_remove(node->handshacking.buff, 0, min_bytes_count);
        // handshaking is done ?
        node->handshacking.completed = (status == kSocks4StatusGranted);
    }
    else if (self->type == tnet_proxy_type_socks5) {
        const uint8_t* buff = (const uint8_t*)node->handshacking.buff->data;
        TSK_DEBUG_INFO("Socks5 state = %s", __socks5_state_to_string(node->handshacking.socks5_state));
        if (node->handshacking.socks5_state == socks5_state_greeting) {
            // response to greeting
            if (node->handshacking.buff->size < 2/*1-byte version + 1-byte authentication method*/) {
                TSK_DEBUG_INFO("Not enough data in the SOCKS5 handshaking buffer yet");
                goto bail;
            }
            if (buff[0] != 0x05) {
                TSK_DEBUG_ERROR("Invalid version (%d)", buff[0]);
                ret = -3;
                goto bail;
            }
            if (buff[1] == kSocks5AuthMethodNone || buff[1] == kSocks5AuthMethodUsrPwd || buff[1] == kSocks5AuthMethodGSSAPI) {
                node->handshacking.socks5_auth_method = (socks5_auth_method_t)buff[1];
                TSK_DEBUG_INFO("Server selected Socks5 authentication method = %s", __socks5_method_to_string(node->handshacking.socks5_auth_method));
                tsk_buffer_remove(node->handshacking.buff, 0, 2); // remove parsed bytes
                if (node->handshacking.socks5_auth_method == kSocks5AuthMethodNone) {
                    node->handshacking.socks5_state = socks5_state_conn_req;
                    // FIXME:
                    TSK_DEBUG_ERROR("Not implemented yet");
                    ret = -3;
                    goto bail;
                }
                else {
                    if (node->handshacking.socks5_auth_method == kSocks5AuthMethodUsrPwd) {
                        tsk_size_t userlen = tsk_strlen(self->login), pwdlen = tsk_strlen(self->password);
                        tsk_size_t size_to_reserve = 1 /* version number */
                                                     + 1 /* username length */
                                                     + userlen /* username */
                                                     + 1 /* password length */
                                                     + pwdlen;
                        node->handshacking.pending_data_ptr = (uint8_t*)tsk_realloc(node->handshacking.pending_data_ptr, size_to_reserve);
                        if (!node->handshacking.pending_data_ptr) {
                            TSK_DEBUG_ERROR("Failed to allocate buffer with size = %u", (unsigned)size_to_reserve);
                            node->handshacking.pending_data_len = 0;
                            ret = -5;
                            goto bail;
                        }
                        node->handshacking.pending_data_len = size_to_reserve;

                        node->handshacking.pending_data_ptr[0] = 0x01;
                        node->handshacking.pending_data_ptr[1] = (userlen & 0xFF);
                        if (userlen > 0) {
                            memcpy(&node->handshacking.pending_data_ptr[2], self->login, userlen);
                        }
                        node->handshacking.pending_data_ptr[2 + userlen] = (uint8_t)pwdlen;
                        if (pwdlen > 0) {
                            memcpy(&node->handshacking.pending_data_ptr[2 + userlen + 1], self->password, pwdlen);
                        }
                        node->handshacking.socks5_state = socks5_state_auth_req;
                    }
                    else if (node->handshacking.socks5_auth_method == kSocks5AuthMethodGSSAPI) {
#if TNET_SOCKS5_HAVE_AUTH_GSSAPI
                        if ((ret = __socks5_gss_import_name(self))) {
                            TSK_DEBUG_ERROR("gss_import_name() failed");
                            goto bail;
                        }
                        if ((ret = __socks5_gss_init_sec_context(self))) {
                            TSK_DEBUG_ERROR("gss_import_name() failed");
                            goto bail;
                        }

#else
                        TSK_DEBUG_ERROR("GSSAPI not supported");
                        ret = -3;
                        goto bail;
#endif
                    }
                    else {
                        TSK_DEBUG_ERROR("Not implemented yet");
                        ret = -3;
                        goto bail;
                    }
                }
            }
            else {
                TSK_DEBUG_ERROR("Invalid authentication method (%d)", buff[1]);
                ret = -3;
                goto bail;
            }
        }
        else if (node->handshacking.socks5_state == socks5_state_auth_req) {
            // response to authentication
            if (node->handshacking.socks5_auth_method == kSocks5AuthMethodUsrPwd) {
                if (node->handshacking.buff->size < 2/*1-byte version + 1-byte status code*/) {
                    TSK_DEBUG_INFO("Not enough data in the SOCKS5 handshaking buffer yet");
                    goto bail;
                }
                if (buff[0] != 0x1) {
                    TSK_DEBUG_ERROR("Invalid version :%d", buff[0]);
                    ret = -3;
                    goto bail;
                }
                TSK_DEBUG_INFO("Socks5 authentication status code: %d", buff[1]);
                if (buff[1] != 0x00) {
                    TSK_DEBUG_ERROR("Authentication failed with status code :%d", buff[1]);
                    ret = -3;
                    goto bail;
                }
                tsk_buffer_remove(node->handshacking.buff, 0, 2); // remove parsed bytes
                node->handshacking.socks5_state = socks5_state_conn_req;
            }
            else if (node->handshacking.socks5_auth_method == kSocks5AuthMethodGSSAPI) {
                // FIXME:
                TSK_DEBUG_ERROR("Not implemented yet");
                ret = -3;
                goto bail;
            }
            else {
                TSK_DEBUG_ERROR("Invalid authentication method (%d)", buff[1]);
                ret = -3;
                goto bail;
            }

            // State changed from "auth_req" to "conn_req" : build connection request
            if (node->handshacking.socks5_state == socks5_state_conn_req) {
#define kAddrTypeIPv4           0x01
#define kAddrTypeDomaineName    0x03
#define kAddrTypeIPv6           0x04
                struct sockaddr_storage addr;
                tsk_bool_t is_ip = tsk_false; // ip or domain name
                uint8_t addr_type = 0x00;
                tsk_size_t dst_addr_len = 0, size_to_reserve = 1 /* version number */
                                          + 1 /* command code */
                                          + 1 /* reserved, must be 0x00 */
                                          + 1 /* address type */
                                          + 0 /* destination address (==to be computed later==) */
                                          + 2 /* port number in a network byte order */
                                          ;
                if ((ret = tnet_sockaddr_init(self->dst_host, self->dst_port, self->socket.type, &addr)) != 0) {
                    TSK_DEBUG_WARN("tnet_sockaddr_init(%s, %d, %d) failed", self->dst_host, self->dst_port, self->socket.type);
                    // maybe DNS issue (e.g UDP blocked), do not exit, up to the server to resolve it
                    is_ip = tsk_false;
                }
                else {
                    tnet_ip_t ip = {0};
                    tnet_get_sockip((const struct sockaddr *)&addr, &ip);
                    is_ip = tsk_striequals(self->dst_host, ip);
                }
                if (is_ip) {
                    dst_addr_len = (addr.ss_family == AF_INET6) ? 16 : 4;
                    addr_type = (addr.ss_family == AF_INET6) ? kAddrTypeIPv6 : kAddrTypeIPv4;
                }
                else {
                    dst_addr_len = 1 /* length*/ + tsk_strlen(self->dst_host);
                    addr_type = kAddrTypeDomaineName;
                }
                size_to_reserve += dst_addr_len;
                node->handshacking.pending_data_ptr = (uint8_t*)tsk_realloc(node->handshacking.pending_data_ptr, size_to_reserve);
                if (!node->handshacking.pending_data_ptr) {
                    TSK_DEBUG_ERROR("Failed to allocate buffer with size = %u", (unsigned)size_to_reserve);
                    node->handshacking.pending_data_len = 0;
                    ret = -5;
                    goto bail;
                }
                node->handshacking.pending_data_len = size_to_reserve;

                node->handshacking.pending_data_ptr[0] = 0x05; // version number
                node->handshacking.pending_data_ptr[1] = 0x01; // establish a TCP/IP stream connection
                node->handshacking.pending_data_ptr[2] = 0x00; // reserved, must be 0x00
                node->handshacking.pending_data_ptr[3] = addr_type;
                if (addr_type == kAddrTypeIPv4) {
                    memcpy(&node->handshacking.pending_data_ptr[4], (void*)&((const struct sockaddr_in*)&addr)->sin_addr.s_addr, 4);
                }
                else if (addr_type == kAddrTypeIPv6) {
                    memcpy(&node->handshacking.pending_data_ptr[4], (void*)&((const struct sockaddr_in6*)&addr)->sin6_addr, 16);
                }
                else  { // DomaineName
                    node->handshacking.pending_data_ptr[4] = (uint8_t)(dst_addr_len - 1);/* length */;
                    memcpy(&node->handshacking.pending_data_ptr[5], self->dst_host, (dst_addr_len - 1));
                }
                node->handshacking.pending_data_ptr[4 + dst_addr_len] = (self->dst_port >> 8) & 0xFF;
                node->handshacking.pending_data_ptr[4 + dst_addr_len + 1] = (self->dst_port & 0xFF);
            }
        }
        else if (node->handshacking.socks5_state == socks5_state_conn_req) {
            // response to connection request
            if (node->handshacking.buff->size < 2/*1-byte version + 1-byte status*/) {
                TSK_DEBUG_INFO("Not enough data in the SOCKS5 handshaking buffer yet");
                goto bail;
            }
            if (buff[0] != 0x05) {
                TSK_DEBUG_ERROR("Invalid version (%d)", buff[0]);
                ret = -3;
                goto bail;
            }
            TSK_DEBUG_INFO("Socks5 connection request status code: %d", buff[1]);
            if (buff[1] != 0x00) {
                TSK_DEBUG_ERROR("Socks5 connection request failed with status code :%d", buff[1]);
                ret = -3;
                goto bail;
            }
            tsk_buffer_remove(node->handshacking.buff, 0, 2); // remove parsed bytes
            node->handshacking.socks5_state = socks5_state_conn_accept;
            node->handshacking.completed = tsk_true;
        }
        else if (node->handshacking.socks5_state == socks5_state_conn_accept) {
            TSK_DEBUG_INFO("Socks5 connection accepted");
            // nothing else to do
        }
        else {
            // FIXME:
            TSK_DEBUG_ERROR("Not implemented yet");
            ret = -3;
            goto bail;
        }
    }
    else {
        TSK_DEBUG_ERROR("Invalid proxy type:%d", self->type);
        ret = -3;
        goto bail;
    }
#endif

bail:
    if (ret != 0) {
        if (self->type == tnet_proxy_type_socks5) {
            node->handshacking.socks5_state = socks5_state_error;
        }
    }
    tsk_safeobj_unlock(node);
    return ret;
}

static int _tnet_proxy_node_socks_plugin_get_handshaking_pending_data(tnet_proxy_node_t* self, void** data_pptr, tsk_size_t* data_psize)
{
    tnet_proxy_node_socks_plugin_t* node = (tnet_proxy_node_socks_plugin_t*)self;
    int ret = -1;

    // input parameters already checked by the caller

    tsk_safeobj_lock(node);

#if USING_CFSTREAM
    *data_psize = 0; // no pending data
    ret = 0;
#else
    if (node->handshacking.pending_data_ptr && node->handshacking.pending_data_len > 0) {
        if ((*data_pptr = tsk_realloc(*data_pptr, node->handshacking.pending_data_len))) {
            memcpy(*data_pptr, node->handshacking.pending_data_ptr, (tsk_size_t) node->handshacking.pending_data_len);
            *data_psize = node->handshacking.pending_data_len;
            ret = 0;
        }
        // reset the pending data. Up to the caller to hold the returned data and send it as many as required (e.g. when using unreliable transport)
        TSK_FREE(node->handshacking.pending_data_ptr);
        node->handshacking.pending_data_len = 0;
    }
#endif

    tsk_safeobj_unlock(node);
    return ret;
}

static int _tnet_proxy_node_socks_plugin_get_handshaking_completed(tnet_proxy_node_t* self, tsk_bool_t* completed)
{
    tnet_proxy_node_socks_plugin_t* node = (tnet_proxy_node_socks_plugin_t*)self;
    int ret = 0;

    // input parameters already checked by the caller

    tsk_safeobj_lock(node);
    *completed = node->handshacking.completed;
    tsk_safeobj_unlock(node);
    return ret;
}

static const char* __socks5_state_to_string(socks5_state_t state)
{
    switch (state) {
    case socks5_state_none:
        return "none";
    case socks5_state_greeting:
        return "greeting";
    case socks5_state_auth_req:
        return "auth_req";
    case socks5_state_conn_req:
        return "conn_req";
    case socks5_state_conn_accept:
        return "conn_accept";
    case socks5_state_error:
        return "error";
    default:
        return "unknown";
    }
}

static const char* __socks5_method_to_string(socks5_auth_method_t method)
{
    switch (method) {
    case kSocks5AuthMethodNone:
        return "none";
    case kSocks5AuthMethodGSSAPI:
        return "gssapi";
    case kSocks5AuthMethodUsrPwd:
        return "usr/pwd";
    default:
        return "unknown";
    }
}

#if TNET_SOCKS5_HAVE_AUTH_GSSAPI

// rfc1961 - 3.1 Preparation
static int __socks5_gss_import_name(tnet_proxy_node_t* self)
{
    int i, ret = 0;
    tnet_proxy_node_socks_plugin_t* node = (tnet_proxy_node_socks_plugin_t*)self;
    gss_buffer_desc input_name_buffer = GSS_C_EMPTY_BUFFER;

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    // reset context
    node->handshacking.gss.ctx = GSS_C_NO_CONTEXT;
    // reset pending handshaking data
    TSK_FREE(node->handshacking.pending_data_ptr);
    node->handshacking.pending_data_len = 0;

    /*
     For example, when using Kerberos V5 naming, the imported name may be
     of the form "SERVICE:socks@socks_server_hostname" where
     "socks_server_hostname" is the fully qualified host name of the
     server with all letters in lower case.
     */
    input_name_buffer.length = tsk_sprintf((char**)&input_name_buffer.value, "SERVICE:socks@%s", self->proxy_host);
    for (i = 0; i < input_name_buffer.length; ++i) {
        ((char*)input_name_buffer.value)[i] = tolower(((char*)input_name_buffer.value)[i]);
    }

    /*
     The client should call gss_import_name to obtain an internal
     representation of the server name.  For maximal portability the
     default name_type GSS_C_NULL_OID should be used to specify the
     default name space, and the input name_string should treated by the
     client's code as an opaque name-space specific input.
     */
    node->handshacking.gss.status_major = gss_import_name(&node->handshacking.gss.status_minor, &input_name_buffer, GSS_C_NULL_OID, &node->handshacking.gss.server_name);
    TSK_DEBUG_INFO("gss_import_name(%.*s, GSS_C_NULL_OID): minor_status = %u, major_status = %u", (int)input_name_buffer.length, (const char*)input_name_buffer.value, node->handshacking.gss.status_minor, node->handshacking.gss.status_major);
    if (node->handshacking.gss.status_major != GSS_S_COMPLETE) {
        __socks_gss_print_error("gss_import_name failed", node->handshacking.gss.status_major, node->handshacking.gss.status_minor);
        ret = -2;
        goto bail;
    }
    /* debug */{
        gss_OID	output_name_type;
        gss_buffer_desc output_name = GSS_C_EMPTY_BUFFER;
        node->handshacking.gss.status_major = gss_display_name(&node->handshacking.gss.status_minor,
                                              node->handshacking.gss.server_name,
                                              &output_name,
                                              &output_name_type);
        TSK_DEBUG_INFO("gss_display_name(%.*s): minor_status = %u, major_status = %u, output = %.*s", (int)input_name_buffer.length, (const char*)input_name_buffer.value, node->handshacking.gss.status_minor, node->handshacking.gss.status_major, (int)output_name.length, (const char*)output_name.value);
        node->handshacking.gss.status_major = gss_release_buffer(&node->handshacking.gss.status_minor, &output_name);
    }

bail:
    gss_release_buffer(&node->handshacking.gss.status_minor, &input_name_buffer);
    return 0;
}

// 3.2 Client Context Establishment
// 3.3 Client Context Establishment Major Status codes
static int __socks5_gss_init_sec_context(tnet_proxy_node_t* self)
{
    int ret = 0;
    OM_uint32 req_flags;
    tnet_proxy_node_socks_plugin_t* node = (tnet_proxy_node_socks_plugin_t*)self;
    gss_buffer_desc input_token = GSS_C_EMPTY_BUFFER, *input_token_ptr = GSS_C_NO_BUFFER;
    gss_buffer_desc output_token = GSS_C_EMPTY_BUFFER;

    /*
        +------+------+------+.......................+
        + ver  | mtyp | len  |       token           |
        +------+------+------+.......................+
        + 0x01 | 0x01 | 0x02 | up to 2^16 - 1 octets |
        +------+------+------+.......................+
    */
#define kTokenMsgHdrLongLen 4 // with "len" field
#define kTokenMsgHdrShortLen 2 // without "len" field

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (node->handshacking.buff && node->handshacking.buff->size >= kTokenMsgHdrShortLen) {
        const uint8_t* buff = (const uint8_t*)node->handshacking.buff->data;
        if (buff[1] == kSocks5AuthMethodGSSAPI) {
            if (node->handshacking.buff->size >= kTokenMsgHdrLongLen) {
                uint16_t len = ((buff[2] << 8) & 0x00FF) | (buff[3] & 0xFF);
                if (len >= kTokenMsgHdrLongLen + node->handshacking.buff->size) {
                    input_token.value = (void*) (buff + kTokenMsgHdrLongLen);
                    input_token.length = (size_t)len;
                    input_token_ptr = &input_token;
                    TSK_DEBUG_INFO("GSS Input token: ver=%d, mtyp=%d, len=%d", buff[0], buff[1], len);
                }
            }
        }
        else {
            TSK_DEBUG_ERROR("GSS invalid mtyp(%u)", buff[1]);
            ret = -5;
            goto bail;
        }
    }

    req_flags = GSS_C_MUTUAL_FLAG | GSS_C_REPLAY_FLAG | GSS_C_DELEG_FLAG;
    // However, GSS_C_SEQUENCE_FLAG should only be passed in for TCP-based clients, not for UDP-based clients.
    if (TNET_SOCKET_TYPE_IS_STREAM(self->socket.type)) {
        req_flags |= GSS_C_SEQUENCE_FLAG;
    }

    node->handshacking.gss.status_major = gss_delete_sec_context(&node->handshacking.gss.status_minor, &node->handshacking.gss.ctx, NULL);

    node->handshacking.gss.ctx = GSS_C_NO_CONTEXT;
    node->handshacking.gss.status_major = gss_init_sec_context(&node->handshacking.gss.status_minor,
                                          GSS_C_NO_CREDENTIAL, // GSS_C_NO_CREDENTIAL into cred_handle to specify the default credential (for initiator usage)
                                          &node->handshacking.gss.ctx,
                                          node->handshacking.gss.server_name,
                                          GSS_C_NULL_OID, // GSS_C_NULL_OID into mech_type to specify the default mechanism
                                          req_flags,
                                          GSS_C_INDEFINITE,
                                          GSS_C_NO_CHANNEL_BINDINGS,
                                          input_token_ptr,
                                          tsk_null,
                                          &output_token,
                                          tsk_null,
                                          tsk_null);


    // Only "GSS_S_COMPLETE" and "GSS_S_CONTINUE_NEEDED" are acceptable
    if (node->handshacking.gss.status_major != GSS_S_COMPLETE && node->handshacking.gss.status_major != GSS_S_CONTINUE_NEEDED) {
        __socks_gss_print_error("gss_init_sec_context failed", node->handshacking.gss.status_major, node->handshacking.gss.status_minor);
        ret = -2;
        goto bail;
    }

    // reset pending handshaking data
    TSK_FREE(node->handshacking.pending_data_ptr);
    node->handshacking.pending_data_len = 0;

    if (output_token.length > 0 && output_token.value) {
        node->handshacking.pending_data_len = kTokenMsgHdrLongLen + output_token.length;
        node->handshacking.pending_data_ptr = tsk_realloc(node->handshacking.pending_data_ptr, node->handshacking.pending_data_len);
        if (!node->handshacking.pending_data_ptr) {
            TSK_DEBUG_ERROR("Failed to allocate buffer with size = %u", (unsigned)node->handshacking.pending_data_len);
            node->handshacking.pending_data_len = 0;
            ret = -3;
            goto bail;
        }
        node->handshacking.pending_data_ptr[0] = 0x01;
        node->handshacking.pending_data_ptr[1] = kSocks5AuthMethodGSSAPI;
        node->handshacking.pending_data_ptr[2] = ((output_token.length >> 8) & 0xFF);
        node->handshacking.pending_data_ptr[3] = output_token.length & 0xFF;
        memcpy(&node->handshacking.pending_data_ptr[4], output_token.value, output_token.length);
    }

    if (input_token.length > 0) {
        tsk_buffer_remove(node->handshacking.buff, 0, kTokenMsgHdrLongLen + input_token.length);
    }

    // update "init_sec_complete"
    node->handshacking.gss.init_sec_complete = (ret == 0) && (node->handshacking.gss.status_major == GSS_S_COMPLETE);

bail:
    return ret;
}

static void __socks_gss_print_error(const char* info, OM_uint32 status_major, OM_uint32 status_minor)
{
    OM_uint32 m1, m2, s1, s2;
    gss_buffer_desc out1 = {.value = tsk_null, .length = 0}, out2 = {.value = tsk_null, .length = 0};

    // print the error
    m1 = 0, s1 = 0;
    gss_display_status(&s1, status_major, GSS_C_GSS_CODE, GSS_C_NULL_OID, &m1, &out1);
    m2 = 0, s2 = 0;
    gss_display_status(&s2, status_minor, GSS_C_MECH_CODE, GSS_C_NULL_OID, &m2, &out2);
    TSK_DEBUG_ERROR("%s (status_major=%u, status_minor=%u): GSS_C_GSS_CODE(%u, %s), GSS_C_MECH_CODE(%u, %s)",
                    info,
                    status_major, status_minor,
                    s1, (const char*)out1.value, s2, (const char*)out2.value);
    gss_release_buffer(&s1, &out1);
    gss_release_buffer(&s2, &out2);
}

#endif /* TNET_SOCKS5_HAVE_AUTH_GSSAPI */

/* constructor */
static tsk_object_t* tnet_proxy_node_socks_plugin_ctor(tsk_object_t * self, va_list * app)
{
    tnet_proxy_node_socks_plugin_t *node = self;
    if (node) {
        /* init base */
        tnet_proxy_node_init(TNET_PROXY_NODE(node));
        /* init self */
        tsk_safeobj_init(node);
#if TNET_SOCKS5_HAVE_AUTH_GSSAPI
        node->handshacking.gss.server_name = GSS_C_NO_NAME;
        node->handshacking.gss.ctx = GSS_C_NO_CONTEXT;
#endif
        TSK_DEBUG_INFO("Create SOCKS(4/4a/5) proxy node");
    }
    return self;
}
/* destructor */
static tsk_object_t* tnet_proxy_node_socks_plugin_dtor(tsk_object_t * self)
{
    tnet_proxy_node_socks_plugin_t *node = self;
    if (node) {
        /* deinit base */
        tnet_proxy_node_deinit(TNET_PROXY_NODE(node));
        /* deinit self */
        TSK_FREE(node->handshacking.pending_data_ptr);
        TSK_OBJECT_SAFE_FREE(node->handshacking.buff);
#if TNET_SOCKS5_HAVE_AUTH_GSSAPI
        gss_release_name(&node->handshacking.gss.status_minor, &node->handshacking.gss.server_name);
        if (node->handshacking.gss.ctx != GSS_C_NO_CONTEXT) {
            gss_delete_sec_context(&node->handshacking.gss.status_minor, &node->handshacking.gss.ctx, GSS_C_NO_BUFFER);
        }
#endif /* TNET_SOCKS5_HAVE_AUTH_GSSAPI */
        tsk_safeobj_deinit(node);

        TSK_DEBUG_INFO("*** Socks(4/4a/5) proxy node destroyed ***");
    }
    return self;
}

/* object definition */
static const tsk_object_def_t thttp_proxy_node_def_s = {
    sizeof(tnet_proxy_node_socks_plugin_t),
    tnet_proxy_node_socks_plugin_ctor,
    tnet_proxy_node_socks_plugin_dtor,
    tsk_null,
};
/* plugin definition*/
static const struct tnet_proxy_node_plugin_def_s tnet_proxy_node_socks_plugin_def_s = {
    &thttp_proxy_node_def_s,

    (tnet_proxy_type_socks4 | tnet_proxy_type_socks4a | tnet_proxy_type_socks5),

    "SOCKS(4/4a/5) proxy node plugin",

    _tnet_proxy_node_socks_plugin_configure,
    _tnet_proxy_node_socks_plugin_start_handshaking,
    _tnet_proxy_node_socks_plugin_set_handshaking_data,
    _tnet_proxy_node_socks_plugin_get_handshaking_pending_data,
    _tnet_proxy_node_socks_plugin_get_handshaking_completed
};
const struct tnet_proxy_node_plugin_def_s *tnet_proxy_node_socks_plugin_def_t = &tnet_proxy_node_socks_plugin_def_s;
