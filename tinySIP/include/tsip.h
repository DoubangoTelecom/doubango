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

/**@file tsip.h
 * @brief SIP (RFC 3261) and 3GPP IMS/LTE (TS 24.229) implementation.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_TSIP_H
#define TINYSIP_TSIP_H

#include "tinysip_config.h"

#include "tinysip/tsip_ssession.h"
#include "tinysip/tsip_timers.h"
#include "tinysip/tsip_event.h"
#include "tinysip/transports/tsip_transport.h"

#include "tinysip/tsip_uri.h"

#include "tinysip/sigcomp/tsip_sigcomp.h"

#include "tinynet.h"

#include "tsk_runnable.h"

TSIP_BEGIN_DECLS

#define TSIP_IARI_GSMAIS				"urn%3Aurn-7%3A3gpp-application.ims.iari.gsma-is"
#define TSIP_IARI_QUOTED_GSMAIS			"\"" TSIP_IARI_GSMAIS "\""
#define TSIP_ICSI_MMTEL_PSVOICE			"urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel"
#define TSIP_ICSI_QUOTED_MMTEL_PSVOICE	"\"" TSIP_ICSI_MMTEL_PSVOICE "\""

#define TSIP_STACK(self)		((tsip_stack_t*)(self))

typedef uint8_t amf_t[2];
typedef uint8_t operator_id_t[16];

// @tinyWRAP
typedef enum tsip_stack_mode_e {
    tsip_stack_mode_ua,
    tsip_stack_mode_p2p,
    tsip_stack_mode_webrtc2sip,
    tsip_stack_mode_mcu
}
tsip_stack_mode_t;
#define TSIP_STACK_MODE_IS_SERVER(stack) ((stack)->network.mode == tsip_stack_mode_webrtc2sip || (stack)->network.mode == tsip_stack_mode_mcu)
#define TSIP_STACK_MODE_IS_CLIENT(stack) (!TSIP_STACK_MODE_IS_SERVER((stack)))

typedef enum tsip_stack_param_type_e {
    tsip_pname_null = 0,

    /* === Identity === */
    tsip_pname_display_name,
    tsip_pname_impu,
    tsip_pname_preferred_id,
    tsip_pname_impi,
    tsip_pname_password,

    /* === SigComp === */
    tsip_pname_sigcomp,
    tsip_pname_sigcomp_add_compartment,
    tsip_pname_sigcomp_remove_compartment,

    /* === Network === */
    tsip_pname_realm,
    tsip_pname_local_ip,
    tsip_pname_local_port,
    tsip_pname_aor,
    tsip_pname_discovery_naptr,
    tsip_pname_discovery_dhcp,
    tsip_pname_proxy_cscf,
    tsip_pname_dnsserver,
    tsip_pname_max_fds,
    tsip_pname_mode,


    /* === Security === */
    tsip_pname_early_ims,
    tsip_pname_secagree_ipsec,
    tsip_pname_secagree_tls,
    tsip_pname_amf,
    tsip_pname_operator_id,
    tsip_pname_tls_certs,
    tsip_pname_ipsec_params,

    /* === Dummy Headers === */
    tsip_pname_header,

    /* Nat Traversal */
    tsip_pname_stun_enabled,
    tsip_pname_stun_server,
    tsip_pname_stun_cred,

    /* === User Data === */
    tsip_pname_userdata,
}
tsip_stack_param_type_t;

/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_NULL
* Ends the stack parameters. Mandatory and should be the last one.
*/
#define TSIP_STACK_SET_NULL()							tsip_pname_null

