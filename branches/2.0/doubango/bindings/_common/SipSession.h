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
#ifndef TINYWRAP_SIPSESSION_H
#define TINYWRAP_SIPSESSION_H

#include "tinysip.h"
#include "tinymedia/tmedia_qos.h"
#include "ActionConfig.h"

class SipUri;
class SipStack;
class MsrpCallback;
class MediaSessionMgr;

/* ======================== SipSession ========================*/
class SipSession
{
public:
	SipSession(SipStack* stack);
#if !defined(SWIG)
	SipSession(SipStack* stack, tsip_ssession_handle_t* pHandle);
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
	bool setFromUri(const char* fromUriString);
	bool setFromUri(const SipUri* fromUri);
	bool setToUri(const char* toUriString);
	bool setToUri(const SipUri* toUri);
	bool setSilentHangup(bool silent);
	bool addSigCompCompartment(const char* compId);
	bool removeSigCompCompartment();
	unsigned getId()const;

#if !defined(SWIG)
	const SipStack* getStack() const;
#endif
	
private:
	void init(SipStack* stack, tsip_ssession_handle_t* pHandle=tsk_null);

protected:
	tsip_ssession_handle_t* m_pHandle;
	const SipStack* m_pStack;
};

/* ======================== InviteSession ========================*/
class InviteSession : public SipSession
{
public: /* ctor() and dtor() */
	InviteSession(SipStack* Stack);
#if !defined(SWIG)
	InviteSession(SipStack* pStack, tsip_ssession_handle_t* pHandle);
#endif
	virtual ~InviteSession();

public: /* Public functions */
	bool accept(ActionConfig* config=tsk_null);
	bool hangup(ActionConfig* config=tsk_null);
	bool reject(ActionConfig* config=tsk_null);
	bool sendInfo(const void* payload, unsigned len, ActionConfig* config=tsk_null);
	const MediaSessionMgr* getMediaMgr();

private:
	MediaSessionMgr* m_pMediaMgr;
};


/* ======================== CallSession ========================*/
class CallSession : public InviteSession
{
public: /* ctor() and dtor() */
	CallSession(SipStack* pStack);
#if !defined(SWIG)
	CallSession(SipStack* pStack, tsip_ssession_handle_t* pHandle);
#endif
	virtual ~CallSession();

public: /* Public functions */
	bool callAudio(const char* remoteUriString, ActionConfig* config=tsk_null);
	bool callAudio(const SipUri* remoteUri, ActionConfig* config=tsk_null);
	bool callAudioVideo(const char* remoteUriString, ActionConfig* config=tsk_null);
	bool callAudioVideo(const SipUri* remoteUri, ActionConfig* config=tsk_null);
	bool callVideo(const char* remoteUriString, ActionConfig* config=tsk_null);
	bool callVideo(const SipUri* remoteUri, ActionConfig* config=tsk_null);
	bool setSessionTimer(unsigned timeout, const char* refresher);
	bool set100rel(bool enabled);
	bool setICE(bool enabled);
	bool setQoS(tmedia_qos_stype_t type, tmedia_qos_strength_t strength);
	bool hold(ActionConfig* config=tsk_null);
	bool resume(ActionConfig* config=tsk_null);
	bool transfer(const char* referToUriString, ActionConfig* config=tsk_null);
	bool acceptTransfer(ActionConfig* config=tsk_null);
	bool rejectTransfer(ActionConfig* config=tsk_null);
	bool sendDTMF(int number);
	unsigned getSessionTransferId();
};

/* ======================== MsrpSession ========================*/
class MsrpSession : public InviteSession
{
public: /* ctor() and dtor() */
	MsrpSession(SipStack* pStack, MsrpCallback* pCallback);
#if !defined(SWIG)
	MsrpSession(SipStack* pStack, tsip_ssession_handle_t* pHandle);
#endif
	virtual ~MsrpSession();

public: /* Public functions */
	bool setCallback(MsrpCallback* pCallback);
	bool callMsrp(const char* remoteUriString, ActionConfig* config=tsk_null);
	bool callMsrp(const SipUri* remoteUri, ActionConfig* config=tsk_null);
	bool sendMessage(const void* payload, unsigned len, ActionConfig* config=tsk_null);
	bool sendFile(ActionConfig* config=tsk_null);

	public: /* Public helper function */
#if !defined(SWIG)
		inline MsrpCallback* getCallback()const{
			return m_pCallback;
		}
#endif

private:
	MsrpCallback* m_pCallback;
};



/* ======================== MessagingSession ========================*/
class MessagingSession : public SipSession
{
public: /* ctor() and dtor() */
	MessagingSession(SipStack* pStack);
#if !defined(SWIG)
	MessagingSession(SipStack* pStack, tsip_ssession_handle_t* pHandle);
#endif
	virtual ~MessagingSession();

public: /* Public functions */
	bool send(const void* payload, unsigned len, ActionConfig* config=tsk_null);
	bool accept(ActionConfig* config=tsk_null);
	bool reject(ActionConfig* config=tsk_null);
};

/* ======================== InfoSession ========================*/
class InfoSession : public SipSession
{
public: /* ctor() and dtor() */
	InfoSession(SipStack* pStack);
#if !defined(SWIG)
	InfoSession(SipStack* pStack, tsip_ssession_handle_t* pHandle);
#endif
	virtual ~InfoSession();

public: /* Public functions */
	bool send(const void* payload, unsigned len, ActionConfig* config=tsk_null);
	bool accept(ActionConfig* config=tsk_null);
	bool reject(ActionConfig* config=tsk_null);
};

/* ======================== OptionsSession ========================*/
class OptionsSession : public SipSession
{
public: /* ctor() and dtor() */
	OptionsSession(SipStack* pStack);
#if !defined(SWIG)
	OptionsSession(SipStack* pStack, tsip_ssession_handle_t* pHandle);
#endif
	virtual ~OptionsSession();

public: /* Public functions */
	bool send(ActionConfig* config=tsk_null);
	bool accept(ActionConfig* config=tsk_null);
	bool reject(ActionConfig* config=tsk_null);
};



/* ======================== PublicationSession ========================*/
class PublicationSession : public SipSession
{
public: /* ctor() and dtor() */
	PublicationSession(SipStack* pStack);
	virtual ~PublicationSession();

public: /* Public functions */
	bool publish(const void* payload, unsigned len, ActionConfig* config=tsk_null);
	bool unPublish(ActionConfig* config=tsk_null);
};


/* ======================== RegistrationSession ========================*/
class RegistrationSession : public SipSession
{
public: /* ctor() and dtor() */
	RegistrationSession(SipStack* pStack);
#if !defined(SWIG)
	RegistrationSession(SipStack* pStack, tsip_ssession_handle_t* pHandle);
#endif
	virtual ~RegistrationSession();

public: /* Public functions */
	bool register_(ActionConfig* config=tsk_null);
	bool unRegister(ActionConfig* config=tsk_null);
	bool accept(ActionConfig* config=tsk_null);
	bool reject(ActionConfig* config=tsk_null);
};


/* ======================== SubscriptionSession ========================*/
class SubscriptionSession : public SipSession
{
public: /* ctor() and dtor() */
	SubscriptionSession(SipStack* pStack);
	virtual ~SubscriptionSession();

public: /* Public functions */
	bool subscribe();
	bool unSubscribe();
};

#endif /* TINYWRAP_SIPSESSION_H */
