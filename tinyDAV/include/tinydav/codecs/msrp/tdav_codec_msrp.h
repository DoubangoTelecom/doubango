/*
* Copyright (C) 2010-2011 Mamadou DIOP.
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

/**@file tdav_codec_msrp.h
 * @brief The Message Session Relay Protocol (MSRP) fake codec.
 * Used for both Message (RFC 4975) and file transfer (RFC 5547).
 */
#ifndef TINYDAV_CODEC_MSRP_H
#define TINYDAV_CODEC_MSRP_H

#include "tinydav_config.h"

#include "tinymedia/tmedia_codec.h"

TDAV_BEGIN_DECLS

/** MSRP codec */
typedef struct tdav_codec_msrp_s
{
	TMEDIA_DECLARE_CODEC_MSRP;
}
tdav_codec_msrp_t;

TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_msrp_plugin_def_t;

TDAV_END_DECLS

#endif /* TINYDAV_CODEC_MSRP_H */
