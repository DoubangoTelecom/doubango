/* Copyright (C) 2010-2014 Mamadou DIOP
* Copyright (C) 2011-2014 Doubango Telecom <http://www.doubango.org>
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

/**@file tipsec.c
 * @brief IPSec plugin and context managers.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 */
#include "tipsec.h"

#include "tsk_plugin.h"
#include "tsk_debug.h"

/** Max number of plugins (consumer types) we can create */
#if !defined(TIPSEC_MAX_PLUGINS)
#	define TIPSEC_MAX_PLUGINS			0x04
#endif /* TIPSEC_MAX_PLUGINS */

/* pointer to all registered consumers */
static const tipsec_plugin_def_t* __tipsec_plugins[TIPSEC_MAX_PLUGINS] = { tsk_null };

/**
* Create an IPSec context to manage the SAs. 
* Before calling this function at least one special implementation must be registered using @ref tipsec_plugin_register_static().
* @param ipproto IPSec internet protocol.
* @param use_ipv6 Whether to use IPv6 or not (IPv4).
* @param mode IPSec mode.
* @param ealg IPSec encryption algorithm.
* @param alg IPSec algorithm.
* @param protocol IPSec protocol.
* @param pp_ctx Pointer holding the newly created context. Valid only if the retured code is @ref tipsec_error_success. This object must be destroyed using @a TSK_OBJECT_SAFE_FREE().
* @retval @ref tipsec_error_success if no error; otherwise error code.
*/
tipsec_error_t tipsec_ctx_create(
    tipsec_ipproto_t ipproto,
    tsk_bool_t use_ipv6,
    tipsec_mode_t mode,
    tipsec_ealg_t ealg,
    tipsec_alg_t alg,
    tipsec_proto_t protocol,
    tipsec_ctx_t** pp_ctx)
{
    tipsec_error_t err = tipsec_error_success;
    tsk_size_t i = 0;
    const tipsec_plugin_def_t* pc_plugin = tsk_null;
    tipsec_ctx_t* p_ctx = tsk_null;

    if (!pp_ctx || *pp_ctx) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tipsec_error_invalid_param;
    }

    // Create the context using the plugins registry
    while ((i < TIPSEC_MAX_PLUGINS) && (pc_plugin = __tipsec_plugins[i++])) {
        if (pc_plugin->objdef) {
            if ((p_ctx = tsk_object_new(pc_plugin->objdef))) {
                /* initialize the newly IPSec context */
                p_ctx->pc_plugin = pc_plugin;
                p_ctx->initialized = 0;
                p_ctx->started = 0;
                p_ctx->state = tipsec_state_initial;
                p_ctx->use_ipv6 = use_ipv6;
                p_ctx->mode = mode;
                p_ctx->ealg = ealg;
                p_ctx->alg = alg;
                p_ctx->protocol = protocol;
				p_ctx->ipproto = ipproto;
                break;
            }
        }
    }

	if (!pc_plugin || !p_ctx) {
		TSK_DEBUG_ERROR("Failed to find/create a plugin instance");
        return tipsec_error_notfound;
	}

    // Initialize the newly created context
    err = pc_plugin->init(p_ctx);
    if (err) {
        goto bail;
    }

bail:
    if (err) {
        TSK_OBJECT_SAFE_FREE(p_ctx);
    }
    *pp_ctx = p_ctx;
    return err;
}

/**
* Ensure the SAs. This function must be called before sending/receiving any data.
* @param p_ctx Pointer to a context created using @ref tipsec_ctx_create().
* @retval @ref tipsec_error_success if no error; otherwise error code.
*/
tipsec_error_t tipsec_ctx_start(tipsec_ctx_t* p_ctx)
{
    if (!p_ctx || !p_ctx->pc_plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tipsec_error_invalid_param;
    }
	if (!p_ctx->initialized || p_ctx->state != tipsec_state_full) {
		TSK_DEBUG_ERROR("Invalid state (not initialized or not in full state)");
		return tipsec_error_invalid_state;
	}
    return p_ctx->pc_plugin->start(p_ctx);
}

