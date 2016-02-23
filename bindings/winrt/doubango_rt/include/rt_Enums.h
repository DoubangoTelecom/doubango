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

#include "rt_config.h"

#include "tinysip.h"
#include "tinymsrp.h"
#include "tinymedia.h"

#include "Common.h"

namespace doubango_rt
{
namespace BackEnd
{
public enum class rtConstants
{
    rt_tsip_event_code_dialog_transport_error		= tsip_event_code_dialog_transport_error,
    rt_tsip_event_code_dialog_global_error			= tsip_event_code_dialog_global_error,
    rt_tsip_event_code_dialog_message_error			= tsip_event_code_dialog_message_error,

    // 8xx ==> success
    rt_tsip_event_code_dialog_request_incoming		= tsip_event_code_dialog_request_incoming,
    rt_tsip_event_code_dialog_request_outgoing		= tsip_event_code_dialog_request_outgoing,
    rt_tsip_event_code_dialog_request_cancelled		= tsip_event_code_dialog_request_cancelled,
    rt_tsip_event_code_dialog_request_sent			= tsip_event_code_dialog_request_sent,

    // 9xx ==> Informational
    rt_tsip_event_code_dialog_connecting			= tsip_event_code_dialog_connecting,
    rt_tsip_event_code_dialog_connected				= tsip_event_code_dialog_connected,
    rt_tsip_event_code_dialog_terminating			= tsip_event_code_dialog_terminating,
    rt_tsip_event_code_dialog_terminated			= tsip_event_code_dialog_terminated,
    rt_tsip_event_code_stack_starting				= tsip_event_code_stack_starting,
    rt_tsip_event_code_stack_started				= tsip_event_code_stack_started,
    rt_tsip_event_code_stack_stopping				= tsip_event_code_stack_stopping,
    rt_tsip_event_code_stack_stopped				= tsip_event_code_stack_stopped,
    rt_tsip_event_code_stack_failed_to_start		= tsip_event_code_stack_failed_to_start,
    rt_tsip_event_code_stack_failed_to_stop			= tsip_event_code_stack_failed_to_stop
};

public enum class rt_twrap_media_type_t
{
    twrap_media_none = twrap_media_none,

    twrap_media_audio = twrap_media_audio,
    twrap_media_video = twrap_media_video,
    twrap_media_msrp = twrap_media_msrp,
    twrap_media_t140 = twrap_media_t140,
    twrap_media_bfcp = twrap_media_bfcp,
    twrap_media_bfcp_audio = twrap_media_bfcp_audio,
    twrap_media_bfcp_video = twrap_media_bfcp_video,

    twrap_media_audiovideo = twrap_media_audiovideo, /* @deprecated */
    twrap_media_audio_video = twrap_media_audio_video,
};

public enum class rt_tmedia_bandwidth_level_t
{
    tmedia_bl_low = tmedia_bl_low,
    tmedia_bl_medium = tmedia_bl_medium,
    tmedia_bl_hight = tmedia_bl_hight,
    tmedia_bl_unrestricted = tmedia_bl_unrestricted
};

public enum class rt_tmedia_pref_video_size_t
{
    /* must be sorted like this */
    tmedia_pref_video_size_sqcif = tmedia_pref_video_size_sqcif, // 128 x 98
    tmedia_pref_video_size_qcif = tmedia_pref_video_size_qcif, // 176 x 144
    tmedia_pref_video_size_qvga = tmedia_pref_video_size_qvga, // 320 x 240
    tmedia_pref_video_size_cif = tmedia_pref_video_size_cif, // 352 x 288
    tmedia_pref_video_size_hvga = tmedia_pref_video_size_hvga, // 480 x 320
    tmedia_pref_video_size_vga = tmedia_pref_video_size_vga, // 640 x 480
    tmedia_pref_video_size_4cif = tmedia_pref_video_size_4cif, // 704 x 576
    tmedia_pref_video_size_wvga = tmedia_pref_video_size_wvga, // 800 x 480
    tmedia_pref_video_size_svga = tmedia_pref_video_size_svga, // 800 x 600
    tmedia_pref_video_size_480p = tmedia_pref_video_size_480p, // 852 x 480
    tmedia_pref_video_size_xga = tmedia_pref_video_size_xga, // 1024 x 768
    tmedia_pref_video_size_720p = tmedia_pref_video_size_720p, // 1280 x 720
    tmedia_pref_video_size_16cif = tmedia_pref_video_size_16cif, // 1408 x 1152
    tmedia_pref_video_size_1080p = tmedia_pref_video_size_1080p, // 1920 x 1080
};

public enum class rt_tmedia_profile_t
{
    tmedia_profile_default = tmedia_profile_default,
    tmedia_profile_rtcweb = tmedia_profile_rtcweb
};

public enum class rt_tmedia_qos_strength_t
{
    tmedia_qos_strength_none = tmedia_qos_strength_none,
    tmedia_qos_strength_failure = tmedia_qos_strength_failure,
    tmedia_qos_strength_unknown = tmedia_qos_strength_unknown,
    tmedia_qos_strength_optional = tmedia_qos_strength_optional,
    tmedia_qos_strength_mandatory = tmedia_qos_strength_mandatory
};

public enum class rt_tmedia_qos_stype_t
{
    tmedia_qos_stype_none = tmedia_qos_stype_none,

