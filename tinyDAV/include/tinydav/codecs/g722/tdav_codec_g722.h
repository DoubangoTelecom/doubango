/*
* Copyright (C) 2011 Doubango Telecom <http://www.doubango.org>
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

/**@file tdav_codec_g722.h
 * @brief G.722 codec plugins.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 */
#ifndef TINYDAV_CODEC_G722_H
#define TINYDAV_CODEC_G722_H

#include "tinydav_config.h"

#include "tinydav/codecs/g722/g722_enc_dec.h"

#include "tinymedia/tmedia_codec.h"

TDAV_BEGIN_DECLS

TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_g722_plugin_def_t;

TDAV_END_DECLS

#endif /* TINYDAV_CODEC_G722_H */
