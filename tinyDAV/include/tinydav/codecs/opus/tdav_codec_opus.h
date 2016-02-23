/*
* Copyright (C) 2010-2013 Doubango Telecom <http://www.doubango.org>.
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

/**@file tdav_codec_opus.h
 * @brief OPUS audio codec.
 */
#ifndef TINYDAV_CODEC_OPUS_H
#define TINYDAV_CODEC_OPUS_H

#include "tinydav_config.h"

#if HAVE_LIBOPUS

#include "tinymedia/tmedia_codec.h"

TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_opus_plugin_def_t;

#endif /* HAVE_LIBOPUS */

#endif /* TINYDAV_CODEC_OPUS_H */
