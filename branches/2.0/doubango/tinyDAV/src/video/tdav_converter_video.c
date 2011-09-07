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
/**@file tdav_converter_video.c
* @brief Video converter.
*
* @author Mamadou Diop <diopmamadou(at)doubango.org>
* @author Alex Vishnev (Added support for rotation)
*
* @date Created: Sat Nov 8 16:54:58 2009 mdiop
*/
#include "tinydav/video/tdav_converter_video.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

// use macro for performance reasons keep (called (15x3) times per seconds)
#define rotate90(srcw, srch, srcdata, dstdata) \
{ \
	register int i,j; \
	register int newx = 0; \
	for (i = 0; i < (int)srcw; i ++ ){ \
		for( j = srch-1; j >=0; j -- ){ \
			dstdata[newx++] = srcdata[j * srcw + i]; \
		} \
	} \
}

static inline enum PixelFormat _tdav_converter_video_get_pixfmt(tmedia_chroma_t chroma)
{
	switch(chroma){
		case tmedia_chroma_rgb24:
			return PIX_FMT_RGB24;
		case tmedia_chroma_bgr24:
			return PIX_FMT_BGR24;
		case tmedia_chroma_rgb32:
			return PIX_FMT_RGB32;
		case tmedia_chroma_rgb565le:
			return PIX_FMT_RGB565LE;
		case tmedia_chroma_rgb565be:
			return PIX_FMT_RGB565BE;
		case tmedia_chroma_nv21:
			return PIX_FMT_NV21;
		case tmedia_chroma_nv12:
			return PIX_FMT_NV12;
		case tmedia_chroma_yuv422p:
			return PIX_FMT_YUV422P;
		case tmedia_chroma_uyvy422:
			return PIX_FMT_UYVY422;
		case tmedia_chroma_yuv420p:
			return PIX_FMT_YUV420P;
		default:
			TSK_DEBUG_ERROR("Invalid chroma %d", (int)chroma);
			return PIX_FMT_NONE;
	}
}


