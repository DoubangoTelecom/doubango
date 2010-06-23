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
/**@file tdav_converter_video.c
 * @brief Video converter.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/video/tdav_converter_video.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

tdav_converter_video_t* tdav_converter_video_create(tsk_size_t width, tsk_size_t height)
{
	tdav_converter_video_t* converter;

	if(!(converter = tsk_object_new(tdav_converter_video_def_t))){
		TSK_DEBUG_ERROR("Failed to create Video Converter object");
		return tsk_null;
	}

	// Set values
	converter->width = width;
	converter->height = height;

	return converter;
}

tsk_size_t tdav_converter_video_2YUV420(tdav_converter_video_t* self, const void* rgb24, void** yuv420)
{
	int ret;
	int size;

	if(!self || !rgb24 || !yuv420){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	/* Context */
	if(!self->ctx2YUV){
		self->ctx2YUV = sws_getContext(
			self->width, self->height, PIX_FMT_BGR24,
			self->width, self->height, PIX_FMT_YUV420P,
			SWS_FAST_BILINEAR, NULL, NULL, NULL);

		if(!self->ctx2YUV){
			TSK_DEBUG_ERROR("Failed to create YUV420P context");
			return 0;
		}
	}

	/* Pictures */
	if(!self->rgb24Frame){
		if(!(self->rgb24Frame = avcodec_alloc_frame())){
			TSK_DEBUG_ERROR("Failed to create RGB24 picture");
			return 0;
		}
	}
	if(!self->yuv420Frame){
		if(!(self->yuv420Frame = avcodec_alloc_frame())){
			TSK_DEBUG_ERROR("Failed to create YUV420 picture");
			return 0;
		}
	}
	
	/* remove old output buffer */
	if(*yuv420){
		TSK_FREE(*yuv420);
	}
	size = avpicture_get_size(PIX_FMT_YUV420P, self->width, self->height);
	if(!(*yuv420 = tsk_calloc(size, sizeof(uint8_t)))){
		TSK_DEBUG_ERROR("Failed to allocate YUV420 buffer");
		return 0;
	}

	/* Wrap RGB24 buffer */
	ret = avpicture_fill((AVPicture *)self->rgb24Frame, (uint8_t*)rgb24, PIX_FMT_RGB24, self->width, self->height);
	/* Wrap YUV420 buffer */
	ret = avpicture_fill((AVPicture *)self->yuv420Frame, (uint8_t*)*yuv420, PIX_FMT_YUV420P, self->width, self->height);

	/* performs conversion */
	ret = sws_scale(self->ctx2YUV, self->rgb24Frame->data, self->rgb24Frame->linesize, 0, 0, 
		self->yuv420Frame->data, self->yuv420Frame->linesize);
	if(ret != 0){
		// delete the allocated buffer
		TSK_FREE(*yuv420);
	}

	return (ret == 0) ? size : 0;
}

tsk_size_t tdav_converter_video_2RGB24(tdav_converter_video_t* self, const void* yuv420, void** rgb24)
{
	if(!self || !rgb24 || !yuv420){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	return 0;
}

//=================================================================================================
//	Video Converter object definition
//
static tsk_object_t* tdav_converter_video_ctor(tsk_object_t * self, va_list * app)
{
	tdav_converter_video_t *converter = self;
	if(converter){
		
	}
	return self;
}

static tsk_object_t* tdav_converter_video_dtor(tsk_object_t * self)
{ 
	tdav_converter_video_t *converter = self;
	if(converter){
		if(converter->ctx2RGB){
			sws_freeContext(converter->ctx2RGB);
		}
		if(converter->ctx2YUV){
			sws_freeContext(converter->ctx2YUV);
		}
		if(converter->rgb24Frame){
			av_free(converter->rgb24Frame);
		}
		if(converter->yuv420Frame){
			av_free(converter->yuv420Frame);
		}
	}

	return self;
}

static const tsk_object_def_t tdav_converter_video_def_s = 
{
	sizeof(tdav_converter_video_t),
	tdav_converter_video_ctor, 
	tdav_converter_video_dtor,
	tsk_null, 
};
const tsk_object_def_t *tdav_converter_video_def_t = &tdav_converter_video_def_s;
