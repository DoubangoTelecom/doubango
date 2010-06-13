/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@doubango.org>
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

#include "tinydav/tdav.h"

#include "SipSession.h"
#include "SipEvent.h"

#include "DDebug.h"

#include "Common.h"

unsigned SipStack::count = 0;


/* === ANSI-C functions (local use) === */
int stack_callback(const tsip_event_t *sipevent);
int session_handle_event(const tsip_event_t *sipevent);

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
	int ret = tsip_stack_start(this->handle);
	return (ret == 0);
}

bool SipStack::setDebugCallback(DDebugCallback* callback)
{
	if((this->debugCallback = callback)){
		tsk_debug_set_arg_data(this);
		tsk_debug_set_info_cb(DDebugCallback::debug_info_cb);
		tsk_debug_set_warn_cb(DDebugCallback::debug_warn_cb);
		tsk_debug_set_error_cb(DDebugCallback::debug_error_cb);
		tsk_debug_set_fatal_cb(DDebugCallback::debug_fatal_cb);
		return true;
	}

	return false;
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
	int ret = tsip_stack_set(this->handle,
		TSIP_STACK_SET_IMPI(impi),
		TSIP_STACK_SET_NULL());
	return (ret == 0);
}

bool SipStack::setIMPU(const char* impu_uri)
{
	int ret = tsip_stack_set(this->handle,
		TSIP_STACK_SET_IMPU(impu_uri),
		TSIP_STACK_SET_NULL());
	return (ret == 0);
}

bool SipStack::setPassword(const char* password)
{
	int ret = tsip_stack_set(this->handle,
		TSIP_STACK_SET_PASSWORD(password),
		TSIP_STACK_SET_NULL());
	return (ret == 0);
}

bool SipStack::setProxyCSCF(const char* fqdn, unsigned port, const char* transport, const char* ipversion)
{
	int ret = tsip_stack_set(this->handle,
			TSIP_STACK_SET_PROXY_CSCF(fqdn, port, transport, ipversion),
			TSIP_STACK_SET_NULL());
	return (ret == 0);
}

bool SipStack::setLocalIP(const char* ip)
{
	int ret = tsip_stack_set(this->handle,
		TSIP_STACK_SET_LOCAL_IP(ip),
		TSIP_STACK_SET_NULL());
	return (ret == 0);
}

bool SipStack::setLocalPort(unsigned port)
{
	int ret = tsip_stack_set(this->handle,
		TSIP_STACK_SET_LOCAL_PORT(port),
		TSIP_STACK_SET_NULL());
	return (ret == 0);
}

bool SipStack::setEarlyIMS(bool enabled){
	int ret = tsip_stack_set(this->handle,
		TSIP_STACK_SET_EARLY_IMS(enabled? tsk_true : tsk_false),
		TSIP_STACK_SET_NULL());
	return (ret == 0);
}

bool SipStack::addHeader(const char* name, const char* value)
{
	int ret = tsip_stack_set(this->handle,
		TSIP_STACK_SET_HEADER(name, value),
		TSIP_STACK_SET_NULL());
	return (ret == 0);
}

bool SipStack::removeHeader(const char* name)
{
	int ret = tsip_stack_set(this->handle,
		TSIP_STACK_UNSET_HEADER(name),
		TSIP_STACK_SET_NULL());
	return (ret == 0);
}

bool SipStack::addDnsServer(const char* ip)
{
	int ret = tsip_stack_set(this->handle,
		TSIP_STACK_SET_DNS_SERVER(ip),
		TSIP_STACK_SET_NULL());
	return (ret == 0);
}

bool SipStack::setAoR(const char* ip, int port)
{
	int ret = tsip_stack_set(this->handle,
		TSIP_STACK_SET_AOR(ip, port),
		TSIP_STACK_SET_NULL());
	return (ret == 0);
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

tsip_stack_handle_t* SipStack::getHandle()const
{
	return this->handle;
}

SipCallback* SipStack::getCallback()const
{
	return this->callback;
}

DDebugCallback* SipStack::getDebugCallback() const
{
	return this->debugCallback;
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

