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

/**@file tmedia_content_cpim.h
 * @brief Common Presence and Instant Messaging (CPIM): Message Format (RFC 3862)
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *
 */
#ifndef TINYMEDIA_CONTENT_CPIM_H
#define TINYMEDIA_CONTENT_CPIM_H

#include "tinymedia_config.h"

#include "tmedia_content.h"

TMEDIA_BEGIN_DECLS

#define TMEDIA_CONTENT_CPIM_TYPE "message/CPIM"

/** message/CPIM content */
typedef struct tmedia_content_cpim_s
{
	TMEDIA_DECLARE_CONTENT;

	tmedia_content_headers_L_t* m_headers; /**< MIME headers for the overall message */
	tmedia_content_headers_L_t* h_headers; /**< message headers */
	tsk_buffer_t* e; /**< encapsulated MIME object containing the message content */
	tsk_buffer_t* x; /**< MIME security multipart message wrapper */
}
tmedia_content_cpim_t;

#define TMEDIA_CONTENT_CPIM(self) ((tmedia_content_cpim_t*)(self))
#define TMEDIA_CONTENT_IS_CPIM(self) ( (self) && (TMEDIA_CONTENT((self))->plugin==tmedia_content_cpim_plugin_def_t) )

TINYMEDIA_GEXTERN const tmedia_content_plugin_def_t *tmedia_content_cpim_plugin_def_t;

TMEDIA_END_DECLS

#endif /* TINYMEDIA_CONTENT_CPIM_H */
