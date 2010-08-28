/*
* Copyright (C) 2009-2010 Mamadou Diop.
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

/**@file tdav_codec_g729.h
 * @brief G729a codec.
 * Source from: http://www.itu.int/rec/T-REC-G.729-199611-S!AnnA/en
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYDAV_CODEC_G729_H
#define TINYDAV_CODEC_G729_H

#include "tinydav_config.h"

#if HAVE_G729

#include "tinymedia/tmedia_codec.h"

#include "g729a/TYPEDEF.H"
#include "g729a/BASIC_OP.H"
#include "g729a/LD8A.H"

TDAV_BEGIN_DECLS

/** G.729a codec */
typedef struct tdav_codec_g729a_s
{
	TMEDIA_DECLARE_CODEC_AUDIO;

	struct{
		Word16 prm[PRM_SIZE]; // Analysis parameters
	} encoder;
}
tdav_codec_g729a_t;

TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_g729a_plugin_def_t;

TDAV_END_DECLS

#endif /* TINYDAV_CODEC_G729_H */

#endif HAVE_G729
