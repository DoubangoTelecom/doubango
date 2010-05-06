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

/**@file tmedia_common.h
 * @brief Common functions and definitions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYMEDIA_COMMON_H
#define TINYMEDIA_COMMON_H

#include "tinymedia_config.h"

TMEDIA_BEGIN_DECLS

/** List of all supported media types */
typedef enum tmedia_type_e
{
	tmed_audio,
	tmed_video,
	tmed_msrp,
	tmed_t38
}
tmedia_type_t;


TMEDIA_END_DECLS

#endif /* TINYMEDIA_COMMON_H */
