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
#include "SipSession.h"
#include "SipStack.h"

/* ======================== SipSession ========================*/
SipSession::SipSession(SipStack* stack)
{
	this->init(stack);
}

SipSession::SipSession(SipStack* stack, tsip_ssession_handle_t* handle)
{
	this->init(stack, handle);
}

SipSession::~SipSession()
{
	tsip_ssession_set(this->handle,
		TSIP_SSESSION_SET_USERDATA(tsk_null),
		TSIP_SSESSION_SET_NULL());

	TSK_OBJECT_SAFE_FREE(this->handle);
}

void SipSession::init(SipStack* _stack, tsip_ssession_handle_t* _handle/*=tsk_null*/)
{
	if(_handle){
		/* "server-side-session" */
		if(tsip_ssession_take_ownership(_handle)){ /* should never happen */
			TSK_DEBUG_ERROR("Failed to take ownership");
			return;
		}
		this->handle = _handle;
	}
	else{
		/* "client-side-session" */
		this->handle = tsip_ssession_create(_stack->getHandle(),
			TSIP_SSESSION_SET_USERDATA(this),
			TSIP_SSESSION_SET_NULL());
	}

	/* set userdata (context) and ref. the stack handle */
	tsip_ssession_set(this->handle,
			TSIP_SSESSION_SET_USERDATA(this),
			TSIP_SSESSION_SET_NULL());
	this->stack = _stack;
}