tdav_converter_video_t* tdav_converter_video_create(tsk_size_t srcWidth, tsk_size_t srcHeight, tmedia_chroma_t srcChroma, tsk_size_t dstWidth, tsk_size_t dstHeight, tmedia_chroma_t dstChroma)
{
#if HAVE_FFMPEG || HAVE_SWSSCALE
	tdav_converter_video_t* converter;
	enum PixelFormat srcPixfmt, dstPixfmt;

	TSK_DEBUG_INFO("Creating new Video Converter src=(%dx%d) dst=(%dx%d)", srcWidth, srcHeight, dstWidth, dstHeight);

	if((srcPixfmt = _tdav_converter_video_get_pixfmt(srcChroma)) == PIX_FMT_NONE){
		TSK_DEBUG_ERROR("Invalid source chroma");
		return tsk_null;
	}
	if((dstPixfmt = _tdav_converter_video_get_pixfmt(dstChroma)) == PIX_FMT_NONE){
		TSK_DEBUG_ERROR("Invalid destination chroma");
		return tsk_null;
	}

	if(!(converter = tsk_object_new(tdav_converter_video_def_t))){
		TSK_DEBUG_ERROR("Failed to create Video Converter object");
		return tsk_null;
	}

	// Set values
	converter->srcFormat = srcPixfmt;
	converter->dstFormat = dstPixfmt;
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
#if HAVE_FFMPEG || HAVE_SWSSCALE
	int ret, size;

	if(!self || !buffer || !output){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
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

	size = avpicture_get_size(self->dstFormat, self->dstWidth, self->dstHeight);
	if((int)*output_max_size <size){
		if(!(*output = tsk_realloc(*output, (size + FF_INPUT_BUFFER_PADDING_SIZE)))){
			*output_max_size = 0;
			TSK_DEBUG_ERROR("Failed to allocate buffer");
			return 0;
		}
		*output_max_size = size;
	}

	/* Wrap the source buffer */
	ret = avpicture_fill((AVPicture *)self->srcFrame, (uint8_t*)buffer, self->srcFormat, self->srcWidth, self->srcHeight);
	/* Wrap the destination buffer */
	ret = avpicture_fill((AVPicture *)self->dstFrame, (uint8_t*)*output, self->dstFormat, self->dstWidth, self->dstHeight);

	/* === performs conversion === */
	/* Context */
	if(!self->context){
		self->context = sws_getContext(
			self->srcWidth, self->srcHeight, self->srcFormat,
			self->dstWidth, self->dstHeight, self->dstFormat,
			SWS_FAST_BILINEAR, NULL, NULL, NULL);

		if(!self->context){
			TSK_DEBUG_ERROR("Failed to create context");
			return 0;
		}
	}
	// flip
	if(self->flip){
		tdav_converter_video_flip(self->srcFrame, self->srcHeight);
	}

	// chroma conversion
	ret = sws_scale(self->context, (const uint8_t* const*)self->srcFrame->data, self->srcFrame->linesize, 0, self->srcHeight,
		self->dstFrame->data, self->dstFrame->linesize);
	if(ret < 0){
		TSK_FREE(*output);
		return 0;
	}

	// Rotation
	if(self->rotation && (PIX_FMT_YUV420P == self->dstFormat) && self->rotation==90/*FIXME: For now only 90° rotation is supported */){
		// because we rotated 90 width = original height, height = original width
		int w = self->dstHeight;
		int h = self->dstWidth;

		// allocation rotation frame if not already done
		if(!(self->rot.frame) && !(self->rot.frame = avcodec_alloc_frame())){
			TSK_DEBUG_ERROR("failed to allocate rotation frame");
			TSK_FREE(*output);
			return(0);
		}

		// allocate rotation temporary buffer
		if(!self->rot.buffer){
			int buff_size = avpicture_get_size(self->dstFormat, w, h);
			if (!(self->rot.buffer = (uint8_t *)av_malloc(buff_size))){
				TSK_DEBUG_ERROR("failed to allocate new buffer for the frame");
				TSK_FREE(*output);
				return(0);
			}
		}
		
		//wrap
		avpicture_fill((AVPicture *)self->rot.frame, self->rot.buffer, self->dstFormat, w, h);
		// rotate
		rotate90(self->dstWidth, self->dstHeight, self->dstFrame->data[0], self->rot.frame->data[0]);
		rotate90(self->dstWidth/2, self->dstHeight/2, self->dstFrame->data[1], self->rot.frame->data[1]);
		rotate90(self->dstWidth/2, self->dstHeight/2, self->dstFrame->data[2], self->rot.frame->data[2]);
		
#if 1
		{
			static int y_shift = 1;
			static int x_shift = 1;
			int r_size, r_w, r_h, left_band, top_band;
			int pad = ((int)self->dstWidth-w)>((int)self->dstHeight-h)?((int)self->dstWidth-w):((int)self->dstHeight-h);
			if(pad<0){
				pad=0;
			}
			r_size;
			r_w = w+pad;
			r_h = h+pad;
			left_band = (r_w-self->dstWidth)/2;
			top_band = (r_h-self->dstHeight)/3;
			
			if(!self->rot.context){
				if(!(self->rot.context = sws_getContext(w, h, self->dstFormat, r_w, r_h, self->dstFormat, SWS_FAST_BILINEAR, NULL, NULL, NULL))){
					TSK_DEBUG_ERROR("Failed to create context");
					TSK_FREE(*output);
					return 0;
				}
			}
			
			r_size = avpicture_get_size(self->dstFormat, r_w, r_h);
			if((int)*output_max_size <r_size){
				if(!(*output = tsk_realloc(*output, (r_size + FF_INPUT_BUFFER_PADDING_SIZE)))){
					*output_max_size = 0;
					TSK_DEBUG_ERROR("Failed to allocate buffer");
					return 0;
				}
				*output_max_size = r_size;
			}
			
			// re-wrap
			avpicture_fill((AVPicture *)self->dstFrame, (uint8_t*)*output, self->dstFormat, r_w, r_h);
			
			// pad
			sws_scale(self->rot.context, (const uint8_t* const*)self->rot.frame->data, self->rot.frame->linesize, 
					  0, h, self->dstFrame->data, self->dstFrame->linesize);
			
			// crop
			self->dstFrame->data[0] = self->dstFrame->data[0] + (top_band * self->dstFrame->linesize[0]) + left_band;
			self->dstFrame->data[1] = self->dstFrame->data[1] + ((top_band >> y_shift) * self->dstFrame->linesize[1]) + (left_band >> x_shift);
			self->dstFrame->data[2] = self->dstFrame->data[2] + ((top_band >> y_shift) * self->dstFrame->linesize[2]) + (left_band >> x_shift);
			
			avpicture_layout((const AVPicture*)self->dstFrame, self->dstFormat, self->dstWidth, self->dstHeight, *output, *output_max_size);
		}
		
		
#else
        
		// Context
		if(!self->rot.context){
			if(!(self->rot.context = sws_getContext(w, h,self->dstFormat, h, w, self->dstFormat, SWS_FAST_BILINEAR, NULL, NULL, NULL))){
				TSK_DEBUG_ERROR("Failed to create context");
				TSK_FREE(*output);
				return 0;
			}
		}
		
		// Copy frame
		if((ret = sws_scale(self->rot.context, (const uint8_t* const*)self->rot.frame->data, self->rot.frame->linesize, 
					0, h, self->dstFrame->data, self->dstFrame->linesize)) < 0)
		{
			TSK_DEBUG_ERROR("Failed to copy frame");
			TSK_FREE(*output);
			return 0;
		}
#endif
	}//end of rotation

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
#if HAVE_FFMPEG || HAVE_SWSSCALE
		if(converter->context){
			sws_freeContext(converter->context);
		}
		if(converter->srcFrame){
			av_free(converter->srcFrame);
		}
		if(converter->dstFrame){
			av_free(converter->dstFrame);
		}

		// Rotation
		if(converter->rot.context){
			sws_freeContext(converter->rot.context);
		}
		if(converter->rot.frame){
			av_free(converter->rot.frame);
		}
		if(converter->rot.buffer){
			TSK_FREE(converter->rot.buffer);
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
