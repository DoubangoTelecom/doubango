/*
 * Copyright (C) 2010-2011 Mamadou Diop.
 * Copyright (C) 2012 Doubango Telecom <http://doubango.org>
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

/**@file tsip.c
 * @brief SIP (RFC 3261) and 3GPP IMS/LTE (TS 24.229) implementation.
 */
#include "tsip.h"

#include "tinysip/tsip_event.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tinysip/transactions/tsip_transac_layer.h"
#include "tinysip/dialogs/tsip_dialog_layer.h"
#include "tinysip/transports/tsip_transport_layer.h"

#include "tinysip/api/tsip_api_register.h"
#include "tinysip/api/tsip_api_subscribe.h"
#include "tinysip/api/tsip_api_message.h"

#include "tinymedia/tmedia_defaults.h"

#include "tnet.h"

#include "tsk_memory.h"
#include "tsk_debug.h"
#include "tsk_time.h"

#include <stdarg.h>
#include <string.h>

static void* TSK_STDCALL run(void* self);

/* For tests:
 * http://www.voip-info.org/wiki/view/PROTOS+Test-Suite
 * http://tools.ietf.org/html/rfc4475
 * http://portal.etsi.org/docbox/EC_Files/EC_Files/ts_10202702v030101p.pdf
 */


/**@defgroup tsip_stack_group 3GPP IMS/LTE Stack
 */

extern tsip_event_t* tsip_event_create(tsip_ssession_t* ss, short code, const char* phrase, const tsip_message_t* sipmessage, tsip_event_type_t type);
#define TSIP_STACK_SIGNAL(self, code, phrase) \
{ \
tsip_event_t* e; \
if((e = tsip_event_create(tsk_null, code, phrase, tsk_null, tsip_event_stack))){ \
TSK_RUNNABLE_ENQUEUE_OBJECT(TSK_RUNNABLE(self), e); \
} \
}

static int __tsip_stack_get_transport_idx_by_name(tsip_stack_t *self, const char* name)
{
    if(tsk_strnullORempty(name) && TSIP_STACK_MODE_IS_CLIENT(self)) {
        return self->network.transport_idx_default; // for backward compatibility
    }
    return tsip_transport_get_idx_by_name(name);
}

