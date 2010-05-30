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
#ifndef TINYWRAP_SIPSESSION_H
#define TINYWRAP_SIPSESSION_H

#include "tinysip.h"

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
	bool Send(const void* payload, unsigned len);
	bool Accept();
	bool Reject();
};

/* ======================== OptionsSession ========================*/
class OptionsSession : public SipSession
{
public: /* ctor() and dtor() */
	OptionsSession(SipStack* Stack);
	virtual ~OptionsSession();

public: /* Public functions */
	bool Send();
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
	bool Publish(const void* payload, unsigned len);
	bool UnPublish();
};


/* ======================== RegistrationSession ========================*/
class RegistrationSession : public SipSession
{
public: /* ctor() and dtor() */
	RegistrationSession(SipStack* Stack);
	virtual ~RegistrationSession();

public: /* Public functions */
	bool Register();
	bool UnRegister();
};


/* ======================== SubscriptionSession ========================*/
class SubscriptionSession : public SipSession
{
public: /* ctor() and dtor() */
	SubscriptionSession(SipStack* Stack);
	virtual ~SubscriptionSession();

public: /* Public functions */
	bool Subscribe();
	bool UnSubscribe();
};

#endif /* TINYWRAP_SIPSESSION_H */
