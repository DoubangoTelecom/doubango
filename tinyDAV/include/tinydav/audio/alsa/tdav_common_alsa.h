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
#ifndef TINYDAV_COMMON_ALSA_H
#define TINYDAV_COMMON_ALSA_H

#include "tinydav_config.h"

#if HAVE_ALSA_ASOUNDLIB_H

TDAV_BEGIN_DECLS

#include "tsk_thread.h"
#include "tsk_memory.h"
#include "tsk_safeobj.h"
#include "tsk_timer.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#include <alsa/asoundlib.h>

typedef struct tdav_common_alsa_s {
    tsk_bool_t b_initialized;
    tsk_bool_t b_muted;
    tsk_bool_t b_started;
    tsk_bool_t b_prepared;
    tsk_bool_t b_paused;
    tsk_bool_t b_capture;

    tsk_size_t n_buff_size_in_bytes;
    tsk_size_t n_buff_size_in_samples;
    uint8_t* p_buff_ptr;

    int channels;
    int sample_rate;

    snd_pcm_t *p_handle;
    snd_pcm_hw_params_t *p_params;
    char* p_device_name;

    TSK_DECLARE_SAFEOBJ;
}
tdav_common_alsa_t;

int tdav_common_alsa_init(tdav_common_alsa_t* p_self);
int tdav_common_alsa_lock(tdav_common_alsa_t* p_self);
int tdav_common_alsa_unlock(tdav_common_alsa_t* p_self);
int tdav_common_alsa_prepare(tdav_common_alsa_t* p_self, tsk_bool_t is_capture, int ptime, int channels, int sample_rate);
int tdav_common_alsa_unprepare(tdav_common_alsa_t* p_self);
int tdav_common_alsa_start(tdav_common_alsa_t* p_self);
int tdav_common_alsa_stop(tdav_common_alsa_t* p_self);
int tdav_common_alsa_deinit(tdav_common_alsa_t* p_self);

TDAV_END_DECLS

#endif /* HAVE_ALSA_ASOUNDLIB_H */

#endif /* TINYDAV_COMMON_ALSA_H */

