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

// FIXME: FFmpeg implementation do not support "scale_rotated_frames" option

#if HAVE_LIBYUV

#include <libyuv/libyuv.h>

using namespace libyuv;

typedef struct tdav_converter_video_libyuv_s
{
	TMEDIA_DECLARE_CONVERTER_VIDEO;
	
	enum FourCC srcFormat;
	enum FourCC dstFormat;
	
	tsk_bool_t toI420;
	tsk_bool_t fromI420;

	struct{
		uint8* ptr;
		int size;
	}chroma;
	struct{
		uint8* ptr;
		int size;
	}rotate;
	struct{
		uint8* ptr;
		int size;
	}scale;
}
tdav_converter_video_libyuv_t;

#define TDAV_CONVERTER_VIDEO_LIBYUV(self) ((tdav_converter_video_libyuv_t*)(self))
#define LIBYUV_INPUT_BUFFER_PADDING_SIZE	32

static inline tsk_size_t _tdav_converter_video_libyuv_get_size(tmedia_chroma_t chroma, tsk_size_t w, tsk_size_t h)
{
	switch(chroma){
		case tmedia_chroma_rgb24:
		case tmedia_chroma_bgr24:
			return (w * h * 3);
		case tmedia_chroma_rgb565le:
			return ((w * h) << 1);
		case tmedia_chroma_rgb32:
			return ((w * h) << 2);
		case tmedia_chroma_nv21:
			return ((w * h * 3) >> 1);
		case tmedia_chroma_nv12:
			return ((w * h * 3) >> 1);
		case tmedia_chroma_yuv422p:
			return ((w * h) << 1);
		case tmedia_chroma_uyvy422:
			return ((w * h) << 1);
		case tmedia_chroma_yuv420p:
			return ((w * h * 3) >> 1);
		default:
			TSK_DEBUG_ERROR("Invalid chroma %d", (int)chroma);
			return 0;
	}
}

static inline enum FourCC _tdav_converter_video_libyuv_get_pixfmt(tmedia_chroma_t chroma)
{
	switch(chroma){
		//case tmedia_chroma_rgb24:
		case tmedia_chroma_bgr24:
			return FOURCC_24BG;
		case tmedia_chroma_rgb565le:
			return FOURCC_RGBP;
		case tmedia_chroma_rgb32:
			return FOURCC_ARGB;
		case tmedia_chroma_nv21:
			return FOURCC_NV21;
		case tmedia_chroma_nv12:
			return FOURCC_NV12;
		case tmedia_chroma_yuv422p:
			return FOURCC_I422;
		case tmedia_chroma_uyvy422:
			return FOURCC_YUY2;
		case tmedia_chroma_yuv420p:
			return FOURCC_I420;
		default:
			TSK_DEBUG_ERROR("Invalid chroma %d", (int)chroma);
			return FOURCC_ANY;
	}
}



static int tdav_converter_video_libyuv_init(tmedia_converter_video_t* self, tsk_size_t srcWidth, tsk_size_t srcHeight, tmedia_chroma_t srcChroma, tsk_size_t dstWidth, tsk_size_t dstHeight, tmedia_chroma_t dstChroma)
{
	TSK_DEBUG_INFO("Initializing new LibYUV Video Converter src=(%dx%d@%d) dst=(%dx%d@%d)", srcWidth, srcHeight, srcChroma, dstWidth, dstHeight, dstChroma);

	if((TDAV_CONVERTER_VIDEO_LIBYUV(self)->srcFormat = _tdav_converter_video_libyuv_get_pixfmt(srcChroma)) == FOURCC_ANY){
		TSK_DEBUG_ERROR("Invalid source chroma");
		return -2;
	}
	if((TDAV_CONVERTER_VIDEO_LIBYUV(self)->dstFormat = _tdav_converter_video_libyuv_get_pixfmt(dstChroma)) == FOURCC_ANY){
		TSK_DEBUG_ERROR("Invalid destination chroma");
		return -3;
	}

	TDAV_CONVERTER_VIDEO_LIBYUV(self)->toI420 = (TDAV_CONVERTER_VIDEO_LIBYUV(self)->dstFormat == FOURCC_I420);
	TDAV_CONVERTER_VIDEO_LIBYUV(self)->fromI420 = (TDAV_CONVERTER_VIDEO_LIBYUV(self)->srcFormat == FOURCC_I420);
	if(!TDAV_CONVERTER_VIDEO_LIBYUV(self)->toI420 && !TDAV_CONVERTER_VIDEO_LIBYUV(self)->fromI420)
	{
		TSK_DEBUG_ERROR("LIBYUV only support from/to I420");
		return -1;
	}
	return 0;
}

