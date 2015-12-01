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
/**@file tdav_converter_video.h
 * @brief Video converter.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#ifndef TINYDAV_CONVERTER_VIDEO_H
#define TINYDAV_CONVERTER_VIDEO_H

#include "tinydav_config.h"

#include "tinymedia/tmedia_converter_video.h"

TDAV_BEGIN_DECLS

#if HAVE_LIBYUV
extern const tmedia_converter_video_plugin_def_t *tdav_converter_video_libyuv_plugin_def_t;
#endif /* HAVE_LIBYUV */
#if HAVE_FFMPEG || HAVE_SWSSCALE
extern const tmedia_converter_video_plugin_def_t *tdav_converter_video_ffmpeg_plugin_def_t;
#endif /* if HAVE_FFMPEG || HAVE_SWSSCALE */

TDAV_END_DECLS

#endif /* TINYDAV_CONVERTER_VIDEO_H */
