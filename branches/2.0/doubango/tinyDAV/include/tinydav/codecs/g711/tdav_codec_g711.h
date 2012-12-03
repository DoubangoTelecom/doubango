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

/**@file tdav_codec_g711.h
 * @brief G.711u and G.711a (a.k.a PCMU and PCMA) codec plugins.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 */
#ifndef TINYDAV_CODEC_G711_H
#define TINYDAV_CODEC_G711_H

#include "tinydav_config.h"

#include "tinymedia/tmedia_codec.h"

TDAV_BEGIN_DECLS

/** G.711u codec */
typedef struct tdav_codec_g711u_s
{
	TMEDIA_DECLARE_CODEC_AUDIO;
}
tdav_codec_g711u_t;

/** G.711a codec */
typedef struct tdav_codec_g711a_s
{
	TMEDIA_DECLARE_CODEC_AUDIO;
}
tdav_codec_g711a_t;


TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_g711a_plugin_def_t;
TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_g711u_plugin_def_t;

TDAV_END_DECLS

#endif /* TINYDAV_CODEC_G711_H */
