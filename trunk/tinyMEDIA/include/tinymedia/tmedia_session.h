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

/**@file tmedia_session.h
 * @brief Base session object.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYMEDIA_SESSION_H
#define TINYMEDIA_SESSION_H

#include "tinymedia_config.h"

TMEDIA_BEGIN_DECLS

typedef enum tmedia_session_type_e
{
	
}
tmedia_session_type_t;

/** Base objct used for all media sessions */
typedef struct tmedia_session_s
{
	TSK_DECLARE_OBJECT;
}
tmedia_session_t;

TMEDIA_END_DECLS

#endif /* TINYMEDIA_SESSION_H */
