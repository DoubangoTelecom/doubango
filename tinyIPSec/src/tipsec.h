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

/**@file tipsec.h
 * @brief IPSec plugin and context managers.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 */
#ifndef TINYIPSEC_IPSEC_H
#define TINYIPSEC_IPSEC_H

#include "tinyipsec_config.h"

#include "tsk_string.h"

TIPSEC_BEGIN_DECLS

// Forward declaration
struct tsk_plugin_s;

/** Converts any IPSec context (XP, Vista, Linux IPSec-Tools ...) to the common IPSec context.
* @param self The context to convert. MUST be declared using @ref TIPSEC_DECLARE_CTX.
* @retval A pointer to @ref tipsec_ctx_t.
*/
#define TIPSEC_CTX(self)		((tipsec_ctx_t*)(self))

/**@def TIPSEC_IPPROTO_FROM_STR
* Converts IPSec IP protocol string to enum value.
* @param str_ipproto Must be "tcp", "udp" or "icmp"
* @retval @ref tipsec_ipproto_t value.
*/
/**@def TIPSEC_IPPROTO_TO_STR
* Converts IPSec IP protocol enum to string value.
* @param enum_ipproto @ref tipsec_ipproto_t value.
* @retval "tcp", "udp" or "icmp" string value.
*/
#define TIPSEC_IPPROTO_FROM_STR(str_ipproto) (tsk_strequals(str_ipproto, "tcp") ? tipsec_ipproto_tcp : (tsk_strequals(str_ipproto, "icmp") ? tipsec_ipproto_icmp : tipsec_ipproto_udp))
#define TIPSEC_IPPROTO_TO_STR(enum_ipproto)	(enum_ipproto == tipsec_ipproto_tcp ? "tcp" : (enum_ipproto == tipsec_ipproto_icmp ? "icmp" : "udp"))


/**@def TIPSEC_MODE_FROM_STR
* Converts IPSec mode string to enum value.
* @param str_mode Must be "tun" (tunnel) or "trans" (transport).
* @retval @ref tipsec_mode_t value.
*/
/**@def TIPSEC_MODE_TO_STR
* Converts IPSec mode enum to string value.
* @param enum_mode @ref tipsec_mode_t value.
* @retval "tun" (tunnel) or "trans" (transport) string value.
*/
#define TIPSEC_MODE_FROM_STR(str_mode) (tsk_strequals(str_mode, "tun") ? tipsec_mode_tun : tipsec_mode_trans)
#define TIPSEC_MODE_TO_STR(enum_mode)	(enum_mode == tipsec_mode_tun ? "tun" : "trans")

/** @def TIPSEC_EALG_FROM_STR
* Converts IPSec encryption algorithm string to enum value.
* @param str_ealg Must be "des-ede3-cbc", "aes" or "null".
* @retval @ref tipsec_ealg_t value.
*/
/**@def TIPSEC_EALG_TO_STR
* Converts IPSec encryption algorithm enum to string value.
* @param enum_ealg @ref tipsec_ealg_t value.
* @retval "des-ede3-cbc", "aes" or "null" string value.
*/
#define TIPSEC_EALG_FROM_STR(str_ealg) (tsk_strequals(str_ealg, "des-ede3-cbc") ? tipsec_ealg_des_ede3_cbc : (tsk_strequals(str_ealg, "aes-cbc") ? tipsec_ealg_aes : tipsec_ealg_null))
#define TIPSEC_EALG_TO_STR(enum_ealg)	(enum_ealg == tipsec_ealg_des_ede3_cbc ? "des-ede3-cbc" : (enum_ealg == tipsec_ealg_aes ? "aes-cbc" : "null"))


/** @def TIPSEC_ALG_FROM_STR
* Converts IPSec algorithm string to enum value.
* @param str_alg Must be "hmac-sha-1-96" or "hmac-md5-96".
* @retval @ref tipsec_alg_t value.
*/
/**@def TIPSEC_ALG_TO_STR
* Converts IPSec algorithm enum to string value.
* @param enum_alg @ref tipsec_alg_t value.
* @retval "hmac-sha-1-96" or "hmac-md5-96" string value.
*/
#define TIPSEC_ALG_FROM_STR(str_alg) (tsk_strequals(str_alg, "hmac-sha-1-96") ? tipsec_alg_hmac_sha_1_96 : tipsec_alg_hmac_md5_96)
#define TIPSEC_ALG_TO_STR(enum_alg)	(enum_alg == tipsec_alg_hmac_sha_1_96 ? "hmac-sha-1-96" : "hmac-md5-96")

/**@def TIPSEC_PROTOCOL_FROM_STR
* Converts IPSec protocol string to enum value.
* @param str_protocol Must be "ah", "esp" or "ah/esp".
* @retval @ref tipsec_proto_t value.
*/
/**@def TIPSEC_PROTOCOL_TO_STR
* Converts IPSec protocol enum to string value.
* @param enum_protocol @ref tipsec_proto_t value.
* @retval "ah", "esp" or "ah/esp" string value.
*/
#define TIPSEC_PROTOCOL_FROM_STR(str_protocol) (tsk_strequals(str_protocol, "ah") ? tipsec_proto_ah : ((tsk_strequals(str_protocol, "ah/esp")) ? tipsec_proto_both : tipsec_proto_esp))
#define TIPSEC_PROTOCOL_TO_STR(enum_protocol)	(enum_protocol == tipsec_proto_ah ? "ah" : (enum_protocol == tipsec_proto_both ? "ah/esp" : "esp"))