/* Internal function used to set all user's parameters */
static int __tsip_stack_set(tsip_stack_t *self, va_list* app)
{
    tsip_stack_param_type_t curr;

    while((curr = va_arg(*app, tsip_stack_param_type_t)) != tsip_pname_null) {
        switch(curr) {

            /* === Identity === */
        case tsip_pname_display_name: {
            /* (const char*)NAME_STR */
            const char* NAME_STR = va_arg(*app, const char*);
            tsk_strupdate(&self->identity.display_name, NAME_STR);
			if (self->identity.impu) {
				tsk_strupdate(&self->identity.impu->display_name, NAME_STR);
			}
            break;
        }
        case tsip_pname_impu:
        case tsip_pname_preferred_id: {
            /* (const char*)URI_STR */
            const char* URI_STR = va_arg(*app, const char*);
            if(!tsk_strnullORempty(URI_STR)) {
                tsip_uri_t *uri = tsip_uri_parse(URI_STR, tsk_strlen(URI_STR));
                if(uri) {
					tsk_strupdate(&uri->display_name, self->identity.display_name);
                    if(curr == tsip_pname_impu) {
                        TSK_OBJECT_SAFE_FREE(self->identity.impu);
                        self->identity.impu = uri;
                    }
                    else {
                        TSK_OBJECT_SAFE_FREE(self->identity.preferred);
                        self->identity.preferred = uri;
                    }
                }
                else {
                    TSK_DEBUG_ERROR("'%s' is an invalid SIP/TEL URI", URI_STR);
                    if(curr == tsip_pname_impu) {
                        return -1; /* IMPU is mandatory but P-Preferred-Identity isn't. */
                    }
                }
            }
            else if(curr == tsip_pname_impu) {
                TSK_DEBUG_ERROR("IMPU (IMS Public Identity) is mandatory.");
                return -1;
            }
            break;
        }
        case tsip_pname_impi: {
            /* (const char*)IMPI_STR */
            const char* IMPI_STR = va_arg(*app, const char*);
            if(tsk_strnullORempty(IMPI_STR)) {
                TSK_DEBUG_ERROR("IMPI (IMS Private Identity) is mandatory.");
                return -1; /* mandatory */
            }
            tsk_strupdate(&self->identity.impi, IMPI_STR);
            break;
        }
        case tsip_pname_password: {
            /* (const char*)PASSORD_STR */
            const char* PASSORD_STR = va_arg(*app, const char*);
            tsk_strupdate(&self->identity.password, PASSORD_STR);
            break;
        }

        /* === SigComp === */
        case tsip_pname_sigcomp: {
            /* (unsigned)DMS_UINT, (unsigned)SMS_UINT, (unsigned)CPB_UINT, (tsk_bool_t)PRES_DICT_BOOL */
            self->sigcomp.dms = va_arg(*app, unsigned);
            self->sigcomp.sms = va_arg(*app, unsigned);
            self->sigcomp.cpb = va_arg(*app, unsigned);
            self->sigcomp.pres_dict = va_arg(*app, tsk_bool_t);
            break;
        }
        case tsip_pname_sigcomp_add_compartment: {
            /* (const char*)COMPARTMENT_ID_STR */
            if(!self->sigcomp.handle) {
                self->sigcomp.handle = tsip_sigcomp_handler_create(self->sigcomp.cpb, self->sigcomp.dms, self->sigcomp.sms);
                tsip_sigcomp_handler_set_dicts(self->sigcomp.handle, self->sigcomp.sip_dict, self->sigcomp.pres_dict);
            }
            tsip_sigcomp_handler_add_compartment(self->sigcomp.handle, va_arg(*app, const char*));
            break;
        }
        case tsip_pname_sigcomp_remove_compartment: {
            /* (const char*)COMPARTMENT_ID_STR */
            if(self->sigcomp.handle) {
                tsip_sigcomp_handler_remove_compartment(self->sigcomp.handle, va_arg(*app, const char*));
            }
            break;
        }

        /* === Network === */
        case tsip_pname_realm: {
            /* (const char*)URI_STR */
            const char* URI_STR = va_arg(*app, const char*);
            tsip_uri_t *uri;
            if(!tsk_strnullORempty(URI_STR) && (uri = tsip_uri_parse(URI_STR, tsk_strlen(URI_STR)))) {
                if(uri->type == uri_unknown) { /* scheme is missing or unsupported? */
                    tsk_strupdate(&uri->scheme, "sip");
                    uri->type = uri_sip;
                }
                TSK_OBJECT_SAFE_FREE(self->network.realm); /* delete old */
                self->network.realm = uri;
            }
            else {
                TSK_DEBUG_ERROR("'%s' is an invalid SIP/TEL URI", URI_STR);
                return -1; /* mandatory */
            }
            break;
        }
        case tsip_pname_local_ip: {
            /* (const char*)TRANSPORT_STR, (const char*)IP_STR */
            const char* TRANSPORT_STR = va_arg(*app, const char*);
            const char* IP_STR = va_arg(*app, const char*);
            int t_idx = __tsip_stack_get_transport_idx_by_name(self, TRANSPORT_STR);
            if(t_idx < 0) {
                TSK_DEBUG_ERROR("%s not valid as transport or you're probably using deprecated function", TRANSPORT_STR);
                return -1;
            }

            tsk_strupdate(&self->network.local_ip[t_idx], IP_STR);
            self->network.local_ip_is_set_by_user[t_idx] = tsk_true; // do not query best source when stack is re-started
            if(TSIP_STACK_MODE_IS_SERVER(self) && !tsk_strnullORempty(TRANSPORT_STR)) {
                self->network.transport_types[t_idx] = tsip_transport_get_type_by_name(TRANSPORT_STR);
            }
            break;
        }
        case tsip_pname_local_port: {
            /* (const char*)TRANSPORT_STR, (unsigned)PORT_UINT */
            const char* TRANSPORT_STR = va_arg(*app, const char*);
            unsigned PORT_UINT = va_arg(*app, unsigned);
            int t_idx = __tsip_stack_get_transport_idx_by_name(self, TRANSPORT_STR);
            if(t_idx < 0) {
                TSK_DEBUG_ERROR("%s not valid as transport or you're probably using deprecated function", TRANSPORT_STR);
                return -1;
            }

            self->network.local_port[t_idx] = PORT_UINT;
            if(TSIP_STACK_MODE_IS_SERVER(self) && !tsk_strnullORempty(TRANSPORT_STR)) {
                self->network.transport_types[t_idx] = tsip_transport_get_type_by_name(TRANSPORT_STR);
            }
            break;
        }
        case tsip_pname_aor: {
            /* (const char*)TRANSPORT_STR, (const char*)IP_STR, (unsigned)PORT_UINT */
            const char* TRANSPORT_STR = va_arg(*app, const char*);
            const char* IP_STR = va_arg(*app, const char*);
            tnet_port_t PORT_UINT = (tnet_port_t)va_arg(*app, unsigned);
            int t_idx = __tsip_stack_get_transport_idx_by_name(self, TRANSPORT_STR);
            if(t_idx < 0) {
                TSK_DEBUG_ERROR("%s not valid as transport or you're probably using deprecated function", TRANSPORT_STR);
                return -1;
            }

            if(!tsk_strnullORempty(IP_STR)) {
                tsk_strupdate(&self->network.aor.ip[t_idx], IP_STR);
            }
            if(PORT_UINT) {
                self->network.aor.port[t_idx] = PORT_UINT;
            }
            break;
        }
        case tsip_pname_discovery_naptr: {
            /* (tsk_bool_t)ENABLED_BOOL */
            self->network.discovery_naptr = va_arg(*app, tsk_bool_t);
            break;
        }
        case tsip_pname_discovery_dhcp: {
            /* (tsk_bool_t)ENABLED_BOOL */
            self->network.discovery_dhcp = va_arg(*app, tsk_bool_t);
            break;
        }
        case tsip_pname_proxy_cscf: {
            /* (const char*)FQDN_STR, (unsigned)PORT_UINT, (const char*)TRANSPORT_STR, (const char*)IP_VERSION_STR */
            const char* FQDN_STR = va_arg(*app, const char*);
            tnet_port_t PORT_UINT = va_arg(*app, unsigned);
            const char* TRANSPORT_STR = va_arg(*app, const char*);
            const char* IP_VERSION_STR = va_arg(*app, const char*);
            int t_idx = __tsip_stack_get_transport_idx_by_name(self, TRANSPORT_STR);
            if(t_idx < 0) {
                TSK_DEBUG_ERROR("%s not valid as transport or you're probably using deprecated function", TRANSPORT_STR);
                return -1;
            }

            if(TSIP_STACK_MODE_IS_CLIENT(self)) {
                // "client" mode support a unique proxy_cscf -> reset previous transports
                int k;
                for(k = 0; k < sizeof(self->network.proxy_cscf_type)/sizeof(self->network.proxy_cscf_type[0]); ++k) {
                    self->network.proxy_cscf_type[k] = tnet_socket_type_invalid;
                }
            }

            /* IP Address */
            tsk_strupdate(&self->network.proxy_cscf[t_idx], FQDN_STR);

            /* Port */
            if(PORT_UINT) {
                self->network.proxy_cscf_port[t_idx] = PORT_UINT;
            }

            /* Transport */
            if(tsk_strnullORempty(TRANSPORT_STR) || tsk_striequals(TRANSPORT_STR, "UDP")) {
                TNET_SOCKET_TYPE_SET_UDP(self->network.proxy_cscf_type[t_idx]);
            }
            else if(tsk_striequals(TRANSPORT_STR, "DTLS")) {
                TNET_SOCKET_TYPE_SET_DTLS(self->network.proxy_cscf_type[t_idx]);
            }
            else if(tsk_striequals(TRANSPORT_STR, "TCP")) {
                TNET_SOCKET_TYPE_SET_TCP(self->network.proxy_cscf_type[t_idx]);
            }
            else if(tsk_striequals(TRANSPORT_STR, "TLS")) {
                TNET_SOCKET_TYPE_SET_TLS(self->network.proxy_cscf_type[t_idx]);
            }
            else if(tsk_striequals(TRANSPORT_STR, "SCTP")) {
                TNET_SOCKET_TYPE_SET_SCTP(self->network.proxy_cscf_type[t_idx]);
            }
            else if(tsk_striequals(TRANSPORT_STR, "WS")) {
                TNET_SOCKET_TYPE_SET_WS(self->network.proxy_cscf_type[t_idx]);
            }
            else if(tsk_striequals(TRANSPORT_STR, "WSS")) {
                TNET_SOCKET_TYPE_SET_WSS(self->network.proxy_cscf_type[t_idx]);
            }
            TNET_SOCKET_TYPE_SET_IPV4(self->network.proxy_cscf_type[t_idx]); // IPv4 is the default version
            /* whether to use ipv6 or not */
            if(!tsk_strnullORempty(IP_VERSION_STR)) {
                if(tsk_strcontains(IP_VERSION_STR, tsk_strlen(IP_VERSION_STR), "6")) {
                    TNET_SOCKET_TYPE_SET_IPV6Only(self->network.proxy_cscf_type[t_idx]); // "only" to clear IPv4 flag
                }
                if(tsk_strcontains(IP_VERSION_STR, tsk_strlen(IP_VERSION_STR), "4")) {
                    TNET_SOCKET_TYPE_SET_IPV4(self->network.proxy_cscf_type[t_idx]); /* Not IPV4only ==> '46'/'64' */
                }
            }
            /* use same transport type as the proxy-cscf */
            self->network.transport_types[t_idx] = self->network.proxy_cscf_type[t_idx];
            /* set default transport */
            self->network.transport_idx_default = t_idx;
            break;
        }
        case tsip_pname_dnsserver: {
            /* (const char*)IP_STR */
            const char* IP_STR = va_arg(*app, const char*);
            if(tnet_dns_add_server(self->dns_ctx, IP_STR)) {
                TSK_DEBUG_ERROR("Failed to add [%s] as DNS server", IP_STR);
            }
            break;
        }
        case tsip_pname_max_fds: {
            /* (unsigned)MAX_FDS_UINT */
            self->network.max_fds = va_arg(*app, unsigned);
            break;
        }
        case tsip_pname_mode: {
            /* (tsip_stack_mode_t)MODE_ENUM */
            self->network.mode = va_arg(*app, tsip_stack_mode_t);
            break;
        }



        /* === Security === */
        case tsip_pname_early_ims: {
            /* (tsk_bool_t)ENABLED_BOOL */
            self->security.earlyIMS = va_arg(*app, tsk_bool_t);
            break;
        }
        case tsip_pname_secagree_ipsec: {
            /* (const char*)TRANSPORT_STR, (tsk_bool_t)ENABLED_BOOL */
            const char* TRANSPORT_STR = va_arg(*app, const char*);
            tsk_bool_t ENABLED_BOOL = va_arg(*app, tsk_bool_t);
            int t_idx = __tsip_stack_get_transport_idx_by_name(self, TRANSPORT_STR);
            if(t_idx < 0) {
                TSK_DEBUG_ERROR("%s not valid as transport or you're probably using deprecated function", TRANSPORT_STR);
                return -1;
            }
            if(ENABLED_BOOL) {
                tsk_strupdate(&self->security.secagree_mech, "ipsec-3gpp");
                TNET_SOCKET_TYPE_SET_IPSEC(self->network.proxy_cscf_type[t_idx]);
            }
            else {
                TNET_SOCKET_TYPE_UNSET(self->network.proxy_cscf_type[t_idx], IPSEC);
            }
            break;
        }
        case tsip_pname_secagree_tls: {
            /* (tsk_bool_t)ENABLED_BOOL */
            if((self->security.enable_secagree_tls = va_arg(*app, tsk_bool_t))) {
                tsk_strupdate(&self->security.secagree_mech, "tls");
            }
            break;
        }
        case tsip_pname_amf: {
            /* (uint16_t)AMF_UINT16 */
            unsigned amf = va_arg(*app, unsigned);
            self->security.amf[0] = (amf >> 8);
            self->security.amf[1] = (amf & 0xFF);
            break;
        }
        case tsip_pname_operator_id: {
            /* (const char*)OPID_HEX_STR */
            const char* hexstr = va_arg(*app, const char*);
            tsk_size_t len = tsk_strlen(hexstr);
            if(len && !(len & 0x01)) {
                tsk_size_t i, j;
                if(tsk_strindexOf(hexstr, tsk_strlen(hexstr), "0x") == 0) {
                    hexstr += 2;
                    len -= 2;
                }
                /* reset old value */
                memset(self->security.operator_id, 0, sizeof(self->security.operator_id));

                /* set new value */
                if(len) { /* perhaps there were only 2 chars*/
                    for(i = 0, j = 0; (i<(sizeof(operator_id_t) * 2) && i<len); i+=2, j++) {
#if 0	/* Could cause SIGBUS error (if memory misaligned) */
                        sscanf(&hexstr[i], "%2x", &self->security.operator_id[j]);
#else
                        static unsigned _1bytes; /* do not use neither int8_t nor uint8_t */
                        sscanf(&hexstr[i], "%2x", &_1bytes);
                        self->security.operator_id[j] = (_1bytes & 0xFF);
#endif
                    }
                }
            }
            else {
                TSK_DEBUG_ERROR("%s is invalid for an Operator Id value.", hexstr);
            }
            break;
        }
        case tsip_pname_ipsec_params: {
            /* (const char*)ALG_STR, (const char*)EALG_STR, (const char*)MODE_STR, (const char*)PROTOCOL_STR*/
            tsk_strupdate(&self->security.ipsec.alg, va_arg(*app, const char*));
            tsk_strupdate(&self->security.ipsec.ealg, va_arg(*app, const char*));
            tsk_strupdate(&self->security.ipsec.mode, va_arg(*app, const char*));
            tsk_strupdate(&self->security.ipsec.protocol, va_arg(*app, const char*));
            break;
        }
        case tsip_pname_tls_certs: {
            /* (const char*)CA_FILE_STR, (const char*)PUB_FILE_STR, (const char*)PRIV_FILE_STR, (tsk_bool_t)VERIF_BOOL */
            tsk_strupdate(&self->security.tls.ca, va_arg(*app, const char*));
            tsk_strupdate(&self->security.tls.pbk, va_arg(*app, const char*));
            tsk_strupdate(&self->security.tls.pvk, va_arg(*app, const char*));
            self->security.tls.verify = va_arg(*app, tsk_bool_t);
            break;
        }


        /* === Dummy Headers === */
        case tsip_pname_header: {
            /* (const char*)NAME_STR, (const char*)VALUE_STR */
            const char* NAME_STR = va_arg(*app, const char*);
            const char* VALUE_STR = va_arg(*app, const char*);
            if(VALUE_STR == ((const char*)-1)) { /* UNSET */
                tsk_params_remove_param(self->headers, NAME_STR);
            }
            else { /* SET */
                tsk_params_add_param(&self->headers, NAME_STR, VALUE_STR);
            }
            break;
        }


        /* === Nat Traversal === */
        case tsip_pname_stun_server: {
            /* (const char*)IP_STR, (unsigned)PORT_UINT */
            const char* IP_STR = va_arg(*app, const char*);
            unsigned PORT_UINT = va_arg(*app, unsigned);
            /* do not check, Null==> disable STUN */
            tsk_strupdate(&self->natt.stun.ip, IP_STR);
            self->natt.stun.port = PORT_UINT;
            break;
        }
        case tsip_pname_stun_cred: {
            /* (const char*)USR_STR, (const char*)PASSORD_STR */
            const char* USR_STR = va_arg(*app, const char*);
            const char* PASSORD_STR = va_arg(*app, const char*);
            tsk_strupdate(&self->natt.stun.login, USR_STR);
            tsk_strupdate(&self->natt.stun.pwd, PASSORD_STR);
            break;
        }
        case tsip_pname_stun_enabled: {
            /* (tsk_bool_t)ENABLED_BOOL */
            self->natt.stun.enabled = va_arg(*app, tsk_bool_t);
            break;
        }
        /* === User Data === */
        case tsip_pname_userdata: {
            /* (const void*)DATA_PTR */
            self->userdata = va_arg(*app, const void*);
            break;
        }



        default: {
            /* va_list will be unsafe ==> must exit */
            TSK_DEBUG_WARN("Found unknown pname.");
            goto bail;
        }
        }/* switch */
    }/* while */

bail:
    return 0;
}

