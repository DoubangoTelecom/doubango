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

/**@file tdav_codec_bv16.h
 * @brief BroadVoice16 codec
 * The payloader/depayloader follow RFC 4298
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#ifndef TINYDAV_CODEC_BV16_H
#define TINYDAV_CODEC_BV16_H

#include "tinydav_config.h"

#if HAVE_BV16

#include "tinymedia/tmedia_codec.h"


TDAV_BEGIN_DECLS

/** BV16 codec */
typedef struct tdav_codec_bv16_s
{
	TMEDIA_DECLARE_CODEC_AUDIO;

	struct {
		void *state;
		void *bs;
		void *x;
	} encoder;

	struct {
		void *state;
		void *bs;
		void *x;
	} decoder;
}
tdav_codec_bv16_t;

TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_bv16_plugin_def_t;

TDAV_END_DECLS

#endif /* HAVE_BV16 */

#endif /* TINYDAV_CODEC_BV16_H */
