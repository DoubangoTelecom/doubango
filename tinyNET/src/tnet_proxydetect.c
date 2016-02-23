/*
 * Copyright (C) 2010-2015 Mamadou DIOP.
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
#include "tnet_proxydetect.h"
#include "tnet_utils.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/* "tnet_proxyinfo_t" object definition */
static tsk_object_t* _tnet_proxyinfo_ctor(tsk_object_t * self, va_list * app)
{
    tnet_proxyinfo_t *info = (tnet_proxyinfo_t *)self;
    if (info) {

    }
    return self;
}
static tsk_object_t* _tnet_proxyinfo_dtor(tsk_object_t * self)
{
    tnet_proxyinfo_t *info = (tnet_proxyinfo_t *)self;
    if (info) {
        TSK_FREE(info->autoconfig_url);
        TSK_FREE(info->bypass_list);
        TSK_FREE(info->hostname);
        TSK_FREE(info->username);
        TSK_FREE(info->password);
    }

    return self;
}
static const tsk_object_def_t tnet_proxyinfo_def_s = {
    sizeof(tnet_proxyinfo_t),
    _tnet_proxyinfo_ctor,
    _tnet_proxyinfo_dtor,
    tsk_null,
};
const tsk_object_def_t *tnet_proxyinfo_def_t = &tnet_proxyinfo_def_s;

tnet_proxyinfo_t* tnet_proxyinfo_create()
{
    tnet_proxyinfo_t* info = tsk_object_new(tnet_proxyinfo_def_t);
    if (!info) {
        TSK_DEBUG_ERROR("Failed to creatr 'tnet_proxyinfo_t' instance");
        return info;
    }

    return info;
}



/******** Windows ************/
#if TNET_UNDER_WINDOWS && !TNET_UNDER_WINDOWS_RT && !TNET_UNDER_WINDOWS_CE
#   include <Windows.h>
#   include <winhttp.h>

tnet_proxyinfo_t* tnet_proxydetect_get_info(const char* url, tnet_socket_type_t socket_type, tsk_bool_t long_operation)
{
    TSK_DEBUG_WARN("Proxy detection not supported on your OS");
    return tsk_null;
}

/******** iOS and OSX ************/
#elif TNET_UNDER_APPLE

#import <CFNetwork/CFNetwork.h>

#define CFSafeRelease(pp_cf) if (pp_cf && *pp_cf) CFRelease(*pp_cf), *pp_cf = NULL;
#if !defined (TNET_AUTODETECT_RUNLOOP_MODE)
#   define TNET_AUTODETECT_RUNLOOP_MODE         "org.doubango.proxydetect.auto"
#endif

typedef struct appl_proxyinfo_xs {
    CFStringRef host;
    int port;
    CFStringRef username;
    CFStringRef password;
    CFStringRef type;
}
appl_proxyinfo_xt;

static tnet_proxy_type_t _appl_proxy_type_convert(CFStringRef cfProxyType);
static void _appl_proxyinfo_release(appl_proxyinfo_xt * info);
static tsk_bool_t _appl_proxyinfo_is_valid(const appl_proxyinfo_xt * info);
static void _ProxyAutoConfigurationResultCallback(void *client, CFArrayRef proxyList, CFErrorRef error);
static void _appl_find_best_proxy(CFURLRef cfTargetURL, CFArrayRef _cfProxies, appl_proxyinfo_xt *_proxyInfo);

