
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
%feature("director") T140Callback;

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


/* ====== From "tinySIP\include\tsip.h"  ====== */
typedef enum tsip_stack_mode_e
{
	tsip_stack_mode_ua,
	tsip_stack_mode_p2p,
	tsip_stack_mode_mediaproxy,
	tsip_stack_mode_mcu
}
tsip_stack_mode_t;

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
#define tsip_event_code_dialog_request_prechecking	805

// 9xx ==> Informational
#define tsip_event_code_dialog_connecting			900
#define tsip_event_code_dialog_connected			901
#define tsip_event_code_dialog_terminating			902
#define tsip_event_code_dialog_terminated			903
#define tsip_event_code_stack_starting				950
#define tsip_event_code_stack_started				951
#define tsip_event_code_stack_stopping				952
#define tsip_event_code_stack_stopped				953
#define tsip_event_code_stack_failed_to_start		954
#define tsip_event_code_stack_failed_to_stop		955
#define tsip_event_code_stack_disconnected			956

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
    tsip_i_prechecking,

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
	tmedia_chroma_mjpeg, // Compressed. e.g. used by VirtualBox (Windows as host and Ubuntu as guest)
	tmedia_chroma_yuyv422, // YUYV422 (V4L2 preferred format)
}
tmedia_chroma_t;

/* ====== From "tinymedia/tmedia_common.h"  ====== */
typedef enum tmedia_mode_e
{
	tmedia_mode_none,
	tmedia_mode_optional,
	tmedia_mode_mandatory
}
tmedia_mode_t;

/* ====== From "tinymedia/tmedia_common.h"  ====== */
typedef enum tmedia_srtp_mode_e
{
	tmedia_srtp_mode_none,
	tmedia_srtp_mode_optional,
	tmedia_srtp_mode_mandatory
}
tmedia_srtp_mode_t;

/* ====== From "tinymedia/tmedia_common.h"  ====== */
typedef enum tmedia_srtp_type_e
{
	tmedia_srtp_type_none = 0x00,
	tmedia_srtp_type_sdes = 0x01,
	tmedia_srtp_type_dtls = 0x02,
	tmedia_srtp_type_sdes_dtls = (0x01 | 0x02)
}
tmedia_srtp_type_t;

/* ====== From "tinymedia/tmedia_common.h"  ====== */
typedef enum tmedia_t140_data_type_e
{
	tmedia_t140_data_type_utf8,
	tmedia_t140_data_type_zero_width_no_break_space = 0xefbbbf,
	tmedia_t140_data_type_backspace = 0x08,
	tmedia_t140_data_type_esc = 0x1b,
	tmedia_t140_data_type_cr = 0x0d,
	tmedia_t140_data_type_lf = 0x0a,
	tmedia_t140_data_type_cr_lf = 0x0d0a,
	tmedia_t140_data_type_interrupt2 = 0x61,
	tmedia_t140_data_type_bell = 0x07,
	tmedia_t140_data_type_sos = 0x98,
	tmedia_t140_data_type_string_term = 0x9c,
	tmedia_t140_data_type_graphic_start = 0x9b,
	tmedia_t140_data_type_graphic_end = 0x6d,
	tmedia_t140_data_type_loss_char_char = 0xfffd,
	tmedia_t140_data_type_loss_utf8 = 0xefbfbd,
}
tmedia_t140_data_type_t;

/* ====== From "tinymedia/tmedia_common.h"  ====== */
#typedef enum tmedia_rtcp_event_type_e
#{
#	tmedia_rtcp_event_type_fir, // Full Intra Refresh
#}
#tmedia_rtcp_event_type_t;

/* ====== From "tinymedia/tmedia_common.h"  ====== */
typedef enum tmedia_profile_e
{
	tmedia_profile_default,
	tmedia_profile_rtcweb
}
tmedia_profile_t;

/* ====== From "tinymedia/tmedia_common.h"  ====== */
// @deprecated
typedef enum tmedia_bandwidth_level_e
{
	tmedia_bl_low,
	tmedia_bl_medium,
	tmedia_bl_hight,
	tmedia_bl_unrestricted
}
tmedia_bandwidth_level_t;

/* ====== From "tinymedia/tmedia_common.h"  ====== */
typedef enum tmedia_pref_video_size_s
{ /* must be sorted like this */
	tmedia_pref_video_size_sqcif, // 128 x 98
	tmedia_pref_video_size_qcif, // 176 x 144
	tmedia_pref_video_size_qvga, // 320 x 240
	tmedia_pref_video_size_cif, // 352 x 288
	tmedia_pref_video_size_hvga, // 480 x 320
	tmedia_pref_video_size_vga, // 640 x 480
	tmedia_pref_video_size_4cif, // 704 x 576
	tmedia_pref_video_size_wvga, // 800 x 480
	tmedia_pref_video_size_svga, // 800 x 600
	tmedia_pref_video_size_480p, // 852 x 480
	tmedia_pref_video_size_xga, // 1024 x 768
	tmedia_pref_video_size_720p, // 1280 x 720
	tmedia_pref_video_size_16cif, // 1408 x 1152
	tmedia_pref_video_size_1080p, // 1920 x 1080
	tmedia_pref_video_size_2160p, // 3840 x 2160
}
tmedia_pref_video_size_t;


