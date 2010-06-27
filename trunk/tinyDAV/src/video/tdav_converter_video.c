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

tdav_converter_video_t* tdav_converter_video_create(tsk_size_t width, tsk_size_t height, tmedia_chroma_t chroma, tsk_bool_t toYUV420)
{
	tdav_converter_video_t* converter;
	enum PixelFormat pixfmt;

	switch(chroma){
		case tmedia_rgb24:
			pixfmt = PIX_FMT_RGB24;
			break;
		case tmedia_rgb565le:
			pixfmt = PIX_FMT_RGB565LE;
			break;
		case tmedia_rgb565be:
			pixfmt = PIX_FMT_RGB565BE;
			break;
		case tmedia_nv21:
			pixfmt = PIX_FMT_NV21;
			break;
		default:
			TSK_DEBUG_ERROR("Invalid chroma");
			return tsk_null;
	}

	if(!(converter = tsk_object_new(tdav_converter_video_def_t))){
		TSK_DEBUG_ERROR("Failed to create Video Converter object");
		return tsk_null;
	}

	// Set values
	converter->toYUV420 = toYUV420;
	converter->pixfmt = pixfmt;
	converter->width = width;
	converter->height = height;

	return converter;
}

tsk_size_t tdav_converter_video_convert(tdav_converter_video_t* self, const void* buffer, void** output)
{
	int ret;
	int size;
	enum PixelFormat srcFormat, dstFormat;

	if(!self || !buffer || !output){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	
	/* Formats */
	srcFormat = self->toYUV420 ? self->pixfmt : PIX_FMT_YUV420P;
	dstFormat = self->toYUV420 ? PIX_FMT_YUV420P : self->pixfmt;

	/* Context */
	if(!self->context){
		self->context = sws_getContext(
			self->width, self->height, (srcFormat == PIX_FMT_RGB24) ? PIX_FMT_BGR24 : srcFormat,
			self->width, self->height, dstFormat,
			SWS_BICUBIC, NULL, NULL, NULL);

		if(!self->context){
			TSK_DEBUG_ERROR("Failed to create context");
			return 0;
		}
	}

	/* Pictures */
	if(!self->srcFrame){
		if(!(self->srcFrame = avcodec_alloc_frame())){
			TSK_DEBUG_ERROR("Failed to create picture");
			return 0;
		}
	}
	if(!self->dstFrame){
		if(!(self->dstFrame = avcodec_alloc_frame())){
			TSK_DEBUG_ERROR("Failed to create picture");
			return 0;
		}
	}
	
	/* remove old output buffer */
	if(*output){
		TSK_FREE(*output);
	}
	size = avpicture_get_size(dstFormat, self->width, self->height);
	if(!(*output = tsk_calloc(size, sizeof(uint8_t)))){
		TSK_DEBUG_ERROR("Failed to allocate buffer");
		return 0;
	}

	/* Wrap the source buffer */
	ret = avpicture_fill((AVPicture *)self->srcFrame, (uint8_t*)buffer, srcFormat, self->width, self->height);
	/* Wrap the destination buffer */
	ret = avpicture_fill((AVPicture *)self->dstFrame, (uint8_t*)*output, dstFormat, self->width, self->height);

	/* performs conversion */
	ret = sws_scale(self->context, self->srcFrame->data, self->srcFrame->linesize, 0, self->height,
		self->dstFrame->data, self->dstFrame->linesize);
	if(ret < 0){
		// delete the allocated buffer
		TSK_FREE(*output);
		return 0;
	}

	return size;
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
		if(converter->context){
			sws_freeContext(converter->context);
		}
		if(converter->srcFrame){
			av_free(converter->srcFrame);
		}
		if(converter->dstFrame){
			av_free(converter->dstFrame);
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
