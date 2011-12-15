
%{
#include "ActionConfig.h"
#include "MediaSessionMgr.h"
#include "MediaContent.h"
#include "SipUri.h"
#include "SipMessage.h"
#include "SipEvent.h"
#include "SipSession.h"

#include "ProxyPluginMgr.h"
#include "ProxyConsumer.h"
#include "ProxyProducer.h"

#include "SipCallback.h"
#include "SafeObject.h"
#include "SipStack.h"
%}

/* Callbacks */
%feature("director") SipCallback;
%feature("director") ProxyPluginMgrCallback;
%feature("director") ProxyAudioConsumerCallback;
%feature("director") ProxyVideoConsumerCallback;
%feature("director") ProxyAudioProducerCallback;
%feature("director") ProxyVideoProducerCallback;

%nodefaultctor;
%include "ActionConfig.h"
%include "MediaSessionMgr.h"
%include "MediaContent.h"
%include "SipUri.h"
%include "SipMessage.h"
%include "SipEvent.h"
%include "SipSession.h"

%include "ProxyPluginMgr.h"
%include "ProxyConsumer.h"
%include "ProxyProducer.h"

%include "SipCallback.h"
%include "SafeObject.h"
%include "SipStack.h"
%clearnodefaultctor;



/* ====== From "tinySIP\include\tinysip\tsip_messag_common.h"  ====== */
typedef enum tsip_request_type_e
{
	tsip_NONE = 0,

	tsip_ACK,
	tsip_BYE,
	tsip_CANCEL,
	tsip_INVITE,
	tsip_OPTIONS,
	tsip_REGISTER,
	tsip_SUBSCRIBE,
	tsip_NOTIFY,
	tsip_REFER,
	tsip_INFO,
	tsip_UPDATE,
	tsip_MESSAGE,
	tsip_PUBLISH,
	tsip_PRACK
}
tsip_request_type_t;


/* ====== From "tinySIP\include\tinysip\tsip_event.h"  ====== */
typedef enum tsip_event_type_e
{
	tsip_event_invite,
	tsip_event_message,
	tsip_event_info,
	tsip_event_options,
	tsip_event_publish,
	tsip_event_register,
	tsip_event_subscribe,
	
	tsip_event_dialog
}
tsip_event_type_t;

// 7xx ==> errors
#define tsip_event_code_dialog_transport_error		702
#define tsip_event_code_dialog_global_error			703
#define tsip_event_code_dialog_message_error		704

// 8xx ==> success
#define tsip_event_code_dialog_request_incoming		800
#define tsip_event_code_dialog_request_outgoing		802
#define tsip_event_code_dialog_request_cancelled	803
#define tsip_event_code_dialog_request_sent			804

// 9xx ==> Informational
#define tsip_event_code_dialog_connecting			900
#define tsip_event_code_dialog_connected			901
#define tsip_event_code_dialog_terminating			902
#define tsip_event_code_dialog_terminated			903
#define tsip_event_code_stack_started				950
#define tsip_event_code_stack_stopped				951
#define tsip_event_code_stack_failed_to_start		952
#define tsip_event_code_stack_failed_to_stop		953

/* ====== From "tinySIP\include\tinysip\tsip_api_register.h"  ====== */
typedef enum tsip_register_event_type_e
{
	tsip_i_newreg,

	tsip_i_register, // refresh
	tsip_ao_register,

	tsip_i_unregister,
	tsip_ao_unregister,
}
tsip_register_event_type_t;

/* ====== From "tinySIP\include\tinysip\tsip_api_subscribe.h"  ====== */
typedef enum tsip_subscribe_event_type_e
{
	tsip_i_subscribe,
	tsip_ao_subscribe,
	
	tsip_i_unsubscribe,
	tsip_ao_unsubscribe,

	tsip_i_notify,
	tsip_ao_notify
}
tsip_subscribe_event_type_t;

/* ====== From "tinySIP\include\tinysip\tsip_api_publish.h"  ====== */
typedef enum tsip_publish_event_type_e
{
	tsip_i_publish,
	tsip_ao_publish,
	
	tsip_i_unpublish,
	tsip_ao_unpublish
}
tsip_publish_event_type_t;

/* ====== From "tinySIP\include\tinysip\tsip_api_message.h"  ====== */
typedef enum tsip_message_event_type_e
{
	tsip_i_message,
	tsip_ao_message,
}
tsip_message_event_type_t;

/* ====== From "tinySIP\include\tinysip\tsip_api_info.h"  ====== */
typedef enum tsip_info_event_type_e
{
	tsip_i_info,
	tsip_ao_info,
}
tsip_info_event_type_t;

/* ====== From "tinySIP\include\tinysip\tsip_api_options.h"  ====== */
typedef enum tsip_options_event_type_e
{
	tsip_i_options,
	tsip_ao_options,
}
tsip_options_event_type_t;