static tsk_size_t tdav_converter_video_libyuv_process(tmedia_converter_video_t* _self, const void* buffer, void** output, tsk_size_t* output_max_size)
{
	#define RESIZE_BUFFER(buff, curr_size, new_size) \
		if((int)(curr_size) < (new_size)){ \
			if(!((buff) = (uint8*)tsk_realloc((buff), (new_size)))){ \
				(curr_size) = 0; \
				TSK_DEBUG_ERROR("Failed to allocate buffer"); \
				return 0; \
			} \
			(curr_size) = (new_size); \
		}
	static const int crop_x = 0;
	static const int crop_y = 0;

	int ret;
	tdav_converter_video_libyuv_t* self = TDAV_CONVERTER_VIDEO_LIBYUV(_self);
	tsk_bool_t scale = ((_self->dstWidth != _self->srcWidth) || (_self->dstHeight != _self->srcHeight));
	int s, ls, src_y_stride, src_u_stride, src_v_stride, dst_y_stride, dst_u_stride, dst_v_stride;
	int src_w, src_h, dst_w, dst_h;
	uint8 *dst_y, *dst_u, *dst_v, *src_y, *src_u, *src_v;

	RotationMode rotation = kRotate0;

	switch(_self->rotation){
		case 90: rotation = kRotate90; break;
		case 180: rotation = kRotate180; break;
		case 270: rotation = kRotate270; break;
	}
	//rotation = kRotate0;
	
	// not square and rotaion=270/90 -> requires scaling unless disabled
	if((rotation == kRotate90 || rotation == kRotate270) && _self->scale_rotated_frames){
		scale |= (_self->dstWidth != _self->dstHeight) && (rotation == kRotate90 || rotation == kRotate270);
	}
	
	src_w = _self->srcWidth , src_h = _self->srcHeight;
	
	if(self->toI420){
		dst_w = src_w, dst_h = src_h; // because no scaling when converting to I420
        ls = src_w * src_h;
		s = ((ls * 3) >> 1);
		if(scale || rotation != kRotate0){
			RESIZE_BUFFER(self->chroma.ptr, self->chroma.size, s);
			dst_y = self->chroma.ptr;
		}
		else{
			RESIZE_BUFFER((*output), (*output_max_size), s);
			dst_y = (uint8*)*output;
		}
		dst_u = (dst_y + ls);
		dst_v = dst_u + (ls >> 2);
		src_y_stride = dst_y_stride = src_w;
		src_u_stride = src_v_stride = dst_u_stride = dst_v_stride = ((dst_y_stride + 1) >> 1);
		
		// convert to I420 without scaling or rotation
		ret = ConvertToI420(
					(const uint8*)buffer, _tdav_converter_video_libyuv_get_size(_self->srcChroma, src_w, src_h),
					dst_y, dst_y_stride,
					dst_u, dst_u_stride,
					dst_v, dst_v_stride,
					crop_x, crop_y,
					_self->srcWidth, (_self->flip ? (_self->srcHeight * -1) : _self->srcHeight),
					_self->srcWidth, _self->srcHeight,
					kRotate0,
					(uint32) self->srcFormat);
		
		if(ret){
			TSK_DEBUG_ERROR("ConvertToI420 failed with error code = %d", ret);
			return 0;
		}

		// rotate
		if(rotation != kRotate0){
			dst_w = (rotation == kRotate90 || rotation == kRotate270) ? _self->srcHeight : _self->srcWidth;
			dst_h = (rotation == kRotate90 || rotation == kRotate270) ? _self->srcWidth : _self->srcHeight;
			
			src_y = dst_y, src_u = dst_u, src_v = dst_v;
			src_y_stride = src_y_stride, src_u_stride = src_u_stride, src_v_stride = src_v_stride;
			dst_y_stride = dst_w;
			dst_u_stride = dst_v_stride = ((dst_y_stride + 1) >> 1);
			
			if(scale){
				RESIZE_BUFFER(self->rotate.ptr, self->rotate.size, s);
				dst_y = self->rotate.ptr;
			}
			else{// last step
				RESIZE_BUFFER((*output), (*output_max_size), s);
				dst_y = (uint8*)*output;
			}
			
			dst_u = (dst_y + ls);
			dst_v = dst_u + (ls >> 2);
			ret = I420Rotate(
					src_y, src_y_stride,
					src_u, src_u_stride,
					src_v, src_v_stride,
					dst_y, dst_y_stride,
					dst_u, dst_u_stride,
					dst_v, dst_v_stride,
					_self->srcWidth, _self->srcHeight, rotation);
			if(ret){
				TSK_DEBUG_ERROR("I420Rotate failed with error code = %d", ret);
				return 0;
			}
			
			// scale to fit ratio, pad, crop then copy
			if((rotation == kRotate90 || rotation == kRotate270) && _self->scale_rotated_frames){
                int iwidth = _self->srcHeight;
                int iheight = _self->srcWidth;
                
                src_y = dst_y, src_u = dst_u, src_v = dst_v;
                src_w = dst_w, src_h = dst_h;
                src_y_stride = dst_y_stride, src_u_stride = dst_u_stride, src_v_stride = dst_v_stride;
                
                if (_self->dstWidth != _self->dstHeight) {
                    if (iwidth * _self->srcHeight > iheight * _self->srcWidth) {
                        iwidth = (iheight * _self->srcWidth / _self->srcHeight) & ~1;
                        int iwidth_offset = (_self->srcHeight - iwidth) >> 1;
                        src_y += iwidth_offset;
                        src_u += iwidth_offset >> 1;
                        src_v += iwidth_offset >> 1;
                    } else if (iwidth * _self->srcHeight < iheight * _self->srcWidth) {
                        iheight = iwidth * _self->srcHeight / _self->srcWidth;
                        int iheight_offset = (_self->srcWidth - iheight) >> 2;
                        iheight_offset <<= 1;
                        src_y += iheight_offset * src_y_stride;
                        src_u += (iheight_offset >> 1) * src_u_stride;
                        src_v += (iheight_offset >> 1) * src_v_stride;
                    }
                    
                    src_w = iwidth, src_h = iheight;
                    src_y_stride = src_w;
                    src_u_stride = src_v_stride = ((src_y_stride + 1) >> 1);
                    
                    dst_w = _self->dstWidth;
                    dst_h = _self->dstHeight;
                    ls = dst_w * dst_h;
                    s = ((ls * 3) >> 1);
                    RESIZE_BUFFER((*output), (*output_max_size), s);
                    dst_y_stride = dst_w;
                    dst_u_stride = dst_v_stride = ((dst_y_stride + 1) >> 1);
                    uint8* dst_y = (uint8*)*output;
                    uint8* dst_u = (dst_y + ls);
                    uint8* dst_v = dst_u + (ls >> 2);
                    
                    ret = I420Scale(
                                    src_y, src_y_stride,
                                    src_u, src_u_stride,
                                    src_v, src_v_stride,
                                    src_w, src_h,
                                    dst_y, dst_y_stride,
                                    dst_u, dst_u_stride,
                                    dst_v, dst_v_stride,
                                    dst_w, dst_h,
                                    kFilterBox);
                    if(ret){
                        TSK_DEBUG_ERROR("I420Scale failed with error code = %d", ret);
                        return 0;
                    }
                    return s;
                }
            }
        }

		// scale
		if(scale){
			src_w = dst_w, src_h = dst_h;
			dst_w = ((rotation == kRotate90 || rotation == kRotate270) && !_self->scale_rotated_frames) ? _self->dstHeight : _self->dstWidth;
			dst_h = ((rotation == kRotate90 || rotation == kRotate270) && !_self->scale_rotated_frames) ? _self->dstWidth : _self->dstHeight;
			src_y = dst_y, src_u = dst_u, src_v = dst_v;
			src_y_stride = dst_y_stride, src_u_stride = dst_u_stride, src_v_stride = dst_v_stride;
			dst_y_stride = dst_w;
			dst_u_stride = dst_v_stride = ((dst_y_stride + 1) >> 1);
			
            ls = dst_w * dst_h;
			s = ((ls * 3) >> 1);
			RESIZE_BUFFER((*output), (*output_max_size), s);
			dst_y = (uint8*)*output;
			dst_u = (dst_y + ls);
			dst_v = dst_u + (ls >> 2);
			
			ret = I420Scale(
					src_y, src_y_stride,
					src_u, src_u_stride,
					src_v, src_v_stride,
					src_w, src_h,
					dst_y, dst_y_stride,
					dst_u, dst_u_stride,
					dst_v, dst_v_stride,
					dst_w, dst_h,
					kFilterNone);
			if(ret){
				TSK_DEBUG_ERROR("I420Scale failed with error code = %d", ret);
				return 0;
			}
		}

		return ((dst_w * dst_h * 3) >> 1);
	}
	else if(self->fromI420){
		static const int dst_sample_stride = 0;

		dst_w = _self->dstWidth, dst_h = _self->dstHeight;
		src_y = (uint8*)buffer;
		src_u = (src_y + (src_w * src_h));
		src_v = (src_u + ((src_w * src_h) >> 2));
		src_y_stride = src_w;
		src_u_stride = src_v_stride = ((src_y_stride + 1) >> 1);

		if(scale){
            ls = dst_w * dst_h;
			s = ((ls * 3) >> 1);
			RESIZE_BUFFER(self->scale.ptr, self->scale.size, s);
			dst_y = self->scale.ptr;
			dst_u = (dst_y + (dst_w * dst_h));
			dst_v = (dst_u + ((dst_w * dst_h) >> 2));
			dst_y_stride = dst_w;
			dst_u_stride = dst_v_stride = ((dst_y_stride + 1) >> 1);
			
			ret = I420Scale(
					src_y, src_y_stride,
					src_u, src_u_stride,
					src_v, src_v_stride,
					src_w, src_h,
					dst_y, dst_y_stride,
					dst_u, dst_u_stride,
					dst_v, dst_v_stride,
					dst_w, dst_h,
					kFilterNone);
			
			if(ret){
				TSK_DEBUG_ERROR("I420Scale failed with error code = %d", ret);
				return 0;
			}
			
			src_y = dst_y;
			src_u = (dst_y + ls);
			src_v = (dst_u + (ls >> 2));
			src_y_stride = dst_y_stride;
			src_u_stride = src_v_stride = ((src_y_stride + 1) >> 1);
		}
		
		s = _tdav_converter_video_libyuv_get_size(_self->dstChroma, _self->srcWidth, _self->srcHeight);
		RESIZE_BUFFER((*output), (*output_max_size), s);
		
		ret = ConvertFromI420(
					src_y, src_y_stride,
                    src_u, src_u_stride,
                    src_v, src_v_stride,
                    (uint8*)*output, dst_sample_stride,
					_self->dstWidth, (_self->flip ? (_self->dstHeight * -1) : _self->dstHeight),
					(uint32) self->dstFormat);
		if(ret){
			TSK_DEBUG_ERROR("ConvertFromI420 failed with error code = %d", ret);
			return 0;
		}

		return s;
	}
	
	// Must be from/to I420
	TSK_DEBUG_ERROR("Not expected code called");
	return 0;
}

