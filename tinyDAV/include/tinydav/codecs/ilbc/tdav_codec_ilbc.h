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

/**@file tdav_codec_ilbc.h
 * @brief iLBC codec
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#ifndef TINYDAV_CODEC_ILBC_H
#define TINYDAV_CODEC_ILBC_H

#include "tinydav_config.h"

#if HAVE_ILBC

#include "tinymedia/tmedia_codec.h"

#include <ilbc/iLBC_define.h>
#include <ilbc/iLBC_encode.h>
#include <ilbc/iLBC_decode.h>

TDAV_BEGIN_DECLS

/** iLBC codec */
typedef struct tdav_codec_ilbc_s
{
	TMEDIA_DECLARE_CODEC_AUDIO;

	iLBC_Enc_Inst_t encoder;
	iLBC_Dec_Inst_t decoder;

	float encblock[BLOCKL_MAX];
	float decblock[BLOCKL_MAX];
}
tdav_codec_ilbc_t;

TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_ilbc_plugin_def_t;

TDAV_END_DECLS

#endif /* HAVE_ILBC */

#endif /* TINYDAV_CODEC_ILBC_H */