/**
* Set local information. On windows Vista and later this function must be called to request local SPIs.
* @param p_ctx Pointer to a context created using @ref tipsec_ctx_create().
* @param addr_local Local IP address (e.g. "192.168.0.5"). The IP version depends on whether @a use_param (0 or 1) when @ref tipsec_ctx_create() was used to create the context.
* @param addr_remote Remote IP address (e.g. "192.168.0.5"). The IP version depends on whether @a use_param (0 or 1) when @ref tipsec_ctx_create() was used to create the context.
* @param port_uc Local client port used to send data. Must be within [1024-65535].
* @param port_us Local server port used to received data. Must be within [1024-65535].
* @retval @ref tipsec_error_success if no error; otherwise error code.
*/
tipsec_error_t tipsec_ctx_set_local(tipsec_ctx_t* p_ctx, const char* addr_local, const char* addr_remote, tipsec_port_t port_uc, tipsec_port_t port_us)
{
    if (!p_ctx || !p_ctx->pc_plugin || !addr_local || !addr_remote || port_uc < 1024 || port_us < 1024) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tipsec_error_invalid_param;
    }
	if (!p_ctx->initialized || p_ctx->state != tipsec_state_initial) {
		TSK_DEBUG_ERROR("Invalid state (not initialized or not in initial state)");
		return tipsec_error_invalid_state;
	}
    return p_ctx->pc_plugin->set_local(p_ctx, addr_local, addr_remote, port_uc, port_us);
}

/**
* Set Integrity (IK) and Confidentiality (CK) Keys.
* On the UE, the IK and CK are built using the "nonce" value using the 494 from the P-CSCF which means the function must be called after the REGISTER<->494 round trip.
* @param p_ctx Pointer to a context created using @ref tipsec_ctx_create().
* @param ik The Integrity Key.
* @param ck The Confidentiality Key.
* @retval @ref tipsec_error_success if no error; otherwise error code.
*/
tipsec_error_t tipsec_ctx_set_keys(tipsec_ctx_t* p_ctx, const tipsec_key_t* ik, const tipsec_key_t* ck)
{
    if (!p_ctx || !p_ctx->pc_plugin || !ik || !ck) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tipsec_error_invalid_param;
    }
    return p_ctx->pc_plugin->set_keys(p_ctx, ik, ck);
}

/**
* Set the remote information.
* @param p_ctx Pointer to a context created using @ref tipsec_ctx_create().
* @param spi_pc Remote client SPI (Security Parameter Index) used by the remote party to send data.
* @param spi_ps Remote server SPI (Security Parameter Index) used by the remote party to receive data.
* @param port_pc Remote client port used by the remote party to send data. Must be within [1024-65535].
* @param port_ps Remote server port used by the remote party to receive data. Must be within [1024-65535].
* @param lifetime The SA lifetime (in seconds). Must not be null. Should be 2xSipRegistrationTimeout. On Windows vista and later, the maximum allowed value is @a 172799 seconds.
* @retval @ref tipsec_error_success if no error; otherwise error code.
*/
tipsec_error_t tipsec_ctx_set_remote(tipsec_ctx_t* p_ctx, tipsec_spi_t spi_pc, tipsec_spi_t spi_ps, tipsec_port_t port_pc, tipsec_port_t port_ps, tipsec_lifetime_t lifetime)
{
    if (!p_ctx || !p_ctx->pc_plugin || port_pc < 1024 || port_ps < 1024 || !lifetime) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tipsec_error_invalid_param;
    }
	if (!p_ctx->initialized || p_ctx->state != tipsec_state_inbound) {
		TSK_DEBUG_ERROR("Invalid state (not initialized or not in initial state)");
		return tipsec_error_invalid_state;
	}
    return p_ctx->pc_plugin->set_remote(p_ctx, spi_pc, spi_ps, port_pc, port_ps, lifetime);
}

/**
* Shutdown all SAs associated to this context. It's no longer allowed to send/recv data after calling this function.
* @param p_ctx Pointer to a context created using @ref tipsec_ctx_create().
* @retval @ref tipsec_error_success if no error; otherwise error code.
*/
tipsec_error_t tipsec_ctx_stop(tipsec_ctx_t* p_ctx)
{
    if (!p_ctx || !p_ctx->pc_plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tipsec_error_invalid_param;
    }
    return p_ctx->pc_plugin->stop(p_ctx);
}

/**
* Register an IPSec special implementation (e.g. Windows XP, Windows Vista or Linux IPSec-Tools) using a static definition.
* @param pc_plugin pointer to the static definition.
* @retval @ref tipsec_error_success if no error; otherwise error code.
* @sa @ref tipsec_plugin_unregister_static(), @ref tipsec_plugin_unregister_file()
*/
tipsec_error_t tipsec_plugin_register_static(const tipsec_plugin_def_t* pc_plugin)
{
    tsk_size_t i;
    if (!pc_plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tipsec_error_invalid_param;
    }

    /* add or replace the plugin */
    for (i = 0; i < TIPSEC_MAX_PLUGINS; ++i) {
        if (!__tipsec_plugins[i] || (__tipsec_plugins[i] == pc_plugin)) {
            __tipsec_plugins[i] = pc_plugin;
            TSK_DEBUG_INFO("Register IPSec implementation: %s", pc_plugin->desc);
            return tipsec_error_success;
        }
    }

    TSK_DEBUG_ERROR("There are already %d plugins.", TIPSEC_MAX_PLUGINS);
    return tipsec_error_outofbound;
}

