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

/**@file tsdp_header.h
 * @brief Defines a SDP header/line (<type>=<value>).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSDP_HEADER_H
#define TINYSDP_HEADER_H

#include "tinysdp_config.h"

#include "tsk_ragel_state.h"
#include "tsk_list.h"

TSDP_BEGIN_DECLS

struct tsdp_header_s;

#define TSDP_HEADER(self)					((tsdp_header_t*)(self))
#define TSDP_HEADER_CONST(self)				((const tsdp_header_t*)(self))
#define TSDP_HEADER_VALUE_TOSTRING_F(self)	((tsdp_header_value_tostring_f)(self))

typedef int (*tsdp_header_value_tostring_f)(const struct tsdp_header_s* header, tsk_buffer_t* output);
typedef struct tsdp_header_s* (*tsdp_header_clone_f)(const struct tsdp_header_s* header);

#define TSDP_HTYPE_V_RANK		0
#define TSDP_HTYPE_O_RANK		1
#define TSDP_HTYPE_S_RANK		2
#define TSDP_HTYPE_I_RANK		3
#define TSDP_HTYPE_U_RANK		4
#define TSDP_HTYPE_E_RANK		5
#define TSDP_HTYPE_P_RANK		6
#define TSDP_HTYPE_C_RANK		7
#define TSDP_HTYPE_B_RANK		8
#define TSDP_HTYPE_Z_RANK		11
#define TSDP_HTYPE_K_RANK		12
#define TSDP_HTYPE_A_RANK		13
#define TSDP_HTYPE_T_RANK		9
#define TSDP_HTYPE_R_RANK		10
#define TSDP_HTYPE_M_RANK		14
//#define TSDP_HTYPE_I_RANK		15
//#define TSDP_HTYPE_C_RANK		16
//#define TSDP_HTYPE_B_RANK		17

#define TSDP_HTYPE_DUMMY_RANK	255

/**
 * @enum	tsdp_header_type_e
 *
 * @brief	List of all supported headers.
**/
typedef enum tsdp_header_type_e
{
	tsdp_htype_A,
	tsdp_htype_B,
	tsdp_htype_C,
	tsdp_htype_Dummy,
	tsdp_htype_E,
	tsdp_htype_I,
	tsdp_htype_K,
	tsdp_htype_M,
	tsdp_htype_O,
	tsdp_htype_P,
	tsdp_htype_R,
	tsdp_htype_S,
	tsdp_htype_T,
	tsdp_htype_U,
	tsdp_htype_V,
	tsdp_htype_Z
}
tsdp_header_type_t;

/*================================
*/
typedef struct tsdp_header_s
{
	TSK_DECLARE_OBJECT;
	tsdp_header_type_t type;
	//! Because all SDP headers shall appear in a fixed order, the rank is used to place each header. 
	// Info: RFC 4566 - 5. SDP Specification.
	uint8_t rank;
	tsdp_header_value_tostring_f tostring;
	tsdp_header_clone_f clone;
}
tsdp_header_t;

#define TSDP_DECLARE_HEADER tsdp_header_t __header__
typedef tsk_list_t tsdp_headers_L_t; /**< List of @ref tsdp_header_t elements. */
/*
================================*/

int tsdp_header_rank_cmp(const tsdp_header_t*, const tsdp_header_t*);
TINYSDP_API tsdp_header_t* tsdp_header_clone(const tsdp_header_t*);
TINYSDP_API char tsdp_header_get_name(tsdp_header_type_t type);
TINYSDP_API char tsdp_header_get_nameex(const tsdp_header_t *self);
TINYSDP_API int tsdp_header_serialize(const tsdp_header_t *self, tsk_buffer_t *output);
TINYSDP_API char* tsdp_header_tostring(const tsdp_header_t *self);

TSDP_END_DECLS

#endif /* TINYSDP_HEADER_H */