/* === Identity === */
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_DISPLAY_NAME
* Sets the user's display name. Used in SIP 'From' and 'To' headers.
* @param NAME_STR The user's display name.
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_DISPLAY_NAME("john doe"),
*              TSIP_STACK_SET_NULL());
* @endcode
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_IMPU
* Sets the user's IMPU (IMS Public Identity).
* @param URI_STR A valid SIP/SIPS/tel URI string.
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_IMPU("sip:johndoe@open-ims.test"),
*              TSIP_STACK_SET_NULL());
* @endcode
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_PREFERRED_IDENTITY
* Sets the user's Preferred Identity. Used to populate the P-Preferred-Identity header.
* @param URI_STR A valid SIP/SIPS/tel URI string.
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_PREFERRED_IDENTITY("sip:johndoe@open-ims.test"),
*              TSIP_STACK_SET_NULL());
* @endcode
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_IMPI
* Sets the user's IMPI (IMS Private Identity).
* @param IMPI_STR Any string.
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_IMPI("johndoe@open-ims.test"),
*              TSIP_STACK_SET_NULL());
* @endcode
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_PASSWORD
* Sets the user's password.
* @param PASSORD_STR Any string.
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_PASSWORD("mysecret"),
*              TSIP_STACK_SET_NULL());
* @endcode
*/
#define TSIP_STACK_SET_DISPLAY_NAME(NAME_STR)			tsip_pname_display_name, (const char*)NAME_STR
#define TSIP_STACK_SET_IMPU(URI_STR)					tsip_pname_impu, (const char*)URI_STR
#define TSIP_STACK_SET_PREFERRED_IDENTITY(URI_STR)		tsip_pname_preferred_id, (const char*)URI_STR
#define TSIP_STACK_SET_IMPI(IMPI_STR)					tsip_pname_impi, (const char*)IMPI_STR
#define TSIP_STACK_SET_PASSWORD(PASSORD_STR)			tsip_pname_password, (const char*)PASSORD_STR


/* === SigComp === */
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_SIGCOMP
* Sets the SigComp parameters. It's not mandatory to call this function in order to use SigComp.
* It should be called to change default parameters. As these parameters will be shared by all compartments,
* you should call this function before adding any compartment.
* @param DMS_UINT Decompression Memory Size. Default value is 8192.
* @param SMS_UINT State Memory Size. Default value is 8192.
* @param CPB_UINT Cycles Per Bit. Default value is 64.
* @param PRES_DICT_BOOL Indicates whether to enable Presence dictionary (RFC 5112) or not. Default value is @a tsk_false.
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_SIGCOMP(8192, 8192, 64, tsk_true),
*              TSIP_STACK_SET_NULL());
* @endcode
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_SIGCOMP_NEW_COMPARTMENT
* Add New unique compartment identifier to the stack. This compartment will be closed (not removed) when the stack is stopped.
* A any time, you can remove this compartment by using @ref TSIP_STACK_UNSET_SIGCOMP_COMPARTMENT().
* @param COMPARTMENT_ID_STR The id of the compartment to add. Should be unique.
* The identifier will be used to in the sip headers (as per RFC 5049) "AS IS".<br>
* @sa @ref TSIP_STACK_UNSET_SIGCOMP_COMPARTMENT()
*
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_NEW_SIGCOMP_COMPARTMENT("urn:uuid:0C67446E-F1A1-11D9-94D3-000A95A0E128"),
*              TSIP_STACK_SET_NULL());
* @endcode
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_UNSET_SIGCOMP_COMPARTMENT
* Removes a SigComp compartment. This will close the compartment and release all its states.
* You should only close a compartment if there is no outgoing session using it. In all cases, all compartments
* will be closed when the stack is destroyed or stopped.
* @param COMPARTMENT_ID_STR The id of the compartment to close.
* @sa @ref TSIP_STACK_SET_SIGCOMP_NEW_COMPARTMENT()
*
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_UNSET_SIGCOMP_COMPARTMENT("urn:uuid:0C67446E-F1A1-11D9-94D3-000A95A0E128"),
*              TSIP_STACK_SET_NULL());
* @endcode
*/
#define TSIP_STACK_SET_SIGCOMP(DMS_UINT, SMS_UINT, CPB_UINT, PRES_DICT_BOOL) tsip_pname_sigcomp, (unsigned)DMS_UINT, (unsigned)SMS_UINT, (unsigned)CPB_UINT, (tsk_bool_t)PRES_DICT_BOOL
#define TSIP_STACK_SET_SIGCOMP_NEW_COMPARTMENT(COMPARTMENT_ID_STR)			tsip_pname_sigcomp_add_compartment, (const char*)COMPARTMENT_ID_STR
#define TSIP_STACK_UNSET_SIGCOMP_COMPARTMENT(COMPARTMENT_ID_STR)			tsip_pname_sigcomp_remove_compartment, (const char*)COMPARTMENT_ID_STR

