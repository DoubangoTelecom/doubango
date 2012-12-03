/*
* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
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

/**@file tmedia_imageattr.h
 * @brief 'image-attr' parser as per RFC 6236
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *
 */
#ifndef TINYMEDIA_imageattr_H
#define TINYMEDIA_imageattr_H

#include "tinymedia_config.h"

#include "tmedia_common.h"

TMEDIA_BEGIN_DECLS

#define TMEDIA_imageattr_ARRAY_MAX_SIZE	16

typedef int32_t xyvalue_t;
typedef double qvalue_t;
typedef double spvalue_t;

typedef struct tmedia_imageattr_srange_xs
{
	unsigned is_range:1;
	union{
		struct{
			spvalue_t start;
			spvalue_t end;
		}range;
		struct{
			spvalue_t values[TMEDIA_imageattr_ARRAY_MAX_SIZE + 1];
			tsk_size_t count;
		}array;
	};
}
tmedia_imageattr_srange_xt;

typedef struct tmedia_imageattr_xyrange_xs
{
	unsigned is_range:1;
	union{
		struct{
			xyvalue_t start;
			xyvalue_t step;
			xyvalue_t end;
		}range;
		struct{
			xyvalue_t values[TMEDIA_imageattr_ARRAY_MAX_SIZE + 1];
			tsk_size_t count;
		}array;
	};
}
tmedia_imageattr_xyrange_xt;

typedef struct tmedia_imageattr_set_xs
{
	tmedia_imageattr_xyrange_xt xrange;
	tmedia_imageattr_xyrange_xt yrange;
	tmedia_imageattr_srange_xt srange;
	struct{
		unsigned is_present:1;
		spvalue_t start;
		spvalue_t end;
	}prange;
	qvalue_t qvalue;
}
tmedia_imageattr_set_xt;

typedef struct tmedia_imageattr_xs
{
	struct{
		tmedia_imageattr_set_xt sets[TMEDIA_imageattr_ARRAY_MAX_SIZE + 1];
		tsk_size_t count;
	}send;
	struct{
		tmedia_imageattr_set_xt sets[TMEDIA_imageattr_ARRAY_MAX_SIZE + 1];
		tsk_size_t count;
	}recv;
}
tmedia_imageattr_xt;

TINYMEDIA_API int tmedia_imageattr_parse(tmedia_imageattr_xt* self, const void* in_data, tsk_size_t in_size);

TMEDIA_END_DECLS

#endif /* TINYMEDIA_imageattr_H */
