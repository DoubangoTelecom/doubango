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
#include "tinymedia/tmedia_session.h"

#include "tinymedia/tmedia_session_ghost.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

/**@defgroup tmedia_session_group Media Session
* For more information about the SOA, please refer to http://betelco.blogspot.com/2010/03/sdp-offeranswer-soa_2993.html
*/

extern const tmedia_codec_plugin_def_t* __tmedia_codec_plugins[TMED_CODEC_MAX_PLUGINS];

/* pointer to all registered sessions */
const tmedia_session_plugin_def_t* __tmedia_session_plugins[TMED_SESSION_MAX_PLUGINS] = {0};

/* === local functions === */
int _tmedia_session_mgr_load_sessions(tmedia_session_mgr_t* self);
int _tmedia_session_prepare_lo(tmedia_session_t* self);
int _tmedia_session_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m);
int _tmedia_session_load_codecs(tmedia_session_t* self);

const char* tmedia_session_get_media(const tmedia_session_t* self);
const tsdp_header_M_t* tmedia_session_get_lo(tmedia_session_t* self);
int tmedia_session_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m);


/*== Predicate function to find session object by media */
int __pred_find_session_by_media(const tsk_list_item_t *item, const void *media)
{
	if(item && item->data){
		return tsk_stricmp(tmedia_session_get_media((const tmedia_session_t *)item->data), (const char*)media);
	}
	return -1;
}

/*== Predicate function to find session object by type */
int __pred_find_session_by_type(const tsk_list_item_t *item, const void *type)
{
	if(item && item->data){
		return ((const tmedia_session_t *)item->data)->type - *((tmedia_type_t*)type);
	}
	return -1;
}

/**@ingroup tmedia_session_group
* Initializes a newly created media session.
* @param self the media session to initialize.
* @param type the type of the session to initialize.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tmedia_session_init(tmedia_session_t* self, tmedia_type_t type)
{
	int ret = 0;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	if(!self->initialized){
		/* set values */
		self->type = type;
		self->initialized = tsk_true;
		/* load associated codecs */
		ret = _tmedia_session_load_codecs(self);
	}

	return 0;
}

/**@ingroup tmedia_session_group
* Generic function to compare two sessions.
* @param sess1 The first session to compare.
* @param sess2 The second session to compare.
* @retval Returns an integral value indicating the relationship between the two sessions:
* <0 : @a sess1 less than @a sess2.<br>
* 0  : @a sess1 identical to @a sess2.<br>
* >0 : @a sess1 greater than @a sess2.<br>
*/
int tmedia_session_cmp(const tsk_object_t* sess1, const tsk_object_t* sess2)
{	
	return (TMEDIA_SESSION(sess1) - TMEDIA_SESSION(sess2));
}

