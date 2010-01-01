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

/**@file tsk_debug.h
 * @brief Useful functions for debugging.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TINYSAK_DEBUG_H_
#define _TINYSAK_DEBUG_H_

#include "tinySAK_config.h"
#include <stdio.h>

TSK_BEGIN_DECLS

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL DEBUG_LEVEL_ERROR
#endif

#define DEBUG_LEVEL_INFO		4 /**< Info level */
#define DEBUG_LEVEL_WARN		3 /**< Warn level */
#define DEBUG_LEVEL_ERROR		2 /**< Error level */
#define DEBUG_LEVEL_FATAL		1 /**< Fatal level */

/**@ref TSK_DEBUG_INFO
* Level >= @ref DEBUG_LEVEL_INFO: Nothing important (informational about application progress).
*/
#if (DEBUG_LEVEL >= DEBUG_LEVEL_INFO)
#define TSK_DEBUG_INFO(FMT, ...)		fprintf(stderr, "*INFO: " FMT "\n", __VA_ARGS__);
#else
#define TSK_DEBUG_INFO(FMT, ...)		((void)0)
#endif

/**@ref TSK_DEBUG_WARN
* Level >= @ref DEBUG_LEVEL_WARN: Take care.
*/
#if (DEBUG_LEVEL >= DEBUG_LEVEL_WARN)
#define TSK_DEBUG_WARN(FMT, ...)		fprintf(stderr, "**WARN: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, __VA_ARGS__);
#else
#define TSK_DEBUG_WARN(FMT, ...)		((void)0)
#endif

/**@ref TSK_ERROR_WARN
* Level >= @ref DEBUG_LEVEL_ERROR: Could cause crash or instability.
*/
#if (DEBUG_LEVEL >= DEBUG_LEVEL_ERROR)
#define TSK_DEBUG_ERROR(FMT, ...)		fprintf(stderr, "***ERROR: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, __VA_ARGS__);
#else
#define TSK_DEBUG_ERROR(FMT, ...)		((void)0)
#endif

/**@ref TSK_DEBUG_FATAL
* Level >= @ref DEBUG_LEVEL_FATAL: The application should abort or behave unexpectedly.
*/
#if (DEBUG_LEVEL >= DEBUG_LEVEL_FATAL)
#define TSK_DEBUG_FATAL(FMT, ...)		fprintf(stderr, "****FATAL ERROR***: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, __VA_ARGS__);
#else
#define TSK_DEBUG_FATAL(FMT, ...)		((void)0)
#endif

//__GNUC_VA_LIST

/* FIXME */
#if defined(__SYMBIAN32__) || defined(ANDROID) || defined(__GNUC__)

#undef TSK_DEBUG_INFO
#define TSK_DEBUG_INFO printf

#undef TSK_DEBUG_WARN
#define TSK_DEBUG_WARN printf

#undef TSK_DEBUG_ERROR
#define TSK_DEBUG_ERROR printf

#undef TSK_DEBUG_FATAL
#define  TSK_DEBUG_FATAL printf

#endif

TSK_END_DECLS

#endif /* _TINYSAK_DEBUG_H_ */
