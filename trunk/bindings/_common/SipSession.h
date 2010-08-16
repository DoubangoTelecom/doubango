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
#ifndef TINYWRAP_SIPSESSION_H
#define TINYWRAP_SIPSESSION_H

#include "tinysip.h"
#include "tinymedia/tmedia_qos.h"
#include "ActionConfig.h"

class SipStack;
class MsrpCallback;

/* ======================== SipSession ========================*/
class SipSession
{
public:
	SipSession(SipStack* stack);
#if !defined(SWIG)
	SipSession(SipStack* stack, tsip_ssession_handle_t* handle);
#endif
	virtual ~SipSession();

public:
	bool haveOwnership();
	bool addHeader(const char* name, const char* value);
	bool removeHeader(const char* name);
	bool addCaps(const char* name, const char* value);
	bool addCaps(const char* name);
	bool removeCaps(const char* name);
	bool setExpires(unsigned expires);
	bool setFromUri(const char* fromUri);
	bool setToUri(const char* toUri);
	bool setSilentHangup(bool silent);
	bool addSigCompCompartment(const char* compId);
	bool removeSigCompCompartment();
	unsigned getId();

#if !defined(SWIG)
	const SipStack* getStack() const;
#endif
	
private:
	void init(SipStack* stack, tsip_ssession_handle_t* handle=tsk_null);

protected:
	tsip_ssession_handle_t* handle;
	const SipStack* stack;
};

/* ======================== InviteSession ========================*/
class InviteSession : public SipSession
{
public: /* ctor() and dtor() */
	InviteSession(SipStack* Stack);
#if !defined(SWIG)
	InviteSession(SipStack* Stack, tsip_ssession_handle_t* handle);
#endif
	virtual ~InviteSession();

public: /* Public functions */
	bool accept(ActionConfig* config=tsk_null);
	bool hangup(ActionConfig* config=tsk_null);
	bool reject(ActionConfig* config=tsk_null);
};


/* ======================== CallSession ========================*/
class CallSession : public InviteSession
{
public: /* ctor() and dtor() */
	CallSession(SipStack* Stack);
#if !defined(SWIG)
	CallSession(SipStack* Stack, tsip_ssession_handle_t* handle);
#endif
	virtual ~CallSession();

public: /* Public functions */
	bool callAudio(const char* remoteUri, ActionConfig* config=tsk_null);
	bool callAudioVideo(const char* remoteUri, ActionConfig* config=tsk_null);
	bool callVideo(const char* remoteUri, ActionConfig* config=tsk_null);
	bool setSessionTimer(unsigned timeout, const char* refresher);
	bool set100rel(bool enabled);
	bool setQoS(tmedia_qos_stype_t type, tmedia_qos_strength_t strength);
	bool hold(ActionConfig* config=tsk_null);
	bool resume(ActionConfig* config=tsk_null);
	bool sendDTMF(int number);
};

/* ======================== MsrpSession ========================*/
class MsrpSession : public InviteSession
{
public: /* ctor() and dtor() */
	MsrpSession(SipStack* Stack, MsrpCallback* callback);
#if !defined(SWIG)
	MsrpSession(SipStack* Stack, tsip_ssession_handle_t* handle);
#endif
	virtual ~MsrpSession();

public: /* Public functions */
	bool setCallback(MsrpCallback* callback);
	bool callMsrp(const char* remoteUri, ActionConfig* config=tsk_null);
	bool sendLMessage(ActionConfig* config=tsk_null);
	bool sendFile(ActionConfig* config=tsk_null);

	public: /* Public helper function */
#if !defined(SWIG)
		inline MsrpCallback* getCallback()const{
			return this->callback;
		}
#endif

private:
	MsrpCallback* callback;
};



/* ======================== MessagingSession ========================*/
class MessagingSession : public SipSession
{
public: /* ctor() and dtor() */
	MessagingSession(SipStack* Stack);
#if !defined(SWIG)
	MessagingSession(SipStack* Stack, tsip_ssession_handle_t* handle);
#endif
	virtual ~MessagingSession();

public: /* Public functions */
	bool send(const void* payload, unsigned len);
	bool accept();
	bool reject();
};

/* ======================== OptionsSession ========================*/
class OptionsSession : public SipSession
{
public: /* ctor() and dtor() */
	OptionsSession(SipStack* Stack);
	virtual ~OptionsSession();

public: /* Public functions */
	bool send();
	/* bool Accept(); */
	/* bool Reject(); */
};



/* ======================== PublicationSession ========================*/
class PublicationSession : public SipSession
{
public: /* ctor() and dtor() */
	PublicationSession(SipStack* Stack);
	virtual ~PublicationSession();

public: /* Public functions */
	bool publish(const void* payload, unsigned len);
	bool unPublish();
};


/* ======================== RegistrationSession ========================*/
class RegistrationSession : public SipSession
{
public: /* ctor() and dtor() */
	RegistrationSession(SipStack* Stack);
	virtual ~RegistrationSession();

public: /* Public functions */
	bool register_();
	bool unRegister();
};


/* ======================== SubscriptionSession ========================*/
class SubscriptionSession : public SipSession
{
public: /* ctor() and dtor() */
	SubscriptionSession(SipStack* Stack);
	virtual ~SubscriptionSession();

public: /* Public functions */
	bool subscribe();
	bool unSubscribe();
};

#endif /* TINYWRAP_SIPSESSION_H */
