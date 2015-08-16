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

/**@file tdav_codec_g729.h
 * @brief G729a codec.
 * Source from: http://www.itu.int/rec/T-REC-G.729-199611-S!AnnA/en
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 */
#ifndef TINYDAV_CODEC_G729_H
#define TINYDAV_CODEC_G729_H

#include "tinydav_config.h"

#if HAVE_G729

#include "tinymedia/tmedia_codec.h"

#include "g729b/typedef.h"
#include "g729b/ld8a.h"


TDAV_BEGIN_DECLS

/** G.729abb codec */
typedef struct tdav_codec_g729ab_s
{
	TMEDIA_DECLARE_CODEC_AUDIO;

	struct{
		Word16 prm[PRM_SIZE+1];        /* Analysis parameters + frame type      */
		Word16 serial[SERIAL_SIZE];    /* Output bitstream buffer               */

		Word16 frame;                  /* frame counter */

		/* For G.729B */
		Word16 vad_enable;
	} encoder;

	struct{
		Word16  serial[SERIAL_SIZE];          /* Serial stream               */
		Word16  synth_buf[L_FRAME+M], *synth; /* Synthesis                   */
		Word16  parm[PRM_SIZE+2];             /* Synthesis parameters        */
		Word16  Az_dec[MP1*2];                /* Decoded Az for post-filter  */
		Word16  T2[2];                        /* Pitch lag for 2 subframes   */

		/* For G.729B */
		Word16  Vad;
	} decoder;
}
tdav_codec_g729ab_t;

TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_g729ab_plugin_def_t;

TDAV_END_DECLS

#endif /* TINYDAV_CODEC_G729_H */

#endif
