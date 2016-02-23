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

/**@file tsk_debug.c
 * @brief Utility functions for debugging purpose.
 */
#include "tsk_debug.h"

/**@defgroup tsk_debug_group Utility functions for debugging purpose.
*/

#if TSK_HAVE_DEBUG_H
// Nothing to do --> all is up to the end-user application
#else

static const void* tsk_debug_arg_data = tsk_null;
static tsk_debug_f tsk_debug_info_cb = tsk_null;
static tsk_debug_f tsk_debug_warn_cb = tsk_null;
static tsk_debug_f tsk_debug_error_cb = tsk_null;
static tsk_debug_f tsk_debug_fatal_cb = tsk_null;
static int tsk_debug_level = DEBUG_LEVEL;

/**@ingroup tsk_debug_group
* Defines the callback data. Will be the @a arg parameter for the callback function.
* @param arg_data The callback data.
* @sa @ref tsk_debug_get_arg_data()
*/
void tsk_debug_set_arg_data(const void* arg_data)
{
    tsk_debug_arg_data = arg_data;
}
/**@ingroup tsk_debug_group
* Gets the callback data passed to the framework using @ref tsk_debug_set_arg_data().
* @retval The callback data.
* @sa @ref tsk_debug_set_arg_data()
*/
const void* tsk_debug_get_arg_data()
{
    return tsk_debug_arg_data;
}
/**@ingroup tsk_debug_group
* Sets the callback function to call when @ref TSK_DEBUG_INFO() is internally used.
* @param cb A pointer to the callback function.
* @sa @ref tsk_debug_set_warn_cb() @ref tsk_debug_set_error_cb() @ref tsk_debug_set_fatal_cb().
*/
void tsk_debug_set_info_cb(tsk_debug_f cb)
{
    tsk_debug_info_cb = cb;
}
/**@ingroup tsk_debug_group
* Gets the callback function defined using @ref tsk_debug_set_info_cb().
* @retval A pointer to the callback function.
*/
tsk_debug_f tsk_debug_get_info_cb()
{
    return tsk_debug_info_cb;
}
/**@ingroup tsk_debug_group
* Sets the callback function to call when @ref TSK_DEBUG_WARN() is internally used.
* @param cb A pointer to the callback function.
* @sa @ref tsk_debug_set_info_cb() @ref tsk_debug_set_error_cb() @ref tsk_debug_set_fatal_cb().
*/
void tsk_debug_set_warn_cb(tsk_debug_f cb)
{
    tsk_debug_warn_cb = cb;
}
/**@ingroup tsk_debug_group
* Gets the callback function defined using @ref tsk_debug_set_warn_cb().
* @retval A pointer to the callback function.
*/
tsk_debug_f tsk_debug_get_warn_cb()
{
    return tsk_debug_warn_cb;
}
/**@ingroup tsk_debug_group
* Sets the callback function to call when @ref TSK_DEBUG_ERROR() is internally used.
* @param cb A pointer to the callback function.
* @sa @ref tsk_debug_set_info_cb() @ref tsk_debug_set_warn_cb() @ref tsk_debug_set_fatal_cb().
*/
void tsk_debug_set_error_cb(tsk_debug_f cb)
{
    tsk_debug_error_cb = cb;
}
/**@ingroup tsk_debug_group
* Gets the callback function defined using @ref tsk_debug_set_error_cb().
* @retval A pointer to the callback function.
*/
tsk_debug_f tsk_debug_get_error_cb()
{
    return tsk_debug_error_cb;
}
/**@ingroup tsk_debug_group
* Sets the callback function to call when @ref TSK_DEBUG_FATAL() is internally used.
* @param cb A pointer to the callback function.
* @sa @ref tsk_debug_set_info_cb() @ref tsk_debug_set_warn_cb() @ref tsk_debug_set_error_cb().
*/
void tsk_debug_set_fatal_cb(tsk_debug_f cb)
{
    tsk_debug_fatal_cb = cb;
}
/**@ingroup tsk_debug_group
* Gets the callback function defined using @ref tsk_debug_set_fatal_cb().
* @retval A pointer to the callback function.
*/
tsk_debug_f tsk_debug_get_fatal_cb()
{
    return tsk_debug_fatal_cb;
}
/**@ingroup tsk_debug_group
* Gets the debug level defined using @ref tsk_debug_set_level.
* @retval The debug level. <b>Must be</b>: @ref DEBUG_LEVEL_INFO, @ref DEBUG_LEVEL_WARN, @ref DEBUG_LEVEL_ERROR or @ref DEBUG_LEVEL_FATAL.
* @sa @ref tsk_debug_set_level()
*/
int tsk_debug_get_level()
{
    return tsk_debug_level;
}
/**@ingroup tsk_debug_group
* Sets the debug level.
* @param level The debug level. <b>Must be</b>: @ref DEBUG_LEVEL_INFO, @ref DEBUG_LEVEL_WARN, @ref DEBUG_LEVEL_ERROR or @ref DEBUG_LEVEL_FATAL.
* @sa @ref tsk_debug_get_level()
*/
void tsk_debug_set_level(int level)
{
    tsk_debug_level = level;
}

#endif /* TSK_HAVE_DEBUG_H */


