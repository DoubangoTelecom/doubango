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

#include "SipSession.h"
#include "RegistrationEvent.h"
#include "RegistrationSession.h"

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
	this->callback = callback_;

	/* Initialize network layer */
	if(SipStack::count == 0){
		tnet_startup();
	}

	/* Creates stack handle */
	this->handle = tsip_stack_create(stack_callback, realm_uri, impi_uri, impu_uri,
			TSIP_STACK_SET_LOCAL_IP(DEFAULT_LOCAL_IP),
			TSIP_STACK_SET_USERDATA(this), /* used as context */
			TSIP_STACK_SET_NULL());

	SipStack::count++;
}

SipStack::~SipStack()
{
	this->Stop();

	/* Destroy stack handle */
	TSK_OBJECT_SAFE_FREE(this->handle);

	/* DeInitialize the network layer (only if last stack) */
	if(--SipStack::count == 0){
		tnet_cleanup();
	}
}

bool SipStack::Start()
{
	int ret = tsip_stack_start(this->handle);
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

bool SipStack::Stop()
{
	int ret = tsip_stack_stop(this->handle);
	return (ret == 0);
}

tsip_stack_handle_t* SipStack::getHandle()
{
	return this->handle;
}

SipCallback* SipStack::getCallback()
{
	return this->callback;
}

int stack_callback(const tsip_event_t *sipevent)
{
	int ret = 0;
	SipStack* Stack = tsk_null;
	SipSession* Session = tsk_null;
	SipEvent* e = tsk_null;

	if(!sipevent){ /* should never happen ...but who know? */
		TSK_DEBUG_WARN("Null SIP event.");
		return -1;
	}
	else{
		/* retrive the stack from the context */
		if((Stack = (SipStack*)sipevent->user_data)){
			Session = dyn_cast<SipSession*>((SipSession*)tsip_ssession_get_context(sipevent->ss));
		}
	}

	if(!Stack || !Session){
		TSK_DEBUG_WARN("Invalid SIP event.");
		return -2;
	}

	Stack->Lock();

	switch(sipevent->type){
		case tsip_event_register:
			{	/* REGISTER */
				if(Stack->getCallback()){
					const tsip_register_event_t* reg_event = TSIP_REGISTER_EVENT(sipevent);
					e = new RegistrationEvent(sipevent->code, sipevent->phrase, dyn_cast<RegistrationSession*>(Session));
					((RegistrationEvent*)e)->setType(reg_event->type);
					Stack->getCallback()->OnRegistrationChanged((const RegistrationEvent*)e);
				}
				//ret = register_handle_event(sipevent);
				break;
			}
		case tsip_event_invite:
			{	/* INVITE */
				//ret = invite_handle_event(sipevent);
				break;
			}
		case tsip_event_message:
			{	/* MESSAGE */
				//ret = message_handle_event(sipevent);
				break;
			}
		case tsip_event_options:
			{ /* OPTIONS */
				//ret = options_handle_event(sipevent);
				break;
			}
		case tsip_event_publish:
			{ /* PUBLISH */
				//ret = publish_handle_event(sipevent);
				break;
			}
		case tsip_event_subscribe:
			{	/* SUBSCRIBE */
				//ret = subscribe_handle_event(sipevent);
				break;
			}

		case tsip_event_dialog:
			{	/* Common to all dialogs */
				//ret = session_handle_event(sipevent);
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