/* === Network === */
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_REALM
* Sets the domain name.
* @param URI_STR A valid SIP/SIPS URI string.
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_REALM("sip:open-ims.test"),
*              TSIP_STACK_SET_NULL());
* @endcode
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_LOCAL_IP
* Sets the user's local IP address.
* @param IP_STR IPv4/IPv6 IP address or FQDN.
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_LOCAL_IP("fe80::"),
*              TSIP_STACK_SET_NULL());
* @endcode
*
* @sa @ref  TSIP_STACK_SET_LOCAL_PORT()<br>@ref TSIP_STACK_SET_AOR()
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_LOCAL_PORT
* Sets the user's local Port to bind to.
* @param PORT_UINT Port number. Should be between 0x0400 and 0xFFFF.
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_LOCAL_PORT(5060),
*              TSIP_STACK_SET_NULL());
* @endcode
*
* @sa @ref TSIP_STACK_SET_LOCAL_IP()<br>@ref TSIP_STACK_SET_AOR()
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_AOR
* Sets the SIP AOR to use for all outgoing requests.
* @param IP_STR The IP address. Should be numeric IP string. e.g. 192.168.0.16.
* Will be ignored if null or empty.
* @param PORT_UINT Port number. Should be between 0x0400 and 0xFFFF.
* Will be ignored if equals to zero.
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_AOR("fe80::", 1234),
*              TSIP_STACK_SET_NULL());
* @endcode
*
* @sa @ref TSIP_STACK_SET_LOCAL_IP()<br>@ref TSIP_STACK_SET_LOCAL_PORT()
*/

/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_DISCOVERY_NAPTR
* Whether to use DNS NAPTR for Proxy-CSCF discovery. Default is false. However, if the Proxy-CSCF IP address
* is missing, then this option will be enabled by default.
* @param ENABLED_BOOL @a tsk_true (1) or @a tsk_false (0).
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_DISCOVERY_NAPTR(tsk_true),
*              TSIP_STACK_SET_NULL());
* @endcode
*
* @sa @ref TSIP_STACK_SET_DISCOVERY_DHCP()
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_DISCOVERY_DHCP
* Whether to use DHCPv4/v6 for Proxy-CSCF discovery. Default is false. The IP version (v4/v6) of the DHCP protocol
* to use will depend on the Proxy-CSCF address type.
* @param ENABLED_BOOL @a tsk_true (1) or @a tsk_false (0).
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_DISCOVERY_DHCP(tsk_false),
*              TSIP_STACK_SET_NULL());
* @endcode
*
* @sa @ref TSIP_STACK_SET_DISCOVERY_NAPTR()
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_PROXY_CSCF
* Configures the Proxy-CSCF.
* @param FQDN_STR The IP address (v4/v6) or FQDN of the Proxy-CSCF.
* @param PORT_UINT The port.
* @param TRANSPORT_STR The transport type. Should be: @a "udp" or @a "tcp" or @a "tls" or @a "sctp". Default is @a "udp".
* @param IP_VERSION_STR The IP version to use. Should be:  @a "ipv4" or @a "ipv6" or @a "ipv46". This parameter is useful when
* @a FQDN_STR parameter is a domain name. default is @a "ipv4".
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_PROXY_CSCF("pcscf-doubango.org", 4060, "tcp", "ipv6"),
*              TSIP_STACK_SET_NULL());
* @endcode
*
* @sa @ref TSIP_STACK_SET_DISCOVERY_DHCP()<br>@ref TSIP_STACK_SET_DISCOVERY_NAPTR()
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_DNS_SERVER
* Adds a DNS server.
* @param IP_STR The IP address or FQDN of the DNS server to add.
* @code
 // Add two new DNS servers (IPv6 and IPv4)
int ret = tsip_stack_set(stack,
              TSIP_STACK_SET_DNS_SERVER("192.16.16.9"),
			  TSIP_STACK_SET_DNS_SERVER("fe::"),
              TSIP_STACK_SET_NULL());
* @endcode
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_MODE_SERVER
* Configure the stack to be used as server. Cannot be changed later.
* @code
int ret = tsip_stack_set(stack,
              TSIP_STACK_SET_MODE_SERVER(),
              TSIP_STACK_SET_NULL());
