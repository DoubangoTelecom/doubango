/*Copyright (C) 2013 Doubango Telecom <http://www.doubango.org>
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
*/
#pragma once

#include "rt_Config.h"
#include "rt_SipStack.h"
#include "rt_SipUri.h"
#include "rt_String.h"
#include "rt_ActionConfig.h"
#include "rt_MediaSessionMgr.h"

#include "tinysip.h"

class SipSession;
class InviteSession;
class CallSession;
class MsrpSession;
class MessagingSession;
class InfoSession;
class OptionsSession;
class PublicationSession;
class RegistrationSession;
class SubscriptionSession;
class SipStack;

class T140CallbackData;
class T140Callback;

// error C4585: 'doubango_rt::rtSipSession' : A WinRT 'public ref class' must either be sealed or derive from an existing unsealed class
#define rtISession_Declare() \
	virtual unsigned getId(); \
	virtual bool haveOwnership(); \
	virtual bool addHeader(Platform::String^ name, Platform::String^ value); \
	virtual bool removeHeader(Platform::String^ name); \
	[Windows::Foundation::Metadata::DefaultOverload] \
	virtual bool addCaps(Platform::String^ name, Platform::String^ value); \
	OVERRIDE_FUNC(virtual bool addCaps(Platform::String^ name);) \
	virtual bool removeCaps(Platform::String^ name); \
	virtual bool setExpires(unsigned expires); \
	OVERRIDE_FUNC(virtual bool setFromUri(Platform::String^ fromUriString);) \
	[Windows::Foundation::Metadata::DefaultOverload] \
	virtual bool setFromUri(rtSipUri^ fromUri); \
	OVERRIDE_FUNC(virtual bool setToUri(Platform::String^ toUriString);) \
	[Windows::Foundation::Metadata::DefaultOverload] \
	virtual bool setToUri(rtSipUri^ toUri); \
	virtual bool setSilentHangup(bool silent); \
	virtual bool addSigCompCompartment(Platform::String^ compId); \
	virtual bool removeSigCompCompartment(); \
 
#define rtISession_Implement(cls) \
	unsigned cls##::getId(){ \
	return m_pSipSession->getId(); \
} \
	bool cls##::haveOwnership(){ \
	return m_pSipSession->haveOwnership(); \
} \
	bool cls##::addHeader(Platform::String^ name, Platform::String^ value){ \
	return m_pSipSession->addHeader(rtString::toUtf8(name).data(), rtString::toUtf8(value).data()); \
} \
	bool cls##::removeHeader(Platform::String^ name){ \
	return m_pSipSession->removeHeader(rtString::toUtf8(name).data()); \
} \
	bool cls##::addCaps(Platform::String^ name, Platform::String^ value){ \
	return m_pSipSession->addCaps(rtString::toUtf8(name).data(), rtString::toUtf8(value).data()); \
} \
OVERRIDE_FUNC(bool cls##::addCaps(Platform::String^ name){ \
	return addCaps(name, nullptr); \
}) \
	bool cls##::removeCaps(Platform::String^ name){ \
	return m_pSipSession->removeCaps(rtString::toUtf8(name).data()); \
} \
	bool cls##::setExpires(unsigned expires){ \
	return m_pSipSession->setExpires(expires); \
} \
OVERRIDE_FUNC(bool cls##::setFromUri(Platform::String^ fromUriString){ \
	return m_pSipSession->setFromUri(rtString::toUtf8(fromUriString).data()); \
}) \
	bool cls##::setFromUri(rtSipUri^ fromUri){ \
	return m_pSipSession->setFromUri(fromUri->getWrappedUri()); \
} \
OVERRIDE_FUNC(bool cls##::setToUri(Platform::String^ toUriString){ \
	return m_pSipSession->setToUri(rtString::toUtf8(toUriString).data()); \
}) \
	bool cls##::setToUri(rtSipUri^ toUri){ \
	return m_pSipSession->setToUri(toUri->getWrappedUri()); \
} \
	bool cls##::setSilentHangup(bool silent){ \
	return m_pSipSession->setSilentHangup(silent); \
} \
	bool cls##::addSigCompCompartment(Platform::String^ compId){ \
	return m_pSipSession->addSigCompCompartment(rtString::toUtf8(compId).data()); \
} \
	bool cls##::removeSigCompCompartment(){ \
	return m_pSipSession->removeSigCompCompartment(); \
}\
 

