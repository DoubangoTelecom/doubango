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

#ifndef _TINYXCAP_XDM_UTILS_H_
#define _TINYXCAP_XDM_UTILS_H_

#include "tinyxcap_config.h"
#include "xdm.h"

char* xdm_url_encode(const char* url);
char* xdm_url_decode(const char* url);

TINYXCAP_API void xdm_strcat(char** destination, const char* source);
TINYXCAP_API int xdm_sprintf(char** str, const char* format, ...);
TINYXCAP_API char* xdm_strdup(const char* s1);
TINYXCAP_API void xdm_strupdate(char** str, const char* newval);
TINYXCAP_API int xdm_stricmp(const char* s1, const char* s2);

#endif /* _TINYXCAP_XDM_UTILS_H_ */