/**@def TIPSEC_KEY_LEN
* Default size for IK (Integrity Key) and CK (Confidentiality Key).
**/
/**@def TIPSEC_CK_LEN
* Size of CK (Confidentiality Key).
*/
/**@def TIPSEC_IK_LEN
* Size of IK (Integrity Key).
*/
#define TIPSEC_KEY_LEN			16
#define TIPSEC_IK_LEN			20
#define TIPSEC_CK_LEN			24

/**@def tipsec_lifetime_t
*/
/**@def tipsec_spi_t
*/
/**@def tipsec_port_t
*/
/**@def tipsec_key_t
*/
typedef uint64_t tipsec_lifetime_t;
typedef uint32_t tipsec_spi_t;
typedef uint16_t tipsec_port_t;
typedef void tipsec_key_t;

/**@ingroup tipsec_common_group
 * List of IPSec modes.
**/
typedef enum tipsec_mode_e {
    //! IPSec transport mode.
    tipsec_mode_trans,
    //! IPSec tunneling mode.
    tipsec_mode_tun
}
tipsec_mode_t;

/** List of supported IPSec protocols.
**/
typedef enum tipsec_proto_e {
    //! AH protocol ("ah").
    tipsec_proto_ah = (0x01 << 0),
    //! ESP protocol ("esp").
    tipsec_proto_esp = (0x01 << 1),
    //! Both AH and ESP protocols ("ah/esp").
    tipsec_proto_both = (tipsec_proto_ah | tipsec_proto_esp)
}
tipsec_proto_t;

/**List of supported Internet protocols for IPSec.
**/
typedef enum tipsec_ipproto_e {
    //! UDP.
    tipsec_ipproto_udp,
    //! TCP.
    tipsec_ipproto_tcp,
    //! ICMP.
    tipsec_ipproto_icmp,
    //! ALL IP protocols
    tipsec_ipproto_all
}
tipsec_ipproto_t;

/**List of IPSec IPSec algorithms.
**/
typedef enum tipsec_alg_e {
    //! "hmac-md5-96" algorithm.
    tipsec_alg_hmac_md5_96,
    //! "hmac-sha-1-96" algorithm.
    tipsec_alg_hmac_sha_1_96
}
tipsec_alg_t;

/**List of supported IPSec encryption algorithms.
**/
typedef enum tipsec_ealg_e {
    //! "des-ede3-cbc" encryption algorithm.
    tipsec_ealg_des_ede3_cbc,
    //! "aes" encryption algorithm.
    tipsec_ealg_aes,
    //! "null" encryption algorithm.
    tipsec_ealg_null
}
tipsec_ealg_t;

/** List of IPSec states.
**/
typedef enum tipsec_state_e {
    //! The default state. At this state no SA is created. It's the first and default state.
    tipsec_state_initial,
    //! Partial state. At this state only inbound SAs (with their SPIs) have been created.
    tipsec_state_inbound,
    //! Full state. At this state both inbound and outbound SAs have been create. It's the final state.
    tipsec_state_full,
    //! All SAs are in active mode.
    tipsec_state_active
}
tipsec_state_t;

/** List of supported IPSec errors
*/
typedef enum tipsec_error_e {
    tipsec_error_success = 0, /**< Success */
    tipsec_error_invalid_param, /**< Invalid parameter */
    tipsec_error_invalid_state, /**< Invalid state */
    tipsec_error_access_violation, /**< Access violation */
    tipsec_error_permission_denied, /**< Permission denied */
    tipsec_error_outofmemory, /**< Out of memory */
    tipsec_error_outofbound, /**< Out of bound */
    tipsec_error_notfound, /**< Not found */
    tipsec_error_notimplemented, /**< Not implemented */
    tipsec_error_sys, /**< System error */
}
tipsec_error_t;

/** List of supported IPSec implementations
*/
typedef enum tipsec_impl_type_e {
    //! Windows XP only. This implementation works with IPv6 only.
    tipsec_impl_type_xp,
    //! Windows Vista or later. Using Windows Filtering Platform (http://msdn.microsoft.com/en-us/windows/hardware/gg463267.aspx).
    tipsec_impl_type_vista,
    //! Linux IPSec tools (http://ipsec-tools.sourceforge.net/)
    tipsec_impl_type_ltools,
}
tipsec_impl_type_t;

