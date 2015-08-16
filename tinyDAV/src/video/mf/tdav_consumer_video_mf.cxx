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
/**@file tdav_consumer_video_mf.cxx
 * @brief Microsoft Media Foundation video consumer (source).
 *	http://msdn.microsoft.com/en-us/library/windows/desktop/ms694197(v=vs.85).aspx
 */
#include "tinydav/video/mf/tdav_consumer_video_mf.h"

#if HAVE_MF

#include "tsk_string.h"
#include "tsk_debug.h"

typedef struct tdav_consumer_video_mf_s
{
	TMEDIA_DECLARE_CONSUMER;
}
tdav_consumer_video_mf_t;

/* ============ Media Producer Interface ================= */
int tdav_consumer_video_mf_set(tmedia_consumer_t *self, const tmedia_param_t* param)
{
	int ret = 0;

	if(!self || !param){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	return ret;
}


int tdav_consumer_video_mf_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
	tdav_consumer_video_mf_t* consumer = (tdav_consumer_video_mf_t*)self;

	if(!consumer || !codec || !codec->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
#if 0
	TMEDIA_CONSUMER(consumer)->decoder.codec_id = (tmedia_codec_id_t)(tmedia_codec_id_h264_bp | tmedia_codec_id_h264_mp);
#endif

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

int tdav_consumer_video_mf_start(tmedia_consumer_t* self)
{
	tdav_consumer_video_mf_t* consumer = (tdav_consumer_video_mf_t*)self;

	if(!consumer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	return 0;
}

int tdav_consumer_video_mf_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
	tdav_consumer_video_mf_t* consumer = (tdav_consumer_video_mf_t*)self;
	
	return 0;
}

int tdav_consumer_video_mf_pause(tmedia_consumer_t* self)
{
	tdav_consumer_video_mf_t* consumer = (tdav_consumer_video_mf_t*)self;

	if(!consumer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	return 0;
}

int tdav_consumer_video_mf_stop(tmedia_consumer_t* self)
{
	tdav_consumer_video_mf_t* consumer = (tdav_consumer_video_mf_t*)self;

	TSK_DEBUG_INFO("tdav_consumer_video_mf_stop");

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	return 0;
}


//
//	Windows Media Foundation video consumer object definition
//
/* constructor */
static tsk_object_t* tdav_consumer_video_mf_ctor(tsk_object_t * self, va_list * app)
{
	tdav_consumer_video_mf_t *consumer = (tdav_consumer_video_mf_t *)self;
	if(consumer){
		/* init base */
		tmedia_consumer_init(TMEDIA_CONSUMER(consumer));
		TMEDIA_CONSUMER(consumer)->video.display.chroma = tmedia_chroma_yuv420p; // To avoid chroma conversion

		/* init self */
		TMEDIA_CONSUMER(consumer)->video.fps = 15;
		TMEDIA_CONSUMER(consumer)->video.display.width = 352;
		TMEDIA_CONSUMER(consumer)->video.display.height = 288;
		TMEDIA_CONSUMER(consumer)->video.display.auto_resize = tsk_true;
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_consumer_video_mf_dtor(tsk_object_t * self)
{ 
	tdav_consumer_video_mf_t *consumer = (tdav_consumer_video_mf_t *)self;
	if(consumer){

		/* stop */
		//if(consumer->started){
			tdav_consumer_video_mf_stop((tmedia_consumer_t*)self);
		//}

		/* deinit base */
		tmedia_consumer_deinit(TMEDIA_CONSUMER(consumer));
		/* deinit self */
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_consumer_video_mf_def_s = 
{
	sizeof(tdav_consumer_video_mf_t),
	tdav_consumer_video_mf_ctor, 
	tdav_consumer_video_mf_dtor,
	tsk_null, 
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t tdav_consumer_video_mf_plugin_def_s = 
{
	&tdav_consumer_video_mf_def_s,
	
	tmedia_video,
	"Microsoft Windows Media Foundation consumer (Video)",
	
	tdav_consumer_video_mf_set,
	tdav_consumer_video_mf_prepare,
	tdav_consumer_video_mf_start,
	tdav_consumer_video_mf_consume,
	tdav_consumer_video_mf_pause,
	tdav_consumer_video_mf_stop
};
const tmedia_consumer_plugin_def_t *tdav_consumer_video_mf_plugin_def_t = &tdav_consumer_video_mf_plugin_def_s;

#endif /*HAVE_MF  */
