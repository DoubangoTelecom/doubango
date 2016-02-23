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

/**@file tcomp_compressor_dummy.h
 * @brief  SigComp dummy compresor. Used if none match. See RFC 4896 subclause 11.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#ifndef TCOMP_COMPRESSOR_DUMMY_H
#define TCOMP_COMPRESSOR_DUMMY_H

#include "tinysigcomp_config.h"
#include "tcomp_compartment.h"

TCOMP_BEGIN_DECLS

tsk_bool_t tcomp_compressor_dummy_compress(tcomp_compartment_t *lpCompartment, const void *input_ptr, tsk_size_t input_size, void *output_ptr, tsk_size_t *output_size, tsk_bool_t stream);

TCOMP_END_DECLS

#endif /* TCOMP_COMPRESSOR_DUMMY_H */

