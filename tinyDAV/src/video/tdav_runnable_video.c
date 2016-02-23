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
/**@file tdav_runnable_video.c
 * @brief Video runnable used by codecs.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinydav/video/tdav_runnable_video.h"

#include "tsk_buffer.h"
#include "tsk_debug.h"

tdav_runnable_video_t* tdav_runnable_video_create(tsk_runnable_func_run run_f, const void* userdata)
{
    tdav_runnable_video_t* runnable;

    if((runnable = tsk_object_new(tdav_runnable_video_def_t))) {
        TSK_RUNNABLE(runnable)->run = run_f;
        runnable->userdata = userdata;
    }
    return runnable;
}

int tdav_runnable_video_start(tdav_runnable_video_t* self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return tsk_runnable_start(TSK_RUNNABLE(self), tsk_buffer_def_t);
}

int tdav_runnable_video_stop(tdav_runnable_video_t* self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return tsk_runnable_stop(TSK_RUNNABLE(self));
}


//=================================================================================================
//	Video Runable object definition
//
static tsk_object_t* tdav_runnable_video_ctor(tsk_object_t * self, va_list * app)
{
    tdav_runnable_video_t *runnable = self;
    if(runnable) {

    }
    return self;
}

static tsk_object_t* tdav_runnable_video_dtor(tsk_object_t * self)
{
    tdav_runnable_video_t *runnable = self;
    if(runnable) {
        tsk_runnable_stop(TSK_RUNNABLE(runnable));
    }

    return self;
}

static const tsk_object_def_t tdav_runnable_video_def_s = {
    sizeof(tdav_runnable_video_t),
    tdav_runnable_video_ctor,
    tdav_runnable_video_dtor,
    tsk_null,
};
const tsk_object_def_t *tdav_runnable_video_def_t = &tdav_runnable_video_def_s;