    tmedia_qos_stype_segmented = tmedia_qos_stype_segmented,
    tmedia_qos_stype_e2e = tmedia_qos_stype_e2e,
};

public enum class rt_tmedia_srtp_mode_t
{
    tmedia_srtp_mode_none = tmedia_srtp_mode_none,
    tmedia_srtp_mode_optional = tmedia_srtp_mode_optional,
    tmedia_srtp_mode_mandatory = tmedia_srtp_mode_mandatory
};

public enum class rt_tmedia_srtp_type_t
{
    tmedia_srtp_type_none = tmedia_srtp_type_none,
    tmedia_srtp_type_sdes = tmedia_srtp_type_sdes,
    tmedia_srtp_type_dtls = tmedia_srtp_type_dtls,
    tmedia_srtp_type_sdes_dtls = tmedia_srtp_type_sdes_dtls
};

public enum class rt_tmedia_t140_data_type_t
{
    tmedia_t140_data_type_utf8 = tmedia_t140_data_type_utf8,
    tmedia_t140_data_type_zero_width_no_break_space = tmedia_t140_data_type_zero_width_no_break_space,
    tmedia_t140_data_type_backspace = tmedia_t140_data_type_backspace,
    tmedia_t140_data_type_esc = tmedia_t140_data_type_esc,
    tmedia_t140_data_type_cr = tmedia_t140_data_type_cr,
    tmedia_t140_data_type_lf = tmedia_t140_data_type_lf,
    tmedia_t140_data_type_cr_lf = tmedia_t140_data_type_cr_lf,
    tmedia_t140_data_type_bell = tmedia_t140_data_type_bell,
    tmedia_t140_data_type_sos = tmedia_t140_data_type_sos,
    tmedia_t140_data_type_string_term = tmedia_t140_data_type_string_term,
    tmedia_t140_data_type_graphic_start = tmedia_t140_data_type_graphic_start,
    tmedia_t140_data_type_graphic_end = tmedia_t140_data_type_graphic_end,
    tmedia_t140_data_type_loss_char_char = tmedia_t140_data_type_loss_char_char,
    tmedia_t140_data_type_loss_utf8 = tmedia_t140_data_type_loss_utf8,
};

public enum class rt_tmedia_type_t
{
    tmedia_none = tmedia_none,
    tmedia_ghost = tmedia_ghost,

    tmedia_audio = tmedia_audio,
    tmedia_video = tmedia_video,
    tmedia_chat = tmedia_chat,
    tmedia_file = tmedia_file,
    tmedia_t38 = tmedia_t38,
    tmedia_t140 = tmedia_t140,

    tmedia_msrp = tmedia_msrp,
    tmedia_audiovideo = tmedia_audiovideo,

    tmedia_all = tmedia_all
};

public enum class rt_tmsrp_event_type_t
{
    tmsrp_event_type_none = tmsrp_event_type_none,
    tmsrp_event_type_connected = tmsrp_event_type_connected,
    tmsrp_event_type_disconnected = tmsrp_event_type_disconnected,
    tmsrp_event_type_message = tmsrp_event_type_message,
};

public enum class rt_tmsrp_request_type_t
{
    tmsrp_NONE = tmsrp_NONE,

    tmsrp_SEND = tmsrp_SEND,
    tmsrp_REPORT = tmsrp_REPORT,
    tmsrp_AUTH = tmsrp_AUTH
};

public enum class rt_tsip_info_event_type_t
{
    tsip_i_info = tsip_i_info,
    tsip_ao_info = tsip_ao_info,
};

public enum class rt_tsip_invite_event_type_t
{
    tsip_i_newcall = tsip_i_newcall,

    tsip_i_request = tsip_i_request,
    tsip_ao_request = tsip_ao_request,

    tsip_o_ect_trying = tsip_o_ect_trying,
    tsip_o_ect_accepted = tsip_o_ect_accepted,
    tsip_o_ect_completed = tsip_o_ect_completed,
    tsip_o_ect_failed = tsip_o_ect_failed,
    tsip_o_ect_notify = tsip_o_ect_notify,
    tsip_i_ect_requested = tsip_i_ect_requested,
    tsip_i_ect_newcall = tsip_i_ect_newcall,
    tsip_i_ect_completed = tsip_i_ect_completed,
    tsip_i_ect_failed = tsip_i_ect_failed,
    tsip_i_ect_notify = tsip_i_ect_notify,

    tsip_m_early_media = tsip_m_early_media,
    tsip_m_updating = tsip_m_updating,
    tsip_m_updated = tsip_m_updated,

