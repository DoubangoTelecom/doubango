/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
#include "SipStack.h"

#include "SipSession.h"
#include "SipEvent.h"

#include "DDebug.h"

#include "Common.h"

bool SipStack::g_bInitialized = false;


/* === ANSI-C functions (local use) === */
static int stack_callback(const tsip_event_t *sipevent);
static int session_handle_event(const tsip_event_t *sipevent);

SipStack::SipStack(SipCallback* pCallback, const char* realm_uri, const char* impi_uri, const char* impu_uri)
    :SafeObject()
{
    m_pDebugCallback = tsk_null;
    m_pCallback = pCallback;

    /* Initialize network and media layers */
    if(!SipStack::initialize()) {
        return;// isValid() will be false
    }

    /* Creates stack handle */
    m_pHandle = tsip_stack_create(stack_callback, realm_uri, impi_uri, impu_uri,
                                  TSIP_STACK_SET_USERDATA(this), /* used as context (useful for server-initiated requests) */
                                  TSIP_STACK_SET_NULL());
}

SipStack::~SipStack()
{
    this->stop();

    /* Destroy stack handle */
    TSK_OBJECT_SAFE_FREE(m_pHandle);
}

bool SipStack::start()
{
    bool ret = (tsip_stack_start(m_pHandle) == 0);
    return ret;
}

bool SipStack::setDebugCallback(DDebugCallback* pCallback)
{
    if(pCallback) {
        m_pDebugCallback = pCallback;
        tsk_debug_set_arg_data(this);
        tsk_debug_set_info_cb(DDebugCallback::debug_info_cb);
        tsk_debug_set_warn_cb(DDebugCallback::debug_warn_cb);
        tsk_debug_set_error_cb(DDebugCallback::debug_error_cb);
        tsk_debug_set_fatal_cb(DDebugCallback::debug_fatal_cb);
    }
    else {
        m_pDebugCallback = tsk_null;
        tsk_debug_set_arg_data(tsk_null);
        tsk_debug_set_info_cb(tsk_null);
        tsk_debug_set_warn_cb(tsk_null);
        tsk_debug_set_error_cb(tsk_null);
        tsk_debug_set_fatal_cb(tsk_null);
    }

    return true;
}

bool SipStack::setDisplayName(const char* display_name)
{
    int ret = tsip_stack_set(m_pHandle,
                             TSIP_STACK_SET_DISPLAY_NAME(display_name),
                             TSIP_STACK_SET_NULL());
    return (ret == 0);
}

bool SipStack::setRealm(const char* realm_uri)
{
    int ret = tsip_stack_set(m_pHandle,
                             TSIP_STACK_SET_REALM(realm_uri),
                             TSIP_STACK_SET_NULL());
    return (ret == 0);
}

