/*
* Copyright (C) 2010-2011 Mamadou Diop.
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

/**@file tsk_debug.h
 * @brief Useful functions for debugging purpose.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#ifndef _TINYSAK_DEBUG_H_
#define _TINYSAK_DEBUG_H_

#include "tinysak_config.h"
#include <stdio.h>

TSK_BEGIN_DECLS

#if !defined(DEBUG_LEVEL)
#	define DEBUG_LEVEL DEBUG_LEVEL_ERROR
#endif

/**@ingroup tsk_debug_group
* @def DEBUG_LEVEL_INFO
* @a INFO level (4). This is the lowest possible level and will turn on all logging.
*/
/**@ingroup tsk_debug_group
* @def DEBUG_LEVEL_WARN
* @a WARN level (3). Warning are error which could change the normal process without blocking the application.
*/
/**@ingroup tsk_debug_group
* @def DEBUG_LEVEL_ERROR
* @a ERROR level (2). This level log error which might change the application behavior.
*/
/**@ingroup tsk_debug_group
* @def DEBUG_LEVEL_FATAL
* @a FATAL level (1). This level log fatal errors which might abort the application.
*/
#define DEBUG_LEVEL_INFO		4
#define DEBUG_LEVEL_WARN		3
#define DEBUG_LEVEL_ERROR		2
#define DEBUG_LEVEL_FATAL		1

#if TSK_HAVE_DEBUG_H
#	include <my_debug.h>
#else
	typedef int (*tsk_debug_f)(const void* arg, const char* fmt, ...);

	/* INFO */
#	if (DEBUG_LEVEL >= DEBUG_LEVEL_INFO)
#		define TSK_DEBUG_INFO(FMT, ...)		\
			if(tsk_debug_get_info_cb()) \
				tsk_debug_get_info_cb()(tsk_debug_get_arg_data(), "*INFO: " FMT "\n", ##__VA_ARGS__); \
			else \
				fprintf(stderr, "*INFO: " FMT "\n", ##__VA_ARGS__);
#	else
#		define TSK_DEBUG_INFO(FMT, ...)		((void)0)
#	endif
	/* WARN */
#	if (DEBUG_LEVEL >= DEBUG_LEVEL_WARN)
#		define TSK_DEBUG_WARN(FMT, ...)		\
			if(tsk_debug_get_warn_cb()) \
				tsk_debug_get_warn_cb()(tsk_debug_get_arg_data(), "**WARN: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__); \
			else \
				fprintf(stderr, "**WARN: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__);
#	else
#		define TSK_DEBUG_WARN(FMT, ...)		((void)0)
#	endif
	/* ERROR */
#	if (DEBUG_LEVEL >= DEBUG_LEVEL_ERROR)
#		define TSK_DEBUG_ERROR(FMT, ...) 		\
			if(tsk_debug_get_error_cb()) \
				tsk_debug_get_error_cb()(tsk_debug_get_arg_data(), "***ERROR: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__); \
			else \
				fprintf(stderr, "***ERROR: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__); 
#	else
#		define TSK_DEBUG_ERROR(FMT, ...)		((void)0)
#	endif
	/* FATAL */
#	if (DEBUG_LEVEL >= DEBUG_LEVEL_FATAL)
#		define TSK_DEBUG_FATAL(FMT, ...)		\
			if(tsk_debug_get_fatal_cb()) \
				tsk_debug_get_fatal_cb()(tsk_debug_get_arg_data(), "****FATAL: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__); \
			else \
				fprintf(stderr, "****FATAL: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__);
#	else
#		define TSK_DEBUG_FATAL(FMT, ...)		((void)0)
#	endif
TINYSAK_API void tsk_debug_set_arg_data(const void*);
TINYSAK_API const void* tsk_debug_get_arg_data();
TINYSAK_API void tsk_debug_set_info_cb(tsk_debug_f );
TINYSAK_API tsk_debug_f tsk_debug_get_info_cb();
TINYSAK_API void tsk_debug_set_warn_cb(tsk_debug_f );
TINYSAK_API tsk_debug_f tsk_debug_get_warn_cb();
TINYSAK_API void tsk_debug_set_error_cb(tsk_debug_f );
TINYSAK_API tsk_debug_f tsk_debug_get_error_cb( );
TINYSAK_API void tsk_debug_set_fatal_cb(tsk_debug_f );
TINYSAK_API tsk_debug_f tsk_debug_get_fatal_cb( );

#endif /* TSK_HAVE_DEBUG_H */


TSK_END_DECLS

#endif /* _TINYSAK_DEBUG_H_ */

