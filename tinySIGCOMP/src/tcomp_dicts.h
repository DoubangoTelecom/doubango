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

/**@file tcomp_dicts.h
 * @brief SigComp Dictionaries
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */

#ifndef _TINYSIGCOMP_DICTS_H_
#define _TINYSIGCOMP_DICTS_H_

#include "tinysigcomp_config.h"
#include "tcomp_state.h"

TCOMP_BEGIN_DECLS

extern tcomp_dictionary_t* tcomp_dicts_create_presence_dict();
extern tcomp_dictionary_t* tcomp_dicts_create_sip_dict();

TCOMP_END_DECLS

#endif /* _TINYSIGCOMP_DICTS_H_ */