/**@page _Page_TinySAK_AnsiC_Debugging Debugging
*
* - @ref _Anchor_TinySAK_Debugging_Output "Output"
*  - @ref _Anchor_TinySAK_Debugging_StdErr "stderr"
*  - @ref _Anchor_TinySAK_Debugging_Callback_Functions "Callback functions"
*  - @ref _Anchor_TinySAK_Debugging_Custom_Debug_Header "Custom debug header"
* - @ref _Anchor_TinySAK_Debugging_Public_Fuctions "Public functions"
*
* <hr />
*
* The framework offers 4 debugging levels. The minimum level could be defined by setting the value of @ref DEBUG_LEVEL macro in your <b>CFLAGS</b> or using @ref tsk_debug_set_level. <br />
* Supported level values are @ref DEBUG_LEVEL_INFO, @ref DEBUG_LEVEL_WARN, @ref DEBUG_LEVEL_ERROR or @ref DEBUG_LEVEL_FATAL.
* -	@ref DEBUG_LEVEL_INFO is the minimum value. Used to log user information, application progress, configuration ...
* -	@ref DEBUG_LEVEL_WARN signals that something that could change the normal process happened. This type of error should not block the application.
* -	@ref DEBUG_LEVEL_ERROR is the <b>default</b> minimum value. Signals that the current operation has failed. If the operation was critical (e.g. thread creation) then, this will badly change the application behavior, otherwise (e.g. sending data over network) the application will continue to behave as expected.
* -	@ref DEBUG_LEVEL_FATAL is the maximum value. This kind of errors are signaled just before the application crashes or enters in a state from which it's impossible to recover from.
*
* At any time you can change the debug level using @ref tsk_debug_set_level().
*
* <h2>@anchor _Anchor_TinySAK_Debugging_Output Output</h2>
* This section is about the ouput where the debug messages are printed. <br />
* The framework uses @ref TSK_DEBUG_INFO(), @ref TSK_DEBUG_WARN(), @ref TSK_DEBUG_ERROR() and @ref TSK_DEBUG_FATAL() macros to print the message. <br />
* <i>Example:</i>
* @code
* TSK_DEBUG_INFO("Program = %s, version = %f", "Doubango SIP client SDK", 3.0f);
* @endcode
*
* <h3>@anchor _Anchor_TinySAK_Debugging_StdErr stderr</h3>
* By default, all these macros will print the message to <a target=_blank href="http://www.cplusplus.com/reference/cstdio/stderr/">stderr</a>. <br />
* There are two ways to change this behavior: use @ref _Anchor_TinySAK_Debugging_Callback_Functions "callback functions" or @ref _Anchor_TinySAK_Debugging_Public_Fuctions "custom debug header".
*
* <h3>@anchor _Anchor_TinySAK_Debugging_Callback_Functions Callback functions</h3>
* You can provide your own callback functions to be called when the framework calls @ref TSK_DEBUG_INFO(), @ref TSK_DEBUG_WARN(), @ref TSK_DEBUG_ERROR() and @ref TSK_DEBUG_FATAL(). <br />
* Instead of writting the message to <a target=_blank href="http://www.cplusplus.com/reference/cstdio/stderr/">stderr</a> output your callback functions will be called. <br />
* @code
//
// This code shows how to write the logs to a file instead of stderr
//

FILE* file = fopen("logs.txt", "w+"); // do not forget to close the file using fclose().
// defines your INFO callback
*static int _debug_info_write_to_file(const void* arg, const char* fmt, ...)
{
	va_list ap;
	char* msg = tsk_null;
	const this_t* _this = (this_t*)arg; // value passed to the framework using tsk_debug_set_arg_data()
	va_start(ap, fmt);
	tsk_sprintf_2(&msg, fmt, &ap);
	fputs(msg, file);
	TSK_FREE(msg);
	va_end(ap);

	return 0;
}
// optional: set callback data (will be passed to _debug_info_write_to_file() as "arg" parameter)
const this_t* _this = tsk_null;
tsk_debug_set_arg_data(_this);
// register your callback
tsk_debug_set_info_cb(_debug_info_write_to_file);
* @endcode
* Anywhere in Doubango code when @ref TSK_DEBUG_INFO() is called then, your callback function will be called to write the message to a file. <br />
* This is the best way to use custom loggers such as @a log4j, @a log4net or @a logfile.
*
* <h3>@anchor _Anchor_TinySAK_Debugging_Custom_Debug_Header Custom debug header</h3>
* This is an ANSI-C header file (<b>*.h</b>) overriding the macros used to print the debug messages. <br />
* This file:
* - must be named @a my_debug.h and in the same folder as @a tsk_debug.h.
* - must contain your own definitions of  @ref TSK_DEBUG_INFO(), @ref TSK_DEBUG_WARN(), @ref TSK_DEBUG_ERROR() and @ref TSK_DEBUG_FATAL().
* - and you must add <i>-DTSK_HAVE_DEBUG_H=1</i> in your <i>CFLAGS</i> before building @a tinySAK project.
*
* <h2>@anchor _Anchor_TinySAK_Debugging_Public_Fuctions Public functions </h2>
* - @ref tsk_debug_get_arg_data
* - @ref tsk_debug_get_error_cb
* - @ref tsk_debug_get_fatal_cb
* - @ref tsk_debug_get_info_cb
* - @ref tsk_debug_get_level
* - @ref tsk_debug_get_warn_cb
* - @ref tsk_debug_set_arg_data
* - @ref tsk_debug_set_error_cb
* - @ref tsk_debug_set_fatal_cb
* - @ref tsk_debug_set_info_cb
* - @ref tsk_debug_set_level
* - @ref tsk_debug_set_warn_cb
*/