/**@ingroup tsip_stack_group
 * Creates new 3GPP IMS/LTE stack handle.
 * As the 3GPP IMS/LTE stack depends on the network library (tinyNET), you MUST call <a href="http://doubango.org/API/tinyNET/tnet_8c.html#affba6c2710347476f615b0135777c640"> tnet_startup()</a> before using any SIP function (tsip_*).
 * <a href="http://doubango.org/API/tinyNET/tnet_8c.html#ac42b22a7ac5831f04326aee9de033c84"> tnet_cleanup()</a> is used to terminate use of network functions.
 * @param callback Callback function to call to alert the application for new SIP or media events.
 * @param realm_uri The realm is the name of the domain name to authenticate to. It should be a valid SIP URI (e.g. sip:open-ims.test).
 * @param impi_uri The IMPI is a unique identifier assigned to a user (or UE) by the home network.
 * It could be either a SIP URI (e.g. sip:bob@open-ims.test), a tel URI (e.g. tel:+33100000) or any alphanumeric string (e.g. bob@open-ims.test or bob).
 * It is used to authenticate the UE (username field in SIP Authorization/Proxy-Authorization header).
 * @param impu_uri As its name says, it’s you public visible identifier where you are willing to receive calls or any demands.
 * An IMPU could be either a SIP or tel URI (e.g. tel:+33100000 or sip:bob@open-ims.test). In IMS world, a user can have multiple IMPUs associated to its unique IMPI.
 * @param ... Any TSIP_STACK_SET_*() macros.
 * @retval A valid handle if succeed and Null-handle otherwise. As a stack is a well-defined object, you should use @a TSK_OBJECT_SAFE_FREE() to safely destroy the handle.
 *
 * @code
 int app_callback(const tsip_event_t *sipevent);

 const char* realm_uri = "sip:open-ims.test";
 const char* impi_uri = "bob@open-ims.test";
 const char* impu_uri = "sip:bob@open-ims.test";

 tsip_stack_handle_t* stack = tsip_stack_create(app_callback, realm_uri, impi_uri, impu_uri,
 TSIP_STACK_SET_PASSWORD("mysecret"),
 // ...other macros...
 TSIP_STACK_SET_NULL());

	// ...whatever

	TSK_OBJECT_SAFE_FREE(stack);
 * @endcode
 * @sa @ref tsip_stack_set()<br>@ref tsip_stack_start()
 */
