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
#include "rt_SipSession.h"
#include "rt_String.h"
#include "rt_SipStack.h"
#include "rt_SipUri.h"
#include "rt_Msrp.h"

#include "SipSession.h"
#include "MediaSessionMgr.h"

using namespace doubango_rt::BackEnd;
using namespace Platform;
using namespace std;

/* ======================== T140Callback ========================*/
rtT140CallbackData::rtT140CallbackData(enum tmedia_t140_data_type_e data_type, const void* data_ptr, unsigned data_size)
{
    m_pData = new T140CallbackData(data_type, data_ptr, data_size);
}

rtT140CallbackData::~rtT140CallbackData()
{
    rtSafeDelete(m_pData);
}

/* ======================== rtT140Callback ========================*/

rtT140Callback::rtT140Callback()
{
    m_pCallback = new T140Callback();
}

rtT140Callback::~rtT140Callback()
{
    rtSafeDelete(m_pCallback);
}

/* ======================== rtSipSession ========================*/
rtISession_Implement(rtSipSession);

rtSipSession::rtSipSession(rtSipStack^ pStack)
{
    m_pSipSession = new SipSession(const_cast<SipStack*>(pStack->getWrappedStack()));
}

rtSipSession::rtSipSession(SipStack* pStack, tsip_ssession_handle_t* pHandle)
{
    m_pSipSession = new SipSession(pStack, pHandle);
}

rtSipSession::~rtSipSession()
{
    rtSafeDelete(m_pSipSession);
}


/* ======================== rtInviteSession ========================*/
rtISession_Implement(rtInviteSession);
rtIInviteSession_Implement(rtInviteSession);

rtInviteSession::rtInviteSession(rtSipStack^ pStack)
{
    m_pSipSession = new InviteSession(const_cast<SipStack*>(pStack->getWrappedStack()));
}

rtInviteSession::rtInviteSession(SipStack* pStack, tsip_ssession_handle_t* pHandle)
{
    m_pSipSession = new InviteSession(pStack, pHandle);
}

rtInviteSession::~rtInviteSession()
{
    rtSafeDelete(m_pSipSession);
}


/* ======================== rtCallSession ========================*/
rtISession_Implement(rtCallSession);
rtIInviteSession_Implement(rtCallSession);

rtCallSession::rtCallSession(rtSipStack^ pStack)
{
    m_pSipSession = new CallSession(const_cast<SipStack*>(pStack->getWrappedStack()));
}

rtCallSession::rtCallSession(SipStack* pStack, tsip_ssession_handle_t* pHandle)
{
    m_pSipSession = new CallSession(pStack, pHandle);
}

rtCallSession::~rtCallSession()
{
    rtSafeDelete(m_pSipSession);
}


bool rtCallSession::call(String^ remoteUriString, rt_twrap_media_type_t media, rtActionConfig^ config)
{
    return m_pSipSession->call(rtString::toUtf8(remoteUriString).data(), (twrap_media_type_t)media, config ? config->getWrappedActionConfig() : tsk_null);

}

bool rtCallSession::call(String^ remoteUriString, rt_twrap_media_type_t media)
{
    return call(remoteUriString, media, nullptr);
}

