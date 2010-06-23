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
/**@file tdav_converter_video.h
 * @brief Video converter.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYDAV_CONVERTER_VIDEO_H
#define TINYDAV_CONVERTER_VIDEO_H

#include "tinydav_config.h"

#if HAVE_FFMPEG

#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>

#include "tsk_object.h"

TDAV_BEGIN_DECLS

typedef struct tdav_converter_video_s
{
	TSK_DECLARE_OBJECT;

	struct SwsContext *ctx2YUV;
	struct SwsContext *ctx2RGB;

	AVFrame* rgb24Frame;
	AVFrame* yuv420Frame;

	tsk_size_t width;
	tsk_size_t height;
}
tdav_converter_video_t;

tdav_converter_video_t* tdav_converter_video_create(tsk_size_t width, tsk_size_t height);

tsk_size_t tdav_converter_video_2YUV420(tdav_converter_video_t* self, const void* rgb24, void** yuv420);
tsk_size_t tdav_converter_video_2RGB24(tdav_converter_video_t* self, const void* yuv420, void** rgb24);

TINYDAV_GEXTERN const tsk_object_def_t *tdav_converter_video_def_t;

TDAV_END_DECLS


#endif /* #if HAVE_FFMPEG */

#endif /* TINYDAV_CONVERTER_VIDEO_H */
