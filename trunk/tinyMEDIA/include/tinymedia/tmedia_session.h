/*
* Copyright (C) 2009 Mamadou Diop.
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

/**@file tmedia_session.h
 * @brief Base session object.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYMEDIA_SESSION_H
#define TINYMEDIA_SESSION_H

#include "tinymedia_config.h"

#include "tmedia_codec.h"

#include "tinysdp/tsdp_message.h"

#include "tsk_list.h"

TMEDIA_BEGIN_DECLS

#define TMEDIA_SESSION(self)	((tmedia_session_t*)(self))

/**Max number of plugins (session types) we can create */
#define TMED_SESSION_MAX_PLUGINS			0x0F

#define TMEDIA_SESSION_SET();

/** Base objct used for all media sessions */
typedef struct tmedia_session_s
{
	TSK_DECLARE_OBJECT;
	
	tmedia_type_t type;
	//! list of codecs managed by this session
	tmedia_codecs_L_t* codecs;
	//! negociated codec
	tmedia_codec_t* negociated_codec;
	char* negociated_format;
	//! whether the ro have been prepared (up to the manager to update the value)
	tsk_bool_t ro_changed;
	//! whether the session have been initialized (up to the manager to update the value)
	tsk_bool_t initialized;
	//! whether the session have been prepared (up to the manager to update the value)
	tsk_bool_t prepared;

	struct{
		tsdp_header_M_t* lo;
		tsdp_header_M_t* ro;
	} M;

	//! plugin used to create the session
	const struct tmedia_session_plugin_def_s* plugin;
}
tmedia_session_t;

/** Virtual table used to define a session plugin */
typedef struct tmedia_session_plugin_def_s
{
	//! object definition used to create an instance of the session
	const tsk_object_def_t* objdef;

	//! the type of the session
	tmedia_type_t type;
	//! the media name. e.g. "audio", "video", "message", "image" etc.
	const char* media;
	
	int (*configure) (tmedia_session_t* , const va_list *app);
	int (* prepare) (tmedia_session_t* );
	int (* start) (tmedia_session_t* );
	int (* pause) (tmedia_session_t* );
	int (* stop) (tmedia_session_t* );

	const tsdp_header_M_t* (* get_local_offer) (tmedia_session_t* );
	/* return zero if can handle the ro and non-zero otherwise */
	int (* set_remote_offer) (tmedia_session_t* , const tsdp_header_M_t* );
}
tmedia_session_plugin_def_t;

TINYMEDIA_API int tmedia_session_init(tmedia_session_t* self, tmedia_type_t type);
TINYMEDIA_API int tmedia_session_cmp(const tsk_object_t* sess1, const tsk_object_t* sess2);
TINYMEDIA_API int tmedia_session_plugin_register(const tmedia_session_plugin_def_t* plugin);
TINYMEDIA_API const tmedia_session_plugin_def_t* tmedia_session_plugin_find_by_media(const char* media);
TINYMEDIA_API int tmedia_session_plugin_unregister(const tmedia_session_plugin_def_t* plugin);
TINYMEDIA_API tmedia_session_t* tmedia_session_create(tmedia_type_t type);
TINYMEDIA_API const tmedia_codec_t* tmedia_session_match_codec(tmedia_session_t* self, const tsdp_header_M_t* M, char** format);
TINYMEDIA_API int tmedia_session_deinit(tmedia_session_t* self);
typedef tsk_list_t tmedia_sessions_L_t; /**< List of @ref tmedia_session_t objects */
#define TMEDIA_DECLARE_SESSION tmedia_session_t __session__

/** Audio Session */
typedef struct tmedia_session_audio_s
{
	TMEDIA_DECLARE_SESSION;
	
	/** Will be passed to the rtp session which will call this function to signal that
	* there is new data (from the network). It's up to the session to decode the data using
	* the negociated codec. Once the data is decoded, it should be passed to the consumer.
	*/
	int (* from_rtp_cb) (struct tmedia_session_audio_s* self, const void* data, tsk_size_t size);
	/**! Will be passed to the producer which will call this callback to signal
	* that there is new data to send. @a data contains raw audio buffer (most likely PCM 16bit).
	* The data should be encoded using the negociated codec
	* before to be sent over the network (using the rtp session).
	*/
	int (* from_producer_cb) (struct tmedia_session_audio_s* self, const void* data, tsk_size_t size);
}
tmedia_session_audio_t;
#define tmedia_session_audio_init(self)	tmedia_session_init(TMEDIA_SESSION(self), tmed_sess_type_audio)
#define tmedia_session_audio_deinit(self) tmedia_session_deinit(TMEDIA_SESSION(self))
#define tmedia_session_audio_create() tmedia_session_create(tmed_sess_type_audio)
#define TMEDIA_DECLARE_SESSION_AUDIO tmedia_session_t __session_audio__