/* ====== From "tinymedia/tmedia_codec.h"  ====== */
typedef enum tmedia_codec_id_e
{
	tmedia_codec_id_none = 0x00000000,
	
	tmedia_codec_id_amr_nb_oa = 0x00000001<<0,
	tmedia_codec_id_amr_nb_be = 0x00000001<<1,
	tmedia_codec_id_amr_wb_oa = 0x00000001<<2,
	tmedia_codec_id_amr_wb_be = 0x00000001<<3,
	tmedia_codec_id_gsm = 0x00000001<<4,
	tmedia_codec_id_pcma = 0x00000001<<5,
	tmedia_codec_id_pcmu = 0x00000001<<6,
	tmedia_codec_id_ilbc = 0x00000001<<7,
	tmedia_codec_id_speex_nb = 0x00000001<<8,
	tmedia_codec_id_speex_wb = 0x00000001<<9,
	tmedia_codec_id_speex_uwb = 0x00000001<<10,
	tmedia_codec_id_bv16 = 0x00000001<<11,
	tmedia_codec_id_bv32 = 0x00000001<<12,
	tmedia_codec_id_opus = 0x00000001<<13,
	tmedia_codec_id_g729ab = 0x00000001<<14,
	tmedia_codec_id_g722 = 0x00000001<<15,
	
	/* room for new Audio codecs */
	
	tmedia_codec_id_h261 = 0x00010000<<0,
	tmedia_codec_id_h263 = 0x00010000<<1,
	tmedia_codec_id_h263p = 0x00010000<<2,
	tmedia_codec_id_h263pp = 0x00010000<<3,
	tmedia_codec_id_h264_bp = 0x00010000<<4,
	tmedia_codec_id_h264_mp = 0x00010000<<5,
	tmedia_codec_id_h264_hp = 0x00010000<<6,
	tmedia_codec_id_h264_bp10 = tmedia_codec_id_h264_bp, // @deprecated
	tmedia_codec_id_h264_bp20 = tmedia_codec_id_h264_bp, // @deprecated
	tmedia_codec_id_h264_bp30 = tmedia_codec_id_h264_bp, // @deprecated
	tmedia_codec_id_h264_svc = 0x00010000<<7,
	tmedia_codec_id_theora = 0x00010000<<8,
	tmedia_codec_id_mp4ves_es = 0x00010000<<9,
	tmedia_codec_id_vp8 = 0x00010000<<10,

	/* room for new Video codecs */

	tmedia_codec_id_t140 = 0x00010000<<14,
	tmedia_codec_id_red = 0x00010000<<15,
}
tmedia_codec_id_t;

/* ====== From "tinydav/tdav.h"  ====== */
// @deprecated: to be replaced by "tmedia_codec_id_t" in Doubango 3.0
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
	tdav_codec_id_opus = 0x00000001<<13,
	tdav_codec_id_g729ab = 0x00000001<<14,
	tdav_codec_id_g722 = 0x00000001<<15,
	
	/* room for new Audio codecs */
	
	tdav_codec_id_h261 = 0x00010000<<0,
	tdav_codec_id_h263 = 0x00010000<<1,
	tdav_codec_id_h263p = 0x00010000<<2,
	tdav_codec_id_h263pp = 0x00010000<<3,
	tdav_codec_id_h264_bp = 0x00010000<<4,
	tdav_codec_id_h264_mp = 0x00010000<<5,
	tdav_codec_id_h264_hp = 0x00010000<<6,
	tdav_codec_id_h264_bp10 = tdav_codec_id_h264_bp, // @deprecated
	tdav_codec_id_h264_bp20 = tdav_codec_id_h264_bp, // @deprecated
	tdav_codec_id_h264_bp30 = tdav_codec_id_h264_bp, // @deprecated
	tdav_codec_id_h264_svc = 0x00010000<<7,
	tdav_codec_id_theora = 0x00010000<<8,
	tdav_codec_id_mp4ves_es = 0x00010000<<9,
	tdav_codec_id_vp8 = 0x00010000<<10,

	/* room for new Video codecs */

	tdav_codec_id_t140 = 0x00010000<<14,
	tdav_codec_id_red = 0x00010000<<15,
}
tdav_codec_id_t;
