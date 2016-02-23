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

/**@file tcomp_compressor.h
 * @brief  Compression function definition.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#ifndef TCOMP_COMPRESSOR_H
#define TCOMP_COMPRESSOR_H

#include "tinysigcomp_config.h"
#include "tcomp_compartment.h"

TCOMP_BEGIN_DECLS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @typedef	int (*tcomp_compressor_compress_f)(tcomp_compartment_t *lpCompartment,
/// 			const void *input_ptr, tsk_size_t input_size, void *output_ptr, tsk_size_t *output_size,
/// 			int stream)
///
/// @brief	Function pointer definition for compression method.
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef tsk_bool_t (*tcomp_compressor_compress_f)(tcomp_compartment_t *lpCompartment, const void *input_ptr, tsk_size_t input_size, void *output_ptr, tsk_size_t *output_size, tsk_bool_t stream);

#define TCOMP_COMPRESSOR_COMPRESS_F(self) ((tcomp_compressor_compress_f)(self))

TCOMP_END_DECLS

#endif /* TCOMP_COMPRESSOR_H */