/**
* Base IPSec context wrapping special implementation.
* An instance of this object must be created using @ref tipsec_ctx_create() and destroyed using @ref TSK_OBJECT_SAFE_FREE().
*/
typedef struct tipsec_ctx_s {
    TSK_DECLARE_OBJECT;

    //! Indicates whether the context have been initialized or not.
    unsigned initialized;
    //! Indicates whether the context have been started or not.
    unsigned started:1;

    //! The current state of the IPSec context.
    tipsec_state_t state;

    //! Indicates whether to use IPv6 addresses or not.
    unsigned use_ipv6:1;
    //! The network protocol.
    tipsec_ipproto_t ipproto;

    //! IPSec mode.
    tipsec_mode_t mode;
    //! Encrypt algorithm ().
    tipsec_ealg_t ealg;
    //! Algorithm.
    tipsec_alg_t alg;
    //! IPSec protocol.
    tipsec_proto_t protocol;

    //! Remote address (Proxy-CSCF).
    void* addr_remote;
    //! Proxy-CSCF client SPI (Security Parameter Index).
    tipsec_spi_t spi_pc;
    //! Proxy-CSCF server SPI (Security Parameter Index).
    tipsec_spi_t spi_ps;
    //! Proxy-CSCF client port.
    tipsec_port_t port_pc;
    //! Proxy-CSCF server port.
    tipsec_port_t port_ps;

    //! Local address (UE).
    void* addr_local;
    //! UE client SPI (Security Parameter Index). On Windows Vista and later it's up to the OS to set this value.
    tipsec_spi_t spi_uc;
    //! UE server SPI (Security Parameter Index). On Windows Vista and later it's up to the OS to set this value.
    tipsec_spi_t spi_us;
    //! UE client port.
    tipsec_port_t  port_uc;
    //! UE server port.
    tipsec_port_t port_us;

    //! The confidentiality key.
    tipsec_key_t *ck;
    //! The integrity key.
    tipsec_key_t *ik;

    //! reg-await-auth timer value (in seconds).
    tipsec_lifetime_t lifetime;

    //! Reference to the plugin used to create this context.
    const struct tipsec_plugin_def_s* pc_plugin;
}
tipsec_ctx_t;

/** Declare a struct as a context. Used to simulate inheritence. */
#define TIPSEC_DECLARE_CTX tipsec_ctx_t __ipsec_ctx__

/** Virtual table used to define a special IPSec implentation (XP, Vista or Linux IPSec Tools) plugin */
typedef struct tipsec_plugin_def_s {
    //! object definition used to create an instance of the special implementation
    const tsk_object_def_t* objdef;

    //! the type of the consumer
    enum tipsec_impl_type_e type;
    //! full description (usefull for debugging)
    const char* desc;

    tipsec_error_t (* init) (tipsec_ctx_t* );
    tipsec_error_t (* set_local) (tipsec_ctx_t* , const char* addr_local, const char* addr_remote, tipsec_port_t port_uc, tipsec_port_t port_us);
    tipsec_error_t (* set_remote) (tipsec_ctx_t* , tipsec_spi_t spi_pc, tipsec_spi_t spi_ps, tipsec_port_t port_pc, tipsec_port_t port_ps, tipsec_lifetime_t lifetime);
    tipsec_error_t (* set_keys) (tipsec_ctx_t* , const tipsec_key_t* ik, const tipsec_key_t* ck);
    tipsec_error_t (* start) (tipsec_ctx_t* );
    tipsec_error_t (* stop) (tipsec_ctx_t* );
}
tipsec_plugin_def_t;


TINYIPSEC_API tipsec_error_t tipsec_ctx_create(
    tipsec_ipproto_t ipproto,
    tsk_bool_t use_ipv6,
    tipsec_mode_t mode,
    tipsec_ealg_t ealg,
    tipsec_alg_t alg,
    tipsec_proto_t protocol,
    tipsec_ctx_t** pp_ctx);
TINYIPSEC_API tipsec_error_t tipsec_ctx_start(tipsec_ctx_t* p_ctx);
TINYIPSEC_API tipsec_error_t tipsec_ctx_set_local(tipsec_ctx_t* p_ctx, const char* addr_local, const char* addr_remote, tipsec_port_t port_uc, tipsec_port_t port_us);
TINYIPSEC_API tipsec_error_t tipsec_ctx_set_keys(tipsec_ctx_t* p_ctx, const tipsec_key_t* ik, const tipsec_key_t* ck);
TINYIPSEC_API tipsec_error_t tipsec_ctx_set_remote(tipsec_ctx_t* p_ctx, tipsec_spi_t spi_pc, tipsec_spi_t spi_ps, tipsec_port_t port_pc, tipsec_port_t port_ps, tipsec_lifetime_t lifetime);
TINYIPSEC_API tipsec_error_t tipsec_ctx_stop(tipsec_ctx_t* p_ctx);

TINYIPSEC_API tipsec_error_t tipsec_plugin_register_static(const tipsec_plugin_def_t* pc_plugin);
TINYIPSEC_API tipsec_error_t tipsec_plugin_unregister_static(const tipsec_plugin_def_t* pc_plugin);
TINYIPSEC_API tipsec_error_t tipsec_plugin_register_file(const char* pc_filepath, struct tsk_plugin_s** pp_plugin);
TINYIPSEC_API tipsec_error_t tipsec_plugin_unregister_file(struct tsk_plugin_s* pp_plugin);



TIPSEC_END_DECLS

#endif /* TINYIPSEC_IPSEC_H */