* @endcode
*/
#define TSIP_STACK_SET_REALM(URI_STR)															tsip_pname_realm, (const char*)URI_STR
#define TSIP_STACK_SET_LOCAL_IP_2(TRANSPORT_STR, IP_STR)										tsip_pname_local_ip, (const char*)TRANSPORT_STR, (const char*)IP_STR
#define TSIP_STACK_SET_LOCAL_PORT_2(TRANSPORT_STR, PORT_UINT)									tsip_pname_local_port, (const char*)TRANSPORT_STR, (unsigned)PORT_UINT
#define TSIP_STACK_SET_LOCAL_IP(IP_STR)															TSIP_STACK_SET_LOCAL_IP_2(tsk_null, IP_STR)// @deprecated
#define TSIP_STACK_SET_LOCAL_PORT(PORT_UINT)													TSIP_STACK_SET_LOCAL_PORT_2(tsk_null, PORT_UINT)// @deprecated
#define TSIP_STACK_SET_AOR_2(TRANSPORT_STR, IP_STR, PORT_UINT)									tsip_pname_aor, (const char*)TRANSPORT_STR, (const char*)IP_STR, (unsigned)PORT_UINT
#define TSIP_STACK_SET_AOR(IP_STR, PORT_UINT)													TSIP_STACK_SET_AOR_2(tsk_null, IP_STR, PORT_UINT)// @deprecated
#define TSIP_STACK_SET_DISCOVERY_NAPTR(ENABLED_BOOL)											tsip_pname_discovery_naptr, (tsk_bool_t)ENABLED_BOOL
#define TSIP_STACK_SET_DISCOVERY_DHCP(ENABLED_BOOL)												tsip_pname_discovery_dhcp, (tsk_bool_t)ENABLED_BOOL
#define TSIP_STACK_SET_PROXY_CSCF(FQDN_STR, PORT_UINT, TRANSPORT_STR, IP_VERSION_STR)			tsip_pname_proxy_cscf, (const char*)FQDN_STR, (unsigned)PORT_UINT, (const char*)TRANSPORT_STR, (const char*)IP_VERSION_STR
#define TSIP_STACK_SET_DNS_SERVER(IP_STR)														tsip_pname_dnsserver, (const char*)IP_STR
#define TSIP_STACK_SET_MAX_FDS(MAX_FDS_UINT)													tsip_pname_max_fds, (unsigned)MAX_FDS_UINT
#define TSIP_STACK_SET_MODE(MODE_ENUM)															tsip_pname_mode, (tsip_stack_mode_t)MODE_ENUM

/* === Security === */
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_EARLY_IMS
* Whether to enable 3GPP Early-IMS Security as per 3GPP TS 33.978.
* @param ENABLED_BOOL @a tsk_true (1) or @a tsk_false (0).
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_EARLY_IMS(tsk_false),
*              TSIP_STACK_SET_NULL());
* @endcode
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_SECAGREE_IPSEC
* Whether to enable IPSec security agreement as per IETF RFC 3329.
* @param ENABLED_BOOL @a tsk_true (1) or @a tsk_false (0).
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_SECAGREE_IPSEC(tsk_true),
*              TSIP_STACK_SET_NULL());
* @endcode
*
* @sa @ref TSIP_STACK_SET_IPSEC_PARAMS()
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_SECAGREE_TLS
* Whether to enable TLS security agreement as per IETF RFC 3329.
* @param ENABLED_BOOL @a tsk_true (1) or @a tsk_false (0).
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_SECAGREE_TLS(tsk_true),
*              TSIP_STACK_SET_NULL());
* @endcode
*
* @sa @ref TSIP_STACK_SET_IPSEC_PARAMS()
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_IMS_AKA_AMF
* Sets AMF (Authentication Management Field) value to use for IMS-AKA authentication.
* @param AMF_UINT16 16-bit value.
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_IMS_AKA_AMF(0x3FB6),
*              TSIP_STACK_SET_NULL());
* @endcode
*
* @sa @ref TSIP_STACK_SET_IMS_AKA_OPERATOR_ID()
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_IMS_AKA_OPERATOR_ID
* Sets the operator identifier value to use for IMS-AKA authentication.
* @param OPID_HEX_STR 128-bit value as hex-string. If the length of the value supplied by the user is less than 128 bits,
* then it will be padded with zeros.
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_IMS_AKA_OPERATOR_ID("0x5FABC9"), // Will be padded with zeros
*              TSIP_STACK_SET_NULL());
* @endcode
*
* @sa @ref TSIP_STACK_SET_IMS_AKA_AMF()
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_IPSEC_PARAMS
* Sets the IPSec parameters. To enable IPSec security agreement you should use @ref TSIP_STACK_SET_SECAGREE_IPSEC(tsk_true).
* @param ALG_STR Algorithm. Should be: @a "hmac-sha-1-96" or @a "hmac-md5-96". Default is @a "hmac-md5-96".
* @param EALG_STR Encryption Algorithm. Should be: @a "des-ede3-cbc" or @a "aes" or @a "null". Default is @a "null".
* @param MODE_STR Mode. Should be: @a "tun" or @a "trans". Default is @a "trans".
* @param PROTOCOL_STR Protocol. Should be @a "ah" or @a "esp" or @a "ah/esp". Default is @a "esp".
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_SECAGREE_IPSEC(tsk_true),
               TSIP_STACK_SET_IPSEC_PARAMS("hmac-md5-96", "des-ede3-cbc", "trans", "esp"),
