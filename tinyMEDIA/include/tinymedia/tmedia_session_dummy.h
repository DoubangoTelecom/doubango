/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tmedia_session_dummy.h
 * @brief Dummy sessions used for test only.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYMEDIA_SESSION_DUMMY_H
#define TINYMEDIA_SESSION_DUMMY_H

#include "tinymedia_config.h"

#include "tmedia_session.h"

#include "tsk_object.h"

TMEDIA_BEGIN_DECLS

/** Dummy Audio session */
typedef struct tmedia_session_daudio_s
{
	TMEDIA_DECLARE_SESSION_AUDIO;
	uint16_t local_port;
	uint16_t remote_port;
}
tmedia_session_daudio_t;

/** Dummy Video session */
typedef struct tmedia_session_dvideo_s
{
	TMEDIA_DECLARE_SESSION_VIDEO;
	uint16_t local_port;
	uint16_t remote_port;
}
tmedia_session_dvideo_t;

/** Dummy Msrp session */
typedef struct tmedia_session_dmsrp_s
{
	TMEDIA_DECLARE_SESSION_MSRP;
	uint16_t local_port;
	uint16_t remote_port;
}
tmedia_session_dmsrp_t;


TINYMEDIA_GEXTERN const tmedia_session_plugin_def_t *tmedia_session_daudio_plugin_def_t;
TINYMEDIA_GEXTERN const tmedia_session_plugin_def_t *tmedia_session_dvideo_plugin_def_t;
TINYMEDIA_GEXTERN const tmedia_session_plugin_def_t *tmedia_session_dmsrp_plugin_def_t;

TMEDIA_END_DECLS

#endif /* TINYMEDIA_SESSION_DUMMY_H */

