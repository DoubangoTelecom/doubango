/*
* Copyright (C) 2014-2015 Mamadou DIOP.
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

/**@file tdav_codec_h264_intel.h
* @brief H.264 codec plugin using Intel Media SDK 2014 R2 for Clients (https://software.intel.com/en-us/media-solutions-portal) v1.1 for encoding/decoding.
*/
#ifndef TINYDAV_CODEC_H264_INTEL_H
#define TINYDAV_CODEC_H264_INTEL_H

#include "tinydav_config.h"

#if HAVE_INTEL_MEDIA_SDK

TDAV_BEGIN_DECLS

extern const struct tmedia_codec_plugin_def_s *tdav_codec_h264_intel_base_plugin_def_t;
extern const struct tmedia_codec_plugin_def_s *tdav_codec_h264_intel_main_plugin_def_t;

TDAV_END_DECLS

#endif /* HAVE_INTEL_MEDIA_SDK */

#endif /* TINYDAV_CODEC_H264_INTEL_H */