tnet_proxyinfo_t* tnet_proxydetect_get_info(const char* url, tnet_socket_type_t socket_type, tsk_bool_t long_operation)
{
    tnet_proxyinfo_t* info = tsk_null;
    CFStringRef cfUrl = CFStringCreateWithCString(CFAllocatorGetDefault(), url, kCFStringEncodingUTF8);
    CFURLRef cfTargetUrl = CFURLCreateWithString(CFAllocatorGetDefault(), cfUrl, NULL);
    CFDictionaryRef cfProxySettings = NULL;
    CFArrayRef cfProxies = NULL;
    appl_proxyinfo_xt _info = { 0 };
    if (!cfTargetUrl) {
        TSK_DEBUG_ERROR("Failed to create CFURLRef from %s", url);
        goto resolve_done;
    }
    cfProxySettings = CFNetworkCopySystemProxySettings();
    if (!cfProxySettings) {
        TSK_DEBUG_ERROR("CFNetworkCopySystemProxySettings returned nil");
        goto resolve_done;
    }

    cfProxies = CFNetworkCopyProxiesForURL(cfTargetUrl, cfProxySettings);
    if (!cfProxies) {
        TSK_DEBUG_ERROR("CFNetworkCopyProxiesForURL returned 0-array");
        goto resolve_done;
    }
    // find best proxy
    _appl_find_best_proxy(cfTargetUrl, cfProxies, &_info);


resolve_done:
    if (cfUrl) {
        CFRelease(cfUrl);
    }
    if (cfTargetUrl) {
        CFRelease(cfTargetUrl);
    }
    if (cfProxySettings) {
        CFRelease(cfProxySettings);
    }
    if (cfProxies) {
        CFRelease(cfProxies);
    }

    if (_appl_proxyinfo_is_valid(&_info)) {
        info = tnet_proxyinfo_create();
        if (info) {
            info->type = _appl_proxy_type_convert(_info.type);
            info->socket_type = socket_type;
            info->port = _info.port;
            info->hostname = tsk_strdup(CFStringGetCStringPtr(_info.host, kCFStringEncodingUTF8));
            info->username = tsk_strdup(CFStringGetCStringPtr(_info.username, kCFStringEncodingUTF8));
            info->password = tsk_strdup(CFStringGetCStringPtr(_info.password, kCFStringEncodingUTF8));
        }
    }
    _appl_proxyinfo_release(&_info);

    return info;
}

static tsk_bool_t _appl_proxyinfo_is_valid(const appl_proxyinfo_xt * info)
{
    if (info) {
        return info->port
               && info->type && !CFEqual(info->type, kCFProxyTypeNone)
               && info->host && CFStringGetLength(info->host) > 0 && CFStringCompare(info->host, CFSTR("127.0.0.1"), 0) != kCFCompareEqualTo;
    }
    return tsk_false;
}

static tnet_proxy_type_t _appl_proxy_type_convert(CFStringRef cfProxyType)
{
    if (CFEqual(cfProxyType, kCFProxyTypeHTTP) || CFEqual(cfProxyType, kCFProxyTypeHTTPS)) {
        // kCFProxyTypeHTTPS: means the destination url is "https://" not the proxy connection type is HTTPS
        return tnet_proxy_type_http;
    }
    else if(CFEqual(cfProxyType,kCFProxyTypeSOCKS)) {
        return tnet_proxy_type_socks5;
    }
    else {
        return tnet_proxy_type_none;
    }
}

static void _appl_proxyinfo_release(appl_proxyinfo_xt * info)
{
    if (info) {
        CFSafeRelease(&info->host);
        CFSafeRelease(&info->username);
        CFSafeRelease(&info->password);
        CFSafeRelease(&info->type);
        info->port = 0;
    }
}

static void _ProxyAutoConfigurationResultCallback(void *client, CFArrayRef proxyList, CFErrorRef error)
{
    CFTypeRef* cfResult = (CFTypeRef*)client;
    if (error != NULL) {
        *cfResult = CFRetain(error);
    }
    else {
        *cfResult = CFRetain(proxyList);
    }
    CFRunLoopStop(CFRunLoopGetCurrent());
}

