/*
* Copyright (C) 2009 Mamadou Diop.
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

/**@file tdav_consumer_dsound.h
 * @brief Microsoft DirectSound consumer.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYDAV_CONSUMER_DSOUND_H
#define TINYDAV_CONSUMER_DSOUND_H

#include "tinydav_config.h"

#include "tinydav/audio/tdav_consumer_audio.h"

#define TDAV_DSOUNS_CONSUMER_NOTIF_POS_COUNT		4

typedef struct tdav_consumer_dsound_s
{
	TDAV_DECLARE_CONSUMER_AUDIO;

	tsk_bool_t started;

}
tdav_consumer_dsound_t;

TINYDAV_GEXTERN const tmedia_consumer_plugin_def_t *tmedia_consumer_dsound_plugin_def_t;

#endif /* TINYDAV_CONSUMER_DSOUND_H */