static tsk_object_t* tdav_converter_video_libyuv_ctor(tsk_object_t * self, va_list * app)
{
	tdav_converter_video_libyuv_t *converter = (tdav_converter_video_libyuv_t *)self;
	if(converter){
		
	}
	return self;
}
static tsk_object_t* tdav_converter_video_libyuv_dtor(tsk_object_t * self)
{ 
	tdav_converter_video_libyuv_t *converter = (tdav_converter_video_libyuv_t *)self;
	if(converter){
		TSK_FREE(converter->chroma.ptr);
		TSK_FREE(converter->rotate.ptr);
		TSK_FREE(converter->scale.ptr);
	}

	return self;
}
static const tsk_object_def_t tdav_converter_video_libyuv_def_s = 
{
	sizeof(tdav_converter_video_libyuv_t),
	tdav_converter_video_libyuv_ctor, 
	tdav_converter_video_libyuv_dtor,
	tsk_null, 
};
const tsk_object_def_t *tdav_converter_video_libyuv_def_t = &tdav_converter_video_libyuv_def_s;
static const tmedia_converter_video_plugin_def_t tdav_converter_video_libyuv_plugin_def_s = 
{
	&tdav_converter_video_libyuv_def_s,
	
	tdav_converter_video_libyuv_init,
	tdav_converter_video_libyuv_process
};
const tmedia_converter_video_plugin_def_t *tdav_converter_video_libyuv_plugin_def_t = &tdav_converter_video_libyuv_plugin_def_s;