static void _appl_find_best_proxy(CFURLRef cfTargetURL, CFArrayRef _cfProxies, appl_proxyinfo_xt *_proxyInfo)
{
    CFDictionaryRef cfProxy;
    CFIndex index = 0;
    CFIndex count = CFArrayGetCount(_cfProxies);

    while (index < count && (cfProxy = CFArrayGetValueAtIndex(_cfProxies, index++)) && !_appl_proxyinfo_is_valid(_proxyInfo)) {
        _appl_proxyinfo_release(_proxyInfo);
        CFStringRef cfProxyType = CFDictionaryGetValue(cfProxy, (const void*)kCFProxyTypeKey);
        if (!cfProxyType) {
            continue;
        }

        TSK_DEBUG_INFO("Found at %li proxy type = %s", (index - 1), CFStringGetCStringPtr(cfProxyType, kCFStringEncodingUTF8));
        if (CFEqual(cfProxyType, kCFProxyTypeNone)) {
            continue;
        }
        else if (CFEqual(cfProxyType, kCFProxyTypeHTTP) || CFEqual(cfProxyType, kCFProxyTypeHTTPS) || CFEqual(cfProxyType, kCFProxyTypeSOCKS)) {
            // "kCFProxyTypeHTTPS" means the url is "https://" not the connection should be TLS
            CFStringRef cfHostName = (CFStringRef)CFDictionaryGetValue(cfProxy, (const void*)kCFProxyHostNameKey);
            if (cfHostName) {
                CFNumberRef cfPortNumber = (CFNumberRef)CFDictionaryGetValue(cfProxy, (const void*)kCFProxyPortNumberKey);
                if (cfPortNumber) {
                    int port = 0;
                    if (!CFNumberGetValue(cfPortNumber, kCFNumberSInt32Type, &port)) {
                        continue;
                    }
                    _proxyInfo->port = port;
                    _proxyInfo->host = CFStringCreateCopy(CFAllocatorGetDefault(), cfHostName);
                    _proxyInfo->type = CFStringCreateCopy(CFAllocatorGetDefault(), cfProxyType);
                    CFStringRef cfStringName = (CFStringRef)CFDictionaryGetValue(cfProxy, (const void*)kCFProxyUsernameKey);
                    if (cfStringName) {
                        _proxyInfo->username = CFStringCreateCopy(CFAllocatorGetDefault(), cfStringName);
                    }
                    CFStringRef cfPassword = (CFStringRef)CFDictionaryGetValue(cfProxy, (const void*)kCFProxyPasswordKey);
                    if (cfPassword) {
                        _proxyInfo->password = CFStringCreateCopy(CFAllocatorGetDefault(), cfPassword);
                    }
                }
            }
        }
        else if (CFEqual(cfProxyType, kCFProxyTypeAutoConfigurationURL)) {
            CFURLRef cfPACUrl = (CFURLRef)CFDictionaryGetValue(cfProxy, (const void*)kCFProxyAutoConfigurationURLKey);
            if (cfPACUrl) {
                TSK_DEBUG_INFO("Found at %li PACUrl = %s", (index - 1), CFStringGetCStringPtr(CFURLGetString(cfPACUrl), kCFStringEncodingUTF8));
                CFTypeRef cfResult = NULL;
                CFStreamClientContext context = { 0, &cfResult, NULL, NULL, NULL };
                CFRunLoopSourceRef cfrunLoop = CFNetworkExecuteProxyAutoConfigurationURL(cfPACUrl,
                                               cfTargetURL,
                                               _ProxyAutoConfigurationResultCallback,
                                               &context);
                if (!cfrunLoop) {
                    TSK_DEBUG_ERROR("CFNetworkExecuteProxyAutoConfigurationURL(%li, %s) failed", (index - 1), CFStringGetCStringPtr(CFURLGetString(cfPACUrl), kCFStringEncodingUTF8));
                    continue;
                }
                static const CFStringRef kPrivateRunloopMode = CFSTR(TNET_AUTODETECT_RUNLOOP_MODE);
                CFRunLoopAddSource(CFRunLoopGetCurrent(), cfrunLoop, kPrivateRunloopMode);
                CFRunLoopRunInMode(kPrivateRunloopMode, DBL_MAX, false);
                CFRunLoopRemoveSource(CFRunLoopGetCurrent(), cfrunLoop, kPrivateRunloopMode);
                if (cfResult == NULL) {
                    TSK_DEBUG_INFO("Result from ProxyAutoConfigurationResultCallback is nil");
                    continue;
                }
                if (CFGetTypeID(cfResult) == CFErrorGetTypeID()) {
                    CFStringRef cfErrorDescription = CFErrorCopyDescription ((CFErrorRef)cfResult);
                    TSK_DEBUG_ERROR("Result from ProxyAutoConfigurationResultCallback is error: %s", CFStringGetCStringPtr(cfErrorDescription, kCFStringEncodingUTF8));
                    CFRelease(cfErrorDescription);
                }
                else if (CFGetTypeID(cfResult) == CFArrayGetTypeID()) {
                    TSK_DEBUG_INFO("Result from ProxyAutoConfigurationResultCallback is array");
                    _appl_find_best_proxy(cfTargetURL, (CFArrayRef)cfResult, _proxyInfo);
                }
                CFRelease(cfResult);
            }
            else {
                TSK_DEBUG_INFO("PACUrl at %li is nil", (index - 1));
            }
        }
    }
}

/******** Not supported ************/
#else

tnet_proxyinfo_t* tnet_proxydetect_get_info(const char* url, tnet_socket_type_t socket_type, tsk_bool_t long_operation)
{
    TSK_DEBUG_WARN("Proxy detection not supported on your OS");
    return tsk_null;
}

#endif /* END-OF-CONDITIONAL-OS-IMPL */


tsk_bool_t tnet_proxyinfo_is_valid(const tnet_proxyinfo_t* self)
{
    if (self) {
        return self->port && self->type != tnet_proxy_type_none && !tsk_strnullORempty(self->hostname);
    }
    return tsk_false;
}