tsip_stack_handle_t* tsip_stack_create(tsip_stack_callback_f callback, const char* realm_uri, const char* impi_uri, const char* impu_uri, ...)
{
    tsip_stack_t* stack = tsk_null;
    va_list ap;
    int i;

    /* === check values === */
    if(!realm_uri || !impi_uri || !impu_uri) {
        TSK_DEBUG_ERROR("Invalid parameter.");
        goto bail;
    }

    /* === create the stack === */
    if(!(stack = tsk_object_new(tsip_stack_def_t))) { /* should never happen */
        TSK_DEBUG_ERROR("Failed to create the stack.");
        goto bail;
    }

    /* === Set mandatory values (realm, IMPI and IMPU) === */
    if(tsip_stack_set(stack,
                      TSIP_STACK_SET_REALM(realm_uri),
                      TSIP_STACK_SET_IMPI(impi_uri),
                      TSIP_STACK_SET_IMPU(impu_uri),

                      TSIP_STACK_SET_NULL())) {
        TSK_DEBUG_ERROR("Invalid parameter.");
        TSK_OBJECT_SAFE_FREE(stack);
        goto bail;
    }

    /* === Default values (Network) === */
    stack->network.mode = tsip_stack_mode_ua;
    for(i = 0; i < sizeof(stack->network.local_port)/sizeof(stack->network.local_port[0]); ++i) {
        stack->network.local_port[i] = TNET_SOCKET_PORT_ANY;
    }
    for(i = 0; i < sizeof(stack->network.proxy_cscf_port)/sizeof(stack->network.proxy_cscf_port[0]); ++i) {
        stack->network.proxy_cscf_port[i] = 5060;
    }
    for(i = 0; i < sizeof(stack->network.proxy_cscf_type)/sizeof(stack->network.proxy_cscf_type[0]); ++i) {
        stack->network.proxy_cscf_type[i] = tnet_socket_type_invalid;
    }
    stack->network.max_fds = tmedia_defaults_get_max_fds();

    // all events should be delivered to the user before the stack stop
    tsk_runnable_set_important(TSK_RUNNABLE(stack), tsk_true);

    /* === SigComp === */
    // only create the handler on-demand: when compartment is added
    stack->sigcomp.dms = TSIP_SIGCOMP_DMS;
    stack->sigcomp.sms = TSIP_SIGCOMP_SMS;
    stack->sigcomp.cpb = TSIP_SIGCOMP_CPB;
    stack->sigcomp.sip_dict = TSIP_SIGCOMP_SIP_DICT;
    stack->sigcomp.pres_dict = TSIP_SIGCOMP_PRES_DICT;

    /* === DNS context ===
     * Because of TSIP_STACK_SET_DNS_SERVER(), ctx should be created before calling __tsip_stack_set()
     */
    stack->dns_ctx = tnet_dns_ctx_create();

    /* === DHCP context === */

    /* === NAT Traversal === */
    {
        const char *server_ip, *usr_name, *usr_pwd;
        uint16_t server_port;
        stack->natt.stun.enabled = tmedia_defaults_get_stun_enabled();
        if(tmedia_defaults_get_stun_server(&server_ip, &server_port) == 0) {
            tsk_strupdate(&stack->natt.stun.ip, server_ip);
            stack->natt.stun.port = server_port;
        }
        if(tmedia_defaults_get_stun_cred(&usr_name, &usr_pwd) == 0) {
            tsk_strupdate(&stack->natt.stun.login, usr_name);
            tsk_strupdate(&stack->natt.stun.pwd, usr_pwd);
        }
    }

    /* === Set user supplied parameters === */
    va_start(ap, impu_uri);
    if(__tsip_stack_set(stack, &ap)) {
        TSK_DEBUG_ERROR("Invalid parameter.");
        TSK_OBJECT_SAFE_FREE(stack);
        va_end(ap);
        goto bail;
    }
    va_end(ap);

    /* === Internals === */
    stack->callback = callback;
    if(!stack->ssessions) {
        stack->ssessions = tsk_list_create();
    }
    if(!stack->headers) { /* could be created by tsk_params_add_param() */
        stack->headers = tsk_list_create();
    }

    /* ===	Layers === */
    if(!(stack->layer_dialog = tsip_dialog_layer_create(stack))) {
        TSK_DEBUG_ERROR("Failed to create Dialog layer");
        TSK_OBJECT_SAFE_FREE(stack);
        goto bail;
    }
    if(!(stack->layer_transac = tsip_transac_layer_create(stack))) {
        TSK_DEBUG_ERROR("Failed to create Transac layer");
        TSK_OBJECT_SAFE_FREE(stack);
        goto bail;
    }
    if(!(stack->layer_transport = tsip_transport_layer_create(stack))) {
        TSK_DEBUG_ERROR("Failed to create Transport layer");
        TSK_OBJECT_SAFE_FREE(stack);
        goto bail;
    }

bail:
    return stack;
}


