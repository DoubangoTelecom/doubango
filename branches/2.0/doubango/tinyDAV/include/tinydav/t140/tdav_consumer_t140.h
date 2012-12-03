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

/**@file tdav_consumer_t140.h
 * @brief Consumer for T140 protocol (RFC 4103)
 */
#ifndef TINYDAV_CONSUMER_T140_H
#define TINYDAV_CONSUMER_T140_H

#include "tinydav_config.h"

#include "tinymedia/tmedia_consumer.h"
#include "tinymedia/tmedia_session.h"

#include "tsk_safeobj.h"

TDAV_BEGIN_DECLS

#define TDAV_CONSUMER_T140(self)		((tdav_consumer_t140_t*)(self))

typedef struct tdav_consumer_t140_s
{
	TMEDIA_DECLARE_CONSUMER;

	tsk_bool_t started;

	struct{
		const void* context;
		tmedia_session_t140_ondata_cb_f func;
	} cb_ondata;
	
	TSK_DECLARE_SAFEOBJ;
}
tdav_consumer_t140_t;

int tdav_consumer_t140_set_ondata_cbfn(tdav_consumer_t140_t* self, const void* context, tmedia_session_t140_ondata_cb_f func);

TINYDAV_GEXTERN const tmedia_consumer_plugin_def_t *tdav_consumer_t140_plugin_def_t;

#endif /* TINYDAV_CONSUMER_T140_H */
