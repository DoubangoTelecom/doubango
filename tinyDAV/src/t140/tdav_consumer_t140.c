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

/**@file tdav_consumer_t140.c
 * @brief Consumer for T140 protocol (RFC 4103)
 */
#include "tinydav/t140/tdav_consumer_t140.h"

#include "tsk_debug.h"

static int tdav_consumer_t140_set(tmedia_consumer_t* self, const tmedia_param_t* param)
{
    return 0;
}

static int tdav_consumer_t140_prepare(tmedia_consumer_t* self, const tmedia_codec_t* param)
{
    return 0;
}

static int tdav_consumer_t140_start(tmedia_consumer_t* self)
{
    return 0;
}

// input data = [type:4bytes][data]
static int tdav_consumer_t140_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
    if(!self || (size < 4)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(TDAV_CONSUMER_T140(self)->cb_ondata.func) {
        return TDAV_CONSUMER_T140(self)->cb_ondata.func(TDAV_CONSUMER_T140(self)->cb_ondata.context,
                (enum tmedia_t140_data_type_e)*((int32_t*)buffer),
                &((uint8_t*)buffer)[4],
                (unsigned int)(size - 4));
    }

    return 0;
}

static int tdav_consumer_t140_pause(tmedia_consumer_t* self)
{
    return 0;
}

static int tdav_consumer_t140_stop(tmedia_consumer_t* self)
{
    return 0;
}

int tdav_consumer_t140_set_ondata_cbfn(tdav_consumer_t140_t* self, const void* context, tmedia_session_t140_ondata_cb_f func)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->cb_ondata.context = context;
    self->cb_ondata.func = func;
    return 0;
}

//
//	T.140 consumer object definition
//
/* constructor */
static tsk_object_t* tdav_consumer_t140_ctor(tsk_object_t * self, va_list * app)
{
    tdav_consumer_t140_t *consumer = self;
    if(consumer) {
        /* init base */

        /* init self */

    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_consumer_t140_dtor(tsk_object_t * self)
{
    tdav_consumer_t140_t *consumer = self;
    if(consumer) {
        /* stop */
        if(consumer->started) {
            tdav_consumer_t140_stop(self);
        }

        /* deinit base */

        /* deinit self */
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_consumer_t140_def_s = {
    sizeof(tdav_consumer_t140_t),
    tdav_consumer_t140_ctor,
    tdav_consumer_t140_dtor,
    tsk_null,
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t tdav_consumer_t140_plugin_def_s = {
    &tdav_consumer_t140_def_s,

    tmedia_t140,
    "T.140 consumer",

    tdav_consumer_t140_set,
    tdav_consumer_t140_prepare,
    tdav_consumer_t140_start,
    tdav_consumer_t140_consume,
    tdav_consumer_t140_pause,
    tdav_consumer_t140_stop
};
const tmedia_consumer_plugin_def_t *tdav_consumer_t140_plugin_def_t = &tdav_consumer_t140_plugin_def_s;