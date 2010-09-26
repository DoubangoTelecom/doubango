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

tdav_converter_video_t* tdav_converter_video_create(tsk_size_t srcWidth, tsk_size_t srcHeight, tsk_size_t dstWidth, tsk_size_t dstHeight, tmedia_chroma_t chroma, tsk_bool_t toYUV420)
{
#if HAVE_FFMPEG
	tdav_converter_video_t* converter;
	enum PixelFormat pixfmt;

	TSK_DEBUG_INFO("Creating new Video Converter src=(%dx%d) dst=(%dx%d)", srcWidth, srcHeight, dstWidth, dstHeight);

	switch(chroma){
		case tmedia_rgb24:
			pixfmt = PIX_FMT_RGB24;
			break;
		case tmedia_rgb32:
			pixfmt = PIX_FMT_RGB32;
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
		case tmedia_yuv422p:
			pixfmt = PIX_FMT_YUV422P;
			break;
		case tmedia_uyvy422:
			pixfmt = PIX_FMT_UYVY422;
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
	converter->srcWidth = srcWidth ? srcWidth : dstWidth;
	converter->srcHeight = srcHeight ? srcHeight : dstHeight;
	converter->dstWidth = dstWidth ? dstWidth : srcWidth;
	converter->dstHeight = dstHeight ? dstHeight : srcHeight;

	return converter;
#else
	return tsk_null;
#endif
}

tsk_size_t tdav_converter_video_convert(tdav_converter_video_t* self, const void* buffer, void** output, tsk_size_t* output_max_size)
{
#if HAVE_FFMPEG
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
			self->srcWidth, self->srcHeight, (srcFormat == PIX_FMT_RGB24) ? PIX_FMT_BGR24 : ((srcFormat == PIX_FMT_RGB32) ? PIX_FMT_BGR32 : srcFormat),
			self->dstWidth, self->dstHeight, dstFormat,
			SWS_FAST_BILINEAR/*SWS_BICUBIC*/, NULL, NULL, NULL);

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
	
	size = avpicture_get_size(dstFormat, self->dstWidth, self->dstHeight);
	if((int)*output_max_size <size){
		if(!(*output = tsk_realloc(*output, size))){
			*output_max_size = 0;
			TSK_DEBUG_ERROR("Failed to allocate buffer");
			return 0;
		}
		*output_max_size = size;
	}

	/* Wrap the source buffer */
	ret = avpicture_fill((AVPicture *)self->srcFrame, (uint8_t*)buffer, srcFormat, self->srcWidth, self->srcHeight);
	/* Wrap the destination buffer */
	ret = avpicture_fill((AVPicture *)self->dstFrame, (uint8_t*)*output, dstFormat, self->dstWidth, self->dstHeight);

	/* performs conversion */
	ret = sws_scale(self->context, self->srcFrame->data, self->srcFrame->linesize, 0, self->srcHeight,
		self->dstFrame->data, self->dstFrame->linesize);
	if(ret < 0){
		// delete the allocated buffer
		TSK_FREE(*output);
		return 0;
	}

	return size;
#else
	return 0;
#endif
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
#if HAVE_FFMPEG
		if(converter->context){
			sws_freeContext(converter->context);
		}
		if(converter->srcFrame){
			av_free(converter->srcFrame);
		}
		if(converter->dstFrame){
			av_free(converter->dstFrame);
		}
#endif
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