*              TSIP_STACK_SET_NULL());
* @endcode
*
* @sa @ref TSIP_STACK_SET_TLS_CERTS()
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_TLS_CERTS
* Sets TLS certificate files. To enable TLS security agreement you should use @ref TSIP_STACK_SET_SECAGREE_TLS(tsk_true).
* @param CA_FILE_STR CA file.
* @param PUB_FILE_STR Public cert. file.
* @param PRIV_FILE_STR Private cert. file.
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_TLS_CERTS("C:\\tls\\ca.pki-crt.pem", "C:\\tls\\pub-crt.pem", "C:\\tls\\priv-key.pem"),
*              TSIP_STACK_SET_NULL());
* @endcode
*
* @sa @ref TSIP_STACK_SET_IPSEC_PARAMS()
*/
#define TSIP_STACK_SET_EARLY_IMS(ENABLED_BOOL)												tsip_pname_early_ims, (tsk_bool_t)ENABLED_BOOL
#define TSIP_STACK_SET_SECAGREE_IPSEC_2(TRANSPORT_STR, ENABLED_BOOL)						tsip_pname_secagree_ipsec, (const char*)TRANSPORT_STR, (tsk_bool_t)ENABLED_BOOL
#define TSIP_STACK_SET_SECAGREE_IPSEC(ENABLED_BOOL)											TSIP_STACK_SET_SECAGREE_IPSEC_2(tsk_null, ENABLED_BOOL) // @deprecated
#define TSIP_STACK_SET_SECAGREE_TLS(ENABLED_BOOL)											tsip_pname_secagree_tls, (tsk_bool_t)ENABLED_BOOL
#define TSIP_STACK_SET_IMS_AKA_AMF(AMF_UINT16)												tsip_pname_amf, (uint16_t)AMF_UINT16
#define TSIP_STACK_SET_IMS_AKA_OPERATOR_ID(OPID_HEX_STR)									tsip_pname_operator_id, (const char*)OPID_HEX_STR
#define TSIP_STACK_SET_IPSEC_PARAMS(ALG_STR, EALG_STR, MODE_STR, PROTOCOL_STR)				tsip_pname_ipsec_params, (const char*)ALG_STR, (const char*)EALG_STR, (const char*)MODE_STR, (const char*)PROTOCOL_STR
#define TSIP_STACK_SET_TLS_CERTS(CA_FILE_STR, PUB_FILE_STR, PRIV_FILE_STR)					TSIP_STACK_SET_TLS_CERTS_2(CA_FILE_STR, PUB_FILE_STR, PRIV_FILE_STR, tsk_false)
#define TSIP_STACK_SET_TLS_CERTS_2(CA_FILE_STR, PUB_FILE_STR, PRIV_FILE_STR, VERIF_BOOL)	tsip_pname_tls_certs, (const char*)CA_FILE_STR, (const char*)PUB_FILE_STR, (const char*)PRIV_FILE_STR, (tsk_bool_t)VERIF_BOOL