#endif /* HAVE_LIBYUV */

#if HAVE_FFMPEG || HAVE_SWSSCALE

#ifndef INT64_C
#	define INT64_C(c) (c ## LL)
#	define UINT64_C(c) (c ## ULL)
#endif

#ifdef __cplusplus
extern "C" {
#endif
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#ifdef __cplusplus
}
#endif

typedef struct tdav_converter_video_ffmpeg_s
{
	TMEDIA_DECLARE_CONVERTER_VIDEO;
	
	struct SwsContext *context;

	enum PixelFormat srcFormat;
	enum PixelFormat dstFormat;

	AVFrame* srcFrame;
	AVFrame* dstFrame;

	struct {
		struct SwsContext *context;
		AVFrame* frame;
		uint8_t* buffer;
		tsk_size_t buffer_size;
	} rot;
}
tdav_converter_video_ffmpeg_t;

#define TDAV_CONVERTER_VIDEO_FFMPEG(self)	((tdav_converter_video_ffmpeg_t*)(self))

// use macro for performance reasons keep (called (15x3) times per seconds)
#define _tdav_converter_video_ffmpeg_rotate90(srcw, srch, srcdata, dstdata) \
{ \
	register int i,j; \
	register int newx = 0; \
	for (i = 0; i < (int)(srcw); i ++ ){ \
		for( j = srch-1; j >=0; j -- ){ \
			(dstdata)[newx++] = (srcdata)[j * (srcw) + i]; \
		} \
	} \
}

