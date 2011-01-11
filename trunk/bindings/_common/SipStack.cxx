/*
* Copyright (C) 2009-2010 Mamadou Diop.
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

unsigned SipStack::count = 0;


/* === ANSI-C functions (local use) === */
static int stack_callback(const tsip_event_t *sipevent);
static int session_handle_event(const tsip_event_t *sipevent);

/* === default values === */
#ifndef DEFAULT_LOCAL_IP
//#	ifdef ANDROID /* On the emulator */
//#		define DEFAULT_LOCAL_IP	"10.0.2.15"
//#	else
#		define DEFAULT_LOCAL_IP	TNET_SOCKET_HOST_ANY
//#	endif
#endif

SipStack::SipStack(SipCallback* callback_, const char* realm_uri, const char* impi_uri, const char* impu_uri)
:SafeObject()
{
	this->debugCallback = tsk_null;
	this->callback = callback_;

	/* Initialize network layer */
	if(SipStack::count == 0){
		tdav_init();
		tnet_startup();
	}

	/* Creates stack handle */
	this->handle = tsip_stack_create(stack_callback, realm_uri, impi_uri, impu_uri,
			TSIP_STACK_SET_LOCAL_IP(DEFAULT_LOCAL_IP),
			TSIP_STACK_SET_USERDATA(this), /* used as context (useful for server-initiated requests) */
			TSIP_STACK_SET_NULL());

	SipStack::count++;
}

SipStack::~SipStack()
{
	this->stop();

	/* Destroy stack handle */
	TSK_OBJECT_SAFE_FREE(this->handle);

	/* DeInitialize the network layer (only if last stack) */
	if(--SipStack::count == 0){
		tdav_deinit();
		tnet_cleanup();
	}
}

bool SipStack::start()
{
	return (tsip_stack_start(this->handle) == 0);
}

bool SipStack::setDebugCallback(DDebugCallback* callback)
{
	if(this && callback){
		this->debugCallback = callback;
		tsk_debug_set_arg_data(this);
		tsk_debug_set_info_cb(DDebugCallback::debug_info_cb);
		tsk_debug_set_warn_cb(DDebugCallback::debug_warn_cb);
		tsk_debug_set_error_cb(DDebugCallback::debug_error_cb);
		tsk_debug_set_fatal_cb(DDebugCallback::debug_fatal_cb);
	}
	else if(this){
		this->debugCallback = tsk_null;
		tsk_debug_set_arg_data(tsk_null);
		tsk_debug_set_info_cb(tsk_null);
		tsk_debug_set_warn_cb(tsk_null);
		tsk_debug_set_error_cb(tsk_null);
		tsk_debug_set_fatal_cb(tsk_null);
	}

	return true;
}

bool SipStack::setRealm(const char* realm_uri)
{
	int ret = tsip_stack_set(this->handle,
		TSIP_STACK_SET_REALM(realm_uri),
		TSIP_STACK_SET_NULL());
	return (ret == 0);
}

