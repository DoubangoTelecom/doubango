/*
* Copyright (C) 2009 Mamadou Diop.
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
/**@file tdav.h
 * @brief tinyDAV.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/tdav.h"

#include "tinydav/audio/tdav_session_audio.h"

#include "tinydav/codecs/g711/tdav_codec_g711.h"

int tdav_init()
{
	/* === Register sessions === */
	tmedia_session_plugin_register(tdav_session_audio_plugin_def_t);

	/* === Register codecs === */
	tmedia_codec_plugin_register(tdav_codec_g711a_plugin_def_t);
	tmedia_codec_plugin_register(tdav_codec_g711u_plugin_def_t);

	return 0;
}

int tdav_deinit()
{
	/* === UnRegister sessions === */
	tmedia_session_plugin_unregister(tdav_session_audio_plugin_def_t);

	/* === UnRegister codecs === */
	tmedia_codec_plugin_unregister(tdav_codec_g711a_plugin_def_t);
	tmedia_codec_plugin_unregister(tdav_codec_g711u_plugin_def_t);

	return 0;
}