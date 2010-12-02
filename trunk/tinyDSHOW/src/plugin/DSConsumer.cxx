/*
* Copyright (C) 2009-2010 Mamadou Diop.
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
#include "tinydshow/plugin/DSConsumer.h"

#include <tinydshow/DSDisplay.h>
#include <tinydshow/DSUtils.h>

#include "tsk_debug.h"


#define DSCONSUMER(self)			((tdshow_consumer_t*)(self))

typedef struct tdshow_consumer_s
{
	TMEDIA_DECLARE_CONSUMER;
	
	DSDisplay* display;
	INT64 window;
	
	tsk_bool_t started;
}
tdshow_consumer_t;



/* ============ Media Producer Interface ================= */
int tdshow_consumer_set(tmedia_consumer_t *self, const tmedia_param_t* param)
{
	int ret = 0;

	if(!self || !param){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(param->value_type == tmedia_pvt_int64){
		if(tsk_striequals(param->key, "remote-hwnd")){
			INT64 hWnd = (INT64)*((int64_t*)param->value);
			if(DSCONSUMER(self)->display){
				//if(hWnd){
					DSCONSUMER(self)->display->attach(hWnd);
				//}
				//else{
				//	DSCONSUMER(self)->display->detach();
				//}
			}
			else{
				DSCONSUMER(self)->window = hWnd;
			}
		}
	}

	return ret;
}


int tdshow_consumer_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
	tdshow_consumer_t* consumer = (tdshow_consumer_t*)self;

	if(!consumer || !codec && codec->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	TMEDIA_CONSUMER(consumer)->video.fps = TMEDIA_CODEC_VIDEO(codec)->fps;
	TMEDIA_CONSUMER(consumer)->video.width = TMEDIA_CODEC_VIDEO(codec)->width;
	TMEDIA_CONSUMER(consumer)->video.height = TMEDIA_CODEC_VIDEO(codec)->height;

	return 0;
}

int tdshow_consumer_start(tmedia_consumer_t* self)
{
	tdshow_consumer_t* consumer = (tdshow_consumer_t*)self;
	HRESULT hr;

	if(!consumer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(consumer->started){
		TSK_DEBUG_WARN("Producer already started");
		return 0;
	}

	if(!consumer->display){ /* Last chance to create the display */
		if(!IsMainThread()){
			TSK_DEBUG_WARN("Creating DirectShow objects outside the MainThread");
		}
		// create display
		consumer->display = new DSDisplay(&hr);
		if(FAILED(hr)){
			TSK_DEBUG_ERROR("Failed to created DirectShow Display");
			SAFE_DELETE_PTR(consumer->display);
			return -2;
		}
	}
	// Set parameters
	consumer->display->setFps(TMEDIA_CONSUMER(consumer)->video.fps);
	consumer->display->setSize(TMEDIA_CONSUMER(consumer)->video.width, TMEDIA_CONSUMER(consumer)->video.height);
	if(consumer->window){
		consumer->display->attach(consumer->window);
	}
	
	// Start display
	TSK_DEBUG_INFO("Before starting DirectShow consumer");
	consumer->display->start();
	consumer->started = tsk_true;
	TSK_DEBUG_INFO("After starting DirectShow consumer");

	return 0;
}

int tdshow_consumer_consume(tmedia_consumer_t* self, void** buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
	tdshow_consumer_t* consumer = (tdshow_consumer_t*)self;
	if(consumer && consumer->display && buffer){
		consumer->display->handleVideoFrame(*buffer, TMEDIA_CONSUMER(consumer)->video.width, TMEDIA_CONSUMER(consumer)->video.height);
		return 0;
	}
	else{
		TSK_DEBUG_ERROR("Invlide parameter");
		return -1;
	}
}

int tdshow_consumer_pause(tmedia_consumer_t* self)
{
	tdshow_consumer_t* consumer = (tdshow_consumer_t*)self;

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

int tdshow_consumer_stop(tmedia_consumer_t* self)
{
	tdshow_consumer_t* consumer = (tdshow_consumer_t*)self;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!consumer->started){
		TSK_DEBUG_WARN("Consumer not started");
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
//	WaveAPI consumer object definition
//
/* constructor */
static tsk_object_t* tdshow_consumer_ctor(tsk_object_t * self, va_list * app)
{
	CoInitialize(NULL);

	tdshow_consumer_t *consumer = (tdshow_consumer_t *)self;
	if(consumer){
		HRESULT hr;

		/* init base */
		tmedia_consumer_init(TMEDIA_CONSUMER(consumer));
		TMEDIA_CONSUMER(consumer)->video.chroma = tmedia_bgr24; // RGB24 on x86 (little endians) stored as BGR24

		/* init self */
		TMEDIA_CONSUMER(consumer)->video.fps = 15;
		TMEDIA_CONSUMER(consumer)->video.width = 352;
		TMEDIA_CONSUMER(consumer)->video.height = 288;

		if(IsMainThread()){
			consumer->display = new DSDisplay(&hr);
			if(FAILED(hr)){
				TSK_DEBUG_ERROR("Failed to created DirectShow Display");
				SAFE_DELETE_PTR(consumer->display);
			}
		}
	}
	return self;
}
/* destructor */
static tsk_object_t* tdshow_consumer_dtor(tsk_object_t * self)
{ 
	tdshow_consumer_t *consumer = (tdshow_consumer_t *)self;
	if(consumer){

		/* stop */
		if(consumer->started){
			tdshow_consumer_stop((tmedia_consumer_t*)self);
		}

		/* deinit base */
		tmedia_consumer_deinit(TMEDIA_CONSUMER(consumer));
		/* deinit self */
		SAFE_DELETE_PTR(consumer->display);
		
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdshow_consumer_def_s = 
{
	sizeof(tdshow_consumer_t),
	tdshow_consumer_ctor, 
	tdshow_consumer_dtor,
	tsk_null, 
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t tdshow_consumer_plugin_def_s = 
{
	&tdshow_consumer_def_s,
	
	tmedia_video,
	"Microsoft DirectShow consumer",
	
	tdshow_consumer_set,
	tdshow_consumer_prepare,
	tdshow_consumer_start,
	tdshow_consumer_consume,
	tdshow_consumer_pause,
	tdshow_consumer_stop
};
extern const tmedia_consumer_plugin_def_t *tdshow_consumer_plugin_def_t = &tdshow_consumer_plugin_def_s;
