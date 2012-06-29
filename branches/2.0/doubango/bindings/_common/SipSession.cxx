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
#include "SipSession.h"
#include "SipStack.h"
#include "MediaSessionMgr.h"
#include "SipUri.h"
#include "Msrp.h"

/* ======================== AsyncAction ========================*/
typedef struct twrap_async_action_s
{
	const tsip_ssession_handle_t *session;
	const ActionConfig* config;
}
twrap_async_action_t;




/* ======================== SipSession ========================*/
SipSession::SipSession(SipStack* stack)
{
	init(stack);
}

SipSession::SipSession(SipStack* stack, tsip_ssession_handle_t* pHandle)
{
	init(stack, pHandle);
}

SipSession::~SipSession()
{
	tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_USERDATA(tsk_null),
		TSIP_SSESSION_SET_NULL());

	TSK_OBJECT_SAFE_FREE(m_pHandle);
}

void SipSession::init(SipStack* pStack, tsip_ssession_handle_t* pHandle/*=tsk_null*/)
{
	if(pHandle){
		/* "server-side-session" */
		if(tsip_ssession_take_ownership(pHandle)){ /* should never happen */
			TSK_DEBUG_ERROR("Failed to take ownership");
			return;
		}
		m_pHandle = pHandle;
	}
	else{
		/* "client-side-session" */
		m_pHandle = tsip_ssession_create(pStack->getHandle(),
			TSIP_SSESSION_SET_USERDATA(this),
			TSIP_SSESSION_SET_NULL());
	}

	/* set userdata (context) and ref. the stack handle */
	tsip_ssession_set(m_pHandle,
			TSIP_SSESSION_SET_USERDATA(this),
			TSIP_SSESSION_SET_NULL());
	m_pStack = pStack;
}

bool SipSession::addHeader(const char* name, const char* value)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_HEADER(name, value),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::haveOwnership()
{
	return (tsip_ssession_have_ownership(m_pHandle) == tsk_true);
}

bool SipSession::removeHeader(const char* name)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_UNSET_HEADER(name),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::addCaps(const char* name, const char* value)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_CAPS(name, value),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::addCaps(const char* name)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_CAPS(name, tsk_null),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::removeCaps(const char* name)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_UNSET_CAPS(name),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::setExpires(unsigned expires)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_EXPIRES(expires),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::setFromUri(const char* fromUriString)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_FROM_STR(fromUriString),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::setFromUri(const SipUri* fromUri)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_FROM_OBJ(fromUri->getWrappedUri()),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::setToUri(const char* toUriString)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_TO_STR(toUriString),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::setToUri(const SipUri* toUri)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_TO_OBJ(toUri->getWrappedUri()),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::setSilentHangup(bool silent)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_SILENT_HANGUP(silent ? tsk_true : tsk_false),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::addSigCompCompartment(const char* compId)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_SIGCOMP_COMPARTMENT(compId),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool SipSession::removeSigCompCompartment()
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_UNSET_SIGCOMP_COMPARTMENT(),
		TSIP_SSESSION_SET_NULL()) == 0);
}

// FIXME: should be "uint64_t" instead of "unsigned"
unsigned SipSession::getId()const
{
	return (unsigned)tsip_ssession_get_id(m_pHandle);
}

const SipStack* SipSession::getStack()const
{
	return m_pStack;
}


/* ======================== InviteSession ========================*/

InviteSession::InviteSession(SipStack* pStack)
: SipSession(pStack), m_pMediaMgr(tsk_null)
{
}

InviteSession::InviteSession(SipStack* pStack, tsip_ssession_handle_t* pHandle)
: SipSession(pStack, pHandle), m_pMediaMgr(tsk_null)
{

}

InviteSession::~InviteSession()
{
	if(m_pMediaMgr){
		delete m_pMediaMgr, m_pMediaMgr = tsk_null;
	}
}

#if ANDROID
static void *__droid_hangup(void *param)
{	
	twrap_async_action_t* asyn_action = (twrap_async_action_t*)param;
	const tsip_action_handle_t* action_cfg = asyn_action->config ? asyn_action->config->getHandle() : tsk_null;

	tsip_action_BYE(asyn_action->session,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL());

	return tsk_null;
}