#define _tdav_converter_video_ffmpeg_flip(frame, height) \
    frame->data[0] += frame->linesize[0] * (height -1); \
    frame->data[1] += frame->linesize[1] * ((height -1)>>1); \
    frame->data[2] += frame->linesize[2] * ((height -1)>>1); \
    \
    frame->linesize[0] *= -1; \
    frame->linesize[1] *= -1; \
    frame->linesize[2] *= -1;


static inline enum PixelFormat _tdav_converter_video_ffmpeg_get_pixfmt(tmedia_chroma_t chroma)
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


static int tdav_converter_video_ffmpeg_init(tmedia_converter_video_t* self, tsk_size_t srcWidth, tsk_size_t srcHeight, tmedia_chroma_t srcChroma, tsk_size_t dstWidth, tsk_size_t dstHeight, tmedia_chroma_t dstChroma)
{
	TSK_DEBUG_INFO("Initializing new FFmpeg Video Converter src=(%dx%d@%d) dst=(%dx%d@%d)", srcWidth, srcHeight, srcChroma, dstWidth, dstHeight, dstChroma);

	if((TDAV_CONVERTER_VIDEO_FFMPEG(self)->srcFormat = _tdav_converter_video_ffmpeg_get_pixfmt(srcChroma)) == PIX_FMT_NONE){
		TSK_DEBUG_ERROR("Invalid source chroma");
		return -2;
	}
	if((TDAV_CONVERTER_VIDEO_FFMPEG(self)->dstFormat = _tdav_converter_video_ffmpeg_get_pixfmt(dstChroma)) == PIX_FMT_NONE){
		TSK_DEBUG_ERROR("Invalid destination chroma");
		return -3;
	}

	return 0;
}