/**@ingroup tsip_stack_group
 * Starts a 3GPP IMS/LTE stack. This function MUST be called before you start calling any SIP function (@a tsip_*).
 * @param self The 3GPP IMS/LTE stack to start. This handle should be created using @ref tsip_stack_create().
 * @retval Zero if succeed and non-zero error code otherwise.
 */
int tsip_stack_start(tsip_stack_handle_t *self)
{
    int ret = -1, t_idx, tx_count;
    tsip_stack_t *stack = self;
    tnet_socket_type_t* tx_values;
    const char* stack_error_desc = "Failed to start the stack";

    if(!stack) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(stack->started) {
        TSK_DEBUG_INFO("Stack Already started");
        return 0;
    }

    tsk_safeobj_lock(stack);

    // transports
    if(TSIP_STACK_MODE_IS_SERVER(stack)) {
        TSK_DEBUG_INFO("Stack running in SERVER mode");
        tx_values = &stack->network.transport_types[0];
        tx_count = sizeof(stack->network.transport_types) / sizeof(stack->network.transport_types[0]);
    }
    else {
        TSK_DEBUG_INFO("Stack running in CLIENT mode");
        tx_values = &stack->network.proxy_cscf_type[0];
        tx_count = sizeof(stack->network.proxy_cscf_type) / sizeof(stack->network.proxy_cscf_type[0]);
    }

    /* === Timer manager === */
    if(!stack->timer_mgr_global) {
        stack->timer_mgr_global = tsk_timer_mgr_global_ref();
    }
    if((ret = tsk_timer_manager_start(stack->timer_mgr_global))) {
        goto bail;
    }

    /* === Set P-Preferred-Identity === */
    if(!stack->identity.preferred && stack->identity.impu) {
        stack->identity.preferred = tsk_object_ref((void*)stack->identity.impu);
    }

    /* === Set Max FDs === */
    if (stack->network.max_fds > 0 && stack->network.max_fds < 0xFFFF) {
        TSK_DEBUG_INFO("Setting max FDs to %u", (unsigned)stack->network.max_fds);
        ret = tnet_set_fd_max_allowed(stack->network.max_fds);
        if (ret) {
            TSK_DEBUG_ERROR("Failed to set max FDs to %u", (unsigned)stack->network.max_fds);
            /* goto bail; */ // Not fatal error
        }
    }

    /* === Transport type === */
    if(!tsk_strnullORempty(stack->security.secagree_mech)) {
        if(tsk_striequals(stack->security.secagree_mech, "ipsec-3gpp") && stack->security.enable_secagree_ipsec) {
#if 0
            TNET_SOCKET_TYPE_SET_IPSEC(stack->network.proxy_cscf_type);
#endif
            TSK_DEBUG_ERROR("Not implemented");
        }
        //else if if(tsk_striquals(stack->security.secagree_mech, "ipsec-ike"))
    }

    for(t_idx = 0; t_idx < tx_count; ++t_idx) {
        if(!TNET_SOCKET_TYPE_IS_VALID(tx_values[t_idx])) {
            continue;
        }
        /* === Use DNS NAPTR+SRV for the P-CSCF discovery if we are in client mode === */
        if(TSIP_STACK_MODE_IS_CLIENT(stack)) {
            if(tsk_strnullORempty(stack->network.proxy_cscf[t_idx]) || (stack->network.discovery_dhcp || stack->network.discovery_naptr)) {
                if(stack->network.discovery_dhcp) { /* DHCP v4/v6 */
                    /* FIXME: */
                    TSK_DEBUG_ERROR("Unexpected code called");
                    ret = -2;
                } /* DHCP */
                else { /* DNS NAPTR + SRV*/
                    char* hostname = tsk_null;
                    tnet_port_t port = 0;
                    const char* service = TNET_SOCKET_TYPE_IS_DGRAM(tx_values[t_idx]) ? "SIP+D2U" : (TNET_SOCKET_TYPE_IS_TLS(tx_values[t_idx]) ? "SIPS+D2T" : "SIP+D2T");
                    if((ret = tnet_dns_query_naptr_srv(stack->dns_ctx, stack->network.realm->host, service, &hostname, &port)) == 0) {
                        TSK_DEBUG_INFO("DNS SRV(NAPTR(%s, %s) = [%s / %d]", stack->network.realm->host, service, hostname, port);
                        tsk_strupdate(&stack->network.proxy_cscf[t_idx], hostname);
                        if(!stack->network.proxy_cscf_port[t_idx] || stack->network.proxy_cscf_port[t_idx]==5060) { /* Only if the Proxy-CSCF port is missing or default */
                            stack->network.proxy_cscf_port[t_idx] = port;
                        }
                    }
                    else {
                        TSK_DEBUG_ERROR("P-CSCF discovery using DNS NAPTR failed. The stack will use the user supplied address and port.");
                    }

                    TSK_FREE(hostname);
                } /* NAPTR */
            }

            /* Check Proxy-CSCF IP address */
            if(!tsk_strnullORempty(stack->network.proxy_cscf[t_idx])) {
                TSK_DEBUG_INFO("Proxy-CSCF=[%s]:%d", stack->network.proxy_cscf[t_idx], stack->network.proxy_cscf_port[t_idx]);
            }
            else {
                TSK_DEBUG_ERROR("Proxy-CSCF IP address is Null.");
                ret = -1983;
                goto bail;
            }
        }// !Server mode

        /* === Get Best source address ===  */
        // Best local address must be updated if not defined or none is set by the user.
        // The local address could be no-null if the stack is re-starting: https://code.google.com/p/doubango/issues/detail?id=454 and  https://code.google.com/p/idoubs/issues/detail?id=195
        if (!stack->network.local_ip_is_set_by_user[t_idx] || tsk_strnullORempty(stack->network.local_ip[t_idx]) || tsk_striequals(stack->network.local_ip[t_idx], "127.0.0.1")) { /* loacal-ip is missing? */
            tnet_ip_t bestsource;
            if ((ret = tnet_getbestsource(stack->network.proxy_cscf[t_idx] ? stack->network.proxy_cscf[t_idx] : "google.com",
                                          stack->network.proxy_cscf_port[t_idx] ? stack->network.proxy_cscf_port[t_idx] : 5060,
                                          tx_values[t_idx],
                                          &bestsource))) {
                TSK_DEBUG_ERROR("Failed to get best source [%d]", ret);
                /* do not exit ==> will use default IP address */
            }
            else {
                tsk_strupdate(&stack->network.local_ip[t_idx], bestsource);
                TSK_DEBUG_INFO("Best source at %d: %s", t_idx, bestsource);
            }
        }
        else {
            TSK_DEBUG_INFO("Do not query for best source address at %d, local_ip_is_set_by_user=%d, local_ip=%s", t_idx, stack->network.local_ip_is_set_by_user[t_idx], stack->network.local_ip[t_idx]);
        }
    } /* for (t_idx...) */

    /* === Runnable === */
    TSK_RUNNABLE(stack)->run = run;
    if((ret = tsk_runnable_start(TSK_RUNNABLE(stack), tsip_event_def_t))) {
        stack_error_desc = "Failed to start runnable";
        TSK_DEBUG_ERROR("%s", stack_error_desc);
        goto bail;
    }

    // must be here because the runnable object is only valid after start()
    TSIP_STACK_SIGNAL(self, tsip_event_code_stack_starting, "Stack starting");

    /* === Nat Traversal === */
    // delete previous context
    TSK_OBJECT_SAFE_FREE(stack->natt.ctx);
    if(stack->natt.stun.enabled && !tsk_strnullORempty(stack->natt.stun.ip)) {
        if(stack->natt.stun.port == 0) {
            /* FIXME: for now only UDP(IPv4/IPv6) is supported */
            stack->natt.stun.port = kStunPortDefaultTcpUdp;
        }
        TSK_DEBUG_INFO("STUN server = %s:%u", stack->natt.stun.ip, stack->natt.stun.port);
        stack->natt.ctx = tnet_nat_context_create(TNET_SOCKET_TYPE_IS_IPV6(tx_values[stack->network.transport_idx_default])? tnet_socket_type_udp_ipv6: tnet_socket_type_udp_ipv4,
                          stack->natt.stun.login, stack->natt.stun.pwd);
        ret = tnet_nat_set_server(stack->natt.ctx, stack->natt.stun.ip, stack->natt.stun.port);
    }

    /* === Transport Layer === */
    for(t_idx = 0; t_idx < tx_count; ++t_idx) {
        if(!TNET_SOCKET_TYPE_IS_VALID(tx_values[t_idx])) {
            continue;
        }
        if((ret = tsip_transport_layer_add(stack->layer_transport, stack->network.local_ip[t_idx], stack->network.local_port[t_idx], tx_values[t_idx], "SIP transport"))) {
            stack_error_desc = "Failed to add new transport";
            TSK_DEBUG_ERROR("%s", stack_error_desc);
            goto bail;
        }
    }
    /* Starts the transport Layer */
    if((ret = tsip_transport_layer_start(stack->layer_transport))) {
        stack_error_desc = "Failed to start sip transport";
        TSK_DEBUG_ERROR("%s", stack_error_desc);
        goto bail;
    }

    /* Update the local_ip */
    for(t_idx = 0; t_idx < tx_count; ++t_idx) {
        if(!TNET_SOCKET_TYPE_IS_VALID(tx_values[t_idx])) {
            continue;
        }
        if(tsk_strnullORempty(stack->network.local_ip[t_idx])) {
            const tsip_transport_t* transport = tsip_transport_layer_find_by_type(stack->layer_transport, tx_values[t_idx]);

            if(transport) {
                tnet_ip_t ip;
                if(!tnet_transport_get_ip_n_port_2(transport->net_transport, &ip, tsk_null)) {
                    tsk_strupdate(&stack->network.local_ip[t_idx], ip);
                }
                else {
                    TSK_DEBUG_WARN("Failed to get local_ip for transport type = %d", tx_values[t_idx]);
                    /* Do not exit */
                }
            }
        }
    }


    /* ===	ALL IS OK === */

    stack->started = tsk_true;

    /* Signal to the end-user that the stack has been started */
    TSIP_STACK_SIGNAL(self, tsip_event_code_stack_started, "Stack started");

    TSK_DEBUG_INFO("SIP STACK -- START");

    tsk_safeobj_unlock(stack);

    return 0;


bail:
    TSIP_STACK_SIGNAL(self, tsip_event_code_stack_failed_to_start, stack_error_desc);
    /* stop all running instances */
    if(stack->layer_transport) {
        tsip_transport_layer_shutdown(stack->layer_transport);
    }
    if(TSK_RUNNABLE(stack)->running) {
        tsk_runnable_stop(TSK_RUNNABLE(stack));
    }

    tsk_safeobj_unlock(stack);

    return ret;
}

