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
#include "tinydshow/plugin/DSProducer.h"

#include <tinydshow/DSGrabber.h>
#include <tinydshow/DSDisplay.h>
#include <tinydshow/DSUtils.h>

#include "tsk_debug.h"


#define DSPRODUCER(self)		((tdshow_producer_t*)(self))

typedef struct tdshow_producer_s
{
	TMEDIA_DECLARE_PRODUCER;
	
	DSGrabber* grabber;
	INT64 previewHwnd;
	
	tsk_bool_t started;
}
tdshow_producer_t;

// Producer callback (From DirectShow Grabber to our plugin)
static int tdshow_plugin_cb(const void* callback_data, const void* buffer, tsk_size_t size)
{
	const tdshow_producer_t* producer = (const tdshow_producer_t*)callback_data;

	if(producer && TMEDIA_PRODUCER(producer)->enc_cb.callback){
		TMEDIA_PRODUCER(producer)->enc_cb.callback(TMEDIA_PRODUCER(producer)->enc_cb.callback_data, buffer, size);
	}

	return 0;
}


/* ============ Media Producer Interface ================= */
int tdshow_producer_set(tmedia_producer_t *self, const tmedia_param_t* param)
{
	int ret = 0;

	if(!self || !param){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(param->value_type == tmedia_pvt_int64){
		if(tsk_striequals(param->key, "local-hwnd")){
			if(DSPRODUCER(self)->grabber && DSPRODUCER(self)->grabber->preview){
				DSPRODUCER(self)->grabber->preview->attach((INT64)*((int64_t*)param->value));
			}
			else{
				DSPRODUCER(self)->previewHwnd = (INT64)*((int64_t*)param->value);
			}
		}
	}

	return ret;
}

int tdshow_producer_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
	tdshow_producer_t* producer = (tdshow_producer_t*)self;

	if(!producer || !codec && codec->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	TMEDIA_PRODUCER(producer)->video.fps = TMEDIA_CODEC_VIDEO(codec)->out.fps;
	// FIXME
	TMEDIA_PRODUCER(producer)->video.width = 320;//TMEDIA_CODEC_VIDEO(codec)->out.width;
	TMEDIA_PRODUCER(producer)->video.height = 240;//TMEDIA_CODEC_VIDEO(codec)->out.height;
	
	return 0;
}

int tdshow_producer_start(tmedia_producer_t* self)
{
	tdshow_producer_t* producer = (tdshow_producer_t*)self;
	HRESULT hr;
		
	if(!producer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(producer->started){
		TSK_DEBUG_WARN("Producer already started");
		return 0;
	}

	if(!producer->grabber){ /* Last chance to greate the graber */
		if(!IsMainThread()){
			TSK_DEBUG_WARN("Creating DirectShow objects outside the MainThread");
		}
		producer->grabber = new DSGrabber(&hr);
		if(FAILED(hr)){
			TSK_DEBUG_ERROR("Failed to created DirectShow Grabber");
			SAFE_DELETE_PTR(producer->grabber);
			return -2;
		}
	}

	//set Source device
	producer->grabber->setCaptureDevice("Null");

	// set parameters
	producer->grabber->setCaptureParameters(TMEDIA_PRODUCER(producer)->video.width, TMEDIA_PRODUCER(producer)->video.height, TMEDIA_PRODUCER(producer)->video.fps);

	// set callback function
	producer->grabber->setCallback(tdshow_plugin_cb, producer);

	// attach preview
	if(producer->grabber->preview){
		if(producer->previewHwnd){
			producer->grabber->preview->attach(producer->previewHwnd);
		}
		producer->grabber->preview->setSize(TMEDIA_PRODUCER(producer)->video.width, TMEDIA_PRODUCER(producer)->video.height);
	}
	
	// start grabber
	TSK_DEBUG_INFO("Before starting DirectShow producer");
	producer->grabber->start();
	producer->started = tsk_true;
	TSK_DEBUG_INFO("After starting DirectShow producer");

	return 0;
}

int tdshow_producer_pause(tmedia_producer_t* self)
{
	tdshow_producer_t* producer = (tdshow_producer_t*)self;

	if(!producer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!producer->grabber){
		TSK_DEBUG_ERROR("Invalid internal grabber");
		return -2;
	}

	//producer->grabber->pause();

	return 0;
}

int tdshow_producer_stop(tmedia_producer_t* self)
{
	tdshow_producer_t* producer = (tdshow_producer_t*)self;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!producer->started){
		TSK_DEBUG_WARN("Producer not started");
		return 0;
	}

	if(!producer->grabber){
		TSK_DEBUG_ERROR("Invalid internal grabber");
		return -2;
	}

	producer->grabber->stop();
	producer->started = tsk_false;

	return 0;
}


//
//	WaveAPI producer object definition
//
/* constructor */
static tsk_object_t* tdshow_producer_ctor(tsk_object_t * self, va_list * app)
{
	CoInitialize(NULL);

	tdshow_producer_t *producer = (tdshow_producer_t *)self;
	if(producer){
		HRESULT hr;

		/* init base */
		tmedia_producer_init(TMEDIA_PRODUCER(producer));
		TMEDIA_PRODUCER(producer)->video.chroma = tmedia_bgr24; // RGB24 on x86 (little endians) stored as BGR24
		/* init self with default values*/
		TMEDIA_PRODUCER(producer)->video.fps = 15;
		TMEDIA_PRODUCER(producer)->video.width = 176;
		TMEDIA_PRODUCER(producer)->video.height = 144;

		if(IsMainThread()){
			producer->grabber = new DSGrabber(&hr);
			if(FAILED(hr)){
				TSK_DEBUG_ERROR("Failed to created DirectShow Grabber");
				SAFE_DELETE_PTR(producer->grabber);
			}
		}
	}
	return self;
}
/* destructor */
static tsk_object_t* tdshow_producer_dtor(tsk_object_t * self)
{ 
	tdshow_producer_t *producer = (tdshow_producer_t *)self;
	if(producer){

		/* stop */
		if(producer->started){
			tdshow_producer_stop((tmedia_producer_t*)self);
		}

		/* for safety */
		if(producer->grabber){
			producer->grabber->setCallback(tsk_null, tsk_null);
		}

		/* deinit base */
		tmedia_producer_deinit(TMEDIA_PRODUCER(producer));
		/* deinit self */
		SAFE_DELETE_PTR(producer->grabber);
		
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdshow_producer_def_s = 
{
	sizeof(tdshow_producer_t),
	tdshow_producer_ctor, 
	tdshow_producer_dtor,
	tsk_null, 
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t tdshow_producer_plugin_def_s = 
{
	&tdshow_producer_def_s,
	
	tmedia_video,
	"Microsoft DirectShow producer",
	
	tdshow_producer_set,
	tdshow_producer_prepare,
	tdshow_producer_start,
	tdshow_producer_pause,
	tdshow_producer_stop
};
extern const tmedia_producer_plugin_def_t *tdshow_producer_plugin_def_t = &tdshow_producer_plugin_def_s;
