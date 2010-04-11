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

/**@file tsk_common.h
 * Commons.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#ifndef _TINYSAK_COMMON_H_
#define _TINYSAK_COMMON_H_

typedef int tsk_boolean_t;
#define tsk_bool_t tsk_boolean_t

/**@def tsk_true
* True (1).
*/
/**@def tsk_false
* False (0).
*/
#define tsk_true	1
#define tsk_false	0

#define TSK_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#define TSK_MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define TSK_ABS(a)				(((a)< 0) ? -(a) : (a))

typedef int tsk_ssize_t;
typedef unsigned int tsk_size_t;

typedef enum tsk_error_e
{
	tsk_e_success = 0,
	tsk_e_unitialized = -1,
	
	tsk_e_globalfailure = -2,
}
tsk_error_t;;

#define tsk_null	0

#endif /* _TINYSAK_COMMON_H_ */