static tsk_size_t tdav_converter_video_ffmpeg_process(tmedia_converter_video_t* _self, const void* buffer, void** output, tsk_size_t* output_max_size)
{
	int ret, size;
	tsk_bool_t _rotate = tsk_false;
	tdav_converter_video_ffmpeg_t* self = TDAV_CONVERTER_VIDEO_FFMPEG(_self);

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

	size = avpicture_get_size(self->dstFormat, _self->dstWidth, _self->dstHeight);
	if((int)*output_max_size <size){
		if(!(*output = tsk_realloc(*output, (size + FF_INPUT_BUFFER_PADDING_SIZE)))){
			*output_max_size = 0;
			TSK_DEBUG_ERROR("Failed to allocate buffer");
			return 0;
		}
		*output_max_size = size;
	}

	/* Wrap the source buffer */
	ret = avpicture_fill((AVPicture *)self->srcFrame, (uint8_t*)buffer, self->srcFormat, _self->srcWidth, _self->srcHeight);
	/* Wrap the destination buffer */
	ret = avpicture_fill((AVPicture *)self->dstFrame, (uint8_t*)*output, self->dstFormat, _self->dstWidth, _self->dstHeight);

	/* === performs conversion === */
	/* Context */
	if(!self->context){
		self->context = sws_getContext(
			_self->srcWidth, _self->srcHeight, self->srcFormat,
			_self->dstWidth, _self->dstHeight, self->dstFormat,
			SWS_FAST_BILINEAR, NULL, NULL, NULL);

		if(!self->context){
			TSK_DEBUG_ERROR("Failed to create context");
			return 0;
		}
	}

	/*FIXME: For now only 90° rotation is supported this is why we always use libyuv on mobile devices */
	_rotate = (PIX_FMT_YUV420P == self->dstFormat) && _self->rotation==90;

	// if no rotation then, flip while scaling othersize do it after rotation
	if(!_rotate && _self->flip){
		_tdav_converter_video_ffmpeg_flip(self->dstFrame, _self->dstHeight);
	}
	
	// chroma conversion and scaling
	ret = sws_scale(self->context, (const uint8_t* const*)self->srcFrame->data, self->srcFrame->linesize, 0, _self->srcHeight,
		self->dstFrame->data, self->dstFrame->linesize);
	if(ret < 0){
		TSK_FREE(*output);
		return 0;
	}

	// Rotation
	if(_rotate){
		// because we rotated 90 width = original height, height = original width
		int w = _self->dstHeight;
		int h = _self->dstWidth;

		// allocation rotation frame if not already done
		if(!(self->rot.frame) && !(self->rot.frame = avcodec_alloc_frame())){
			TSK_DEBUG_ERROR("failed to allocate rotation frame");
			TSK_FREE(*output);
			return(0);
		}

		// allocate rotation temporary buffer
		size = avpicture_get_size(self->dstFormat, w, h);
		if(self->rot.buffer_size != size){
			if (!(self->rot.buffer = (uint8_t *)av_realloc(self->rot.buffer, size))){
				TSK_DEBUG_ERROR("failed to allocate new buffer for the frame");
				self->rot.buffer_size = 0;
				return(0);
			}
			self->rot.buffer_size = size;
		}
		
		//wrap
		avpicture_fill((AVPicture *)self->rot.frame, self->rot.buffer, self->dstFormat, w, h);
		// rotate
		_tdav_converter_video_ffmpeg_rotate90(_self->dstWidth, _self->dstHeight, self->dstFrame->data[0], self->rot.frame->data[0]);
		_tdav_converter_video_ffmpeg_rotate90((_self->dstWidth>>1), (_self->dstHeight>>1), self->dstFrame->data[1], self->rot.frame->data[1]);
		_tdav_converter_video_ffmpeg_rotate90((_self->dstWidth>>1), (_self->dstHeight>>1), self->dstFrame->data[2], self->rot.frame->data[2]);
		// flip
		if(_self->flip){
			_tdav_converter_video_ffmpeg_flip(self->rot.frame, h);
		}
		
		{
			static const int y_shift = 1;
			static const int x_shift = 1;
			int r_size, r_w, r_h, left_band, top_band;
			int pad = ((int)_self->dstWidth-w)>((int)_self->dstHeight-h)?((int)_self->dstWidth-w):((int)_self->dstHeight-h);
			if(pad<0){
				pad=0;
			}
			r_size;
			r_w = w+pad;
			r_h = h+pad;
			left_band = (r_w-_self->dstWidth)/2;
			top_band = (r_h-_self->dstHeight)/3;
			
			if(!self->rot.context){
				if(!(self->rot.context = sws_getContext(w, h, self->dstFormat, r_w, r_h, self->dstFormat, SWS_FAST_BILINEAR, NULL, NULL, NULL))){
					TSK_DEBUG_ERROR("Failed to create context");
					TSK_FREE(*output);
					return 0;
				}
			}
			
			r_size = avpicture_get_size(self->dstFormat, r_w, r_h);
			if((int)*output_max_size < r_size){
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
			
			avpicture_layout((const AVPicture*)self->dstFrame, self->dstFormat, _self->dstWidth, _self->dstHeight, (unsigned char *)*output, *output_max_size);
		}
		
	}//end of rotation

	return size;
}



//=================================================================================================
//	Video Converter object definition
//
static tsk_object_t* tdav_converter_video_ffmpeg_ctor(tsk_object_t * self, va_list * app)
{
	tdav_converter_video_ffmpeg_t *converter = (tdav_converter_video_ffmpeg_t *)self;
	if(converter){
		
	}
	return self;
}

static tsk_object_t* tdav_converter_video_ffmpeg_dtor(tsk_object_t * self)
{ 
	tdav_converter_video_ffmpeg_t *converter = (tdav_converter_video_ffmpeg_t *)self;
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

		// Rotation
		if(converter->rot.context){
			sws_freeContext(converter->rot.context);
		}
		if(converter->rot.frame){
			av_free(converter->rot.frame);
		}
		if(converter->rot.buffer){
			av_free(converter->rot.buffer);
		}
	}

	return self;
}

static const tsk_object_def_t tdav_converter_video_ffmpeg_def_s = 
{
	sizeof(tdav_converter_video_ffmpeg_t),
	tdav_converter_video_ffmpeg_ctor, 
	tdav_converter_video_ffmpeg_dtor,
	tsk_null, 
};
const tsk_object_def_t *tdav_converter_video_ffmpeg_def_t = &tdav_converter_video_ffmpeg_def_s;

/* plugin definition*/
static const tmedia_converter_video_plugin_def_t tdav_converter_video_ffmpeg_plugin_def_s = 
{
	&tdav_converter_video_ffmpeg_def_s,
	
	tdav_converter_video_ffmpeg_init,
	tdav_converter_video_ffmpeg_process
};
const tmedia_converter_video_plugin_def_t *tdav_converter_video_ffmpeg_plugin_def_t = &tdav_converter_video_ffmpeg_plugin_def_s;

#endif /* HAVE_FFMPEG || HAVE_SWSSCALE */

