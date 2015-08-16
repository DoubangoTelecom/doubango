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
		if(tsip_ssession_have_ownership(pHandle)){
			tsk_object_ref(pHandle);
		}
		else if(tsip_ssession_take_ownership(pHandle) != 0){ /* should never happen */
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

bool SipSession::setAuth(const char* authHa1, const char* authIMPI)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_AUTH_HA1(authHa1),
		TSIP_SSESSION_SET_AUTH_IMPI(authIMPI),
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

bool SipSession::setWebSocketSrc(const char* host, int32_t port, const char* proto)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_WEBSOCKET_SRC(host, port, proto),
		TSIP_SSESSION_SET_NULL()) == 0);
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

bool InviteSession::hangup(ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;

	return (tsip_api_invite_send_bye(m_pHandle,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}

bool InviteSession::reject(ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;

	return (tsip_api_common_reject(m_pHandle,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}

bool InviteSession::accept(ActionConfig* config/*=tsk_null*/)
{
	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;

	return (tsip_api_common_accept(m_pHandle,
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}

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
, m_pT140Callback(tsk_null)
, m_pRtcpCallback(tsk_null)
{
}

CallSession::~CallSession()
{
}

/* @deprecated */
bool CallSession::callAudio(const SipUri* remoteUri, ActionConfig* config/*=tsk_null*/)
{
	return call(remoteUri, twrap_media_audio, config);
}

/* @deprecated */
bool CallSession::callAudio(const char* remoteUriString, ActionConfig* config/*=tsk_null*/)
{
	return call(remoteUriString, twrap_media_audio, config);
}

/* @deprecated */
bool CallSession::callAudioVideo(const SipUri* remoteUri, ActionConfig* config/*=tsk_null*/)
{
	return call(remoteUri, twrap_media_audio_video, config);
}

/* @deprecated */
bool CallSession::callAudioVideo(const char* remoteUriString, ActionConfig* config/*=tsk_null*/)
{
	return call(remoteUriString, twrap_media_audio_video, config);
}

/* @deprecated */
bool CallSession::callVideo(const SipUri* remoteUri, ActionConfig* config/*=tsk_null*/)
{
	return call(remoteUri, twrap_media_video, config);
}

/* @deprecated */
bool CallSession::callVideo(const char* remoteUriString, ActionConfig* config/*=tsk_null*/)
{
	return call(remoteUriString, twrap_media_video, config);
}

bool CallSession::call(const char* remoteUriString, twrap_media_type_t media, ActionConfig* config/*=tsk_null*/)
{
	
	SipUri sipUri(remoteUriString);
	if(sipUri.isValid()){
		return call(&sipUri, media, config);
	}
	TSK_DEBUG_ERROR("Failed to parse sip uri=%s", remoteUriString);
	return false;
}

bool CallSession::call(const SipUri* remoteUri, twrap_media_type_t media, ActionConfig* config/*=tsk_null*/)
{
	if(!remoteUri){
		TSK_DEBUG_ERROR("Invalid parameter");
		return false;
	}
	tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_TO_OBJ(remoteUri->getWrappedUri()),
		TSIP_SSESSION_SET_NULL());

	const tsip_action_handle_t* action_cfg = config ? config->getHandle() : tsk_null;
	return (tsip_api_invite_send_invite(m_pHandle, twrap_get_native_media_type(media),
		TSIP_ACTION_SET_CONFIG(action_cfg),
		TSIP_ACTION_SET_NULL()) == 0);
}

bool CallSession::setSupportedCodecs(int32_t codecs)
{
	return (tsip_ssession_set(m_pHandle,
			TSIP_SSESSION_SET_MEDIA(
				TSIP_MSESSION_SET_CODECS(codecs),
				TSIP_MSESSION_SET_NULL()
			),
			TSIP_SSESSION_SET_NULL()) == 0);
}


int32_t CallSession::getNegotiatedCodecs()
{
	return (int32_t) tsip_ssession_get_codecs_neg(m_pHandle);
}

bool CallSession::setMediaSSRC(twrap_media_type_t media, uint32_t ssrc)
{
	return (tsip_ssession_set(m_pHandle,
			TSIP_SSESSION_SET_MEDIA(
				TSIP_MSESSION_SET_RTP_SSRC(twrap_get_native_media_type(media), ssrc),
				TSIP_MSESSION_SET_NULL()
			),
			TSIP_SSESSION_SET_NULL()) == 0);
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

bool CallSession::setSRtpMode(enum tmedia_srtp_mode_e mode)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_MEDIA(
		TSIP_MSESSION_SET_SRTP_MODE(mode),
			TSIP_MSESSION_SET_NULL()
		),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool CallSession::setAvpfMode(enum tmedia_mode_e mode)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_MEDIA(
		TSIP_MSESSION_SET_AVPF_MODE(mode),
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

bool CallSession::setICEStun(bool enabled)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_MEDIA(
		TSIP_MSESSION_SET_ICE_STUN(enabled ? tsk_true : tsk_false),
			TSIP_MSESSION_SET_NULL()
		),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool CallSession::setICETurn(bool enabled)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_MEDIA(
		TSIP_MSESSION_SET_ICE_TURN(enabled ? tsk_true : tsk_false),
			TSIP_MSESSION_SET_NULL()
		),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool CallSession::setSTUNServer(const char* hostname, uint16_t port)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_MEDIA(
		TSIP_MSESSION_SET_STUN_SERVER(hostname, port),
			TSIP_MSESSION_SET_NULL()
		),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool CallSession::setSTUNCred(const char* username, const char* password)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_MEDIA(
		TSIP_MSESSION_SET_STUN_CRED(username, password),
			TSIP_MSESSION_SET_NULL()
		),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool CallSession::setVideoFps(int32_t fps)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_MEDIA(
		TSIP_MSESSION_SET_VIDEO_FPS(fps),
			TSIP_MSESSION_SET_NULL()
		),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool CallSession::setVideoBandwidthUploadMax(int32_t max)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_MEDIA(
		TSIP_MSESSION_SET_VIDEO_BW_UP(max),
			TSIP_MSESSION_SET_NULL()
		),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool CallSession::setVideoBandwidthDownloadMax(int32_t max)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_MEDIA(
		TSIP_MSESSION_SET_VIDEO_BW_DOWN(max),
			TSIP_MSESSION_SET_NULL()
		),
		TSIP_SSESSION_SET_NULL()) == 0);
}

bool CallSession::setVideoPrefSize(tmedia_pref_video_size_t pref_video_size)
{
	return (tsip_ssession_set(m_pHandle,
		TSIP_SSESSION_SET_MEDIA(
		TSIP_MSESSION_SET_VIDEO_PREFSIZE(pref_video_size),
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

bool CallSession::sendT140Data(enum tmedia_t140_data_type_e data_type, const void* data_ptr /*= NULL*/, unsigned data_size /*= 0*/)
{
	const tmedia_session_mgr_t* pWrappedMgr;
	const MediaSessionMgr* pMgr;
	if((pMgr = getMediaMgr()) && (pWrappedMgr = pMgr->getWrappedMgr())){
		return (tmedia_session_mgr_send_t140_data((tmedia_session_mgr_t*)pWrappedMgr, data_type, data_ptr, data_size) == 0);		
	}
	return false;
}

bool CallSession::setT140Callback(const T140Callback* pT140Callback)
{
	const tmedia_session_mgr_t* pWrappedMgr;
	const MediaSessionMgr* pMgr;
	if((pMgr = getMediaMgr()) && (pWrappedMgr = pMgr->getWrappedMgr())){
		if((m_pT140Callback = pT140Callback)){
			return (tmedia_session_mgr_set_t140_ondata_cbfn((tmedia_session_mgr_t*)pWrappedMgr, this, &CallSession::t140OnDataCallback) == 0);
		}
		else{
			return (tmedia_session_mgr_set_t140_ondata_cbfn((tmedia_session_mgr_t*)pWrappedMgr, this, tsk_null) == 0);
		}
	}
	return false;
}

bool CallSession::sendRtcpEvent(enum tmedia_rtcp_event_type_e event_type, twrap_media_type_t media_type, uint32_t ssrc_media /*= 0*/)
{
	const tmedia_session_mgr_t* pWrappedMgr;
	const MediaSessionMgr* pMgr;
	if((pMgr = getMediaMgr()) && (pWrappedMgr = pMgr->getWrappedMgr())){
		return (tmedia_session_mgr_send_rtcp_event((tmedia_session_mgr_t*)pWrappedMgr, twrap_get_native_media_type(media_type), event_type, ssrc_media) == 0);		
	}
	TSK_DEBUG_ERROR("No media manager");
	return false;
}

bool CallSession::setRtcpCallback(const RtcpCallback* pRtcpCallback, twrap_media_type_t media_type)
{
	const tmedia_session_mgr_t* pWrappedMgr;
	const MediaSessionMgr* pMgr;
	if((pMgr = getMediaMgr()) && (pWrappedMgr = pMgr->getWrappedMgr())){
		if((m_pRtcpCallback = pRtcpCallback)){
			return (tmedia_session_mgr_set_onrtcp_cbfn((tmedia_session_mgr_t*)pWrappedMgr, twrap_get_native_media_type(media_type), this, &CallSession::rtcpOnCallback) == 0);
		}
		else{
			return (tmedia_session_mgr_set_onrtcp_cbfn((tmedia_session_mgr_t*)pWrappedMgr, twrap_get_native_media_type(media_type), this, tsk_null) == 0);
		}
	}
	return false;
}

const T140Callback* CallSession::getT140Callback() const
{
	return m_pT140Callback;
}

int CallSession::t140OnDataCallback(const void* context, enum tmedia_t140_data_type_e data_type, const void* data_ptr, unsigned data_size)
{
	const CallSession* session = dyn_cast<const CallSession*>((const CallSession*)context);
	if(session && session->getT140Callback()){
		T140CallbackData* dataObj = new T140CallbackData(data_type, data_ptr, data_size);
		if(dataObj){
			int ret = const_cast<T140Callback*>(session->getT140Callback())->ondata(dataObj);
			delete dataObj;
			return ret;
		}
	}
	return 0;
}

const RtcpCallback* CallSession::getRtcpCallback() const
{
	return m_pRtcpCallback;
}

int CallSession::rtcpOnCallback(const void* context, enum tmedia_rtcp_event_type_e event_type, uint32_t ssrc_media)
{
	const CallSession* session = dyn_cast<const CallSession*>((const CallSession*)context);
	if(session && session->getRtcpCallback()){
		RtcpCallbackData* dataObj = new RtcpCallbackData(event_type, ssrc_media);
		if(dataObj){
			int ret = const_cast<RtcpCallback*>(session->getRtcpCallback())->onevent(dataObj);
			delete dataObj;
			return ret;
		}
	}
	TSK_DEBUG_INFO("Not Sending RTCP packet (no callback)");
	return 0;
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

PublicationSession::PublicationSession(SipStack* pStack, tsip_ssession_handle_t* pHandle)
: SipSession(pStack, pHandle)
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

SubscriptionSession::SubscriptionSession(SipStack* pStack, tsip_ssession_handle_t* pHandle)
: SipSession(pStack, pHandle)
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
