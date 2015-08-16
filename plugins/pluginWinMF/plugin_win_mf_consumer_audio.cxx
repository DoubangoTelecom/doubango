/* Copyright (C) 2013 Mamadou DIOP
* Copyright (C) 2013 Doubango Telecom <http://www.doubango.org>
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
#include "plugin_win_mf_config.h"
#include "internals/mf_utils.h"

#include "tinydav/audio/tdav_consumer_audio.h"

#include "tsk_debug.h"

typedef struct plugin_win_mf_consumer_audio_s
{
	TDAV_DECLARE_CONSUMER_AUDIO;

	bool bStarted;
}
plugin_win_mf_consumer_audio_t;


/* ============ Consumer Interface ================= */
static int plugin_win_mf_consumer_audio_set(tmedia_consumer_t* self, const tmedia_param_t* param)
{
	plugin_win_mf_consumer_audio_t* pSelf = (plugin_win_mf_consumer_audio_t*)self;
	int ret = tdav_consumer_audio_set(TDAV_CONSUMER_AUDIO(self), param);

	if(ret == 0){
		
	}

	return ret;
}

static int plugin_win_mf_consumer_audio_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
	plugin_win_mf_consumer_audio_t* pSelf = (plugin_win_mf_consumer_audio_t*)self;

	if(!pSelf){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	return 0;
}

static int plugin_win_mf_consumer_audio_start(tmedia_consumer_t* self)
{
	plugin_win_mf_consumer_audio_t* pSelf = (plugin_win_mf_consumer_audio_t*)self;

	pSelf->bStarted = true;

	return 0;
}

static int plugin_win_mf_consumer_audio_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
	if(!self || !buffer || !size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	/* buffer is already decoded */
	return tdav_consumer_audio_put(TDAV_CONSUMER_AUDIO(self), buffer, size, proto_hdr);
}

static int plugin_win_mf_consumer_audio_pause(tmedia_consumer_t* self)
{
	return 0;
}

static int plugin_win_mf_consumer_audio_stop(tmedia_consumer_t* self)
{
	plugin_win_mf_consumer_audio_t* pSelf = (plugin_win_mf_consumer_audio_t*)self;

	if(!pSelf){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!pSelf->bStarted){
		TSK_DEBUG_INFO("WinMF audio consumer not started");
		return 0;
	}

	/* should be done here */
	pSelf->bStarted = false;
	
	return 0;
}


//
//	WaveAPI consumer object definition
//
/* constructor */
static tsk_object_t* plugin_win_mf_consumer_audio_ctor(tsk_object_t * self, va_list * app)
{
	MFUtils::Startup();

	plugin_win_mf_consumer_audio_t *pSelf = (plugin_win_mf_consumer_audio_t *)self;
	if(pSelf){
		/* init base */
		tdav_consumer_audio_init(TDAV_CONSUMER_AUDIO(pSelf));
		/* init self */
		
	}
	return self;
}
/* destructor */
static tsk_object_t* plugin_win_mf_consumer_audio_dtor(tsk_object_t * self)
{ 
	plugin_win_mf_consumer_audio_t *pSelf = (plugin_win_mf_consumer_audio_t *)self;
	if(pSelf){
		/* stop */
		if(pSelf->bStarted){
			plugin_win_mf_consumer_audio_stop(TMEDIA_CONSUMER(pSelf));
		}

		/* deinit base */
		tdav_consumer_audio_deinit(TDAV_CONSUMER_AUDIO(pSelf));
		/* deinit self */
		
	}

	return self;
}
/* object definition */
static const tsk_object_def_t plugin_win_mf_consumer_audio_def_s = 
{
	sizeof(plugin_win_mf_consumer_audio_t),
	plugin_win_mf_consumer_audio_ctor, 
	plugin_win_mf_consumer_audio_dtor,
	tdav_consumer_audio_cmp, 
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t plugin_win_mf_consumer_audio_plugin_def_s = 
{
	&plugin_win_mf_consumer_audio_def_s,
	
	tmedia_audio,
	"Windows Media Foundation audio consumer",
	
	plugin_win_mf_consumer_audio_set,
	plugin_win_mf_consumer_audio_prepare,
	plugin_win_mf_consumer_audio_start,
	plugin_win_mf_consumer_audio_consume,
	plugin_win_mf_consumer_audio_pause,
	plugin_win_mf_consumer_audio_stop
};
const tmedia_consumer_plugin_def_t *plugin_win_mf_consumer_audio_plugin_def_t = &plugin_win_mf_consumer_audio_plugin_def_s;
