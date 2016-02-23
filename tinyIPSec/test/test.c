/* Copyright (C) 2013-2014 Mamadou DIOP
* Copyright (C) 2013-2014 Doubango Telecom <http://www.doubango.org>
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
*/
#include "stdafx.h"
#include "tipsec.h"
#include "tsk_plugin.h"
#include "tsk_debug.h"

static tipsec_ipproto_t __ipproto = tipsec_ipproto_udp;
static tsk_bool_t __use_ipv6 = tsk_false;
static tipsec_mode_t __mode = tipsec_mode_trans;
static tipsec_ealg_t __ealg = tipsec_ealg_aes;
static tipsec_alg_t __alg = tipsec_alg_hmac_md5_96;
static tipsec_proto_t __proto = tipsec_proto_esp;

static const char* __addr_local = "192.168.0.37"; // overrided using command line args
static const char* __addr_remote = "192.168.0.31"; // overrided using command line args
static tipsec_port_t __port_local_in = 5064; // PORT_US
static tipsec_port_t __port_local_out = 5062; // PORT_UC
static tipsec_port_t __port_remote_in = 5068; // PORT_PS
static tipsec_port_t __port_remote_out = 5066; // PORT_PC
static tipsec_spi_t __spi_remote_out = 3333; // SPI_PC
static tipsec_spi_t __spi_remote_in = 4444; // SPI_PS
static tipsec_lifetime_t __lifetime = 1800; /* always set it to the maximum value. (Not possible to update the value after REGISTER 200OK. ) */

static const char* __key_ik = "1234567890123456";
static const char* __key_ck =	"1234567890121234";

#if TIPSEC_UNDER_WINDOWS
#include <Windows.h>

static tsk_bool_t test_ipsec_is_winvista_or_later()
{
    /*
    Version Number    Description
    6.1               Windows 7     / Windows 2008 R2
    6.0               Windows Vista / Windows 2008
    5.2               Windows 2003
    5.1               Windows XP
    5.0               Windows 2000
    */
    static DWORD dwMajorVersion = -1;
    static DWORD dwMinorVersion = -1;

    if(dwMajorVersion == -1 || dwMinorVersion == -1) {
        OSVERSIONINFO osvi;
        ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        GetVersionEx(&osvi);
        dwMajorVersion = osvi.dwMajorVersion;
        dwMinorVersion = osvi.dwMinorVersion;
    }

    return (dwMajorVersion >= 6);
}
#else
static tsk_bool_t test_ipsec_is_winvista_or_later()
{
    return 0;
}
#endif

#if defined(_WIN32_WCE)
int _tmain()
#else
int main(int argc, const char* argv[])
#endif
{
    int err = 0;
    tsk_size_t i;
    struct tsk_plugin_s* p_plugin = tsk_null;
    tipsec_ctx_t* p_ctx = tsk_null;
    static const char* __plugins_path[] = /* Visual Studio requires "Debugging" -> "Working Directory" = "$(OutDir)"  */
    {
        "pluginWinIPSecVista.DLL",
        "pluginWinIPSecXP.DLL",
        "pluginLinIPsecTools.so"
    };
    static const tsk_size_t __plugins_count = sizeof(__plugins_path)/sizeof(__plugins_path[0]);

#define BUF_SIZE 1024
    char buffer[BUF_SIZE];

    /* Set debug level to INFO */
    tsk_debug_set_level(DEBUG_LEVEL_INFO);

    // Command line "local_ip" "local_port_in" "local_port_out" "remote_ip"  "remote_port_in" "remote_port_out"
    if (argc == (6 + 1)) {
        __addr_local = argv[1];
        __port_local_in = atoi(argv[2]);
        __port_local_out = atoi(argv[3]);
        __addr_remote = argv[4];
        __port_remote_in = atoi(argv[5]);
        __port_remote_out = atoi(argv[6]);
    }

    TSK_DEBUG_INFO("Local node=%s:%d/%d, remote node=%s:%d/%d",
                   __addr_local, __port_local_in, __port_local_out,
                   __addr_remote, __port_remote_in, __port_remote_out);

    /* Create the plugin */
    for (i = 0; i < __plugins_count; ++i) {
        if (tsk_plugin_file_exist(__plugins_path[i])) {
            tipsec_plugin_register_file(__plugins_path[i], &p_plugin);
            if (p_plugin) {
                break;
            }
        }
    }
    if (!p_plugin) {
        TSK_DEBUG_ERROR("Failed to create IPSec plugin");
        err = -1;
        goto bail;
    }

    /* Create the context */
    err = tipsec_ctx_create(__ipproto, __use_ipv6, __mode, __ealg, __alg, __proto, &p_ctx);
    if (err) {
        goto bail;
    }

    /* Set local */
    err = tipsec_ctx_set_local(p_ctx, __addr_local, __addr_remote, __port_local_out, __port_local_in);
    if (err) {
        goto bail;
    }
    // Dump SPIs created by the OS after calling set_local()
    TSK_DEBUG_INFO("SPI-UC=%u, SPI-US=%u", p_ctx->spi_uc, p_ctx->spi_us);

    // Enter Remote SPI in
    TSK_DEBUG_INFO("Enter remote SPI in:");
    if (fgets(buffer, BUF_SIZE, stdin)) {
        if (buffer[0] != 10 && buffer[1] != 0) {
            __spi_remote_in = strtoul (buffer, NULL, 0);
        }
    }

    // Enter Remote SPI out
    TSK_DEBUG_INFO("Enter remote SPI out:");
    if (fgets(buffer, BUF_SIZE, stdin)) {
        if (buffer[0] != 10 && buffer[1] != 0) {
            __spi_remote_out = strtoul (buffer, NULL, 0);
        }
    }

    TSK_DEBUG_INFO("SPI remote %u/%u", __spi_remote_in, __spi_remote_out);

    /* Set remote */
    err = tipsec_ctx_set_remote(p_ctx, __spi_remote_out, __spi_remote_in, __port_remote_out, __port_remote_in, __lifetime);
    if (err) {
        goto bail;
    }

    /* Set Integrity (IK) and Confidentiality (CK) keys */
    err = tipsec_ctx_set_keys(p_ctx, __key_ik, __key_ck);
    if (err) {
        goto bail;
    }

    /* Start (Setup) the SAs */
    err = tipsec_ctx_start(p_ctx);
    if (err) {
        goto bail;
    }

    TSK_DEBUG_INFO("!!! IPSec SAs started (Press any key to stop) !!!");

    /* Wait */
    getchar();

bail:
    // Stop the SAs, cleanup and destroy the context
    TSK_OBJECT_SAFE_FREE(p_ctx); // must destroy the contect before unloading the plugin (*.DLL or *.SO)

    // Unregister the plugin and close the file handle
    if (p_plugin) {
        tipsec_plugin_unregister_file(p_plugin);
        TSK_OBJECT_SAFE_FREE(p_plugin);
    }
    return err;
}