bool SipStack::setIMPI(const char* impi)
{
	return (tsip_stack_set(this->handle,
		TSIP_STACK_SET_IMPI(impi),
		TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setIMPU(const char* impu_uri)
{
	return (tsip_stack_set(this->handle,
		TSIP_STACK_SET_IMPU(impu_uri),
		TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setPassword(const char* password)
{
	return (tsip_stack_set(this->handle,
		TSIP_STACK_SET_PASSWORD(password),
		TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setAMF(const char* amf)
{
	uint16_t _amf = (uint16_t)tsk_atox(amf);
	return (tsip_stack_set(this->handle,
			TSIP_STACK_SET_IMS_AKA_AMF(_amf),
			TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setOperatorId(const char* opid)
{
	return (tsip_stack_set(this->handle,
			TSIP_STACK_SET_IMS_AKA_OPERATOR_ID(opid),
			TSIP_STACK_SET_NULL()) == 0); 
}

bool SipStack::setProxyCSCF(const char* fqdn, unsigned short port, const char* transport, const char* ipversion)
{
	unsigned _port = port;//promote
	return (tsip_stack_set(this->handle,
			TSIP_STACK_SET_PROXY_CSCF(fqdn, _port, transport, ipversion),
			TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setLocalIP(const char* ip)
{
	return (tsip_stack_set(this->handle,
		TSIP_STACK_SET_LOCAL_IP(ip),
		TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setLocalPort(unsigned short port)
{
	unsigned _port = port;//promote
	return (tsip_stack_set(this->handle,
		TSIP_STACK_SET_LOCAL_PORT(_port),
		TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setEarlyIMS(bool enabled){
	return (tsip_stack_set(this->handle,
		TSIP_STACK_SET_EARLY_IMS(enabled? tsk_true : tsk_false),
		TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::addHeader(const char* name, const char* value)
{
	return (tsip_stack_set(this->handle,
		TSIP_STACK_SET_HEADER(name, value),
		TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::removeHeader(const char* name)
{
	return (tsip_stack_set(this->handle,
		TSIP_STACK_UNSET_HEADER(name),
		TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::addDnsServer(const char* ip)
{
	return (tsip_stack_set(this->handle,
		TSIP_STACK_SET_DNS_SERVER(ip),
		TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setDnsDiscovery(bool enabled)
{
	tsk_bool_t _enabled = enabled;// 32bit/64bit workaround
	return (tsip_stack_set(this->handle,
		TSIP_STACK_SET_DISCOVERY_NAPTR(_enabled),
		TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setAoR(const char* ip, int port)
{
	return (tsip_stack_set(this->handle,
		TSIP_STACK_SET_AOR(ip, port),
		TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setModeServer()
{
	return (tsip_stack_set(this->handle,
		TSIP_STACK_SET_MODE_SERVER(),
		TSIP_STACK_SET_NULL()) == 0); 
}

bool SipStack::setSigCompParams(unsigned dms, unsigned sms, unsigned cpb, bool enablePresDict)
{
	tsk_bool_t _enablePresDict= enablePresDict;// 32bit/64bit workaround
	return (tsip_stack_set(this->handle,
		TSIP_STACK_SET_SIGCOMP(dms, sms, cpb, _enablePresDict),
		TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::addSigCompCompartment(const char* compId)
{
	return (tsip_stack_set(this->handle,
		TSIP_STACK_SET_SIGCOMP_NEW_COMPARTMENT(compId),
		TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::removeSigCompCompartment(const char* compId)
{
	return (tsip_stack_set(this->handle,
		TSIP_STACK_UNSET_SIGCOMP_COMPARTMENT(compId),
		TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setSTUNServer(const char* ip, unsigned short port)
{
	unsigned _port = port;//promote
	return (tsip_stack_set(this->handle,
		TSIP_STACK_SET_STUN_SERVER(ip, _port),
		TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setSTUNCred(const char* login, const char* password)
{
	return (tsip_stack_set(this->handle,
		TSIP_STACK_SET_STUN_CRED(login, password),
		TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setTLSSecAgree(bool enabled)
{
	tsk_bool_t _enable = enabled;
	return (tsip_stack_set(this->handle,
		TSIP_STACK_SET_SECAGREE_TLS(_enable),
		TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setSSLCretificates(const char* privKey, const char* pubKey, const char* caKey)
{
	return (tsip_stack_set(this->handle,
		TSIP_STACK_SET_TLS_CERTS(caKey, pubKey, privKey),
		TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setIPSecSecAgree(bool enabled)
{
	tsk_bool_t _enable = enabled;
	return (tsip_stack_set(this->handle,
		TSIP_STACK_SET_SECAGREE_IPSEC(_enable),
		TSIP_STACK_SET_NULL()) == 0);
}

bool SipStack::setIPSecParameters(const char* algo, const char* ealgo, const char* mode, const char* proto)
{
	return (tsip_stack_set(this->handle,
		TSIP_STACK_SET_IPSEC_PARAMS(algo, ealgo, mode, proto),
		TSIP_STACK_SET_NULL()) == 0);
}

char* SipStack::dnsENUM(const char* service, const char* e164num, const char* domain)
{
	tnet_dns_ctx_t* dnsctx = tsip_stack_get_dnsctx(this->handle);
	char* uri = tsk_null;

	if(dnsctx){
		if(!(uri = tnet_dns_enum_2(dnsctx, service, e164num, domain))){
			TSK_DEBUG_ERROR("ENUM(%s) failed", e164num);
		}
		tsk_object_unref(dnsctx);
		return uri;
	}
	else{
		TSK_DEBUG_ERROR("No DNS Context could be found");
		return tsk_null;
	}
}

char* SipStack::dnsNaptrSrv(const char* domain, const char* service, unsigned short *OUTPUT)
{
	tnet_dns_ctx_t* dnsctx = tsip_stack_get_dnsctx(this->handle);
	char* ip = tsk_null;
	tnet_port_t port;
	*OUTPUT = 0;
	

	if(dnsctx){
		if(!tnet_dns_query_naptr_srv(dnsctx, domain, service, &ip, &port)){
			*OUTPUT = port;
		}
		tsk_object_unref(dnsctx);
		return ip;
	}
	else{
		TSK_DEBUG_ERROR("No DNS Context could be found");
		return tsk_null;
	}
}

char* SipStack::dnsSrv(const char* service, unsigned short* OUTPUT)
{
	tnet_dns_ctx_t* dnsctx = tsip_stack_get_dnsctx(this->handle);
	char* ip = tsk_null;
	tnet_port_t port = 0;
	*OUTPUT = 0;

	if(dnsctx){
		if(!tnet_dns_query_srv(dnsctx, service, &ip, &port)){
			*OUTPUT = port;
		}
		tsk_object_unref(dnsctx);
		return ip;
	}
	else{
		TSK_DEBUG_ERROR("No DNS Context could be found");
		return tsk_null;
	}
}

char* SipStack::getLocalIPnPort(const char* protocol, unsigned short* OUTPUT)
{
	tnet_ip_t ip;
	tnet_port_t port;
	int ret;

	if(!OUTPUT || !protocol){
		TSK_DEBUG_ERROR("invalid parameter");
		return tsk_null;
	}

	if((ret = tsip_stack_get_local_ip_n_port(this->handle, protocol, &port, &ip))){
		TSK_DEBUG_ERROR("Failed to get local ip and port with error code=%d", ret);
		return tsk_null;
	}

	*OUTPUT = port;
	return tsk_strdup(ip); // See Swig %newobject
}

char* SipStack::getPreferredIdentity()
{
	tsip_uri_t* ppid = tsip_stack_get_preferred_id(this->handle);
	char* str_ppid = tsk_null;
	if(ppid){
		str_ppid = tsip_uri_tostring(ppid, tsk_false, tsk_false);
		TSK_OBJECT_SAFE_FREE(ppid);
	}
	return str_ppid;
}

bool SipStack::isValid()
{
	return (this->handle != tsk_null);
}

bool SipStack::stop()
{
	int ret = tsip_stack_stop(this->handle);
	return (ret == 0);
}

void SipStack::setCodecs(tdav_codec_id_t codecs)
{
	tdav_set_codecs(codecs);
}

void SipStack::setCodecs_2(int codecs) // For stupid languages
{
	tdav_set_codecs((tdav_codec_id_t)codecs);
}

bool SipStack::isCodecSupported(tdav_codec_id_t codec)
{
	return tdav_codec_is_supported(codec) ? true : false;
}

int stack_callback(const tsip_event_t *sipevent)
{
	int ret = 0;
	const SipStack* Stack = tsk_null;
	SipEvent* e = tsk_null;

	if(!sipevent){ /* should never happen ...but who know? */
		TSK_DEBUG_WARN("Null SIP event.");
		return -1;
	}
	else {
		if(sipevent->type == tsip_event_stack && sipevent->userdata){
			/* sessionless event */
			Stack = dyn_cast<const SipStack*>((const SipStack*)sipevent->userdata);
		}
		else {
			const void* userdata;
			/* gets the stack from the session */
			const tsip_stack_handle_t* stack_handle = tsip_ssession_get_stack(sipevent->ss);
			if(stack_handle && (userdata = tsip_stack_get_userdata(stack_handle))){
				Stack = dyn_cast<const SipStack*>((const SipStack*)userdata);
			}
		}
	}

	if(!Stack){
		TSK_DEBUG_WARN("Invalid SIP event (Stack is Null).");
		return -2;
	}

	Stack->Lock();

	switch(sipevent->type){
		case tsip_event_register:
			{	/* REGISTER */
				if(Stack->getCallback()){
					e = new RegistrationEvent(sipevent);
					Stack->getCallback()->OnRegistrationEvent((const RegistrationEvent*)e);
				}
				break;
			}
		case tsip_event_invite:
			{	/* INVITE */
				if(Stack->getCallback()){
					e = new InviteEvent(sipevent);
					Stack->getCallback()->OnInviteEvent((const InviteEvent*)e);
				}
				break;
			}
		case tsip_event_message:
			{	/* MESSAGE */
				if(Stack->getCallback()){
					e = new MessagingEvent(sipevent);
					Stack->getCallback()->OnMessagingEvent((const MessagingEvent*)e);
				}
				break;
			}
		case tsip_event_options:
			{ /* OPTIONS */
				if(Stack->getCallback()){
					e = new OptionsEvent(sipevent);
					Stack->getCallback()->OnOptionsEvent((const OptionsEvent*)e);
				}
				break;
			}
		case tsip_event_publish:
			{ /* PUBLISH */
				if(Stack->getCallback()){
					e = new PublicationEvent(sipevent);
					Stack->getCallback()->OnPublicationEvent((const PublicationEvent*)e);
				}
				break;
			}
		case tsip_event_subscribe:
			{	/* SUBSCRIBE */
				if(Stack->getCallback()){
					e = new SubscriptionEvent(sipevent);
					Stack->getCallback()->OnSubscriptionEvent((const SubscriptionEvent*)e);
				}
				break;
			}

		case tsip_event_dialog:
			{	/* Common to all dialogs */
				if(Stack->getCallback()){
					e = new DialogEvent(sipevent);
					Stack->getCallback()->OnDialogEvent((const DialogEvent*)e);
				}
				break;
			}

		case tsip_event_stack:
			{	/* Stack event */
				if(Stack->getCallback()){
					e = new StackEvent(sipevent);
					Stack->getCallback()->OnStackEvent((const StackEvent*)e);
				}
				break;
			}

		default:
			{	/* Unsupported */
				TSK_DEBUG_WARN("%d not supported as SIP event.", sipevent->type);
				ret = -3;
				break;
			}
	}

	Stack->UnLock();

	if(e){
		delete e;
	}

	return ret;
}

