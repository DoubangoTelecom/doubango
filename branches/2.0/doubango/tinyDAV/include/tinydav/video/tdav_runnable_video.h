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
/**@file tdav_runnable_video.h
 * @brief Video runnable used by codecs.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#ifndef TINYDAV_RUNNABLE_VIDEO_H
#define TINYDAV_RUNNABLE_VIDEO_H

#include "tinydav_config.h"

#include "tsk_runnable.h"

TDAV_BEGIN_DECLS

typedef struct tdav_runnable_video_s
{
	TSK_DECLARE_RUNNABLE;

	const void* userdata;
}
tdav_runnable_video_t;

tdav_runnable_video_t* tdav_runnable_video_create(tsk_runnable_func_run run_f, const void* userdata);
int tdav_runnable_video_start(tdav_runnable_video_t* self);
int tdav_runnable_video_stop(tdav_runnable_video_t* self);

TINYDAV_GEXTERN const tsk_object_def_t *tdav_runnable_video_def_t;

TDAV_END_DECLS

#endif /* TINYDAV_RUNNABLE_VIDEO_H */