/**
* UnRegister an IPSec special implementation (e.g. Windows XP, Windows Vista or Linux IPSec-Tools) using a static definition.
* @param pc_plugin pointer to the static definition.
* @retval @ref tipsec_error_success if no error; otherwise error code.
* @sa @ref tipsec_plugin_register_static(), @ref tipsec_plugin_register_file()
*/
tipsec_error_t tipsec_plugin_unregister_static(const tipsec_plugin_def_t* pc_plugin)
{
    tsk_size_t i;
    tsk_bool_t b_found = tsk_false;

    if (!pc_plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tipsec_error_invalid_param;
    }

    /* find the plugin to unregister */
    for (i = 0; i < TIPSEC_MAX_PLUGINS && __tipsec_plugins[i]; ++i) {
        if (__tipsec_plugins[i] == pc_plugin) {
            __tipsec_plugins[i] = tsk_null;
            b_found = tsk_true;
            TSK_DEBUG_INFO("UnRegister IPSec implementation: %s", pc_plugin->desc);
            break;
        }
    }

    /* compact */
    if (b_found) {
        for (; i < (TIPSEC_MAX_PLUGINS - 1); ++i) {
            if (__tipsec_plugins[i+1]) {
                __tipsec_plugins[i] = __tipsec_plugins[i + 1];
            }
            else {
                break;
            }
        }
        __tipsec_plugins[i] = tsk_null;
    }
    return (b_found ? tipsec_error_success : tipsec_error_notfound);
}

/**
* Register an IPSec special implementation (e.g. Windows XP, Windows Vista or Linux IPSec-Tools) using a shared library (*.DLL, *.SO).
* @param pc_filepath Path to the plugin.
* @param pp_plugin Pointer to the newly created plugin. You must call @ref tipsec_plugin_unregister_file() when you no longer need to use the plugin.
* @retval @ref tipsec_error_success if no error; otherwise error code.
* @sa @ref tipsec_plugin_unregister_static(), @ref tipsec_plugin_unregister_file()
*/
tipsec_error_t tipsec_plugin_register_file(const char* pc_filepath, struct tsk_plugin_s** pp_plugin)
{
	struct tsk_plugin_s* p_plugin = tsk_null;
	tsk_plugin_def_ptr_const_t p_def;
	tipsec_error_t err = tipsec_error_success;
	int i = 0, count = 0;

	p_plugin = tsk_plugin_create(pc_filepath);
	if (!p_plugin) {
		return tipsec_error_notfound;
	}
	
	while ((p_def = tsk_plugin_get_def_2(p_plugin, tsk_plugin_def_type_ipsec, tsk_plugin_def_media_type_all, i++))) {
		if (p_def) {
			err = tipsec_plugin_register_static(p_def);
			if (!err) {
				++count;
			}
		}
	}

	if (count <= 0) {
		TSK_DEBUG_ERROR("No plugin in %s", pc_filepath);
		TSK_OBJECT_SAFE_FREE(p_plugin);
		return tipsec_error_notfound;
	}
	
	*pp_plugin = p_plugin;
	return err;
}

/**
* UnRegister an IPSec special implementation (e.g. Windows XP, Windows Vista or Linux IPSec-Tools).
* @param p_plugin Pointer to the plugin previously registered using @ref tipsec_plugin_register_file().
* @retval @ref tipsec_error_success if no error; otherwise error code.
* @sa @ref tipsec_plugin_unregister_static(), @ref tipsec_plugin_unregister_file()
*/
tipsec_error_t tipsec_plugin_unregister_file(struct tsk_plugin_s* p_plugin)
{
	int i = 0;
	tsk_plugin_def_ptr_const_t p_def;
	if (!p_plugin) {
		TSK_DEBUG_ERROR("Invalid parameter");
        return tipsec_error_invalid_param;
	}
	while ((p_def = tsk_plugin_get_def_2(p_plugin, tsk_plugin_def_type_ipsec, tsk_plugin_def_media_type_all, i++))) {
		if (p_def) {
			tipsec_plugin_unregister_static(p_def);
		}
	}
	return tipsec_error_success;
}
