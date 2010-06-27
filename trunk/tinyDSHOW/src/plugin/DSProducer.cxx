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
#include "tinydshow/plugin/DSProducer.h"

#include <tinydshow/DSGrabber.h>
#include <tinydshow/DSUtils.h>

#include "tsk_debug.h"


#define DSPRODUCER(self)		((tdshow_producer_t*)(self))

typedef struct tdshow_producer_s
{
	TMEDIA_DECLARE_PRODUCER;
	
	DSGrabber* grabber;
	int fps;
	int width;
	int height;

	tsk_bool_t started;
}
tdshow_producer_t;

// Producer callback (From DirectShow Grabber to our plugin)
static int tdshow_plugin_cb(const void* callback_data, const void* buffer, tsk_size_t size)
{
	const tdshow_producer_t* producer = (const tdshow_producer_t*)callback_data;

	if(producer && TMEDIA_PRODUCER(producer)->callback){
		TMEDIA_PRODUCER(producer)->callback(TMEDIA_PRODUCER(producer)->callback_data, buffer, size);
	}

	return 0;
}


/* ============ Media Producer Interface ================= */
int tdshow_producer_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
	VIDEOFORMAT format;

	tdshow_producer_t* producer = (tdshow_producer_t*)self;

	if(!producer || !codec && codec->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	if(!producer->grabber){
		TSK_DEBUG_ERROR("Invalid internal grabber");
		return -2;
	}
	
	/* Set Source device */
	producer->grabber->setCaptureDevice("Null");

	/* Set Capture parameters */
	producer->fps = TMEDIA_CODEC_VIDEO(codec)->fps;
	producer->width = TMEDIA_CODEC_VIDEO(codec)->width;
	producer->height = TMEDIA_CODEC_VIDEO(codec)->height;
	SIZE_TO_VIDEOFORMAT(producer->width, producer->height, format);
	producer->grabber->setCaptureParameters(format, producer->fps);

	return 0;
}

int tdshow_producer_start(tmedia_producer_t* self)
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

	if(producer->started){
		TSK_DEBUG_WARN("Producer already started");
		return 0;
	}

	producer->grabber->start();
	producer->started = tsk_true;

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
		TMEDIA_PRODUCER(producer)->video.chroma = tmedia_rgb24;
		/* init self */
		producer->fps = 15;
		producer->width = 176;
		producer->height = 144;
		producer->grabber = new DSGrabber(&hr);
		if(FAILED(hr)){
			TSK_DEBUG_ERROR("Failed to created DirectShow Grabber");
			SAFE_DELETE_PTR(producer->grabber);
		}
		else{
			producer->grabber->setCallback(tdshow_plugin_cb, producer);
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
	
	tdshow_producer_prepare,
	tdshow_producer_start,
	tdshow_producer_pause,
	tdshow_producer_stop
};
extern const tmedia_producer_plugin_def_t *tdshow_producer_plugin_def_t = &tdshow_producer_plugin_def_s;
