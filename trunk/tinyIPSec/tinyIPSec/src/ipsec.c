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
#include "ipsec.h"

/* init ipsec SA */
void ipsec_sa_init(ipsec_sa_t* sa)
{
	memset(sa, 0, sizeof(ipsec_sa_t));
	sa->lifetime = SA_DEFAULT_LIFETIME;
}

/* free ipsec SA */
void ipsec_sa_free(ipsec_sa_t* sa)
{
	if(sa)
	{
		if((sa)->opaque)
		{
			if((sa)->opaque_func_free) (sa)->opaque_func_free(&((sa)->opaque));
			free((sa)->opaque);
			(sa)->opaque = 0;
		}

		free(sa);
		(sa) = 0;
	}
}