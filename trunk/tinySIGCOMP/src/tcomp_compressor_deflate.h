/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tcomp_compressor.h
 * @brief  Deflate compressor.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TCOMP_COMPRESSORDEFLATE_H
#define TCOMP_COMPRESSORDEFLATE_H

#include "tinysigcomp_config.h"
#include "tcomp_compartment.h"

int tcomp_compressor_deflate_compress(tcomp_compartment_t *lpCompartment, const void *input_ptr, size_t input_size, void *output_ptr, size_t *output_size, int stream);

#endif /* TCOMP_COMPRESSORDEFLATE_H */