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

/**@file tdav_codec_theora.h
 * @brief Theora codec plugin
 * RTP payloader/depayloader follows draft-barbato-avt-rtp-theora-01.
 * For more information about Theora, http://www.theora.org/doc/Theora.pdf.
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#ifndef TINYDAV_CODEC_THEORA_H
#define TINYDAV_CODEC_THEORA_H

#include "tinydav_config.h"

#if HAVE_FFMPEG

#include "tinymedia/tmedia_codec.h"

TDAV_BEGIN_DECLS

tsk_bool_t tdav_codec_ffmpeg_theora_is_supported();

TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_theora_plugin_def_t;

TDAV_END_DECLS

#endif /* HAVE_FFMPEG */

#endif /* TINYDAV_CODEC_THEORA_H */