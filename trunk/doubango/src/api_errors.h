/****************************************************************************
			 _             _                             
			| |           | |                            
		  _ | | ___  _   _| | _   ____ ____   ____  ___  
		 / || |/ _ \| | | | || \ / _  |  _ \ / _  |/ _ \ 
		( (_| | |_| | |_| | |_) | ( | | | | ( ( | | |_| |
		 \____|\___/ \____|____/ \_||_|_| |_|\_|| |\___/ 
											(_____|   
	
	Copyright (C) 2009 xxxyyyzzz <imsframework(at)gmail.com>

	This file is part of Open Source Doubango IMS Client Framework project.

    DOUBANGO is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
	
    DOUBANGO is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
	
    You should have received a copy of the GNU General Public License
    along with DOUBANGO.
****************************************************************************/
#ifndef _DOUBANGO_ERRORS_H_
#define _DOUBANGO_ERRORS_H_

#include "pref.h"

PREF_NAMESPACE_START

#define ERR_SUCCEED(code)	(code == PREF_NAMESPACE_VALUE::ERR_SUCCESS)
#define ERR_FAILED(code)	!ERR_SUCCEED(code)

typedef enum tag_ERR
{
	ERR_SUCCESS					= 0x0000,

	ERR_GLOBAL_FAILURE			= -0xFFFF,
	ERR_NOT_IMPLEMENTED,
	ERR_FUNC_OBSOLETE,

	/* ENGINE errors */
	ERR_ENGINE_NOT_INITIALIZED,		/* engine not initialized or initialization failed*/
	ERR_ENGINE_ALREADY_INITIALIZED,	/* engine have been already initialized */

	/* SIP DIALOG */
	ERR_SIP_DIALOG_UNKNOWN,
	ERR_SIP_DIALOG_NOT_FOUND,		/* dialog not found */

	/* STACK errors */
	ERR_STACK_NOT_FOUND,			/* stack not found*/
	ERR_STACK_ALREADY_EXIST,		/* state already exist */
	ERR_STACK_NOT_INITIALIZED,		/* stack not initialized or initialization failed*/
	ERR_STACK_IS_INVALID,			/* stack is in an invalide state (NULL, destroyed, locked, ...) */
	ERR_STACK_NOT_RUNNING,			/* operation failed because the stack is not running */
}
ERR;

PREF_NAMESPACE_END

#endif /* _DOUBANGO_ERRORS_H_ */