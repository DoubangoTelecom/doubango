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

/**@file tdav_producer_dsound.h
 * @brief Microsoft DirectSound producer.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
  */
#ifndef TINYDAV_PRODUCER_DSOUND_H
#define TINYDAV_PRODUCER_DSOUND_H

#include "tinydav_config.h"

#if HAVE_DSOUND_H || HAVE_DSOUND

#include "tinydav/audio/tdav_producer_audio.h"

TDAV_BEGIN_DECLS

TINYDAV_GEXTERN const tmedia_producer_plugin_def_t *tdav_producer_dsound_plugin_def_t;

TDAV_END_DECLS

#endif /* HAVE_DSOUND_H || HAVE_DSOUND */

#endif /* TINYDAV_PRODUCER_DSOUND_H */