bool SipStack::setIMPI(const char* impi)
{
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_IMPI(impi),
                           TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setIMPU(const char* impu_uri)
{
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_IMPU(impu_uri),
                           TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setPassword(const char* password)
{
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_PASSWORD(password),
                           TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setAMF(const char* amf)
{
    uint16_t _amf = (uint16_t)tsk_atox(amf);
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_IMS_AKA_AMF(_amf),
                           TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setOperatorId(const char* opid)
{
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_IMS_AKA_OPERATOR_ID(opid),
                           TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setProxyCSCF(const char* fqdn, unsigned short port, const char* transport, const char* ipversion)
{
    unsigned _port = port;//promote
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_PROXY_CSCF(fqdn, _port, transport, ipversion),
                           TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setLocalIP(const char* ip, const char* transport/*=tsk_null*/)
{
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_LOCAL_IP_2(transport, ip),
                           TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setLocalPort(unsigned short port, const char* transport/*=tsk_null*/)
{
    unsigned _port = port;//promote
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_LOCAL_PORT_2(transport, _port),
                           TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setEarlyIMS(bool enabled)
{
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_EARLY_IMS(enabled? tsk_true : tsk_false),
                           TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::addHeader(const char* name, const char* value)
{
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_HEADER(name, value),
                           TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::removeHeader(const char* name)
{
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_UNSET_HEADER(name),
                           TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::addDnsServer(const char* ip)
{
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_DNS_SERVER(ip),
                           TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setDnsDiscovery(bool enabled)
{
    tsk_bool_t _enabled = enabled;// 32bit/64bit workaround
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_DISCOVERY_NAPTR(_enabled),
                           TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setAoR(const char* ip, int port)
{
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_AOR(ip, port),
                           TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setMode(enum tsip_stack_mode_e mode)
{
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_MODE(mode),
                           TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setSigCompParams(unsigned dms, unsigned sms, unsigned cpb, bool enablePresDict)
{
    tsk_bool_t _enablePresDict= enablePresDict;// 32bit/64bit workaround
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_SIGCOMP(dms, sms, cpb, _enablePresDict),
                           TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::addSigCompCompartment(const char* compId)
{
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_SIGCOMP_NEW_COMPARTMENT(compId),
                           TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::removeSigCompCompartment(const char* compId)
{
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_UNSET_SIGCOMP_COMPARTMENT(compId),
                           TSIP_STACK_SET_NULL()) == 0);
}

// @deprecated
bool SipStack::setSTUNEnabledForICE(bool enabled)
{
#if 0
    tsk_bool_t _enabled = enabled ? tsk_true : tsk_false;
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_ICE_STUN_ENABLED(_enabled),
                           TSIP_STACK_SET_NULL()) == 0);
#else
    // set global value
    return (tmedia_defaults_set_icestun_enabled(enabled ? tsk_true : tsk_false) == 0);
    // to set the value per session, use "CallSession::setICEStun()"
#endif
}

// @deprecated
bool SipStack::setSTUNServer(const char* hostname, unsigned short port)
{
#if 0
    unsigned _port = port;//promote
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_STUN_SERVER(hostname, _port),
                           TSIP_STACK_SET_NULL()) == 0);
#else
    // set global value
    return (tmedia_defaults_set_stun_server(hostname, port) == 0);
    // to set the value per session, use "CallSession::setSTUNServer()"
#endif
}

// @deprecated
bool SipStack::setSTUNCred(const char* login, const char* password)
{
#if 0
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_STUN_CRED(login, password),
                           TSIP_STACK_SET_NULL()) == 0);
#else
    // set global value
    return (tmedia_defaults_set_stun_cred(login, password) == 0);
    // to set the value per session, use "CallSession::setSTUNCred()"
#endif
}

bool SipStack::setSTUNEnabled(bool enabled)
{
    tsk_bool_t _enabled = enabled ? tsk_true : tsk_false;
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_STUN_ENABLED(_enabled),
                           TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setTLSSecAgree(bool enabled)
{
    tsk_bool_t _enable = enabled ? tsk_true : tsk_false;
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_SECAGREE_TLS(_enable),
                           TSIP_STACK_SET_NULL()) == 0);
}

/*@deprecated: typo  */
bool SipStack::setSSLCretificates(const char* privKey, const char* pubKey, const char* caKey, bool verify/* = false*/)
{
    return setSSLCertificates(privKey, pubKey, caKey, verify);
}

bool SipStack::setSSLCertificates(const char* privKey, const char* pubKey, const char* caKey, bool verify/* = false*/)
{
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_TLS_CERTS_2(caKey, pubKey, privKey, (verify ? tsk_true : tsk_false)),
                           TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setIPSecSecAgree(bool enabled)
{
    tsk_bool_t _enable = enabled;
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_SECAGREE_IPSEC(_enable),
                           TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setIPSecParameters(const char* algo, const char* ealgo, const char* mode, const char* proto)
{
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_IPSEC_PARAMS(algo, ealgo, mode, proto),
                           TSIP_STACK_SET_NULL()) == 0);
}

char* SipStack::dnsENUM(const char* service, const char* e164num, const char* domain)
{
    tnet_dns_ctx_t* dnsctx = tsip_stack_get_dnsctx(m_pHandle);
    char* uri = tsk_null;

    if(dnsctx) {
        if(!(uri = tnet_dns_enum_2(dnsctx, service, e164num, domain))) {
            TSK_DEBUG_ERROR("ENUM(%s) failed", e164num);
        }
        tsk_object_unref(dnsctx);
        return uri;
    }
    else {
        TSK_DEBUG_ERROR("No DNS Context could be found");
        return tsk_null;
    }
}

char* SipStack::dnsNaptrSrv(const char* domain, const char* service, unsigned short *OUTPUT)
{
    tnet_dns_ctx_t* dnsctx = tsip_stack_get_dnsctx(m_pHandle);
    char* ip = tsk_null;
    tnet_port_t port;
    *OUTPUT = 0;


    if(dnsctx) {
        if(!tnet_dns_query_naptr_srv(dnsctx, domain, service, &ip, &port)) {
            *OUTPUT = port;
        }
        tsk_object_unref(dnsctx);
        return ip;
    }
    else {
        TSK_DEBUG_ERROR("No DNS Context could be found");
        return tsk_null;
    }
}

char* SipStack::dnsSrv(const char* service, unsigned short* OUTPUT)
{
    tnet_dns_ctx_t* dnsctx = tsip_stack_get_dnsctx(m_pHandle);
    char* ip = tsk_null;
    tnet_port_t port = 0;
    *OUTPUT = 0;

    if(dnsctx) {
        if(!tnet_dns_query_srv(dnsctx, service, &ip, &port)) {
            *OUTPUT = port;
        }
        tsk_object_unref(dnsctx);
        return ip;
    }
    else {
        TSK_DEBUG_ERROR("No DNS Context could be found");
        return tsk_null;
    }
}

bool SipStack::setMaxFDs(unsigned max_fds)
{
    return (tsip_stack_set(m_pHandle,
                           TSIP_STACK_SET_MAX_FDS(max_fds),
                           TSIP_STACK_SET_NULL()) == 0);
}

char* SipStack::getLocalIPnPort(const char* protocol, unsigned short* OUTPUT)
{
    tnet_ip_t ip;
    tnet_port_t port;
    int ret;

    if(!OUTPUT || !protocol) {
        TSK_DEBUG_ERROR("invalid parameter");
        return tsk_null;
    }

    if((ret = tsip_stack_get_local_ip_n_port(m_pHandle, protocol, &port, &ip))) {
        TSK_DEBUG_ERROR("Failed to get local ip and port with error code=%d", ret);
        return tsk_null;
    }

    *OUTPUT = port;
    return tsk_strdup(ip); // See Swig %newobject
}

char* SipStack::getPreferredIdentity()
{
    tsip_uri_t* ppid = tsip_stack_get_preferred_id(m_pHandle);
    char* str_ppid = tsk_null;
    if(ppid) {
        str_ppid = tsip_uri_tostring(ppid, tsk_false, tsk_false);
        TSK_OBJECT_SAFE_FREE(ppid);
    }
    return str_ppid;
}

bool SipStack::isValid()
{
    return (m_pHandle != tsk_null);
}

bool SipStack::stop()
{
    int ret = tsip_stack_stop(m_pHandle);
    return (ret == 0);
}

bool SipStack::initialize()
{
    if (!g_bInitialized) {
        int ret;

        if((ret = tnet_startup())) {
            TSK_DEBUG_ERROR("tnet_startup failed with error code=%d", ret);
            return false;
        }
        if((ret = thttp_startup())) {
            TSK_DEBUG_ERROR("thttp_startup failed with error code=%d", ret);
            return false;
        }
        if((ret = tdav_init())) {
            TSK_DEBUG_ERROR("tdav_init failed with error code=%d", ret);
            return false;
        }
        g_bInitialized = true;
    }
    return true;
}

bool SipStack::deInitialize()
{
    if (SipStack::g_bInitialized) {
        tdav_deinit();
        thttp_cleanup();
        tnet_cleanup();
        SipStack::g_bInitialized = false;
    }
    return false;
}

void SipStack::setCodecs(tdav_codec_id_t codecs)
{
    tdav_set_codecs(codecs);
}

void SipStack::setCodecs_2(int64_t codecs) // For stupid languages
{
    SipStack::setCodecs((tdav_codec_id_t)codecs);
}

bool SipStack::setCodecPriority(tdav_codec_id_t codec_id, int priority)
{
    return tdav_codec_set_priority(codec_id, priority) == 0;
}

bool SipStack::setCodecPriority_2(int codec_id, int priority)// For stupid languages
{
    return SipStack::setCodecPriority((tdav_codec_id_t)codec_id, priority);
}

bool SipStack::isCodecSupported(tdav_codec_id_t codec_id)
{
    return tdav_codec_is_supported(codec_id) ? true : false;
}

bool SipStack::isIPSecSupported()
{
    return tdav_ipsec_is_supported() ? true : false;
}

static int stack_callback(const tsip_event_t *sipevent)
{
    int ret = 0;
    const SipStack* sipStack = tsk_null;
    SipEvent* e = tsk_null;

    if(!sipevent) { /* should never happen ...but who know? */
        TSK_DEBUG_WARN("Null SIP event.");
        return -1;
    }
    else {
        if(sipevent->type == tsip_event_stack && sipevent->userdata) {
            /* sessionless event */
            sipStack = dyn_cast<const SipStack*>((const SipStack*)sipevent->userdata);
        }
        else {
            const void* userdata;
            /* gets the stack from the session */
            const tsip_stack_handle_t* stack_handle = tsip_ssession_get_stack(sipevent->ss);
            if(stack_handle && (userdata = tsip_stack_get_userdata(stack_handle))) {
                sipStack = dyn_cast<const SipStack*>((const SipStack*)userdata);
            }
        }
    }

    if(!sipStack) {
        TSK_DEBUG_WARN("Invalid SIP event (Stack is Null).");
        return -2;
    }

    sipStack->Lock();

    switch(sipevent->type) {
    case tsip_event_register: {
        /* REGISTER */
        if(sipStack->getCallback()) {
            e = new RegistrationEvent(sipevent);
            sipStack->getCallback()->OnRegistrationEvent((const RegistrationEvent*)e);
        }
        break;
    }
    case tsip_event_invite: {
        /* INVITE */
        if(sipStack->getCallback()) {
            e = new InviteEvent(sipevent);
            sipStack->getCallback()->OnInviteEvent((const InviteEvent*)e);
        }
        break;
    }
    case tsip_event_message: {
        /* MESSAGE */
        if(sipStack->getCallback()) {
            e = new MessagingEvent(sipevent);
            sipStack->getCallback()->OnMessagingEvent((const MessagingEvent*)e);
        }
        break;
    }
    case tsip_event_info: {
        /* INFO */
        if(sipStack->getCallback()) {
            e = new InfoEvent(sipevent);
            sipStack->getCallback()->OnInfoEvent((const InfoEvent*)e);
        }
        break;
    }
    case tsip_event_options: {
        /* OPTIONS */
        if(sipStack->getCallback()) {
            e = new OptionsEvent(sipevent);
            sipStack->getCallback()->OnOptionsEvent((const OptionsEvent*)e);
        }
        break;
    }
    case tsip_event_publish: {
        /* PUBLISH */
        if(sipStack->getCallback()) {
            e = new PublicationEvent(sipevent);
            sipStack->getCallback()->OnPublicationEvent((const PublicationEvent*)e);
        }
        break;
    }
    case tsip_event_subscribe: {
        /* SUBSCRIBE */
        if(sipStack->getCallback()) {
            e = new SubscriptionEvent(sipevent);
            sipStack->getCallback()->OnSubscriptionEvent((const SubscriptionEvent*)e);
        }
        break;
    }

    case tsip_event_dialog: {
        /* Common to all dialogs */
        if(sipStack->getCallback()) {
            e = new DialogEvent(sipevent);
            sipStack->getCallback()->OnDialogEvent((const DialogEvent*)e);
        }
        break;
    }

    case tsip_event_stack: {
        /* Stack event */
        if(sipStack->getCallback()) {
            e = new StackEvent(sipevent);
            sipStack->getCallback()->OnStackEvent((const StackEvent*)e);
        }
        break;
    }

    default: {
        /* Unsupported */
        TSK_DEBUG_WARN("%d not supported as SIP event.", sipevent->type);
        ret = -3;
        break;
    }
    }

    sipStack->UnLock();

    if(e) {
        delete e;
    }

    return ret;
}