bool InviteSession::hangup(ActionConfig* config/*=tsk_null*/)
{
	void* tid[1] = {0};
	tsip_ssession_handle_t *handle;
	int ret;
	twrap_async_action_t asyn_action = {0};
	
	handle = tsk_object_ref(m_pHandle);
	asyn_action.config = config;
	asyn_action.session = handle;
	ret = tsk_thread_create(tid, __droid_hangup, &asyn_action);
	tsk_thread_join(tid);
	tsk_object_unref(handle);

	return (ret == 0);
}
#else
bool InviteSession::hangup(ActionConfig* config/*=tsk_null*/)
{
	return (tsip_api_invite_send_bye(m_pHandle,
		TSIP_ACTION_SET_NULL()) == 0);
}
#endif

#if ANDROID
static void *__droid_reject(void *param)
{	
	twrap_async_action_t* asyn_action = (twrap_async_action_t*)param;
	const tsip_action_handle_t* action_cfg = asyn_action->config ? asyn_action->config->getHandle() : tsk_null;

	tsip_api_common_reject(asyn_action->session,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL());

	return tsk_null;
}

bool InviteSession::reject(ActionConfig* config/*=tsk_null*/)
{
	void* tid[1] = {0};
	tsip_ssession_handle_t *handle;
	int ret;
	twrap_async_action_t asyn_action = {0};
	
	handle = tsk_object_ref(m_pHandle);
	asyn_action.config = config;
	asyn_action.session = handle;
	ret = tsk_thread_create(tid, __droid_reject, &asyn_action);
	tsk_thread_join(tid);
	tsk_object_unref(handle);

	return (ret == 0);
}
#else
bool InviteSession::reject(ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;

	return (tsip_api_common_reject(m_pHandle,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}
#endif

#if ANDROID
static void *__droid_accept(void *param)
{	
	twrap_async_action_t* asyn_action = (twrap_async_action_t*)param;
	const tsip_action_handle_t* action_cfg = asyn_action->config ? asyn_action->config->getHandle() : tsk_null;

	tsip_api_common_accept(asyn_action->session,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL());

	return tsk_null;
}

bool InviteSession::accept(ActionConfig* config/*=tsk_null*/)
{
	void* tid[1] = {0};
	tsip_ssession_handle_t *handle;
	int ret;
	twrap_async_action_t asyn_action = {0};
	
	
	handle = tsk_object_ref(m_pHandle);
	asyn_action.config = config;
	asyn_action.session = handle;
	ret = tsk_thread_create(tid, __droid_accept, &asyn_action);
	tsk_thread_join(tid);
	tsk_object_unref(handle);

	return (ret == 0);
}
#else
bool InviteSession::accept(ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;

	return (tsip_api_common_accept(m_pHandle,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}
#endif

bool InviteSession::sendInfo(const void* payload, unsigned len, ActionConfig* config/*=tsk_null*/)
{
	int ret;
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	if(payload && len){
		ret = tsip_api_invite_send_info(m_pHandle,
			TSIP_ACTION_SET_PAYLOAD(payload, len),
			TSIP_ACTION_SET_CONFIG(action_cfg),
			TSIP_ACTION_SET_NULL());
	}
	else{
		ret = tsip_api_invite_send_info(m_pHandle,
			TSIP_ACTION_SET_CONFIG(action_cfg),
			TSIP_ACTION_SET_NULL());
	}
	return (ret == 0);
}

const MediaSessionMgr* InviteSession::getMediaMgr()
{
	if(!m_pMediaMgr && m_pHandle){
		tmedia_session_mgr_t* mgr = tsip_session_get_mediamgr(m_pHandle);
		if(mgr){
			m_pMediaMgr = new MediaSessionMgr(mgr); // new() it's not yours: see "m_pMediaMgr" which is destroy in the destructor()
			tsk_object_unref(mgr);
		}
		else{
			TSK_DEBUG_WARN("No media session associated to this session");
		}
	}
	return m_pMediaMgr;
}


/* ======================== CallSession ========================*/
CallSession::CallSession(SipStack* Stack)
: InviteSession(Stack)
{
}

CallSession::CallSession(SipStack* Stack, tsip_ssession_handle_t* handle)
: InviteSession(Stack, handle)
{
}

CallSession::~CallSession()
{
}

#define ANDROID32 1

#if ANDROID
typedef struct twrap_async_action_call_s
{
	const tsip_ssession_handle_t *session;
	const ActionConfig* config;
	tmedia_type_t media_type;
}
twrap_async_action_call_t;

static void *__droid_call_thread(void *param)
{	
	twrap_async_action_call_t* asyn_action = (twrap_async_action_call_t*)param;
	const tsip_action_handle_t* action_cfg = asyn_action->config ? asyn_action->config->getHandle() : tsk_null;

	tsip_action_INVITE(asyn_action->session, asyn_action->media_type,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL());

	return tsk_null;
}

static bool __droid_call(tsip_ssession_handle_t * session_handle, tmedia_type_t type, ActionConfig* config/*=tsk_null*/)
{
	void* tid[1] = {0};
	tsip_ssession_handle_t *handle;
	int ret;
	twrap_async_action_call_t asyn_action = {0};
	
	handle = tsk_object_ref(session_handle);
	asyn_action.config = config;
	asyn_action.session = handle;
	asyn_action.media_type = type;

	ret = tsk_thread_create(tid, __droid_call_thread, &asyn_action);
	tsk_thread_join(tid);
	tsk_object_unref(handle);

	return (ret == 0);
}
#endif

bool CallSession::callAudio(const SipUri* remoteUri, ActionConfig* config/*=tsk_null*/)
{
	if(!remoteUri){
		TSK_DEBUG_ERROR("Invalid parameter");
		return false;
	}
	tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_TO_OBJ(remoteUri->getWrappedUri()),
		TSIP_SSESSION_SET_NULL());
#if ANDROID
	__droid_call(m_pHandle, tmedia_audio, config);
	return true;
#else
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	return (tsip_api_invite_send_invite(m_pHandle, tmedia_audio,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
#endif
}

bool CallSession::callAudio(const char* remoteUriString, ActionConfig* config/*=tsk_null*/)
{
	SipUri sipUri(remoteUriString);
	if(sipUri.isValid()){
		return callAudio(&sipUri, config);
	}
	TSK_DEBUG_ERROR("Failed to parse sip uri=%s", remoteUriString);
	return false;
}

bool CallSession::callAudioVideo(const SipUri* remoteUri, ActionConfig* config/*=tsk_null*/)
{
	if(!remoteUri){
		TSK_DEBUG_ERROR("Invalid parameter");
		return false;
	}
	tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_TO_OBJ(remoteUri->getWrappedUri()),
		TSIP_SSESSION_SET_NULL());
#if ANDROID
	__droid_call(m_pHandle, (tmedia_type_t)(tmedia_audio | tmedia_video), config);
	return true;
#else
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	return (tsip_api_invite_send_invite(m_pHandle, (tmedia_type_t)(tmedia_audio | tmedia_video),
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
#endif
}

bool CallSession::callAudioVideo(const char* remoteUriString, ActionConfig* config/*=tsk_null*/)
{
	SipUri sipUri(remoteUriString);
	if(sipUri.isValid()){
		return callAudioVideo(&sipUri, config);
	}
	TSK_DEBUG_ERROR("Failed to parse sip uri=%s", remoteUriString);
	return false;
}

bool CallSession::callVideo(const SipUri* remoteUri, ActionConfig* config/*=tsk_null*/)
{
	if(!remoteUri){
		TSK_DEBUG_ERROR("Invalid parameter");
		return false;
	}
	tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_TO_OBJ(remoteUri->getWrappedUri()),
		TSIP_SSESSION_SET_NULL());
#if ANDROID
	__droid_call(m_pHandle, tmedia_video, config);
	return true;
#else
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	return (tsip_api_invite_send_invite(m_pHandle, tmedia_video,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
#endif
}

bool CallSession::callVideo(const char* remoteUriString, ActionConfig* config/*=tsk_null*/)
{
	SipUri sipUri(remoteUriString);
	if(sipUri.isValid()){
		return callVideo(&sipUri, config);
	}
	TSK_DEBUG_ERROR("Failed to parse sip uri=%s", remoteUriString);
	return false;
}

bool CallSession::setSessionTimer(unsigned timeout, const char* refresher)
{
	return (tsip_ssession_set(m_pHandle,
			TSIP_SSESSION_SET_MEDIA(
				TSIP_MSESSION_SET_TIMERS(timeout, refresher),
				TSIP_MSESSION_SET_NULL()
			),
			TSIP_SSESSION_SET_NULL()) == 0);
}

bool CallSession::set100rel(bool enabled)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_MEDIA(
		TSIP_MSESSION_SET_100rel(enabled ? tsk_true : tsk_false),
			TSIP_MSESSION_SET_NULL()
		),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool CallSession::setRtcp(bool enabled)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_MEDIA(
		TSIP_MSESSION_SET_RTCP(enabled ? tsk_true : tsk_false),
			TSIP_MSESSION_SET_NULL()
		),
		TSIP_SSESSION_SET_NULL()) == 0);
	
}

bool CallSession::setRtcpMux(bool enabled)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_MEDIA(
		TSIP_MSESSION_SET_RTCPMUX(enabled ? tsk_true : tsk_false),
			TSIP_MSESSION_SET_NULL()
		),
		TSIP_SSESSION_SET_NULL()) == 0);
	
}

bool CallSession::setICE(bool enabled)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_MEDIA(
		TSIP_MSESSION_SET_ICE(enabled ? tsk_true : tsk_false),
			TSIP_MSESSION_SET_NULL()
		),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool CallSession::setQoS(tmedia_qos_stype_t type, tmedia_qos_strength_t strength)
{
	return (tsip_ssession_set(m_pHandle,
			TSIP_SSESSION_SET_MEDIA(
				TSIP_MSESSION_SET_QOS(type, strength),
				TSIP_MSESSION_SET_NULL()
			),
			TSIP_SSESSION_SET_NULL()) == 0);
}

bool CallSession::hold(ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;

	return (tsip_api_invite_send_hold(m_pHandle, tmedia_all,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) ==0 );
}

bool CallSession::resume(ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;

	return (tsip_api_invite_send_resume(m_pHandle, tmedia_all,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}

bool CallSession::transfer(const char* referToUriString, ActionConfig* config/*=tsk_null*/)
{
	if(tsk_strnullORempty(referToUriString)){
		TSK_DEBUG_ERROR("Invalid parameter");
		return false;
	}
	
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	return (tsip_api_invite_send_ect(m_pHandle, referToUriString,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}

bool CallSession::acceptTransfer(ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	return (tsip_api_invite_send_ect_accept(m_pHandle,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}

bool CallSession::rejectTransfer(ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	return (tsip_api_invite_send_ect_reject(m_pHandle,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}

bool CallSession::sendDTMF(int number)
{
	return (tsip_api_invite_send_dtmf(m_pHandle, number,
		TSIP_ACTION_SET_NULL()) == 0);
}

unsigned CallSession::getSessionTransferId()
{
	return (unsigned)tsip_ssession_get_id_parent(m_pHandle);
}


/* ======================== MsrpSession ========================*/

MsrpSession::MsrpSession(SipStack* pStack, MsrpCallback* pCallback)
: InviteSession(pStack), m_pCallback(pCallback)
{
	tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_MEDIA(
			TSIP_MSESSION_SET_MSRP_CB(twrap_msrp_cb),
			TSIP_MSESSION_SET_NULL()
		),
		TSIP_SSESSION_SET_NULL());
}

MsrpSession::MsrpSession(SipStack* pStack, tsip_ssession_handle_t* pHandle)
: InviteSession(pStack, pHandle), m_pCallback(tsk_null)
{
	tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_MEDIA(
			TSIP_MSESSION_SET_MSRP_CB(twrap_msrp_cb),
			TSIP_MSESSION_SET_NULL()
		),
		TSIP_SSESSION_SET_NULL());
}

MsrpSession::~MsrpSession()
{
}

bool MsrpSession::setCallback(MsrpCallback* pCallback)
{
	m_pCallback = pCallback;
	return true;
}

bool MsrpSession::callMsrp(const SipUri* remoteUri, ActionConfig* config/*=tsk_null*/)
{
	if(!remoteUri){
		TSK_DEBUG_ERROR("Invalid parameter");
		return false;
	}
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;

	tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_TO_OBJ(remoteUri->getWrappedUri()),
		TSIP_SSESSION_SET_NULL());

	return (tsip_api_invite_send_invite(m_pHandle, tmedia_msrp,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}

bool MsrpSession::callMsrp(const char* remoteUriString, ActionConfig* config/*=tsk_null*/)
{
	SipUri sipUri(remoteUriString);
	if(sipUri.isValid()){
		return callMsrp(&sipUri, config);
	}
	TSK_DEBUG_ERROR("Failed to parse sip uri=%s", remoteUriString);
	return false;
}

bool MsrpSession::sendMessage(const void* payload, unsigned len, ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;

	return (tsip_api_invite_send_large_message(m_pHandle,
		TSIP_ACTION_SET_PAYLOAD(payload, len),
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}

bool MsrpSession::sendFile(ActionConfig* config/*=tsk_null*/)
{
	return false;
}

/* ======================== MessagingSession ========================*/
MessagingSession::MessagingSession(SipStack* pStack)
: SipSession(pStack)
{
}

MessagingSession::MessagingSession(SipStack* pStack, tsip_ssession_handle_t* pHandle)
: SipSession(pStack, pHandle)
{
}

MessagingSession::~MessagingSession()
{
}

bool MessagingSession::send(const void* payload, unsigned len, ActionConfig* config/*=tsk_null*/)
{
	int ret;
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	if(payload && len){
		ret = tsip_api_message_send_message(m_pHandle,
			TSIP_ACTION_SET_PAYLOAD(payload, len),
			TSIP_ACTION_SET_CONFIG(action_cfg),
			TSIP_ACTION_SET_NULL());
	}
	else{
		ret = tsip_api_message_send_message(m_pHandle,
			TSIP_ACTION_SET_NULL());
	}
	return (ret == 0);
}

bool MessagingSession::accept(ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	return (tsip_api_common_accept(m_pHandle,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}

bool MessagingSession::reject(ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	return (tsip_api_common_reject(m_pHandle,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}



/* ======================== InfoSession ========================*/
InfoSession::InfoSession(SipStack* pStack)
: SipSession(pStack)
{
}

InfoSession::InfoSession(SipStack* pStack, tsip_ssession_handle_t* pHandle)
: SipSession(pStack, pHandle)
{
}

InfoSession::~InfoSession()
{
}

bool InfoSession::send(const void* payload, unsigned len, ActionConfig* config/*=tsk_null*/)
{
	int ret;
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	if(payload && len){
		ret = tsip_api_info_send_info(m_pHandle,
			TSIP_ACTION_SET_PAYLOAD(payload, len),
			TSIP_ACTION_SET_CONFIG(action_cfg),
			TSIP_ACTION_SET_NULL());
	}
	else{
		ret = tsip_api_info_send_info(m_pHandle,
			TSIP_ACTION_SET_NULL());
	}
	return (ret == 0);
}

bool InfoSession::accept(ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	return (tsip_api_common_accept(m_pHandle,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}

bool InfoSession::reject(ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	return (tsip_api_common_reject(m_pHandle,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}




/* ======================== OptionsSession ========================*/
OptionsSession::OptionsSession(SipStack* pStack)
: SipSession(pStack)
{
}

OptionsSession::OptionsSession(SipStack* pStack, tsip_ssession_handle_t* pHandle)
: SipSession(pStack, pHandle)
{
}

OptionsSession::~OptionsSession()
{
}

bool OptionsSession::send(ActionConfig* config/*=tsk_null*/)
{	
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	return (tsip_api_options_send_options(m_pHandle,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}

bool OptionsSession::accept(ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	return (tsip_api_common_accept(m_pHandle,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}

bool OptionsSession::reject(ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	return (tsip_api_common_reject(m_pHandle,
		TSIP_ACTION_SET_CONFIG(action_cfg),
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

bool PublicationSession::publish(const void* payload, unsigned len, ActionConfig* config/*=tsk_null*/)
{
	int ret;
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	if(payload && len){
		ret = tsip_api_publish_send_publish(m_pHandle,
			TSIP_ACTION_SET_PAYLOAD(payload, len),
			TSIP_ACTION_SET_NULL());
	}
	else{
		ret = tsip_api_publish_send_publish(m_pHandle,
			TSIP_ACTION_SET_CONFIG(action_cfg),
			TSIP_ACTION_SET_NULL());
	}
	return (ret == 0);
}

bool PublicationSession::unPublish(ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	return (tsip_api_publish_send_unpublish(m_pHandle,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}



/* ======================== RegistrationSession ========================*/
RegistrationSession::RegistrationSession(SipStack* pStack)
: SipSession(pStack)
{
}

RegistrationSession::RegistrationSession(SipStack* pStack, tsip_ssession_handle_t* pHandle)
: SipSession(pStack, pHandle)
{

}

RegistrationSession::~RegistrationSession()
{
}

bool RegistrationSession::register_(ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	return (tsip_api_register_send_register(m_pHandle,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}

bool RegistrationSession::unRegister(ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	return (tsip_api_register_send_unregister(m_pHandle,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}

bool RegistrationSession::accept(ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;

	return (tsip_api_common_accept(m_pHandle,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}

bool RegistrationSession::reject(ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;

	return (tsip_api_common_reject(m_pHandle,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}


/* ======================== SubscriptionSession ========================*/
SubscriptionSession::SubscriptionSession(SipStack* pStack)
: SipSession(pStack)
{
}

SubscriptionSession::~SubscriptionSession()
{
}

bool SubscriptionSession::subscribe()
{
	return (tsip_api_subscribe_send_subscribe(m_pHandle,
		TSIP_ACTION_SET_NULL()) == 0);
}

bool SubscriptionSession::unSubscribe()
{
	return (tsip_api_subscribe_send_unsubscribe(m_pHandle,
		TSIP_ACTION_SET_NULL()) == 0);
}