bool SipSession::addHeader(const char* name, const char* value)
{
	return (tsip_ssession_set(this->handle,
		TSIP_SSESSION_SET_HEADER(name, value),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::haveOwnership()
{
	return (tsip_ssession_have_ownership(this->handle) == tsk_true);
}

bool SipSession::removeHeader(const char* name)
{
	return (tsip_ssession_set(this->handle,
		TSIP_SSESSION_UNSET_HEADER(name),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::addCaps(const char* name, const char* value)
{
	return (tsip_ssession_set(this->handle,
		TSIP_SSESSION_SET_CAPS(name, value),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::addCaps(const char* name)
{
	return (tsip_ssession_set(this->handle,
		TSIP_SSESSION_SET_CAPS(name, tsk_null),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::removeCaps(const char* name)
{
	return (tsip_ssession_set(this->handle,
		TSIP_SSESSION_UNSET_CAPS(name),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::setExpires(unsigned expires)
{
	return (tsip_ssession_set(this->handle,
		TSIP_SSESSION_SET_EXPIRES(expires),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::setFromUri(const char* fromUri)
{
	return (tsip_ssession_set(this->handle,
		TSIP_SSESSION_SET_FROM(fromUri),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::setToUri(const char* toUri)
{
	return (tsip_ssession_set(this->handle,
		TSIP_SSESSION_SET_TO(toUri),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::setSilentHangup(bool silent)
{
	return (tsip_ssession_set(this->handle,
		TSIP_SSESSION_SET_SILENT_HANGUP(silent ? tsk_true : tsk_false),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::addSigCompCompartment(const char* compId)
{
	return (tsip_ssession_set(this->handle,
		TSIP_SSESSION_SET_SIGCOMP_COMPARTMENT(compId),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::removeSigCompCompartment()
{
	return (tsip_ssession_set(this->handle,
		TSIP_SSESSION_UNSET_SIGCOMP_COMPARTMENT(),
		TSIP_SSESSION_SET_NULL()) == 0);
}

unsigned SipSession::getId()
{
	return (unsigned)tsip_ssession_get_id(this->handle);
}

const SipStack* SipSession::getStack()const
{
	return this->stack;
}



/* ======================== CallSession ========================*/
CallSession::CallSession(SipStack* Stack)
: SipSession(Stack)
{
}

CallSession::CallSession(SipStack* Stack, tsip_ssession_handle_t* handle)
: SipSession(Stack, handle)
{
}

CallSession::~CallSession()
{
}

bool CallSession::callAudio(const char* remoteUri)
{
	tsip_ssession_set(this->handle,
		TSIP_SSESSION_SET_TO(remoteUri),
		TSIP_SSESSION_SET_NULL());

	return (tsip_action_INVITE(this->handle, tmedia_audio,
		TSIP_ACTION_SET_NULL()) == 0);
}

bool CallSession::callAudioVideo(const char* remoteUri)
{
	tsip_ssession_set(this->handle,
		TSIP_SSESSION_SET_TO(remoteUri),
		TSIP_SSESSION_SET_NULL());

	return (tsip_action_INVITE(this->handle, (tmedia_type_t)(tmedia_audio | tmedia_video),
		TSIP_ACTION_SET_NULL()) == 0);
}

bool CallSession::callVideo(const char* remoteUri)
{
	tsip_ssession_set(this->handle,
		TSIP_SSESSION_SET_TO(remoteUri),
		TSIP_SSESSION_SET_NULL());

	return (tsip_action_INVITE(this->handle, tmedia_video,
		TSIP_ACTION_SET_NULL()) == 0);
}

bool CallSession::setSessionTimer(unsigned timeout, const char* refresher)
{
	return (tsip_ssession_set(this->handle,
			TSIP_SSESSION_SET_MEDIA(
				TSIP_MSESSION_SET_TIMERS(timeout, refresher),
				TSIP_MSESSION_SET_NULL()
			),
			TSIP_SSESSION_SET_NULL()) == 0);
}

bool CallSession::set100rel(bool enabled)
{
	if(enabled){
		return (tsip_ssession_set(this->handle,
			TSIP_SSESSION_SET_MEDIA(
				TSIP_MSESSION_SET_100rel(),
				TSIP_MSESSION_SET_NULL()
			),
			TSIP_SSESSION_SET_NULL()) == 0);
	}
	else{
		return (tsip_ssession_set(this->handle,
			TSIP_SSESSION_SET_MEDIA(
				TSIP_MSESSION_UNSET_100rel(),
				TSIP_MSESSION_SET_NULL()
			),
			TSIP_SSESSION_SET_NULL()) == 0);
	}
}

bool CallSession::setQoS(tmedia_qos_stype_t type, tmedia_qos_strength_t strength)
{
	return (tsip_ssession_set(this->handle,
			TSIP_SSESSION_SET_MEDIA(
				TSIP_MSESSION_SET_QOS(type, strength),
				TSIP_MSESSION_SET_NULL()
			),
			TSIP_SSESSION_SET_NULL()) == 0);
}

bool CallSession::hangup()
{
	int ret = tsip_action_BYE(this->handle,
		TSIP_ACTION_SET_NULL());
	return (ret == 0);
}
#if ANDROID
static void *__droid_accept(void *param)
{	
	tsip_action_ACCEPT((tsip_ssession_handle_t *)param,
		TSIP_ACTION_SET_NULL());

	return tsk_null;
}

bool CallSession::accept()
{
	void* tid[1] = {0};
	tsip_ssession_handle_t *handle;
	int ret;
	
	handle = tsk_object_ref(this->handle);
	ret = tsk_thread_create(tid, __droid_accept, this->handle);
	tsk_thread_join(tid);
	tsk_object_unref(handle);

	return (ret == 0);
}
#else
bool CallSession::accept()
{
	return (tsip_action_ACCEPT(this->handle,
		TSIP_ACTION_SET_NULL()) == 0);
}
#endif


bool CallSession::hold()
{
	return (tsip_action_HOLD(this->handle, tmedia_all,
		TSIP_ACTION_SET_NULL()) ==0 );
}

bool CallSession::resume()
{
	return (tsip_action_RESUME(this->handle, tmedia_all,
		TSIP_ACTION_SET_NULL()) == 0);
}

bool CallSession::sendDTMF(int number)
{
	return (tsip_action_DTMF(this->handle, number,
		TSIP_ACTION_SET_NULL()) == 0);
}


/* ======================== MessagingSession ========================*/
MessagingSession::MessagingSession(SipStack* Stack)
: SipSession(Stack)
{
}

MessagingSession::MessagingSession(SipStack* Stack, tsip_ssession_handle_t* handle)
: SipSession(Stack, handle)
{
}

MessagingSession::~MessagingSession()
{
}

bool MessagingSession::send(const void* payload, unsigned len)
{
	TSK_DEBUG_INFO("MessagingSession::Send()");
	int ret;
	if(payload && len){
		ret = tsip_action_MESSAGE(this->handle,
			TSIP_ACTION_SET_PAYLOAD(payload, len),
			TSIP_ACTION_SET_NULL());
	}
	else{
		ret = tsip_action_PUBLISH(this->handle,
			TSIP_ACTION_SET_NULL());
	}
	return (ret == 0);
}

bool MessagingSession::accept()
{
	return (tsip_action_ACCEPT(this->handle,
		TSIP_ACTION_SET_NULL()) == 0);
}

bool MessagingSession::reject()
{
	return (tsip_action_REJECT(this->handle,
		TSIP_ACTION_SET_NULL()) == 0);
}


/* ======================== OptionsSession ========================*/
OptionsSession::OptionsSession(SipStack* Stack)
: SipSession(Stack)
{
}

OptionsSession::~OptionsSession()
{
}

bool OptionsSession::send()
{	
	return (tsip_action_OPTIONS(this->handle,
		TSIP_ACTION_SET_NULL()) == 0);
}





/* ======================== PublicationSession ========================*/
PublicationSession::PublicationSession(SipStack* Stack)
: SipSession(Stack)
{
}

PublicationSession::~PublicationSession()
{
}

bool PublicationSession::publish(const void* payload, unsigned len)
{
	int ret;
	if(payload && len){
		ret = tsip_action_PUBLISH(this->handle,
			TSIP_ACTION_SET_PAYLOAD(payload, len),
			TSIP_ACTION_SET_NULL());
	}
	else{
		ret = tsip_action_PUBLISH(this->handle,
			TSIP_ACTION_SET_NULL());
	}
	return (ret == 0);
}

bool PublicationSession::unPublish()
{
	return (tsip_action_UNPUBLISH(this->handle,
		TSIP_ACTION_SET_NULL()) == 0);
}



/* ======================== RegistrationSession ========================*/
RegistrationSession::RegistrationSession(SipStack* Stack)
: SipSession(Stack)
{
}

RegistrationSession::~RegistrationSession()
{
}

bool RegistrationSession::register_()
{
	return (tsip_action_REGISTER(this->handle,
		TSIP_ACTION_SET_NULL()) == 0);
}

bool RegistrationSession::unRegister()
{
	return (tsip_action_UNREGISTER(this->handle,
		TSIP_ACTION_SET_NULL()) == 0);
}



/* ======================== SubscriptionSession ========================*/
SubscriptionSession::SubscriptionSession(SipStack* Stack)
: SipSession(Stack)
{
}

SubscriptionSession::~SubscriptionSession()
{
}

bool SubscriptionSession::subscribe()
{
	return (tsip_action_SUBSCRIBE(this->handle,
		TSIP_ACTION_SET_NULL()) == 0);
}

bool SubscriptionSession::unSubscribe()
{
	return (tsip_action_UNSUBSCRIBE(this->handle,
		TSIP_ACTION_SET_NULL()) == 0);
}
