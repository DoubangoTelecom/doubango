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

#include "tinyWRAP_config.h"

#include "tinysip.h"
#include "tinymedia/tmedia_qos.h"
#include "ActionConfig.h"

class SipUri;
class SipStack;
class MsrpCallback;
class MediaSessionMgr;

/* ======================== T140Callback ========================*/
class TINYWRAP_API T140CallbackData{
	public:
#if !defined(SWIG)
	T140CallbackData(enum tmedia_t140_data_type_e data_type, const void* data_ptr, unsigned data_size){
		m_eType = data_type;
		m_pPtr = data_ptr;
		m_nSize = data_size;
	}
#endif
	virtual ~T140CallbackData(){}

	inline enum tmedia_t140_data_type_e getType()const{ return m_eType; }
	inline unsigned getSize()const{ return m_nSize; }
	inline unsigned getData(void* pOutput, unsigned nMaxsize)const{
		unsigned nRetsize = 0;
		if(pOutput && nMaxsize && m_pPtr){
			nRetsize = (m_nSize > nMaxsize) ? nMaxsize : m_nSize;
			memcpy(pOutput, m_pPtr, nRetsize);
		}
		return nRetsize;
	}

	private:
		enum tmedia_t140_data_type_e m_eType;
		const void* m_pPtr;
		unsigned m_nSize;
};

class TINYWRAP_API T140Callback
{
public:
	T140Callback() {}
	virtual ~T140Callback(){}
	virtual int ondata(const T140CallbackData* pData){ return 0; }
};

#if !defined(SWIG)
class RtcpCallbackData{
	public:
	RtcpCallbackData(enum tmedia_rtcp_event_type_e event_type, uint32_t ssrc_media){
		m_eType = event_type;
		m_nSSRC = ssrc_media;
	}
	virtual ~RtcpCallbackData(){}
	inline enum tmedia_rtcp_event_type_e getType()const{ return m_eType; }
	inline uint32_t getSSRC()const{ return m_nSSRC; }
	private:
		enum tmedia_rtcp_event_type_e m_eType;
		uint32_t m_nSSRC;
};

class TINYWRAP_API RtcpCallback
{
public:
	RtcpCallback() {}
	virtual ~RtcpCallback(){}
	virtual int onevent(const RtcpCallbackData* e){ return 0; }
};
#endif /* #if !defined(SWIG) */



/* ======================== SipSession ========================*/
class TINYWRAP_API SipSession
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
#if !defined(SWIG)
	bool setAuth(const char* authHa1, const char* authIMPI);
#endif
	unsigned getId()const;

#if !defined(SWIG)
	bool setWebSocketSrc(const char* host, int32_t port, const char* proto);
	const SipStack* getStack() const;
	const tsip_ssession_handle_t* getWrappedSession() { return m_pHandle; }
#endif
	
private:
	void init(SipStack* stack, tsip_ssession_handle_t* pHandle=tsk_null);

protected:
	tsip_ssession_handle_t* m_pHandle;
	const SipStack* m_pStack;
};

/* ======================== InviteSession ========================*/
class TINYWRAP_API InviteSession : public SipSession
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
class TINYWRAP_API CallSession : public InviteSession
{
public: /* ctor() and dtor() */
	CallSession(SipStack* pStack);
#if !defined(SWIG)
	CallSession(SipStack* pStack, tsip_ssession_handle_t* pHandle);
#endif
	virtual ~CallSession();

public: /* Public functions */
	bool callAudio(const char* remoteUriString, ActionConfig* config=tsk_null); /* @deprecated */
	bool callAudio(const SipUri* remoteUri, ActionConfig* config=tsk_null); /* @deprecated */
	bool callAudioVideo(const char* remoteUriString, ActionConfig* config=tsk_null); /* @deprecated */
	bool callAudioVideo(const SipUri* remoteUri, ActionConfig* config=tsk_null); /* @deprecated */
	bool callVideo(const char* remoteUriString, ActionConfig* config=tsk_null); /* @deprecated */
	bool callVideo(const SipUri* remoteUri, ActionConfig* config=tsk_null); /* @deprecated */

