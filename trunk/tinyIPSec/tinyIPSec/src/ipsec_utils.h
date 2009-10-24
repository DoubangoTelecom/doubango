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
#ifndef _TINYIPSEC_UTILS_H_
#define _TINYIPSEC_UTILS_H_

#include "tinyipsec_config.h"

// FIXME:
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;

#define IPSEC_SAFE_FREE(ptr) if(ptr) {(void)free(ptr); ptr=0;}

/* macro helpers to create/free structures */
#define IPSEC_XXX_CREATE(xxx, prefix)\
	xxx = (ipsec_##prefix##_t*)malloc(sizeof(ipsec_##prefix##_t)); \
	ipsec_##prefix##_init(xxx);
#define IPSEC_XXX_SAFE_FREE(xxx, prefix)\
	if(xxx) { ipsec_##prefix##_free(&(xxx)); xxx = 0; }

TINYIPSEC_API void ipsec_strcat(char** destination, const char* source);
TINYIPSEC_API int ipsec_sprintf(char** str, const char* format, ...);
TINYIPSEC_API char* ipsec_strdup(const char* s1);
TINYIPSEC_API void ipsec_strupdate(char** str, const char* newval);
TINYIPSEC_API int ipsec_stricmp(const char* s1, const char* s2);

#endif /* _TINYIPSEC_UTILS_H_ */