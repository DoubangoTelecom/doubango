/*
* Copyright (C) 2012 Mamadou Diop.
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

/**@file tdav_codec_t140.h
 * @brief T140 codec implementation (RFC 4103)
 */
#ifndef TINYDAV_CODEC_T140_H
#define TINYDAV_CODEC_T140_H

#include "tinydav_config.h"

#include "tinymedia/tmedia_codec.h"

TDAV_BEGIN_DECLS

typedef struct tdav_codec_t140_s
{
	TMEDIA_DECLARE_CODEC;
}
tdav_codec_t140_t;

TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_t140_plugin_def_t;

TDAV_END_DECLS

#endif /* TINYDAV_CODEC_T140_H */
