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

/**@file tdav_producer_t140.h
 * @brief Producer for T.140 protocol (RFC 4103)
 */
#ifndef TINYDAV_PRODUCER_T140_H
#define TINYDAV_PRODUCER_T140_H

#include "tinydav_config.h"

#include "tinymedia/tmedia_producer.h"

TDAV_BEGIN_DECLS

#define TDAV_PRODUCER_T140(self) ((tdav_producer_t140_t*)(self))

typedef struct tdav_producer_t140_s
{
	TMEDIA_DECLARE_PRODUCER;
	
	tsk_bool_t started;	
}
tdav_producer_t140_t;

int tdav_producer_send_data(tdav_producer_t140_t* self, enum tmedia_t140_data_type_e data_type, const void* data_ptr, unsigned data_size);

TINYDAV_GEXTERN const tmedia_producer_plugin_def_t *tdav_producer_t140_plugin_def_t;

TDAV_END_DECLS

#endif /* TINYDAV_PRODUCER_T140_H */
