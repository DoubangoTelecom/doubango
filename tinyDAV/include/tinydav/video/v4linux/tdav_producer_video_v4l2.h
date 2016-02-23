/* Copyright (C) 2014 Mamadou DIOP.
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
#ifndef TINYDAV_PRODUCER_VIDEO_V4L2_H
#define TINYDAV_PRODUCER_VIDEO_V4L2_H

#include "tinydav_config.h"

#if HAVE_LINUX_VIDEODEV2_H

#include "tinymedia/tmedia_producer.h"

TDAV_BEGIN_DECLS

TINYDAV_GEXTERN const tmedia_producer_plugin_def_t *tdav_producer_video_v4l2_plugin_def_t;
TINYDAV_GEXTERN const tmedia_producer_plugin_def_t *tdav_producer_screencast_v4l2_plugin_def_t;

TDAV_END_DECLS

#endif /* HAVE_LINUX_VIDEODEV2_H */

#endif /* TINYDAV_PRODUCER_VIDEO_V4L2_H */

