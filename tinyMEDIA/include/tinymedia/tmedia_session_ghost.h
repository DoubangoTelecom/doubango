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

/**@file tmedia_session_ghost.h
 * @brief Ghost session.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYMEDIA_SESSION_GHOST_H
#define TINYMEDIA_SESSION_GHOST_H

#include "tinymedia_config.h"

#include "tmedia_session.h"

#include "tsk_object.h"

TMEDIA_BEGIN_DECLS

/** Ghost session */
typedef struct tmedia_session_ghost_s {
    TMEDIA_DECLARE_SESSION;
    char* media;
    char* proto;
    char* first_format;
}
tmedia_session_ghost_t;

TINYMEDIA_GEXTERN const tmedia_session_plugin_def_t *tmedia_session_ghost_plugin_def_t;

TMEDIA_END_DECLS

#endif /* TINYMEDIA_SESSION_GHOST_H */