    tsip_m_local_hold_ok = tsip_m_local_hold_ok,
    tsip_m_local_hold_nok = tsip_m_local_hold_nok,
    tsip_m_local_resume_ok = tsip_m_local_resume_ok,
    tsip_m_local_resume_nok = tsip_m_local_resume_nok,
    tsip_m_remote_hold = tsip_m_remote_hold,
    tsip_m_remote_resume = tsip_m_remote_resume,
};

public enum class rt_tsip_message_event_type_t
{
    tsip_i_message = tsip_i_message,
    tsip_ao_message = tsip_ao_message,
};

public enum class rt_tsip_options_event_type_t
{
    tsip_i_options = tsip_i_options,
    tsip_ao_options = tsip_ao_options,
};

public enum class rt_tsip_publish_event_type_t
{
    tsip_i_publish = tsip_i_publish,
    tsip_ao_publish = tsip_ao_publish,

    tsip_i_unpublish = tsip_i_unpublish,
    tsip_ao_unpublish = tsip_ao_unpublish
};

public enum class rt_tsip_register_event_type_t
{
    tsip_i_newreg = tsip_i_newreg,

    tsip_i_register = tsip_i_register,
    tsip_ao_register = tsip_ao_register,

    tsip_i_unregister = tsip_i_unregister,
    tsip_ao_unregister = tsip_ao_unregister,
};

public enum class rt_tsip_request_type_t
{
    tsip_NONE = tsip_NONE,

    tsip_ACK = tsip_ACK,
    tsip_BYE = tsip_BYE,
    tsip_CANCEL = tsip_CANCEL,
    tsip_INVITE = tsip_INVITE,
    tsip_OPTIONS = tsip_OPTIONS,
    tsip_REGISTER = tsip_REGISTER,
    tsip_SUBSCRIBE = tsip_SUBSCRIBE,
    tsip_NOTIFY = tsip_NOTIFY,
    tsip_REFER = tsip_REFER,
    tsip_INFO = tsip_INFO,
    tsip_UPDATE = tsip_UPDATE,
    tsip_MESSAGE = tsip_MESSAGE,
    tsip_PUBLISH = tsip_PUBLISH,
    tsip_PRACK = tsip_PRACK
};

public enum class rt_tsip_subscribe_event_type_t
{
    tsip_i_subscribe = tsip_i_subscribe,
    tsip_ao_subscribe = tsip_ao_subscribe,

    tsip_i_unsubscribe = tsip_i_unsubscribe,
    tsip_ao_unsubscribe = tsip_ao_unsubscribe,

    tsip_i_notify = tsip_i_notify,
    tsip_ao_notify = tsip_ao_notify
};

public enum class rt_tdav_codec_id_t
{
    tdav_codec_id_none = tmedia_codec_id_none,

    tdav_codec_id_amr_nb_oa = tmedia_codec_id_amr_nb_oa,
    tdav_codec_id_amr_nb_be = tmedia_codec_id_amr_nb_be,
    tdav_codec_id_amr_wb_oa = tmedia_codec_id_amr_wb_oa,
    tdav_codec_id_amr_wb_be = tmedia_codec_id_amr_wb_be,
    tdav_codec_id_gsm = tmedia_codec_id_gsm,
    tdav_codec_id_pcma = tmedia_codec_id_pcma,
    tdav_codec_id_pcmu = tmedia_codec_id_pcmu,
    tdav_codec_id_ilbc = tmedia_codec_id_ilbc,
    tdav_codec_id_speex_nb = tmedia_codec_id_speex_nb,
    tdav_codec_id_speex_wb = tmedia_codec_id_speex_wb,
    tdav_codec_id_speex_uwb = tmedia_codec_id_speex_uwb,
    tdav_codec_id_bv16 = tmedia_codec_id_bv16,
    tdav_codec_id_bv32 = tmedia_codec_id_bv32,
    tdav_codec_id_opus = tmedia_codec_id_opus,
    tdav_codec_id_g729ab = tmedia_codec_id_g729ab,
    tdav_codec_id_g722 = tmedia_codec_id_g722,

    tdav_codec_id_h261 = tmedia_codec_id_h261,
    tdav_codec_id_h263 = tmedia_codec_id_h263,
    tdav_codec_id_h263p = tmedia_codec_id_h263p,
    tdav_codec_id_h263pp = tmedia_codec_id_h263pp,
    tdav_codec_id_h264_bp = tmedia_codec_id_h264_bp,
    tdav_codec_id_h264_mp = tmedia_codec_id_h264_mp,
    tdav_codec_id_h264_hp = tmedia_codec_id_h264_hp,
    tdav_codec_id_h264_bp10 = tmedia_codec_id_h264_bp, // @deprecated
    tdav_codec_id_h264_bp20 = tmedia_codec_id_h264_bp, // @deprecated
    tdav_codec_id_h264_bp30 = tmedia_codec_id_h264_bp, // @deprecated
    tdav_codec_id_h264_svc = tmedia_codec_id_h264_svc,
    tdav_codec_id_theora = tmedia_codec_id_theora,
    tdav_codec_id_mp4ves_es = tmedia_codec_id_mp4ves_es,
    tdav_codec_id_vp8 = tmedia_codec_id_vp8,

    tdav_codec_id_t140 = tmedia_codec_id_t140,
#if !COM_VISIBLE
    tdav_codec_id_red = tmedia_codec_id_red,
#endif
    tdav_codec_id_all = tmedia_codec_id_all,
};
}
}