	bool call(const char* remoteUriString, twrap_media_type_t media, ActionConfig* config=tsk_null);
	bool call(const SipUri* remoteUri, twrap_media_type_t media, ActionConfig* config=tsk_null);
#if !defined(SWIG)
	bool setSupportedCodecs(int32_t codecs);
	int32_t getNegotiatedCodecs();
	bool setMediaSSRC(twrap_media_type_t media, uint32_t ssrc);
#endif
	bool setSessionTimer(unsigned timeout, const char* refresher);
	bool set100rel(bool enabled);
	bool setRtcp(bool enabled);
	bool setRtcpMux(bool enabled);
	bool setSRtpMode(enum tmedia_srtp_mode_e mode);
	bool setAvpfMode(enum tmedia_mode_e mode);
	bool setICE(bool enabled);
	bool setICEStun(bool enabled);
	bool setICETurn(bool enabled);
	bool setSTUNServer(const char* hostname, uint16_t port);
	bool setSTUNCred(const char* username, const char* password);
	bool setVideoFps(int32_t fps);
	bool setVideoBandwidthUploadMax(int32_t max);
	bool setVideoBandwidthDownloadMax(int32_t max);
	bool setVideoPrefSize(tmedia_pref_video_size_t pref_video_size);
	bool setQoS(tmedia_qos_stype_t type, tmedia_qos_strength_t strength);
	bool hold(ActionConfig* config=tsk_null);
	bool resume(ActionConfig* config=tsk_null);
	bool transfer(const char* referToUriString, ActionConfig* config=tsk_null);
	bool acceptTransfer(ActionConfig* config=tsk_null);
	bool rejectTransfer(ActionConfig* config=tsk_null);
	bool sendDTMF(int number);
	unsigned getSessionTransferId();
	bool sendT140Data(enum tmedia_t140_data_type_e data_type, const void* data_ptr = NULL, unsigned data_size = 0);
	bool setT140Callback(const T140Callback* pT140Callback);
#if !defined(SWIG)
	bool sendRtcpEvent(enum tmedia_rtcp_event_type_e event_type, twrap_media_type_t media_type, uint32_t ssrc_media = 0);
	bool setRtcpCallback(const RtcpCallback* pRtcpCallback, twrap_media_type_t media_type);
	const T140Callback* getT140Callback() const;
	static int t140OnDataCallback(const void* context, enum tmedia_t140_data_type_e data_type, const void* data_ptr, unsigned data_size);
	const RtcpCallback* getRtcpCallback() const;
	static int rtcpOnCallback(const void* context, enum tmedia_rtcp_event_type_e event_type, uint32_t ssrc_media);
#endif /* #if !defined(SWIG) */

private:
	const T140Callback* m_pT140Callback;
	const RtcpCallback* m_pRtcpCallback;
};

/* ======================== MsrpSession ========================*/
class TINYWRAP_API MsrpSession : public InviteSession
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
class TINYWRAP_API MessagingSession : public SipSession
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
class TINYWRAP_API InfoSession : public SipSession
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
class TINYWRAP_API OptionsSession : public SipSession
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
class TINYWRAP_API PublicationSession : public SipSession
{
public: /* ctor() and dtor() */
	PublicationSession(SipStack* pStack);
#if !defined(SWIG)
	PublicationSession(SipStack* pStack, tsip_ssession_handle_t* pHandle);
#endif
	virtual ~PublicationSession();

public: /* Public functions */
	bool publish(const void* payload, unsigned len, ActionConfig* config=tsk_null);
	bool unPublish(ActionConfig* config=tsk_null);
};


/* ======================== RegistrationSession ========================*/
class TINYWRAP_API RegistrationSession : public SipSession
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
class TINYWRAP_API SubscriptionSession : public SipSession
{
public: /* ctor() and dtor() */
	SubscriptionSession(SipStack* pStack);
#if !defined(SWIG)
	SubscriptionSession(SipStack* pStack, tsip_ssession_handle_t* pHandle);
#endif
	virtual ~SubscriptionSession();

public: /* Public functions */
	bool subscribe();
	bool unSubscribe();
};

#endif /* TINYWRAP_SIPSESSION_H */
