/*Copyright (C) 2013 Mamadou Diop
* Copyright (C) 2013 Doubango Telecom <http://www.doubango.org>
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
/**@file tdav_producer_wasapi.h
 * @brief Microsoft Windows Audio Session API (WASAPI) producer.
 *
 */
#ifndef TINYDAV_PRODUCER_WASAPI_H
#define TINYDAV_PRODUCER_WASAPI_H

#include "tinydav_config.h"

#if HAVE_WASAPI

#include "tinydav/audio/tdav_producer_audio.h"

TDAV_BEGIN_DECLS

TINYDAV_GEXTERN const struct tmedia_producer_plugin_def_s *tdav_producer_wasapi_plugin_def_t;

TDAV_END_DECLS

#endif /* HAVE_WASAPI */

#endif /* TINYDAV_PRODUCER_WASAPI_H */
