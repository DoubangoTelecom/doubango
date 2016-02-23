/* Copyright (C) 2010-2013 Mamadou Diop.
* Copyright (C) 2013 Doubango Telecom <http://doubango.org>
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
* @def DEBUG_LEVEL
* Defines the defaul debug level. Sould be: @ref DEBUG_LEVEL_INFO, @ref DEBUG_LEVEL_WARN, @ref DEBUG_LEVEL_ERROR or @ref DEBUG_LEVEL_FATAL. <br />
* You can set this value at build time using your CFLAGS (e.g. <i>CFLAGS=$CFLAGS -DDEBUG_LEVEL=DEBUG_LEVEL_ERROR</i>). <br />
* At runtime, this value could be changed using @ref tsk_debug_set_level().
*/
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
* @a ERROR level (2). This level logs error which might change the application behavior.
*/
/**@ingroup tsk_debug_group
* @def DEBUG_LEVEL_FATAL
* @a FATAL level (1). This level logs fatal errors which might abort the application.
*/
/**@ingroup tsk_debug_group
* @def TSK_DEBUG_INFO
* Writes the C string pointed by @a FMT to <b>stderr</b> output if debug level is >= 4 (@ref DEBUG_LEVEL_INFO) and no callack function is defined using @ref tsk_debug_set_info_cb. <br />
* <i>Example</i>:
* @code
* TSK_DEBUG_INFO("Project name = %s, version = %f", "Doubango", 3.0);
* @endcode
* @param FMT C string that contains the text to be written to <b>stderr</b>. The string accept same specifiers as <a target=_blank href="http://www.cplusplus.com/reference/cstdio/printf/">printf</a>.
* @sa @ref TSK_DEBUG_WARN @ref TSK_DEBUG_ERROR @ref TSK_DEBUG_FATAL <br />
*/
/**@ingroup tsk_debug_group
* @def TSK_DEBUG_WARN
* Writes the C string pointed by @a FMT to <b>stderr</b> output if debug level is >= 4 (@ref DEBUG_LEVEL_WARN) and no callack function is defined using @ref tsk_debug_set_warn_cb. <br />
* <i>Example</i>:
* @code
* TSK_DEBUG_WARN("Warning code = %d, descripting = %s", 404, "Not found");
* @endcode
* @param FMT C string that contains the text to be written to <b>stderr</b>. The string accept same specifiers as <a target=_blank href="http://www.cplusplus.com/reference/cstdio/printf/">printf</a>.
* @sa @ref TSK_DEBUG_INFO @ref TSK_DEBUG_ERROR @ref TSK_DEBUG_FATAL <br />
*/
/**@ingroup tsk_debug_group
* @def TSK_DEBUG_ERROR
* Writes the C string pointed by @a FMT to <b>stderr</b> output if debug level is >= 4 (@ref DEBUG_LEVEL_ERROR) and no callack function is defined using @ref tsk_debug_set_error_cb. <br />
* <i>Example</i>:
* @code
* TSK_DEBUG_ERROR("Warning code = %d, descripting = %s", 400, "Bad request");
* @endcode
* @param FMT C string that contains the text to be written to <b>stderr</b>. The string accept same specifiers as <a target=_blank href="http://www.cplusplus.com/reference/cstdio/printf/">printf</a>.
* @sa @ref TSK_DEBUG_INFO @ref TSK_DEBUG_WARN @ref TSK_DEBUG_FATAL <br />
*/
/**@ingroup tsk_debug_group
* @def TSK_DEBUG_FATAL
* Writes the C string pointed by @a FMT to <b>stderr</b> output if debug level is >= 4 (@ref DEBUG_LEVEL_FATAL) and no callack function is defined using @ref tsk_debug_set_fatal_cb. <br />
* <i>Example</i>:
* @code
* TSK_DEBUG_ERROR("Warning code = %d, descripting = %s", 403, "Forbidden");
* @endcode
* @param FMT C string that contains the text to be written to <b>stderr</b>. The string accept same specifiers as <a target=_blank href="http://www.cplusplus.com/reference/cstdio/printf/">printf</a>.
* @sa @ref TSK_DEBUG_INFO @ref TSK_DEBUG_WARN @ref TSK_DEBUG_ERROR <br />
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
#define TSK_DEBUG_INFO(FMT, ...)		\
	if(tsk_debug_get_level() >= DEBUG_LEVEL_INFO){ \
		if(tsk_debug_get_info_cb()) \
			tsk_debug_get_info_cb()(tsk_debug_get_arg_data(), "*[DOUBANGO INFO]: " FMT "\n", ##__VA_ARGS__); \
		else \
			fprintf(stderr, "*[DOUBANGO INFO]: " FMT "\n", ##__VA_ARGS__); \
	}


/* WARN */
#define TSK_DEBUG_WARN(FMT, ...)		\
	if(tsk_debug_get_level() >= DEBUG_LEVEL_WARN){ \
		if(tsk_debug_get_warn_cb()) \
			tsk_debug_get_warn_cb()(tsk_debug_get_arg_data(), "**[DOUBANGO WARN]: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__); \
		else \
			fprintf(stderr, "**[DOUBANGO WARN]: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__); \
	}

/* ERROR */
#define TSK_DEBUG_ERROR(FMT, ...) 		\
	if(tsk_debug_get_level() >= DEBUG_LEVEL_ERROR){ \
		if(tsk_debug_get_error_cb()) \
			tsk_debug_get_error_cb()(tsk_debug_get_arg_data(), "***[DOUBANGO ERROR]: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__); \
		else \
			fprintf(stderr, "***[DOUBANGO ERROR]: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__); \
	}


/* FATAL */
#define TSK_DEBUG_FATAL(FMT, ...) 		\
	if(tsk_debug_get_level() >= DEBUG_LEVEL_FATAL){ \
		if(tsk_debug_get_fatal_cb()) \
			tsk_debug_get_fatal_cb()(tsk_debug_get_arg_data(), "****[DOUBANGO FATAL]: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__); \
		else \
			fprintf(stderr, "****[DOUBANGO FATAL]: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__); \
	}


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
TINYSAK_API int tsk_debug_get_level( );
TINYSAK_API void tsk_debug_set_level(int );

#endif /* TSK_HAVE_DEBUG_H */


TSK_END_DECLS

#endif /* _TINYSAK_DEBUG_H_ */