#define rtIInviteSession_Declare() \
	[Windows::Foundation::Metadata::DefaultOverload] \
	virtual bool accept(rtActionConfig^ config); \
	OVERRIDE_FUNC(virtual bool accept();) \
	[Windows::Foundation::Metadata::DefaultOverload] \
	virtual bool hangup(rtActionConfig^ config); \
	OVERRIDE_FUNC(virtual bool hangup();) \
	[Windows::Foundation::Metadata::DefaultOverload] \
	virtual bool reject(rtActionConfig^ config); \
	OVERRIDE_FUNC(virtual bool reject();) \
	[Windows::Foundation::Metadata::DefaultOverload] \
	virtual bool sendInfo(VISIBLE_VOID_PTR payload, unsigned len, rtActionConfig^ config); \
	OVERRIDE_FUNC(virtual bool sendInfo(VISIBLE_VOID_PTR payload, unsigned len);) \
	virtual rtMediaSessionMgr^ getMediaMgr(); \
 
#define rtIInviteSession_Implement(cls) \
bool cls##::accept(rtActionConfig^ config){ \
	return m_pSipSession->accept(config ? config->getWrappedActionConfig() : tsk_null); \
} \
OVERRIDE_FUNC(bool cls##::accept(){ \
	return accept(nullptr); \
}) \
bool cls##::hangup(rtActionConfig^ config){ \
	return m_pSipSession->hangup(config ? config->getWrappedActionConfig() : tsk_null); \
} \
OVERRIDE_FUNC(bool cls##::hangup(){ \
	return hangup(nullptr); \
}) \
bool cls##::reject(rtActionConfig^ config){ \
	return m_pSipSession->reject(config ? config->getWrappedActionConfig() : tsk_null); \
} \
OVERRIDE_FUNC(bool cls##::reject(){ \
	return reject(nullptr); \
}) \
bool cls##::sendInfo(VISIBLE_VOID_PTR payload, unsigned len, rtActionConfig^ config){ \
	return m_pSipSession->sendInfo(VISIBLE_VOID_PTR_2_VOID_PTR(payload), len, config ? config->getWrappedActionConfig() : tsk_null); \
} \
OVERRIDE_FUNC(bool cls##::sendInfo(VISIBLE_VOID_PTR payload, unsigned len){ \
	return sendInfo(payload, len, nullptr); \
}) \
rtMediaSessionMgr^ cls##::getMediaMgr(){ \
	if(m_pSipSession->getMediaMgr()){ \
		return ref new rtMediaSessionMgr(const_cast<struct tmedia_session_mgr_s*>(m_pSipSession->getMediaMgr()->getWrappedMgr())); \
	} \
	return nullptr; \
} \
 
