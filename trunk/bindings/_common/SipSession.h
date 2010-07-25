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

class SipStack;


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


/* ======================== CallSession ========================*/
class CallSession : public SipSession
{
public: /* ctor() and dtor() */
	CallSession(SipStack* Stack);
#if !defined(SWIG)
	CallSession(SipStack* Stack, tsip_ssession_handle_t* handle);
#endif
	virtual ~CallSession();

public: /* Public functions */
	bool callAudio(const char* remoteUri);
	bool callAudioVideo(const char* remoteUri);
	bool callVideo(const char* remoteUri);
	bool setSessionTimer(unsigned timeout, const char* refresher);
	bool set100rel(bool enabled);
	bool setQoS(tmedia_qos_stype_t type, tmedia_qos_strength_t strength);
	bool accept();
	bool hold();
	bool resume();
	bool sendDTMF(int number);
	bool hangup();
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
