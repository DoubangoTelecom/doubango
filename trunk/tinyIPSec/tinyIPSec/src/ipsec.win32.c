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
#include "ipsec.h"

#include <Fwpmu.h>

#if HAVE_WIN32_IPSEC

#define GET_OPAQUE(context) ((ipsec_win32_opaque_t*)(context)->opaque)

/* ipsec opaque for win32 platform */
typedef struct ipsec_win32_opaque_s
{
	HANDLE engine;
}
ipsec_win32_opaque_t;

/* initialize ipsec context */
/* returns 0 if succeed and nonzero otherwise */
ipsec_error_t ipsec_context_init(ipsec_context_t* context)
{
	ipsec_error_t code = ERROR_SUCCESS;

	memset(context, 0, sizeof(ipsec_context_t));

	/* opaque */
	context->opaque =  malloc(sizeof(ipsec_win32_opaque_t));
	memset(context->opaque, 0, sizeof(ipsec_win32_opaque_t));

	/* open engine */
	if(code = FwpmEngineOpen0(NULL, RPC_C_AUTHN_WINNT, NULL, NULL, &GET_OPAQUE(context)->engine))
	{
		return code;
	}
}

/* free ipsec context */
void ipsec_context_free(ipsec_context_t** context)
{
	if(*context)
	{
		/* close engine */
		FwpmEngineClose0(GET_OPAQUE(*context)->engine);
		
		free(*context);
		(*context) = 0;
	}
}

#undef GET_OPAQUE
#endif /* HAVE_WIN32_IPSEC */