namespace doubango_rt
{
namespace BackEnd
{
ref class rtMsrpCallback;
interface class rtIMsrpCallback;


/* ======================== rtT140CallbackData ========================*/
public ref class rtT140CallbackData sealed
{
internal:
    rtT140CallbackData(enum tmedia_t140_data_type_e data_type, const void* data_ptr, unsigned data_size);
public:
    virtual ~rtT140CallbackData();

private:
    T140CallbackData* m_pData;
};

/* ======================== rtT140Callback ========================*/
public interface class rtIT140Callback
{
    virtual int ondata(rtT140CallbackData^ pData);
};
public ref class rtT140Callback sealed
{
internal:
    rtT140Callback();
    const T140Callback* getWrappedCallback() {
        return m_pCallback;
    }

public:
    rtT140Callback(rtIT140Callback^ pI) {
        m_pI = pI;
    }
    virtual ~rtT140Callback();

protected:
    virtual int ondata(rtT140CallbackData^ pData) {
        if(m_pI) {
            return m_pI->ondata(pData);
        }
        return 0;
    }

private:
    T140Callback* m_pCallback;
    rtIT140Callback^ m_pI;
};


/* ======================== rtISipSession ========================*/
public interface class rtISipSession
{
    rtISession_Declare();
};


/* ======================== rtIInviteSession ========================*/
public interface class rtIInviteSession : rtISipSession
{
    rtIInviteSession_Declare();
};

/* ======================== rtSipSession ========================*/
public ref class rtSipSession sealed: rtISipSession
{
public:
    rtSipSession(rtSipStack^ pStack);
    virtual ~rtSipSession();
internal:
    rtSipSession(SipStack* pStack, tsip_ssession_handle_t* pHandle);
    SipSession* getWrappedSession() {
        return m_pSipSession;
    }

public:
    rtISession_Declare();

private:
    SipSession* m_pSipSession;
};

/* ======================== rtInviteSession ========================*/
public ref class rtInviteSession sealed: rtIInviteSession
{
public:
    rtInviteSession(rtSipStack^ pStack);
    virtual ~rtInviteSession();
internal:
    rtInviteSession(SipStack* pStack, tsip_ssession_handle_t* pHandle);
    InviteSession* getWrappedSession() {
        return m_pSipSession;
    }

public:
    rtISession_Declare();
    rtIInviteSession_Declare();

private:
    InviteSession* m_pSipSession;
};

/* ======================== CallSession ========================*/
public ref class rtCallSession sealed : rtIInviteSession
{
public:
    rtCallSession(rtSipStack^ pStack);
    virtual ~rtCallSession();
internal:
    rtCallSession(SipStack* pStack, tsip_ssession_handle_t* pHandle);
    CallSession* getWrappedSession() {
        return m_pSipSession;
    }

public:
    rtISession_Declare();
    rtIInviteSession_Declare();

    [Windows::Foundation::Metadata::DefaultOverload]
    bool call(Platform::String^ remoteUriString, rt_twrap_media_type_t media, rtActionConfig^ config);
    [Windows::Foundation::Metadata::DefaultOverload]
    bool call(Platform::String^ remoteUriString, rt_twrap_media_type_t media);
    bool call(rtSipUri^ remoteUri, rt_twrap_media_type_t media, rtActionConfig^ config);
    bool call(rtSipUri^ remoteUri, rt_twrap_media_type_t media);

    bool setSessionTimer(unsigned timeout, Platform::String^ refresher);
    bool set100rel(bool enabled);
    bool setRtcp(bool enabled);
    bool setRtcpMux(bool enabled);
    bool setICE(bool enabled);
    bool setQoS(rt_tmedia_qos_stype_t type, rt_tmedia_qos_strength_t strength);
    bool setVideoFps(int32_t fps);
    bool setVideoBandwidthUploadMax(int32_t max);
    bool setVideoBandwidthDownloadMax(int32_t max);
    bool setVideoPrefSize(rt_tmedia_pref_video_size_t pref_video_size);
    bool hold(rtActionConfig^ config);
    bool hold();
    bool resume(rtActionConfig^ config);
    bool resume();
    bool transfer(Platform::String^ referToUriString, rtActionConfig^ config);
    bool transfer(Platform::String^ referToUriString);
    bool acceptTransfer(rtActionConfig^ config);
    bool acceptTransfer();
    bool rejectTransfer(rtActionConfig^ config);
    bool rejectTransfer();
    bool sendDTMF(int number);
    unsigned getSessionTransferId();
#if COM_VISIBLE
    bool sendT140Data(rt_tmedia_t140_data_type_t data_type, Platform::String^ data);
#else
    bool sendT140Data(rt_tmedia_t140_data_type_t data_type, Platform::IntPtr data_ptr, unsigned data_size);
#endif
    bool sendT140Data(rt_tmedia_t140_data_type_t data_type);
    bool setT140Callback(rtT140Callback^ pT140Callback);

private:
    CallSession* m_pSipSession;
};


/* ======================== rtMsrpSession ========================*/
public ref class rtMsrpSession sealed : rtIInviteSession
{
public:
    rtMsrpSession(rtSipStack^ pStack, rtIMsrpCallback^ pCallback);
    virtual ~rtMsrpSession();

internal:
    rtMsrpSession(SipStack* pStack, tsip_ssession_handle_t* pHandle);
    MsrpSession* getWrappedSession() {
        return m_pSipSession;
    }

public:
    rtISession_Declare();
    rtIInviteSession_Declare();

    bool setCallback(rtIMsrpCallback^ pCallback);
    [Windows::Foundation::Metadata::DefaultOverload]
    bool callMsrp(Platform::String^ remoteUriString, rtActionConfig^ config);
    [Windows::Foundation::Metadata::DefaultOverload]
    bool callMsrp(Platform::String^ remoteUriString);
    bool callMsrp(rtSipUri^ remoteUri, rtActionConfig^ config);
    bool callMsrp(rtSipUri^ remoteUri);
#if COM_VISIBLE
    bool sendMessage(Platform::String^ payload, rtActionConfig^ config);
    bool sendMessage(Platform::String^ payload);
#else
    bool sendMessage(Platform::IntPtr payload, unsigned len, rtActionConfig^ config);
    bool sendMessage(Platform::IntPtr payload, unsigned len);
#endif

private:
    MsrpSession* m_pSipSession;
    rtMsrpCallback^ m_pCallback;
};

/* ======================== rtMessagingSession ========================*/
public ref class rtMessagingSession sealed : rtISipSession
{
internal:
    rtMessagingSession(SipStack* pStack, tsip_ssession_handle_t* pHandle);
public:
    rtMessagingSession(rtSipStack^ pStack);
    virtual ~rtMessagingSession();

    rtISession_Declare();

#if COM_VISIBLE
    bool send(Platform::String^ payload, rtActionConfig^ config);
    bool send(Platform::String^ payload);
#else
    [Windows::Foundation::Metadata::DefaultOverload]
    bool send(Platform::IntPtr payload, unsigned len, rtActionConfig^ config);
    bool send(Platform::IntPtr payload, unsigned len);
#endif
    [Windows::Foundation::Metadata::DefaultOverload]
    bool accept(rtActionConfig^ config);
    bool accept();
    [Windows::Foundation::Metadata::DefaultOverload]
    bool reject(rtActionConfig^ config);
    bool reject();

private:
    MessagingSession* m_pSipSession;
};


/* ======================== rtInfoSession ========================*/
public ref class rtInfoSession sealed : rtISipSession
{
internal:
    rtInfoSession(SipStack* pStack, tsip_ssession_handle_t* pHandle);
public:
    rtInfoSession(rtSipStack^ pStack);
    virtual ~rtInfoSession();

    rtISession_Declare();

#if COM_VISIBLE
    bool send(Platform::String^ payload, rtActionConfig^ config);
    bool send(Platform::String^ payload);
#else
    [Windows::Foundation::Metadata::DefaultOverload]
    bool send(Platform::IntPtr payload, unsigned len, rtActionConfig^ config);
    bool send(Platform::IntPtr payload, unsigned len);
#endif
    [Windows::Foundation::Metadata::DefaultOverload]
    bool accept(rtActionConfig^ config);
    bool accept();
    [Windows::Foundation::Metadata::DefaultOverload]
    bool reject(rtActionConfig^ config);
    bool reject();

private:
    InfoSession* m_pSipSession;
};


/* ======================== rtOptionsSession ========================*/
public ref class rtOptionsSession sealed : rtISipSession
{
internal:
    rtOptionsSession(SipStack* pStack, tsip_ssession_handle_t* pHandle);
public:
    rtOptionsSession(rtSipStack^ pStack);
    virtual ~rtOptionsSession();

    rtISession_Declare();

    [Windows::Foundation::Metadata::DefaultOverload]
    bool send(rtActionConfig^ config);
    bool send();
    [Windows::Foundation::Metadata::DefaultOverload]
    bool accept(rtActionConfig^ config);
    bool accept();
    [Windows::Foundation::Metadata::DefaultOverload]
    bool reject(rtActionConfig^ config);
    bool reject();

private:
    OptionsSession* m_pSipSession;
};


/* ======================== rtPublicationSession ========================*/
public ref class rtPublicationSession sealed : rtISipSession
{
internal:
    rtPublicationSession(SipStack* pStack, tsip_ssession_handle_t* pHandle);
public:
    rtPublicationSession(rtSipStack^ pStack);
    virtual ~rtPublicationSession();

    rtISession_Declare();

#if COM_VISIBLE
    bool publish(Platform::String^ payload, rtActionConfig^ config);
    bool publish(Platform::String^ payload);
#else
    [Windows::Foundation::Metadata::DefaultOverload]
    bool publish(Platform::IntPtr payload, unsigned len, rtActionConfig^ config);
    bool publish(Platform::IntPtr payload, unsigned len);
#endif
    [Windows::Foundation::Metadata::DefaultOverload]
    bool unPublish(rtActionConfig^ config);
    bool unPublish();

private:
    PublicationSession* m_pSipSession;
};


/* ======================== rtRegistrationSession ========================*/
public ref class rtRegistrationSession sealed : rtISipSession
{
internal:
    rtRegistrationSession(SipStack* pStack, tsip_ssession_handle_t* pHandle);
public:
    rtRegistrationSession(rtSipStack^ pStack);
    virtual ~rtRegistrationSession();

    rtISession_Declare();

    bool register_(rtActionConfig^ config);
    bool register_();
    bool unRegister(rtActionConfig^ config);
    bool unRegister();
    bool accept(rtActionConfig^ config);
    bool accept();
    bool reject(rtActionConfig^ config);
    bool reject();

private:
    RegistrationSession* m_pSipSession;
};

/* ======================== rtSubscriptionSession ========================*/
public ref class rtSubscriptionSession sealed : rtISipSession
{
internal:
    rtSubscriptionSession(SipStack* pStack, tsip_ssession_handle_t* pHandle);
public:
    rtSubscriptionSession(rtSipStack^ pStack);
    virtual ~rtSubscriptionSession();

    rtISession_Declare();

    bool subscribe();
    bool unSubscribe();

private:
    SubscriptionSession* m_pSipSession;
};
}
}