/**@ingroup tsip_stack_group
 * Configures the stack.
 * @param self The 3GPP IMS/LTE stack to configure. This handle should be created using @ref tsip_stack_create().
 * @param ... Any TSIP_STACK_SET_*() or TSIP_STACK_UNSET_*() macros.
 * @retval Zero if succeed and non-zero error code otherwise.
 *
 * @code
 int ret = tsip_stack_set(stack,
 TSIP_STACK_SET_HEADER("User-Agent", "IM-client/OMA1.0 doubango/v1.0.0"),
 TSIP_STACK_SET_NULL());
 * @endcode
 *
 * @sa @ref tsip_stack_create()
 */
int tsip_stack_set(tsip_stack_handle_t *self, ...)
{
    if(self) {
        int ret;
        tsip_stack_t *stack = self;

        va_list ap;
        va_start(ap, self);
        ret = __tsip_stack_set(stack, &ap);
        va_end(ap);
        return ret;
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
}

/**@ingroup tsip_stack_group
 * Gets user's data, previously set using @ref TSIP_STACK_SET_USERDATA() macro.
 * @param self Stack from which to get the user's data.
 */
const void* tsip_stack_get_userdata(const tsip_stack_handle_t *self)
{
    if(self) {
        return ((const tsip_stack_t *)self)->userdata;
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
}

/**@ingroup tsip_stack_group
 */
tnet_dns_ctx_t* tsip_stack_get_dnsctx(tsip_stack_handle_t *self)
{
    if(self) {
        return (tnet_dns_ctx_t*)tsk_object_ref(((tsip_stack_t *)self)->dns_ctx);
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
}

/**@ingroup tsip_stack_group
 */
tsip_uri_t* tsip_stack_get_preferred_id(tsip_stack_handle_t *self)
{
    if(self) {
        return (tsip_uri_t*)tsk_object_ref(((tsip_stack_t *)self)->identity.preferred);
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
}

/**@ingroup tsip_stack_group
 */
int tsip_stack_get_local_ip_n_port(const tsip_stack_handle_t *self, const char* protocol, tnet_port_t *port, tnet_ip_t *ip)
{
    const tsip_stack_t *stack = self;

    if(!stack || !port || !ip) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(stack) {
        tsk_list_item_t *item;
        tsk_list_foreach(item, stack->layer_transport->transports) {
            tsip_transport_t *transport = item->data;

            if(transport) {
                if(tsk_striequals(transport->protocol, protocol)) {
                    return tnet_transport_get_public_ip_n_port(transport->net_transport,transport->connectedFD, ip, port);
                }
            }
        }
    }

    TSK_DEBUG_ERROR("No transport with such protocol (%s) could be found", protocol);
    return -2;
}

/**@ingroup tsip_stack_group
 * Stops the stack.
 * @param self The 3GPP IMS/LTE stack to stop. This handle should be created using @ref tsip_stack_create() and started using tsip_stack_start().
 * This function is also called by the garbage collector when the stack is destroyed but you should call it yourself before destroying the stack.<br>
 * Before stopping, the stack will hangup all SIP dialogs (starting with non-register dialogs) and destroy all sessions. This is called shutdown phase.
 * At the end of this phase, all the SIP sessions will be destroyed.
 * @sa @ref tsip_stack_create()<br>@ref tsip_stack_start()
 */
int tsip_stack_stop(tsip_stack_handle_t *self)
{
    tsip_stack_t *stack = self;

    if(stack) {
        tsk_bool_t one_failed = tsk_false;
        int ret = 0;

        tsk_safeobj_lock(stack);

        if(!stack->started) {
            TSK_DEBUG_INFO("Stack already stopped");
            goto bail;
        }

        TSIP_STACK_SIGNAL(self, tsip_event_code_stack_stopping, "Stack stopping");

        /* Hangup all dialogs starting by REGISTER */
        if((ret = tsip_dialog_layer_shutdownAll(stack->layer_dialog))) {
            TSK_DEBUG_WARN("Failed to hang-up all dialogs");
            one_failed = tsk_true;
        }

        /* do not try to clean up transactions ==> each dialog will cancel its transactions.
         * see tsip_dialog_deinit() which call tsip_transac_layer_cancel_by_dialog() */

        /* Stop the timer manager */
        // not done as it's global (shared). Will be done when all instance are destoyed

        /* Stop the transport layer */
        if((ret = tsip_transport_layer_shutdown(stack->layer_transport))) {
            TSK_DEBUG_WARN("Failed to stop the transport layer");
            one_failed = tsk_true;
        }

        /* Signal to the end-user that the stack has been stopped
         * should be done before tsk_runnable_stop() which will stop the thread
         * responsible for the callbacks. The enqueued data have been marked as "important".
         * As both the timer manager and the transport layer have been stoped there is no
         * chance to got additional events */
        if(one_failed) {
            TSIP_STACK_SIGNAL(self, tsip_event_code_stack_failed_to_stop, "Stack failed to stop");
        }
        else {
            TSIP_STACK_SIGNAL(self, tsip_event_code_stack_stopped, "Stack stopped");
        }

        /* Stop runnable (run() thread) */
        if((ret = tsk_runnable_stop(TSK_RUNNABLE(stack)))) {
            TSK_DEBUG_WARN("Failed to stop the stack");
            one_failed = tsk_true;
        }

        /* Close all SigComp Compartments (do not remove them) */
        if(stack->sigcomp.handle) {
            tsip_sigcomp_close_all(stack->sigcomp.handle);
        }

        /* reset AoR */
        TSK_FREE_TABLE(stack->network.aor.ip);
        memset(stack->network.aor.port, 0, sizeof(stack->network.aor.port));

        /* stops timer manager */
        if(stack->timer_mgr_global) {
            tsk_timer_mgr_global_unref(&stack->timer_mgr_global);
        }

        if(!one_failed) {
            stack->started = tsk_false;
        }

        TSK_DEBUG_INFO("SIP STACK -- STOP");

bail:
        tsk_safeobj_unlock(stack);

        return ret;
    }

    return -1;
}

/* internal function used to construct a valid contact URI */
tsip_uri_t* tsip_stack_get_contacturi(const tsip_stack_t *stack, const char* protocol)
{
    if(stack) {
        tsk_list_item_t *item;
        tsk_list_foreach(item, stack->layer_transport->transports) {
            tsip_transport_t *transport = item->data;

            if(transport) {
                if(tsk_striequals(transport->protocol, protocol)) {
                    tsip_uri_t* uri = tsk_null;
                    if((uri = tsip_transport_get_uri(transport, tsk_false))) {
                        tsk_strupdate(&uri->user_name, stack->identity.impu->user_name);
                        return uri;
                    }
                }
            }
        }
    }
    return tsk_null;
}

/* internal function used to construct a valid Proxy-CSCF URI used as the default first route */
tsip_uri_t* tsip_stack_get_pcscf_uri(const tsip_stack_t *stack, tnet_socket_type_t type, tsk_bool_t lr)
{
    if(stack) {
        const tsip_transport_t *transport = tsk_null;
        if(!TNET_SOCKET_TYPE_IS_VALID(type) && !TSK_LIST_IS_EMPTY(stack->layer_transport->transports)) {
            transport = stack->layer_transport->transports->head->data;
        }
        else {
            transport = tsip_transport_layer_find_by_type(stack->layer_transport, type);
        }

        if(transport) {
            tsip_uri_t* uri = tsk_null;
            tsk_bool_t ipv6 = TNET_SOCKET_TYPE_IS_IPV6(transport->type);
            tsk_bool_t quote_ip = (ipv6 && tsk_strcontains(stack->network.proxy_cscf[transport->idx], tsk_strlen(stack->network.proxy_cscf[transport->idx]), ":")) /* IPv6 IP string?*/;

            char* uristring = tsk_null;
            tsk_sprintf(&uristring, "%s:%s%s%s:%d;%s;transport=%s",
                        transport->scheme,
                        quote_ip ? "[" : "",
                        stack->network.proxy_cscf[transport->idx],
                        quote_ip ? "]" : "",
                        stack->network.proxy_cscf_port[transport->idx],
                        lr ? "lr" : "",
                        transport->protocol);
            if(uristring) {
                if((uri = tsip_uri_parse(uristring, tsk_strlen(uristring)))) {
                    //uri->host_type = ipv6 ? thttp_host_ipv6 : thttp_host_ipv4;
                }
                TSK_FREE(uristring);
            }

            return uri;
        }

    }
    return tsk_null;
}










static void* TSK_STDCALL run(void* self)
{
    tsk_list_item_t *curr;
    tsip_stack_t *stack = self;

    TSK_DEBUG_INFO("SIP STACK::run -- START");

    TSK_RUNNABLE_RUN_BEGIN(stack);

    if((curr = TSK_RUNNABLE_POP_FIRST(stack))) {
        tsip_event_t *sipevent = (tsip_event_t*)curr->data;
        if(stack->callback) {
            sipevent->userdata = stack->userdata; // needed by sessionless events
            stack->callback(sipevent);
        }
        tsk_object_unref(curr);
    }

    TSK_RUNNABLE_RUN_END(self);

    TSK_DEBUG_INFO("SIP STACK::run -- STOP");
    return 0;
}










//========================================================
//	SIP stack object definition
//
static tsk_object_t* tsip_stack_ctor(tsk_object_t * self, va_list * app)
{
    tsip_stack_t *stack = self;
    if(stack) {
        tsk_safeobj_init(stack);
    }
    return self;
}

static tsk_object_t* tsip_stack_dtor(tsk_object_t * self)
{
    tsip_stack_t *stack = self;
    if(stack) {

        /* /!\ Order in which objects are destroyed is very important */

        /* Stop
         * Will try to hangup all dialogs */
        if(stack->started) {
            tsip_stack_stop(stack);
        }

        /* Layers(1/1): Transacs and dialogs use timer_mgr when destroyed
         * Dialogs =>(use)=> transacs =>(use)=> transport. */
        TSK_OBJECT_SAFE_FREE(stack->layer_dialog);
        TSK_OBJECT_SAFE_FREE(stack->layer_transac);
        TSK_OBJECT_SAFE_FREE(stack->layer_transport);

        /* Internals(1/2) */
        if(stack->timer_mgr_global) {
            tsk_timer_mgr_global_unref(&stack->timer_mgr_global);
        }

        /* Identity */
        TSK_FREE(stack->identity.display_name);
        TSK_OBJECT_SAFE_FREE(stack->identity.impu);
        TSK_OBJECT_SAFE_FREE(stack->identity.preferred);
        //TSK_OBJECT_SAFE_FREE(stack->associated_identity);
        TSK_FREE(stack->identity.impi);
        TSK_FREE(stack->identity.password);

        /* Network(1/1) */
        TSK_FREE_TABLE(stack->network.local_ip);
        TSK_OBJECT_SAFE_FREE(stack->network.realm);
        TSK_FREE_TABLE(stack->network.proxy_cscf);
        TSK_OBJECT_SAFE_FREE(stack->paths);

        TSK_FREE_TABLE(stack->network.aor.ip);

        TSK_OBJECT_SAFE_FREE(stack->service_routes);
        TSK_OBJECT_SAFE_FREE(stack->associated_uris);

        /* SigComp (MUST be done after transports) */
        TSK_OBJECT_SAFE_FREE(stack->sigcomp.handle);

        /* Security(1/1) */
        TSK_FREE(stack->security.secagree_mech);
        TSK_FREE(stack->security.ipsec.alg);
        TSK_FREE(stack->security.ipsec.ealg);
        TSK_FREE(stack->security.ipsec.mode);
        TSK_FREE(stack->security.ipsec.protocol);

        TSK_FREE(stack->security.tls.ca);
        TSK_FREE(stack->security.tls.pbk);
        TSK_FREE(stack->security.tls.pvk);


        /* DNS */
        TSK_OBJECT_SAFE_FREE(stack->dns_ctx);

        /* NAT Traversal context */
        TSK_FREE(stack->natt.stun.ip);
        TSK_FREE(stack->natt.stun.login);
        TSK_FREE(stack->natt.stun.pwd);
        TSK_OBJECT_SAFE_FREE(stack->natt.ctx);

        /* DHCP */

        /* features */

        /* QoS */

        /* Internals (2/2) */
        TSK_OBJECT_SAFE_FREE(stack->ssessions);
        TSK_OBJECT_SAFE_FREE(stack->headers);

        tsk_safeobj_deinit(stack);

        TSK_DEBUG_INFO("*** SIP Stack destroyed ***");
    }
    return self;
}

static const tsk_object_def_t tsip_stack_def_s = {
    sizeof(tsip_stack_t),
    tsip_stack_ctor,
    tsip_stack_dtor,
    tsk_null,
};
const tsk_object_def_t *tsip_stack_def_t = &tsip_stack_def_s;