bool rtCallSession::call(rtSipUri^ remoteUri, rt_twrap_media_type_t media, rtActionConfig^ config)
{
    return m_pSipSession->call(remoteUri->getWrappedUri(), (twrap_media_type_t)media, config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtCallSession::call(rtSipUri^ remoteUri, rt_twrap_media_type_t media)
{
    return call(remoteUri, media, nullptr);
}

bool rtCallSession::setSessionTimer(unsigned timeout, String^ refresher)
{
    return m_pSipSession->setSessionTimer(timeout, rtString::toUtf8(refresher).data());
}

bool rtCallSession::set100rel(bool enabled)
{
    return m_pSipSession->set100rel(enabled);
}

bool rtCallSession::setRtcp(bool enabled)
{
    return m_pSipSession->setRtcp(enabled);
}

bool rtCallSession::setRtcpMux(bool enabled)
{
    return m_pSipSession->setRtcpMux(enabled);
}

bool rtCallSession::setICE(bool enabled)
{
    return m_pSipSession->setICE(enabled);
}

bool rtCallSession::setQoS(rt_tmedia_qos_stype_t type, rt_tmedia_qos_strength_t strength)
{
    return m_pSipSession->setQoS((tmedia_qos_stype_t) type, (tmedia_qos_strength_t) strength);
}

bool rtCallSession::setVideoFps(int32_t fps)
{
    return m_pSipSession->setVideoFps(fps);
}

bool rtCallSession::setVideoBandwidthUploadMax(int32_t max)
{
    return m_pSipSession->setVideoBandwidthUploadMax(max);
}

bool rtCallSession::setVideoBandwidthDownloadMax(int32_t max)
{
    return m_pSipSession->setVideoBandwidthDownloadMax(max);
}

bool rtCallSession::setVideoPrefSize(rt_tmedia_pref_video_size_t pref_video_size)
{
    return m_pSipSession->setVideoPrefSize((tmedia_pref_video_size_t)pref_video_size);
}

bool rtCallSession::hold(rtActionConfig^ config)
{
    return m_pSipSession->hold(config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtCallSession::hold()
{
    return hold(nullptr);
}

bool rtCallSession::resume(rtActionConfig^ config)
{
    return m_pSipSession->resume(config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtCallSession::resume()
{
    return resume(nullptr);
}

bool rtCallSession::transfer(String^ referToUriString, rtActionConfig^ config)
{
    return m_pSipSession->transfer(rtString::toUtf8(referToUriString).data(), config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtCallSession::transfer(String^ referToUriString)
{
    return transfer(referToUriString, nullptr);
}

bool rtCallSession::acceptTransfer(rtActionConfig^ config)
{
    return m_pSipSession->acceptTransfer(config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtCallSession::acceptTransfer()
{
    return acceptTransfer(nullptr);
}

bool rtCallSession::rejectTransfer(rtActionConfig^ config)
{
    return m_pSipSession->rejectTransfer(config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtCallSession::rejectTransfer()
{
    return rejectTransfer(nullptr);
}

bool rtCallSession::sendDTMF(int number)
{
    return m_pSipSession->sendDTMF(number);
}

unsigned rtCallSession::getSessionTransferId()
{
    return m_pSipSession->getSessionTransferId();
}

#if COM_VISIBLE
bool rtCallSession::sendT140Data(rt_tmedia_t140_data_type_t data_type, String^ data)
{
    std::vector<char> _data = rtString::toUtf8(data);
    return m_pSipSession->sendT140Data((tmedia_t140_data_type_t) data_type, (const void*)_data.data(), _data.size());
}
#else
bool rtCallSession::sendT140Data(rt_tmedia_t140_data_type_t data_type, IntPtr data_ptr, unsigned data_size)
{
    return m_pSipSession->sendT140Data((tmedia_t140_data_type_t) data_type, (const void*)data_ptr, data_size);
}

#endif

bool rtCallSession::sendT140Data(rt_tmedia_t140_data_type_t data_type)
{
    return sendT140Data(data_type, nullptr
#if !COM_VISIBLE
                        , 0
#endif
                       );
}

bool rtCallSession::setT140Callback(rtT140Callback^ pT140Callback)
{
    return m_pSipSession->setT140Callback(const_cast<T140Callback*>(pT140Callback->getWrappedCallback()));
}


/* ======================== rtMsrpSession ========================*/
rtISession_Implement(rtMsrpSession);
rtIInviteSession_Implement(rtMsrpSession);

rtMsrpSession::rtMsrpSession(rtSipStack^ pStack, rtIMsrpCallback^ pCallback)
{
    m_pCallback = ref new rtMsrpCallback(pCallback);
    m_pSipSession = new MsrpSession(const_cast<SipStack*>(pStack->getWrappedStack()), const_cast<MsrpCallback*>(m_pCallback->getWrappedCallback()));
}

rtMsrpSession::rtMsrpSession(SipStack* pStack, tsip_ssession_handle_t* pHandle)
{
    m_pSipSession = new MsrpSession(pStack, pHandle);
}

rtMsrpSession::~rtMsrpSession()
{
    rtSafeDelete(m_pSipSession);
    rtSafeDelete(m_pCallback);
}

bool rtMsrpSession::setCallback(rtIMsrpCallback^ pCallback)
{
    rtSafeDelete(m_pCallback);
    if(!pCallback) {
        return m_pSipSession->setCallback(tsk_null);
        return true;
    }
    m_pCallback = ref new rtMsrpCallback(pCallback);
    return m_pSipSession->setCallback(const_cast<MsrpCallback*>(m_pCallback->getWrappedCallback()));
}

bool rtMsrpSession::callMsrp(Platform::String^ remoteUriString, rtActionConfig^ config)
{
    return m_pSipSession->callMsrp(rtString::toUtf8(remoteUriString).data(), config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtMsrpSession::callMsrp(Platform::String^ remoteUriString)
{
    return callMsrp(remoteUriString, nullptr);
}

bool rtMsrpSession::callMsrp(rtSipUri^ remoteUri, rtActionConfig^ config)
{
    return m_pSipSession->callMsrp(remoteUri->getWrappedUri(), config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtMsrpSession::callMsrp(rtSipUri^ remoteUri)
{
    return callMsrp(remoteUri, nullptr);
}

#if COM_VISIBLE
bool rtMsrpSession::sendMessage(String^ payload, rtActionConfig^ config)
{
    std::vector<char> _payload = rtString::toUtf8(payload);
    return m_pSipSession->sendMessage((const void*)_payload.data(), _payload.size(), config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtMsrpSession::sendMessage(Platform::String^ payload)
{
    return sendMessage(payload, nullptr);
}

#else
bool rtMsrpSession::sendMessage(Platform::IntPtr payload, unsigned len, rtActionConfig^ config)
{
    return m_pSipSession->sendMessage((const void*)payload, len, config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtMsrpSession::sendMessage(Platform::IntPtr payload, unsigned len)
{
    return sendMessage(payload, len, nullptr);
}
#endif /* COM_VISIBLE */


/* ======================== rtMessagingSession ========================*/
rtISession_Implement(rtMessagingSession);

rtMessagingSession::rtMessagingSession(SipStack* pStack, tsip_ssession_handle_t* pHandle)
{
    m_pSipSession = new MessagingSession(pStack, pHandle);
}

rtMessagingSession::rtMessagingSession(rtSipStack^ pStack)
{
    m_pSipSession = new MessagingSession(const_cast<SipStack*>(pStack->getWrappedStack()));
}

rtMessagingSession::~rtMessagingSession()
{
    rtSafeDelete(m_pSipSession);
}

#if COM_VISIBLE
bool rtMessagingSession::send(String^ payload, rtActionConfig^ config)
{
    std::vector<char>_payload = rtString::toUtf8(payload);
    return m_pSipSession->send((const void*)_payload.data(), _payload.size(), config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtMessagingSession::send(Platform::String^ payload)
{
    return send(payload, nullptr);
}

#else
bool rtMessagingSession::send(IntPtr payload, unsigned len, rtActionConfig^ config)
{
    return m_pSipSession->send((const void*)payload, len, config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtMessagingSession::send(IntPtr payload, unsigned len)
{
    return send(payload, len, nullptr);
}
#endif

bool rtMessagingSession::accept(rtActionConfig^ config)
{
    return m_pSipSession->accept(config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtMessagingSession::accept()
{
    return accept(nullptr);
}

bool rtMessagingSession::reject(rtActionConfig^ config)
{
    return m_pSipSession->reject(config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtMessagingSession::reject()
{
    return reject(nullptr);
}

/* ======================== rtInfoSession ========================*/
rtISession_Implement(rtInfoSession);

rtInfoSession::rtInfoSession(SipStack* pStack, tsip_ssession_handle_t* pHandle)
{
    m_pSipSession = new InfoSession(pStack, pHandle);
}

rtInfoSession::rtInfoSession(rtSipStack^ pStack)
{
    m_pSipSession = new InfoSession(const_cast<SipStack*>(pStack->getWrappedStack()));
}

rtInfoSession::~rtInfoSession()
{
    rtSafeDelete(m_pSipSession);
}

#if COM_VISIBLE
bool rtInfoSession::send(Platform::String^ payload, rtActionConfig^ config)
{
    std::vector<char>_payload = rtString::toUtf8(payload);
    return m_pSipSession->send((const void*)_payload.data(), _payload.size(), config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtInfoSession::send(Platform::String^ payload)
{
    return send(payload, nullptr);
}
#else
bool rtInfoSession::send(IntPtr payload, unsigned len, rtActionConfig^ config)
{
    return m_pSipSession->send((const void*)payload, len, config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtInfoSession::send(IntPtr payload, unsigned len)
{
    return send(payload, len, nullptr);
}
#endif

bool rtInfoSession::accept(rtActionConfig^ config)
{
    return m_pSipSession->accept(config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtInfoSession::accept()
{
    return accept(nullptr);
}

bool rtInfoSession::reject(rtActionConfig^ config)
{
    return m_pSipSession->reject(config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtInfoSession::reject()
{
    return reject(nullptr);
}



/* ======================== rtOptionsSession ========================*/
rtISession_Implement(rtOptionsSession);

rtOptionsSession::rtOptionsSession(SipStack* pStack, tsip_ssession_handle_t* pHandle)
{
    m_pSipSession = new OptionsSession(pStack, pHandle);
}

rtOptionsSession::rtOptionsSession(rtSipStack^ pStack)
{
    m_pSipSession = new OptionsSession(const_cast<SipStack*>(pStack->getWrappedStack()));
}

rtOptionsSession::~rtOptionsSession()
{
    rtSafeDelete(m_pSipSession);
}

bool rtOptionsSession::send(rtActionConfig^ config)
{
    return m_pSipSession->send(config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtOptionsSession::send()
{
    return send(nullptr);
}

bool rtOptionsSession::accept(rtActionConfig^ config)
{
    return m_pSipSession->accept(config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtOptionsSession::accept()
{
    return accept(nullptr);
}

bool rtOptionsSession::reject(rtActionConfig^ config)
{
    return m_pSipSession->reject(config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtOptionsSession::reject()
{
    return reject(nullptr);
}



/* ======================== rtPublicationSession ========================*/
rtISession_Implement(rtPublicationSession);

rtPublicationSession::rtPublicationSession(rtSipStack^ pStack)
{
    m_pSipSession = new PublicationSession(const_cast<SipStack*>(pStack->getWrappedStack()));
}

rtPublicationSession::rtPublicationSession(SipStack* pStack, tsip_ssession_handle_t* pHandle)
{
    m_pSipSession = new PublicationSession(pStack, pHandle);
}

rtPublicationSession::~rtPublicationSession()
{
    rtSafeDelete(m_pSipSession);
}

#if COM_VISIBLE
bool rtPublicationSession::publish(String^ payload, rtActionConfig^ config)
{
    std::vector<char>_payload = rtString::toUtf8(payload);
    return m_pSipSession->publish((const void*)_payload.data(), _payload.size(), config ? config->getWrappedActionConfig() : tsk_null);
}
bool rtPublicationSession::publish(Platform::String^ payload)
{
    return publish(payload, nullptr);
}
#else
bool rtPublicationSession::publish(Platform::IntPtr payload, unsigned len, rtActionConfig^ config)
{
    return m_pSipSession->publish((const void*)payload, len, config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtPublicationSession::publish(Platform::IntPtr payload, unsigned len)
{
    return publish(payload, len, nullptr);
}
#endif

bool rtPublicationSession::unPublish(rtActionConfig^ config)
{
    return m_pSipSession->unPublish(config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtPublicationSession::unPublish()
{
    return unPublish(nullptr);
}


/* ======================== rtRegistrationSession ========================*/
rtISession_Implement(rtRegistrationSession);

rtRegistrationSession::rtRegistrationSession(SipStack* pStack, tsip_ssession_handle_t* pHandle)
{
    m_pSipSession = new RegistrationSession(pStack, pHandle);
}

rtRegistrationSession::rtRegistrationSession(rtSipStack^ pStack)
{
    m_pSipSession = new RegistrationSession(const_cast<SipStack*>(pStack->getWrappedStack()));
}

rtRegistrationSession::~rtRegistrationSession()
{
    rtSafeDelete(m_pSipSession);
}

bool rtRegistrationSession::register_(rtActionConfig^ config)
{
    return m_pSipSession->register_(config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtRegistrationSession::register_()
{
    return register_(nullptr);
}

bool rtRegistrationSession::unRegister(rtActionConfig^ config)
{
    return m_pSipSession->unRegister(config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtRegistrationSession::unRegister()
{
    return unRegister(nullptr);
}

bool rtRegistrationSession::accept(rtActionConfig^ config)
{
    return m_pSipSession->accept(config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtRegistrationSession::accept()
{
    return accept(nullptr);
}

bool rtRegistrationSession::reject(rtActionConfig^ config)
{
    return m_pSipSession->reject(config ? config->getWrappedActionConfig() : tsk_null);
}

bool rtRegistrationSession::reject()
{
    return reject(nullptr);
}


/* ======================== rtSubscriptionSession ========================*/
rtISession_Implement(rtSubscriptionSession);

rtSubscriptionSession::rtSubscriptionSession(rtSipStack^ pStack)
{
    m_pSipSession = new SubscriptionSession(const_cast<SipStack*>(pStack->getWrappedStack()));
}

rtSubscriptionSession::rtSubscriptionSession(SipStack* pStack, tsip_ssession_handle_t* pHandle)
{
    m_pSipSession = new SubscriptionSession(pStack, pHandle);
}

rtSubscriptionSession::~rtSubscriptionSession()
{
    rtSafeDelete(m_pSipSession);
}

bool rtSubscriptionSession::subscribe()
{
    return m_pSipSession->subscribe();
}

bool rtSubscriptionSession::unSubscribe()
{
    return m_pSipSession->unSubscribe();
}