/**@ingroup tmedia_session_group
* Registers a session plugin.
* @param plugin the definition of the plugin.
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref tmedia_session_create()
*/
int tmedia_session_plugin_register(const tmedia_session_plugin_def_t* plugin)
{
	tsk_size_t i;
	if(!plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* add or replace the plugin */
	for(i = 0; i<TMED_SESSION_MAX_PLUGINS; i++){
		if(!__tmedia_session_plugins[i] || (__tmedia_session_plugins[i] == plugin)){
			__tmedia_session_plugins[i] = plugin;
			return 0;
		}
	}
	
	TSK_DEBUG_ERROR("There are already %d plugins.", TMED_SESSION_MAX_PLUGINS);
	return -2;
}

/**@ingroup tmedia_session_group
* Finds a plugin by media.
*/
const tmedia_session_plugin_def_t* tmedia_session_plugin_find_by_media(const char* media)
{
	tsk_size_t i = 0;
	if(tsk_strnullORempty(media)){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	/* add or replace the plugin */
	while((i<TMED_SESSION_MAX_PLUGINS) && (__tmedia_session_plugins[i])){
		if(tsk_striequals(__tmedia_session_plugins[i]->media, media)){
			return __tmedia_session_plugins[i];
		}
		i++;
	}
	return tsk_null;
}

/**@ingroup tmedia_session_group
* UnRegisters a session plugin.
* @param plugin the definition of the plugin.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tmedia_session_plugin_unregister(const tmedia_session_plugin_def_t* plugin)
{
	tsk_size_t i;
	tsk_bool_t found = tsk_false;
	if(!plugin){
		TSK_DEBUG_ERROR("Invalid Parameter");
		return -1;
	}
	
	/* find the plugin to unregister */
	for(i = 0; i<TMED_SESSION_MAX_PLUGINS; i++){
		if(__tmedia_session_plugins[i] == plugin){
			__tmedia_session_plugins[i] = tsk_null;
			found = tsk_true;
			break;
		}
	}
	
	/* compact */
	if(found){
		for(; i<(TMED_SESSION_MAX_PLUGINS - 1); i++){
			if(__tmedia_session_plugins[i+1]){
				__tmedia_session_plugins[i] = __tmedia_session_plugins[i+1];
			}
			else{
				break;
			}
		}
		__tmedia_session_plugins[i] = tsk_null;
	}
	return (found ? 0 : -2);
}

/**@ingroup tmedia_session_group
* Creates a new session using an already registered plugin.
* @param format The type of the codec to create.
* @sa @ref tmedia_codec_plugin_register()
*/
tmedia_session_t* tmedia_session_create(tmedia_type_t type)
{
	tmedia_session_t* session = tsk_null;
	const tmedia_session_plugin_def_t* plugin;
	tsk_size_t i = 0;

	while((i < TMED_SESSION_MAX_PLUGINS) && (plugin = __tmedia_session_plugins[i++])){
		if(plugin->objdef && (plugin->type == type)){
			if((session = tsk_object_new(plugin->objdef))){
				if(!session->initialized){
					tmedia_session_init(session, type);
				}
				session->plugin = plugin;
			}
			break;
		}
	}
	return session;
}

/* internal funtion: prepare lo */
int _tmedia_session_prepare_lo(tmedia_session_t* self)
{
	int ret;
	if(!self || !self->plugin || !self->plugin->prepare){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
	if(self->prepared){
		TSK_DEBUG_WARN("Session already prepared");
		return 0;
	}
	if((ret = self->plugin->prepare(self))){
		TSK_DEBUG_ERROR("Failed to prepare the session");
	}
	else{
		self->prepared = tsk_true;
	}
	return ret;
}

/* internal function used to set remote offer */
int _tmedia_session_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m)
{
	int ret;
	if(!self || !self->plugin || !self->plugin->set_remote_offer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(!(ret = self->plugin->set_remote_offer(self, m))){
		self->ro_changed = tsk_true;
	}
	return ret;
}

/* internal function: get media */
const char* tmedia_session_get_media(const tmedia_session_t* self)
{
	if(!self || !self->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	/* ghost? */
	if(self->plugin == tmedia_session_ghost_plugin_def_t){
		return ((const tmedia_session_ghost_t*)self)->media;
	}
	else{
		return self->plugin->media;
	}
}
/* internal function: get local offer */
const tsdp_header_M_t* tmedia_session_get_lo(tmedia_session_t* self)
{
	const tsdp_header_M_t* m;

	if(!self || !self->plugin || !self->plugin->get_local_offer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
	
	if((m = self->plugin->get_local_offer(self))){
		self->ro_changed = tsk_false; /* we should have a fresh local offer (based on the latest ro) */
	}
	return m;
}

/* Match a codec */
const tmedia_codec_t* tmedia_session_match_codec(tmedia_session_t* self, const tsdp_header_M_t* M, char** format)
{
	const tmedia_codec_t *codec;
	char *rtpmap = tsk_null, *fmtp = tsk_null, *name = tsk_null;
	const tsdp_fmt_t* fmt;
	const tsk_list_item_t *it1, *it2;
	tsk_bool_t found = tsk_false;
	
	if(!self || !M){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
	
	/* foreach format */
	tsk_list_foreach(it1, M->FMTs){
		fmt = it1->data;
		
		/* foreach codec */
		tsk_list_foreach(it2, self->codecs){
			if(!(codec = it2->data) || !codec->plugin){
				continue;
			}
			
			// Dyn. payload type
			if(codec->dyn && (rtpmap = tsdp_header_M_get_rtpmap(M, fmt->value))){
				int32_t rate, channels;
				/* parse rtpmap */
				if(tmedia_parse_rtpmap(rtpmap, &name, &rate, &channels)){
					goto next;
				}
				
				/* compare name and rate. what about channels? */
				if(tsk_striequals(name, codec->name) && (!rate || (codec->plugin->rate == rate))){
					goto compare_fmtp;
				}
			}
			// Fixed payload type
			else{
				if(tsk_striequals(fmt->value, codec->format)){
					goto compare_fmtp;
				}
			}
			
			/* rtpmap do not match: free strings and try next codec */
			goto next;

compare_fmtp:
			if((fmtp = tsdp_header_M_get_fmtp(M, fmt->value))){ /* remote have fmtp? */
				if(tmedia_codec_match_fmtp(codec, fmtp)){ /* fmtp matches? */
					tsk_strupdate(format, fmt->value);
					found = tsk_true;
				}
			}
			else{ /* no fmtp -> always match */
				tsk_strupdate(format, fmt->value);
				found = tsk_true;
			}
next:
			TSK_FREE(name);
			TSK_FREE(fmtp);
			TSK_FREE(rtpmap);
			if(found){
				return codec;
			}
		}
	}
	

	return tsk_null;
}

/**@ingroup tmedia_session_group
* skip unsupported param
*/
int tmedia_session_skip_param(enum tmedia_session_param_type_e type, va_list *app)
{
	switch(type){
		case tmedia_sptype_remote_ip:
			/* (const char*) IP_STR */
			va_arg(*app, const char *);
			break;
		case tmedia_sptype_local_ip:
			/* (const char*) IP_STR, (tsk_bool_t)IPv6_BOOL */
			va_arg(*app, const char *);
			va_arg(*app, tsk_bool_t);
			break;
		case tmedia_sptype_set_rtcp:
			/* (tsk_bool_t)ENABLED_BOOL */
			va_arg(*app, tsk_bool_t);
			break;

		default:
			TSK_DEBUG_ERROR("%d is an unknown parameter", type);
			return -1;
	}

	return 0;
}

/**@ingroup tmedia_session_group
* DeInitializes a media session.
* @param self the media session to deinitialize.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tmedia_session_deinit(tmedia_session_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	/* free codecs */
	TSK_OBJECT_SAFE_FREE(self->codecs);
	TSK_OBJECT_SAFE_FREE(self->negociated_codec);
	TSK_FREE(self->negociated_format);
	
	/* free lo, no and ro */
	TSK_OBJECT_SAFE_FREE(self->M.lo);
	TSK_OBJECT_SAFE_FREE(self->M.ro);

	return 0;
}

/* internal function used to prepare a session */
int _tmedia_session_load_codecs(tmedia_session_t* self)
{
	tsk_size_t i = 0;
	tmedia_codec_t* codec;
	const tmedia_codec_plugin_def_t* plugin;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* remove old codecs */
	TSK_OBJECT_SAFE_FREE(self->codecs);

	/* for each registered plugin create a session instance */
	while((i < TMED_CODEC_MAX_PLUGINS) && (plugin = __tmedia_codec_plugins[i++])){
		if((plugin->type & self->type) == plugin->type){
			if((codec = tmedia_codec_create(plugin->format))){
				if(!self->codecs){
					self->codecs = tsk_list_create();
				}
				tsk_list_push_back_data(self->codecs, (void**)(&codec));
			}
		}
	}
	return 0;
}


/**@ingroup tmedia_session_group
* Creates new session manager.
* @param type the type of the session to create. For example, (@ref tmed_sess_type_audio | @ref tmed_sess_type_video).
* @param addr the local ip address or FQDN to use in the sdp message.
* @param ipv6 indicates whether @a addr is IPv6 address or not. Useful when @a addr is a FQDN.
* @param load_sessions Whether the offerer or not.
* will create an audio/video session.
* @retval new @ref tmedia_session_mgr_t object
*/
tmedia_session_mgr_t* tmedia_session_mgr_create(tmedia_type_t type, const char* addr, tsk_bool_t ipv6, tsk_bool_t offerer)
{
	tmedia_session_mgr_t* mgr;

	if(!(mgr = tsk_object_new(tmedia_session_mgr_def_t))){
		TSK_DEBUG_ERROR("Failed to create Media Session manager");
		return tsk_null;
	}

	/* init */
	mgr->type = type;
	mgr->addr = tsk_strdup(addr);
	mgr->ipv6 = ipv6;

	/* load sessions (will allow us to generate lo) */
	if(offerer){
		if(_tmedia_session_mgr_load_sessions(mgr)){
			/* Do nothing */
			TSK_DEBUG_ERROR("Failed to load sessions");
		}
	}

	return mgr;
}

/**@ingroup tmedia_session_group
* Starts the session manager by starting all underlying sessions.
* You should set both remote and local offers before calling this function.
* @param self The session manager to start.
* @retval Zero if succced and non-zero error code otherwise.
*
* @sa @ref tmedia_session_mgr_stop
*/
int tmedia_session_mgr_start(tmedia_session_mgr_t* self)
{
	int ret;
	tsk_list_item_t* item;
	tmedia_session_t* session;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
    
	if(self->started){
		TSK_DEBUG_WARN("Session manager already started");
		return 0;
	}

	tsk_list_foreach(item, self->sessions){
		if(!(session = item->data) || !session->plugin || !session->plugin->start){
			TSK_DEBUG_ERROR("Invalid session");
			return -2;
		}
		if((ret = session->plugin->start(session))){
			TSK_DEBUG_ERROR("Failed to start %s session", session->plugin->media);
			continue;
		}
	}

	self->started = tsk_true;
	return 0;
}

/**@ingroup tmedia_session_group
* sets one or several sessions.
* @param self The session manager
* @param type The type of the sessions to set
* @param ... Any TMEDIA_SESSION_SET_*() macros
* @retval Zero if succeed and non-zero error code otherwise
*/
int tmedia_session_mgr_set(tmedia_session_mgr_t* self, tmedia_type_t type, ...)
{
	tsk_list_item_t* item;
	tmedia_session_t* session;
	va_list ap;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_list_foreach(item, self->sessions){
		if(!(session = item->data)){
			TSK_DEBUG_ERROR("Invalid session");
			return -2;
		}
		
		/* does not support set() */
		if(!session->plugin->set){
			continue;
		}

		va_start(ap, type);
		if(((session->type & type) == session->type) && session->plugin->set(session, &ap)){
			TSK_DEBUG_ERROR("Failed to set (%s) session", session->plugin->media);
		}
		va_end(ap);
	}

	return 0;
}

/**@ingroup tmedia_session_group
* Stops the session manager by stopping all underlying sessions.
* @param self The session manager to stop.
* @retval Zero if succced and non-zero error code otherwise.
*
* @sa @ref tmedia_session_mgr_start
*/
int tmedia_session_mgr_stop(tmedia_session_mgr_t* self)
{
	int ret;
	tsk_list_item_t* item;
	tmedia_session_t* session;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!self->started){
		TSK_DEBUG_WARN("Session manager not started");
		return 0;
	}

	tsk_list_foreach(item, self->sessions){
		if(!(session = item->data) || !session->plugin || !session->plugin->stop){
			TSK_DEBUG_ERROR("Invalid session");
			return -2;
		}
		if((ret = session->plugin->stop(session))){
			TSK_DEBUG_ERROR("Failed to stop session");
			return ret;
		}
	}
	self->started = tsk_false;
	return 0;
}

/**@ingroup tmedia_session_group
* Gets local offer.
*/
const tsdp_message_t* tmedia_session_mgr_get_lo(tmedia_session_mgr_t* self)
{
	const tsk_list_item_t* item;
	const tmedia_session_t* ms;
	const tsdp_header_M_t* m;
	
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
	
	/* prepare the session manager if not already done (create all sessions) */
	if(TSK_LIST_IS_EMPTY(self->sessions)){
		if(_tmedia_session_mgr_load_sessions(self)){
			TSK_DEBUG_ERROR("Failed to prepare the session manager");
			return tsk_null;
		}
	}
	
	/* creates local sdp if not already done or update it's value (because of set_ro())*/
	if((self->ro_changed || self->state_changed) && self->sdp.lo){
		TSK_OBJECT_SAFE_FREE(self->sdp.lo);
		self->ro_changed = tsk_false;
		self->state_changed = tsk_false;
	}

	if(self->sdp.lo){
		return self->sdp.lo;
	}
	else if((self->sdp.lo = tsdp_message_create_empty(self->addr, self->ipv6))){
		/* Set connection "c=" */
		tsdp_message_add_headers(self->sdp.lo,
			TSDP_HEADER_C_VA_ARGS("IN", self->ipv6 ? "IP6" : "IP4", self->addr),
			tsk_null);
	}else{
		TSK_DEBUG_ERROR("Failed to create empty SDP message");
		return tsk_null;
	}

	/* gets each "m=" line from the sessions and add them to the local sdp */
	tsk_list_foreach(item, self->sessions){
		if(!(ms = item->data) || !ms->plugin){
			TSK_DEBUG_ERROR("Invalid session");
			continue;
		}
		/* prepare the media session */
		if(!ms->prepared && (_tmedia_session_prepare_lo(TMEDIA_SESSION(ms)))){
			TSK_DEBUG_ERROR("Failed to prepare session"); /* should never happen */
			continue;
		}
		
		/* add "m=" line from the session to the local sdp */
		if((m = tmedia_session_get_lo(TMEDIA_SESSION(ms)))){
			tsdp_message_add_header(self->sdp.lo, TSDP_HEADER(m));
		}
		else{
			TSK_DEBUG_ERROR("Failed to get m= line for [%s] media", ms->plugin->media);
		}
	}
	
	return self->sdp.lo;
}


/**@ingroup tmedia_session_group
* Sets remote offer.
*/
int tmedia_session_mgr_set_ro(tmedia_session_mgr_t* self, const tsdp_message_t* sdp)
{
	const tmedia_session_t* ms;
	const tsdp_header_M_t* M;
	const tsdp_header_C_t* C; /* global "c=" line */
	tsk_size_t index = 0;
	tsk_bool_t found;

	if(!self || !sdp){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	/* update remote offer */
	TSK_OBJECT_SAFE_FREE(self->sdp.ro);
	self->sdp.ro = tsk_object_ref((void*)sdp);

	/* prepare the session manager if not already done (create all sessions with their codecs) 
	* if network-initiated: think about tmedia_type_from_sdp() before creating the manager */
	if(TSK_LIST_IS_EMPTY(self->sessions)){
		if(_tmedia_session_mgr_load_sessions(self)){
			TSK_DEBUG_ERROR("Failed to prepare the session manager");
			return -2;
		}
	}
	
	/* get global connection line (common to all sessions) 
	* Each session should override this info if it has a different one in its "m=" line
	*/
	if((C = (const tsdp_header_C_t*)tsdp_message_get_header(sdp, tsdp_htype_C)) && C->addr){
		tmedia_session_mgr_set(self, self->type,
			TMEDIA_SESSION_SET_REMOTE_IP(C->addr),
			TMEDIA_SESSION_SET_NULL());
	}

	/* foreach "m=" line in the remote offer create a session*/
	while((M = (const tsdp_header_M_t*)tsdp_message_get_headerAt(sdp, tsdp_htype_M, index++))){
		found = tsk_false;
		/* Find session by media */
		if((ms = tsk_list_find_object_by_pred(self->sessions, __pred_find_session_by_media, M->media))){
			/* set remote ro at session-level */
			if(_tmedia_session_set_ro(TMEDIA_SESSION(ms), M) == 0){
				found = tsk_true;
			}
		}
		
		if(!found && (self->sdp.lo == tsk_null)){
			/* Session not supported and we are not the initial offerer ==> add ghost session */
			tmedia_session_ghost_t* ghost;
			if((ghost = (tmedia_session_ghost_t*)tmedia_session_create(tmedia_ghost))){
				tsk_strupdate(&ghost->media, M->media); /* copy media */
				tsk_list_push_back_data(self->sessions, (void**)&ghost);
			}
			else{
				TSK_DEBUG_ERROR("Failed to create ghost session");
				continue;
			}
		}
	}

	/* signal that ro has changed (will be used to update lo) */
	self->ro_changed = tsk_true;

	return 0;
}

/**@ingroup tmedia_session_group
* Holds the session as per 3GPP TS 34.610
* @param self the session manager managing the session to hold.
* @param type the type of the sessions to hold (you can combine several medias. e.g. audio|video|msrp).
* @retval Zero if succeed and non zero error code otherwise.
* @sa @ref tmedia_session_mgr_resume
*/
int tmedia_session_mgr_hold(tmedia_session_mgr_t* self, tmedia_type_t type)
{
	const tsk_list_item_t* item;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_list_foreach(item, self->sessions){
		tmedia_session_t* session = TMEDIA_SESSION(item->data);
		if(((session->type & type) == session->type) && session->M.lo){
			if(!tsdp_header_M_hold(session->M.lo)){
				self->state_changed = tsk_true;
			}
		}
	}
	return 0;
}

/**@ingroup tmedia_session_group
* Indicates whether the specified medias are held or not.
* @param self the session manager
* @param type the type of the medias to check (you can combine several medias. e.g. audio|video|msrp)
* @param local whether to check local or remote medias
*/
tsk_bool_t tmedia_session_mgr_is_held(tmedia_session_mgr_t* self, tmedia_type_t type, tsk_bool_t local)
{	
	const tsk_list_item_t* item;
	tsk_bool_t have_these_sessions = tsk_false;
	
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_false;
	}

	tsk_list_foreach(item, self->sessions){
		tmedia_session_t* session = TMEDIA_SESSION(item->data);
		if((session->type & type) == session->type){
			if(local && session->M.lo){
				have_these_sessions = tsk_true;
				if(!tsdp_header_M_is_held(session->M.lo, tsk_true)){
					return tsk_false;
				}
			}
			else if(!local && session->M.ro){
				have_these_sessions = tsk_true;
				if(!tsdp_header_M_is_held(session->M.ro, tsk_false)){
					return tsk_false;
				}
			}
		}
	}
	/* none is held */
	return have_these_sessions ? tsk_true : tsk_false;
}

/**@ingroup tmedia_session_group
* Resumes the session as per 3GPP TS 34.610. Should be previously held
* by using @ref tmedia_session_mgr_hold.
* @param self the session manager managing the session to resume.
* @param type the type of the sessions to resume (you can combine several medias. e.g. audio|video|msrp).
* @retval Zero if succeed and non zero error code otherwise.
* @sa @ref tmedia_session_mgr_hold
*/
int tmedia_session_mgr_resume(tmedia_session_mgr_t* self, tmedia_type_t type)
{
	const tsk_list_item_t* item;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	tsk_list_foreach(item, self->sessions){
		tmedia_session_t* session = TMEDIA_SESSION(item->data);
		if(((session->type & type) == session->type) && session->M.lo){
			if(!tsdp_header_M_resume(session->M.lo)){
				self->state_changed = tsk_true;
			}
		}
	}
	return 0;
}

/**@ingroup tmedia_session_group
* Adds new medias to the manager. A media will only be added if it is missing
* or previously removed (slot with port equal to zero).
* @param self The session manager
* @param The types of the medias to add (ou can combine several medias. e.g. audio|video|msrp)
* @retval Zero if succeed and non zero error code otherwise.
*/
int tmedia_session_mgr_add_media(tmedia_session_mgr_t* self, tmedia_type_t type)
{
	tsk_size_t i = 0;
	tmedia_session_t* session;
	const tmedia_session_plugin_def_t* plugin;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* for each registered plugin match with the supplied type */
	while((i < TMED_SESSION_MAX_PLUGINS) && (plugin = __tmedia_session_plugins[i++])){
		if((plugin->type & type) == plugin->type){
			/* check whether we already support this media */
			if((session = (tmedia_session_t*)tsk_list_find_object_by_pred(self->sessions, __pred_find_session_by_type, &plugin->type)) && session->plugin){
				if(session->prepared){
					TSK_DEBUG_WARN("[%s] already active", plugin->media);
				}
				else{
					/* exist but unprepared(port=0) */
					_tmedia_session_prepare_lo(session);
					if(self->started && session->plugin->start){
						session->plugin->start(session);
					}
					self->state_changed = tsk_true;
				}
			}
			else{
				/* session not supported */
				self->state_changed = tsk_true;
				if((session = tmedia_session_create(plugin->type))){
					if(self->started && session->plugin->start){
						session->plugin->start(session);
					}
					tsk_list_push_back_data(self->sessions, (void**)(&session));
					self->state_changed = tsk_true;
				}
			}
		}
	}

	return self->state_changed ? 0 : -2;
}

/**@ingroup tmedia_session_group
* Removes medias from the manager. This action will stop the media and sets it's port value to zero (up to the session).
* @param self The session manager
* @param The types of the medias to remove (ou can combine several medias. e.g. audio|video|msrp)
* @retval Zero if succeed and non zero error code otherwise.
*/
int tmedia_session_mgr_remove_media(tmedia_session_mgr_t* self, tmedia_type_t type)
{
	const tsk_list_item_t* item;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	tsk_list_foreach(item, self->sessions){
		tmedia_session_t* session = TMEDIA_SESSION(item->data);
		if(((session->type & type) == session->type) && session->plugin->stop){
			if(!session->plugin->stop(session)){
				self->state_changed = tsk_true;
			}
		}
	}
	return 0;
}


/** internal function used to load sessions
*/
int _tmedia_session_mgr_load_sessions(tmedia_session_mgr_t* self)
{
	tsk_size_t i = 0;
	tmedia_session_t* session;
	const tmedia_session_plugin_def_t* plugin;

	if(TSK_LIST_IS_EMPTY(self->sessions)){
		/* for each registered plugin create a session instance */
		while((i < TMED_SESSION_MAX_PLUGINS) && (plugin = __tmedia_session_plugins[i++])){
			if((plugin->type & self->type) == plugin->type){
				if((session = tmedia_session_create(plugin->type))){
					tsk_list_push_back_data(self->sessions, (void**)(&session));
				}
			}
		}
		/* set ldefault values */
		tmedia_session_mgr_set(self, self->type,
			TMEDIA_SESSION_SET_LOCAL_IP(self->addr, self->ipv6),
			TMEDIA_SESSION_SET_NULL());
	}
	return 0;
}

//=================================================================================================
//	Media Session Manager object definition
//
static tsk_object_t* tmedia_session_mgr_ctor(tsk_object_t * self, va_list * app)
{
	tmedia_session_mgr_t *mgr = self;
	if(mgr){
		mgr->sessions = tsk_list_create();
	}
	return self;
}

static tsk_object_t* tmedia_session_mgr_dtor(tsk_object_t * self)
{ 
	tmedia_session_mgr_t *mgr = self;
	if(mgr){
		TSK_OBJECT_SAFE_FREE(mgr->sessions);

		TSK_OBJECT_SAFE_FREE(mgr->sdp.lo);
		TSK_OBJECT_SAFE_FREE(mgr->sdp.ro);

		TSK_FREE(mgr->addr);
	}

	return self;
}

static const tsk_object_def_t tmedia_session_mgr_def_s = 
{
	sizeof(tmedia_session_mgr_t),
	tmedia_session_mgr_ctor, 
	tmedia_session_mgr_dtor,
	tsk_null, 
};
const tsk_object_def_t *tmedia_session_mgr_def_t = &tmedia_session_mgr_def_s;

