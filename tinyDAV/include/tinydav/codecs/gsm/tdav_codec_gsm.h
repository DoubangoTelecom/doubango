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

/**@file tdav_codec_gsm.h
 * @brief GSM Full Rate Codec (Based on libgsm)
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#ifndef TINYDAV_CODEC_GSM_H
#define TINYDAV_CODEC_GSM_H

#include "tinydav_config.h"

#if HAVE_LIBGSM

#include "tinymedia/tmedia_codec.h"

#include <gsm.h>

TDAV_BEGIN_DECLS

/** GSM codec */
typedef struct tdav_codec_gsm_s {
    TMEDIA_DECLARE_CODEC_AUDIO;

    gsm encoder;
    gsm decoder;
}
tdav_codec_gsm_t;

TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_gsm_plugin_def_t;

TDAV_END_DECLS

#endif /* HAVE_LIBGSM */

#endif /* TINYDAV_CODEC_GSM_H */