/* === Headers === */
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_HEADER
* Adds or updates a stack-level header.
* @param NAME_STR The name of the header to add or update.
* @param VALUE_STR The value of the header to add or update.
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_HEADER("User-Agent", "IM-client/OMA1.0 doubango/v1.0.0"),
*              TSIP_STACK_SET_HEADER("Allow", "INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER"),
*              TSIP_STACK_SET_NULL());
* @endcode
*
* @sa @ref TSIP_STACK_UNSET_HEADER()
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_HEADER
* Removes a stack-level header.
* @param NAME_STR The name of the header to remove.
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_UNSET_HEADER("User-Agent"),
*              TSIP_STACK_UNSET_HEADER("Allow"),
*              TSIP_STACK_UNSET_HEADER("My_Header"),
*              TSIP_STACK_SET_NULL());
* @endcode
*
* @sa @ref TSIP_STACK_SET_HEADER()
*/
#define TSIP_STACK_SET_HEADER(NAME_STR, VALUE_STR)		tsip_pname_header, (const char*)NAME_STR, (const char*)VALUE_STR
#define TSIP_STACK_UNSET_HEADER(NAME_STR)				TSIP_STACK_SET_HEADER(NAME_STR, ((const char*)-1))

/* === NAT Traversal === */
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_STUN_SERVER
* Sets the IP address and port of the STUN2 server. Also used for TURN.
* @param IP_STR The IPv4/IPv6 address for FQDN of the STUN2/TURN address.
* @param PORT_UINT The server port (default is 3478 for both TCP and UDP, and 5349 for TLS)
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_STUN_SERVER("numb.viagenie.ca", 3478),
*              TSIP_STACK_SET_NULL());
* @endcode
*
* @sa @ref TSIP_STACK_SET_STUN_CRED()
*/
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_STUN_CRED
* Sets STUN/TURN credentials.
* @param USR_STR The login.
* @param PASSORD_STR The password
* @code
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_STUN_CRED("bob@open-ims.test", "mysecret"),
*              TSIP_STACK_SET_NULL());
* @endcode
*
* @sa @ref TSIP_STACK_SET_STUN_SERVER()
*/
#define TSIP_STACK_SET_STUN_SERVER(IP_STR, PORT_UINT)	tsip_pname_stun_server, (const char*)IP_STR, (unsigned)PORT_UINT
#define TSIP_STACK_SET_STUN_CRED(USR_STR, PASSORD_STR)	tsip_pname_stun_cred, (const char*)USR_STR, (const char*)PASSORD_STR
#define TSIP_STACK_SET_STUN_ENABLED(ENABLED_BOOL)		tsip_pname_stun_enabled, (tsk_bool_t)ENABLED_BOOL

/* === User Data === */
/**@ingroup tsip_stack_group
* @def TSIP_STACK_SET_USERDATA
* Sets opaque user's data returned in each callback. Could be used to store the context.
* @param DATA_PTR Pointer to the user data.
* @code
* const void* context;
* int ret = tsip_stack_set(stack,
*              TSIP_STACK_SET_USERDATA(context),
*              TSIP_STACK_SET_NULL());
* @endcode
*/
#define TSIP_STACK_SET_USERDATA(DATA_PTR)		tsip_pname_userdata, (const void*)DATA_PTR




