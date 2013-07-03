/* Copyright (C) 2011-2013 Doubango Telecom <http://www.doubango.org>
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
#include "internals/DSDisplay.h"
#include "internals/DSUtils.h"

#include "tinymedia/tmedia_consumer.h"

#include "tsk_string.h"
#include "tsk_debug.h"


#define DSCONSUMER(self)			((plugin_video_dshow_consumer_t*)(self))

typedef struct plugin_video_dshow_consumer_s
{
	TMEDIA_DECLARE_CONSUMER;
	
	DSDisplay* display;
	INT64 window;
	
	tsk_bool_t plugin_firefox;
	tsk_bool_t started;
	tsk_bool_t create_on_ui_thread;
}
plugin_video_dshow_consumer_t;



/* ============ Media Consumer Interface ================= */
static int plugin_video_dshow_consumer_set(tmedia_consumer_t *self, const tmedia_param_t* param)
{
	int ret = 0;

	if(!self || !param){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(param->value_type == tmedia_pvt_int64){
		if(tsk_striequals(param->key, "remote-hwnd")){
			DSCONSUMER(self)->window = (INT64)*((int64_t*)param->value);
			if(DSCONSUMER(self)->display){
				if(DSCONSUMER(self)->window){
					DSCONSUMER(self)->display->attach(DSCONSUMER(self)->window);
				}
				else{
					DSCONSUMER(self)->display->detach();
				}
			}
		}
	}
	else if(param->value_type == tmedia_pvt_int32){
		if(tsk_striequals(param->key, "fullscreen")){
			if(DSCONSUMER(self)->display){
				DSCONSUMER(self)->display->setFullscreen(*((int32_t*)param->value) != 0);
			}
		}
		else if(tsk_striequals(param->key, "create-on-current-thead")){
			DSCONSUMER(self)->create_on_ui_thread = *((int32_t*)param->value) ? tsk_false : tsk_true;
		}
		else if(tsk_striequals(param->key, "plugin-firefox")){
			DSCONSUMER(self)->plugin_firefox = (*((int32_t*)param->value) != 0);
			if(DSCONSUMER(self)->display){
				DSCONSUMER(self)->display->setPluginFirefox((DSCONSUMER(self)->plugin_firefox == tsk_true));
			}
		}
	}

	return ret;
}


static int plugin_video_dshow_consumer_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
	plugin_video_dshow_consumer_t* consumer = (plugin_video_dshow_consumer_t*)self;

	if(!consumer || !codec && codec->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	TMEDIA_CONSUMER(consumer)->video.fps = TMEDIA_CODEC_VIDEO(codec)->in.fps;
	TMEDIA_CONSUMER(consumer)->video.in.width = TMEDIA_CODEC_VIDEO(codec)->in.width;
	TMEDIA_CONSUMER(consumer)->video.in.height = TMEDIA_CODEC_VIDEO(codec)->in.height;

	if(!TMEDIA_CONSUMER(consumer)->video.display.width){
		TMEDIA_CONSUMER(consumer)->video.display.width = TMEDIA_CONSUMER(consumer)->video.in.width;
	}
	if(!TMEDIA_CONSUMER(consumer)->video.display.height){
		TMEDIA_CONSUMER(consumer)->video.display.height = TMEDIA_CONSUMER(consumer)->video.in.height;
	}

	return 0;
}

static int plugin_video_dshow_consumer_start(tmedia_consumer_t* self)
{
	plugin_video_dshow_consumer_t* consumer = (plugin_video_dshow_consumer_t*)self;

	if(!consumer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(consumer->started){
		return 0;
	}

	// create display on UI thread
	if(!consumer->display){
		if(consumer->create_on_ui_thread) createOnUIThead(reinterpret_cast<HWND>((void*)consumer->window), (void**)&consumer->display, true);
		else createOnCurrentThead(reinterpret_cast<HWND>((void*)consumer->window), (void**)&consumer->display, true);
		
		if(!consumer->display){
			TSK_DEBUG_ERROR("Failed to create display");
			return -2;
		}
	}
	
	// Set parameters
	consumer->display->setPluginFirefox((consumer->plugin_firefox == tsk_true));
	consumer->display->setFps(TMEDIA_CONSUMER(consumer)->video.fps);
	// do not change the display size: see hook()
	// consumer->display->setSize(TMEDIA_CONSUMER(consumer)->video.display.width, TMEDIA_CONSUMER(consumer)->video.display.height);
	if(consumer->window){
		consumer->display->attach(consumer->window);
	}
	
	// Start display
	consumer->display->start();
	consumer->started = tsk_true;

	return 0;
}

static int plugin_video_dshow_consumer_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
	plugin_video_dshow_consumer_t* consumer = (plugin_video_dshow_consumer_t*)self;
	if(consumer && consumer->display && buffer){
		consumer->display->handleVideoFrame(buffer, TMEDIA_CONSUMER(consumer)->video.display.width, TMEDIA_CONSUMER(consumer)->video.display.height);
		return 0;
	}
	else{
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
}

static int plugin_video_dshow_consumer_pause(tmedia_consumer_t* self)
{
	plugin_video_dshow_consumer_t* consumer = (plugin_video_dshow_consumer_t*)self;

	if(!consumer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!consumer->display){
		TSK_DEBUG_ERROR("Invalid internal grabber");
		return -2;
	}

	//consumer->display->pause();

	return 0;
}

static int plugin_video_dshow_consumer_stop(tmedia_consumer_t* self)
{
	plugin_video_dshow_consumer_t* consumer = (plugin_video_dshow_consumer_t*)self;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!consumer->started){
		return 0;
	}

	if(!consumer->display){
		TSK_DEBUG_ERROR("Invalid internal display");
		return -2;
	}

	TSK_DEBUG_INFO("Before stopping DirectShow consumer");
	consumer->display->stop();
	consumer->started = tsk_false;
	TSK_DEBUG_INFO("After stopping DirectShow consumer");

	return 0;
}


//
//	DirectShow consumer object definition
//
/* constructor */
static tsk_object_t* plugin_video_dshow_consumer_ctor(tsk_object_t * self, va_list * app)
{
	CoInitialize(NULL);

	plugin_video_dshow_consumer_t *consumer = (plugin_video_dshow_consumer_t *)self;
	if(consumer){
		/* init base */
		tmedia_consumer_init(TMEDIA_CONSUMER(consumer));
		TMEDIA_CONSUMER(consumer)->video.display.chroma = tmedia_chroma_bgr24; // RGB24 on x86 (little endians) stored as BGR24

		/* init self */
		consumer->create_on_ui_thread = tsk_true;
		TMEDIA_CONSUMER(consumer)->video.fps = 15;
		TMEDIA_CONSUMER(consumer)->video.display.width = 352;
		TMEDIA_CONSUMER(consumer)->video.display.height = 288;
		TMEDIA_CONSUMER(consumer)->video.display.auto_resize = tsk_true;
	}
	return self;
}
/* destructor */
static tsk_object_t* plugin_video_dshow_consumer_dtor(tsk_object_t * self)
{ 
	plugin_video_dshow_consumer_t *consumer = (plugin_video_dshow_consumer_t *)self;
	if(consumer){

		/* stop */
		if(consumer->started){
			plugin_video_dshow_consumer_stop((tmedia_consumer_t*)self);
		}

		/* deinit base */
		tmedia_consumer_deinit(TMEDIA_CONSUMER(consumer));
		/* deinit self */
		SAFE_DELETE_PTR(consumer->display);
		
	}

	return self;
}
/* object definition */
static const tsk_object_def_t plugin_video_dshow_consumer_def_s = 
{
	sizeof(plugin_video_dshow_consumer_t),
	plugin_video_dshow_consumer_ctor, 
	plugin_video_dshow_consumer_dtor,
	tsk_null, 
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t plugin_video_dshow_consumer_plugin_def_s = 
{
	&plugin_video_dshow_consumer_def_s,
	
	tmedia_video,
	"Microsoft DirectShow consumer",
	
	plugin_video_dshow_consumer_set,
	plugin_video_dshow_consumer_prepare,
	plugin_video_dshow_consumer_start,
	plugin_video_dshow_consumer_consume,
	plugin_video_dshow_consumer_pause,
	plugin_video_dshow_consumer_stop
};
const tmedia_consumer_plugin_def_t *plugin_video_dshow_consumer_plugin_def_t = &plugin_video_dshow_consumer_plugin_def_s;
