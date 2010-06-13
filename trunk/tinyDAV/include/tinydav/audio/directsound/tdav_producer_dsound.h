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

/**@file tdav_producer_dsound.h
 * @brief Microsoft DirectSound producer.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYDAV_PRODUCER_DSOUND_H
#define TINYDAV_PRODUCER_DSOUND_H

#include "tinydav_config.h"

#if HAVE_DSOUND_H

#include "tinydav/audio/tdav_producer_audio.h"

#include <dsound.h>

TDAV_BEGIN_DECLS

#define TDAV_DSOUNS_PRODUCER_NOTIF_POS_COUNT		4

typedef struct tdav_producer_dsound_s
{
	TDAV_DECLARE_PRODUCER_AUDIO;

	tsk_bool_t started;
	tsk_size_t bytes_per_notif;
	void* tid[1];

	LPDIRECTSOUNDCAPTURE device;
	LPDIRECTSOUNDCAPTUREBUFFER captureBuffer;
	HANDLE notifEvents[TDAV_DSOUNS_PRODUCER_NOTIF_POS_COUNT];
}
tdav_producer_dsound_t;

TINYDAV_GEXTERN const tmedia_producer_plugin_def_t *tdav_producer_dsound_plugin_def_t;


TDAV_END_DECLS

#endif /* HAVE_DSOUND_H */

#endif /* TINYDAV_PRODUCER_DSOUND_H */