/** Video Session */
typedef struct tmedia_session_video_s
{
	TMEDIA_DECLARE_SESSION;
	
	/** Will be passed to the rtp session which will call this function to signal that
	* there is new data (from the network). It's up to the session to decode the data using
	* the negociated codec. Once the data is decoded, it should be passed to the consumer.
	*/
	int (* from_rtp_cb) (struct tmedia_session_video_s* self, const void* data, tsk_size_t size);
	/**! Will be passed to the producer which will call this callback to signal
	* that there is new data to send. @a data contains raw audio buffer (most likely PCM 16bit).
	* The data should be encoded using the negociated codec
	* before to be sent over the network (using the rtp session).
	*/
	int (* from_producer_cb) (struct tmedia_session_video_s* self, const void* data, tsk_size_t size);
}
tmedia_session_video_t;
#define tmedia_session_video_init(self)	tmedia_session_init(TMEDIA_SESSION(self), tmed_sess_type_video)
#define tmedia_session_video_deinit(self) tmedia_session_deinit(TMEDIA_SESSION(self))
#define tmedia_session_video_create() tmedia_session_create(tmed_sess_type_video)
#define TMEDIA_DECLARE_SESSION_VIDEO tmedia_session_t __session_video__

/** Msrp Session */
typedef struct tmedia_session_msrp_s
{
	TMEDIA_DECLARE_SESSION;

	int (* send_file) (struct tmedia_session_msrp_s*, const char* path, ...);
	int (* send_message) (struct tmedia_session_msrp_s*, const void* data, tsk_size_t size, ...);
}
tmedia_session_msrp_t;
#define tmedia_session_msrp_init(self)	tmedia_session_init(TMEDIA_SESSION(self), tmed_sess_type_msrp)
#define tmedia_session_msrp_deinit(self) tmedia_session_deinit(TMEDIA_SESSION(self))
#define tmedia_session_msrp_create() tmedia_session_create(tmed_sess_type_msrp)
#define TMEDIA_DECLARE_SESSION_MSRP tmedia_session_t __session_msrp__

/** Session manager */
typedef struct tmedia_session_mgr_s
{
	TSK_DECLARE_OBJECT;
	
	//! local IP address or FQDN
	char* addr;
	//! whether the @a addr is IPv6 or not (useful when @addr is a FQDN)
	tsk_bool_t ipv6;

	struct{
		tsdp_message_t* lo;
		tsdp_message_t* ro;
	} sdp;

	tsk_bool_t started;
	tsk_bool_t ro_changed;
	tsk_bool_t state_changed;

	//! session type
	tmedia_type_t type;

	//! List of all sessions
	tmedia_sessions_L_t* sessions;
}
tmedia_session_mgr_t;

TINYMEDIA_API tmedia_session_mgr_t* tmedia_session_mgr_create(tmedia_type_t type, const char* addr, tsk_bool_t ipv6);
TINYMEDIA_API int tmedia_session_mgr_start(tmedia_session_mgr_t* self);
TINYMEDIA_API int tmedia_session_mgr_configure(tmedia_session_mgr_t* self, tmedia_type_t type, ...);
TINYMEDIA_API int tmedia_session_mgr_stop(tmedia_session_mgr_t* self);
TINYMEDIA_API const tsdp_message_t* tmedia_session_mgr_get_lo(tmedia_session_mgr_t* self);
TINYMEDIA_API int tmedia_session_mgr_set_ro(tmedia_session_mgr_t* self, const tsdp_message_t* sdp);
TINYMEDIA_API int tmedia_session_mgr_hold(tmedia_session_mgr_t* self, tmedia_type_t type);
TINYMEDIA_API tsk_bool_t tmedia_session_mgr_is_held(tmedia_session_mgr_t* self, tmedia_type_t type, tsk_bool_t local);
TINYMEDIA_API int tmedia_session_mgr_resume(tmedia_session_mgr_t* self, tmedia_type_t type);
TINYMEDIA_API int tmedia_session_mgr_add_media(tmedia_session_mgr_t* self, tmedia_type_t type);
TINYMEDIA_API int tmedia_session_mgr_remove_media(tmedia_session_mgr_t* self, tmedia_type_t type);

TINYMEDIA_GEXTERN const tsk_object_def_t *tmedia_session_mgr_def_t;

TMEDIA_END_DECLS

#endif /* TINYMEDIA_SESSION_H */
