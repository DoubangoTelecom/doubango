/*
* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
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

/**@file tdav_session_t140.h
 * @brief Session for T140 protocol (RFC 4103)
 */
#ifndef TINYDAV_SESSION_T140_H
#define TINYDAV_SESSION_T140_H

#include "tinydav_config.h"

#include "tinydav/tdav_session_av.h"
#include "tsk_timer.h"

TDAV_BEGIN_DECLS

#define TDAV_SESSION_T140(self) ((tdav_session_t140_t*)(self))

typedef tsk_list_t tdav_session_t140_blocks_L_t;

typedef struct tdav_session_t140_s {
    TDAV_DECLARE_SESSION_AV;

    tsk_bool_t started;
    tsk_timer_manager_handle_t *h_timer;

    struct {
        const void* context;
        tmedia_session_t140_ondata_cb_f func;
    } cb_ondata;

    struct {
        uint32_t payload_type;
        struct tmedia_codec_s* codec;

        void* buffer;
        tsk_size_t buffer_size;
        tsk_size_t buffer_idx;

        void* red_buffer;
        tsk_size_t red_buffer_size;

        tdav_session_t140_blocks_L_t* blocks;
        int64_t blocks_count;
        struct {
            tsk_timer_id_t id;
            int64_t timeout;
        } timer_buffering;
        struct {
            tsk_timer_id_t id;
            int64_t timeout;
        } timer_idle;
    } encoder;

    struct {
        uint32_t payload_type;
        struct tmedia_codec_s* codec;

        void* buffer;
        tsk_size_t buffer_size;

        void* consumer_buffer;
        tsk_size_t consumer_buffer_size;

        tdav_session_t140_blocks_L_t* blocks;
        int64_t blocks_count;
        int32_t last_seq_num;
        struct {
            tsk_timer_id_t id;
            int64_t timeout;
        } timer_pkt_loss;
    } decoder;
}
tdav_session_t140_t;

TINYDAV_GEXTERN const tmedia_session_plugin_def_t *tdav_session_t140_plugin_def_t;

TDAV_END_DECLS

#endif /* TINYDAV_SESSION_T140_H */
