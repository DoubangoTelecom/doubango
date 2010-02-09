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
#include <time.h>

#if HAVE_LINUX_IPSEC

/* ipsec context for linux platform */
typedef struct ipsec_linux_context_s
{
	int FIXME;
}
ipsec_linux_context_t;

/* initialize ipsec context */
/* returns 0 if succeed and nonzero otherwise */
ipsec_error_t ipsec_context_init(ipsec_context_t* context)
{
	memset(context, 0, sizeof(ipsec_context_t));
	srand (time(0));

	/* SAs */
	for(i=0; i<SA_COUNT; i++)
	{
		ipsec_sa_init(context->SAs[i]);
	}

	return 0;
}

/* free ipsec context */
void ipsec_context_free(ipsec_context_t** context)
{
	if(*context)
	{
		/* flush all SAs */
		ipsec_flush(*context);

		/* SAs */
		for(i=0; i<SA_COUNT; i++)
		{
			ipsec_sa_free(&(context->SAs[i]));
		}
		
		free(*context);
		(*context) = 0;
	}
}

/* flush all associated ipsec SAs */
void ipsec_flush(const ipsec_context_t* context)
{
}

#endif /* HAVE_IPSEC_TOOLS */