/* 3GPP IMS/LTE stack (for internal use). only tsip_stack_handle_t should be visible. */
typedef struct tsip_stack_s {
    TSK_DECLARE_RUNNABLE;
    TSK_DECLARE_SAFEOBJ;

    tsk_timer_manager_handle_t* timer_mgr_global;
    tsk_bool_t started;
    tsip_stack_callback_f callback;

    /* === Identity === */
    struct {
        char* display_name;
        tsip_uri_t *impu;
        tsip_uri_t *preferred;
        char *impi;
        char *password;
    } identity;

    /* === SigComp === */
    struct {
        tsip_sigcomp_handle_t* handle;

        unsigned dms;
        unsigned sms;
        unsigned cpb;
        tsk_bool_t sip_dict;
        tsk_bool_t pres_dict;
    } sigcomp;

    /* === Network === */
    struct {
        tsip_stack_mode_t mode;

        char *local_ip[TSIP_TRANSPORT_IDX_MAX];
        tnet_port_t local_port[TSIP_TRANSPORT_IDX_MAX];
        tsk_bool_t local_ip_is_set_by_user[TSIP_TRANSPORT_IDX_MAX]; // whether the address (ip and port) is set by the user or retrieved

        char *proxy_cscf[TSIP_TRANSPORT_IDX_MAX];
        tnet_port_t proxy_cscf_port[TSIP_TRANSPORT_IDX_MAX];
        tnet_socket_type_t proxy_cscf_type[TSIP_TRANSPORT_IDX_MAX];
        tnet_socket_type_t transport_types[TSIP_TRANSPORT_IDX_MAX];
        int32_t transport_idx_default;

        tsip_uri_t *realm;

        //! IP adddress and port to use as AOR (user-defined)
        struct {
            char* ip[TSIP_TRANSPORT_IDX_MAX];
            tnet_port_t port[TSIP_TRANSPORT_IDX_MAX];
        } aor;

        tsk_bool_t discovery_naptr;
        tsk_bool_t discovery_dhcp;

        tsk_size_t max_fds;
    } network;

    /* === Security === */
    struct {
        char* secagree_mech;
        tsk_bool_t earlyIMS;
        operator_id_t operator_id;
        amf_t amf;

        /* IPSec */
        struct {
            char* alg;
            char* ealg;
            char* mode;
            char* protocol;
        } ipsec;
        tsk_bool_t enable_secagree_ipsec;

        /* TLS */
        struct {
            char* ca;
            char* pbk;
            char* pvk;
            tsk_bool_t verify;
        } tls;
        tsk_bool_t enable_secagree_tls;
    } security;








    tsip_uris_L_t* paths;
    tsip_uris_L_t* service_routes;
    tsip_uris_L_t* associated_uris;

    /* DNS context */
    tnet_dns_ctx_t *dns_ctx;

    /* NAT Traversal context */
    struct {
        // STUN
        struct {
            char* ip;
            tnet_port_t port;
            char* login;
            char* pwd;
            tsk_bool_t enabled;
        } stun;
        struct tnet_nat_ctx_s* ctx;
    } natt;

    /* DHCP context */

    /* QoS */

    /* Internals. */
    //tsk_timer_manager_handle_t* timer_mgr;
    tsip_timers_t timers;
    tsip_ssessions_L_t *ssessions;
    tsk_params_L_t *headers;
    const void* userdata;

    /* Layers */
    struct tsip_dialog_layer_s *layer_dialog;
    struct tsip_transac_layer_s *layer_transac;
    struct tsip_transport_layer_s *layer_transport;
}
tsip_stack_t;




TINYSIP_API tsip_stack_handle_t *tsip_stack_create(tsip_stack_callback_f callback, const char* realm_uri, const char* impi_uri, const char* impu_uri, ...);
TINYSIP_API int tsip_stack_start(tsip_stack_handle_t *self);
TINYSIP_API int tsip_stack_set(tsip_stack_handle_t *self, ...);
TINYSIP_API const void* tsip_stack_get_userdata(const tsip_stack_handle_t *self);
TINYSIP_API tnet_dns_ctx_t* tsip_stack_get_dnsctx(tsip_stack_handle_t *self);
TINYSIP_API tsip_uri_t* tsip_stack_get_preferred_id(tsip_stack_handle_t *self);
TINYSIP_API int tsip_stack_get_local_ip_n_port(const tsip_stack_handle_t *self, const char* protocol, tnet_port_t *port, tnet_ip_t *ip);
TINYSIP_API int tsip_stack_stop(tsip_stack_handle_t *self);

#define TSIP_STACK_EVENT_RAISE(stack, status_code, reason_phrase, incoming, type) \
	TSK_RUNNABLE_ENQUEUE(TSK_RUNNABLE(stack), (const tsip_stack_handle_t*)stack, (short)status_code, (const char*)reason_phrase, (unsigned)incoming, (tsip_event_type_t)type);


TINYSIP_GEXTERN const tsk_object_def_t *tsip_stack_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_TSIP_H */