/* ====== From "tinySIP\include\tinysip\tsip_api_invite.h"  ====== */
typedef enum tsip_invite_event_type_e
{
	// ============================
	//	Sip Events
	//
	tsip_i_newcall,
	
	//! in-dialog requests/reponses
	tsip_i_request,
	tsip_ao_request,
	
	/* 3GPP TS 24.629: Explicit Call Transfer (ECT) */
	tsip_o_ect_trying,
	tsip_o_ect_accepted,
	tsip_o_ect_completed,
	tsip_o_ect_failed,
	tsip_o_ect_notify,
	tsip_i_ect_requested,
	tsip_i_ect_newcall,
	tsip_i_ect_completed,
	tsip_i_ect_failed,
	tsip_i_ect_notify,
	
	// ============================
	//	Media Events
	//

	tsip_m_early_media,
	tsip_m_updating, // Trying to update from Audio -> Video for example
	tsip_m_updated, // succeed to update
	
	/* 3GPP TS 24.610: Communication Hold */
	tsip_m_local_hold_ok,
	tsip_m_local_hold_nok,
	tsip_m_local_resume_ok,
	tsip_m_local_resume_nok,
	tsip_m_remote_hold,
	tsip_m_remote_resume,
}
tsip_invite_event_type_t;



/* ====== From "tinymedia/tmedia_common.h"  ====== */
// used by tinyWRAP
typedef enum tmedia_chroma_e
{
	tmedia_chroma_none=0,
	tmedia_chroma_rgb24,		// will be stored as bgr24 on x86 (little endians) machines; e.g. WindowsPhone7
	tmedia_chroma_bgr24,		// used by windows consumer (DirectShow) - 
	tmedia_chroma_rgb32,       // used by iOS4 consumer (iPhone and iPod touch)
	tmedia_chroma_rgb565le,	// (used by both android and wince consumers)
	tmedia_chroma_rgb565be,
	tmedia_chroma_nv12, // used by iOS4 producer (iPhone and iPod Touch 3GS and 4)
	tmedia_chroma_nv21, // Yuv420 SP (used by android producer)
	tmedia_chroma_yuv422p,
	tmedia_chroma_uyvy422, // used by iOS4 producer (iPhone and iPod Touch 3G)
	tmedia_chroma_yuv420p, // Default
}
tmedia_chroma_t;

/* ====== From "tinymedia/tmedia_qos.h"  ====== */
typedef enum tmedia_qos_stype_e
{
	tmedia_qos_stype_none,/* not part of the RFC */

	tmedia_qos_stype_segmented,
	tmedia_qos_stype_e2e,
}
tmedia_qos_stype_t;

/* ====== From "tinymedia/tmedia_qos.h"  ====== */
typedef enum tmedia_qos_strength_e
{
	/* do no change the order (none -> optional -> manadatory) */
	tmedia_qos_strength_none,	
	tmedia_qos_strength_failure,
	tmedia_qos_strength_unknown,
	tmedia_qos_strength_optional,
	tmedia_qos_strength_mandatory
}
tmedia_qos_strength_t;

/* ====== From "tinymedia/tmedia_common.h"  ====== */
typedef enum tmedia_bandwidth_level_e
{
	tmedia_bl_low,
	tmedia_bl_medium,
	tmedia_bl_hight,
	tmedia_bl_unrestricted
}
tmedia_bandwidth_level_t;


/* ====== From "tinydav/tdav.h"  ====== */
typedef enum tdav_codec_id_e
{
	tdav_codec_id_none = 0x00000000,
	
	tdav_codec_id_amr_nb_oa = 0x00000001<<0,
	tdav_codec_id_amr_nb_be = 0x00000001<<1,
	tdav_codec_id_amr_wb_oa = 0x00000001<<2,
	tdav_codec_id_amr_wb_be = 0x00000001<<3,
	tdav_codec_id_gsm = 0x00000001<<4,
	tdav_codec_id_pcma = 0x00000001<<5,
	tdav_codec_id_pcmu = 0x00000001<<6,
	tdav_codec_id_ilbc = 0x00000001<<7,
	tdav_codec_id_speex_nb = 0x00000001<<8,
	tdav_codec_id_speex_wb = 0x00000001<<9,
	tdav_codec_id_speex_uwb = 0x00000001<<10,
	tdav_codec_id_bv16 = 0x00000001<<11,
	tdav_codec_id_bv32 = 0x00000001<<12,
	tdav_codec_id_evrc = 0x00000001<<13,
	tdav_codec_id_g729ab = 0x00000001<<14,
	tdav_codec_id_g722 = 0x00000001<<15,
	
	/* room for new Audio codecs */
	
	tdav_codec_id_h261 = 0x00010000<<0,
	tdav_codec_id_h263 = 0x00010000<<1,
	tdav_codec_id_h263p = 0x00010000<<2,
	tdav_codec_id_h263pp = 0x00010000<<3,
	tdav_codec_id_h264_bp10 = 0x00010000<<4,
	tdav_codec_id_h264_bp20 = 0x00010000<<5,
	tdav_codec_id_h264_bp30 = 0x00010000<<6,
	tdav_codec_id_h264_svc = 0x00010000<<7,
	tdav_codec_id_theora = 0x00010000<<8,
	tdav_codec_id_mp4ves_es = 0x00010000<<9,
	tdav_codec_id_vp8 = 0x00010000<<10,
}
tdav_codec_id_t;