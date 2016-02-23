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
#ifndef TINYDAV_CONSUMER_ALSA_H
#define TINYDAV_CONSUMER_ALSA_H

#include "tinydav_config.h"

#if HAVE_ALSA_ASOUNDLIB_H

#include "tinydav/audio/tdav_consumer_audio.h"

TDAV_BEGIN_DECLS

extern const tmedia_consumer_plugin_def_t *tdav_consumer_alsa_plugin_def_t;

TDAV_END_DECLS

#endif /* HAVE_ALSA_ASOUNDLIB_H */

#endif /* TINYDAV_CONSUMER_ALSA_H */

