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
#include "txc_api.h" 

#ifndef TEST_IETF_PRESRULES
#define TEST_IETF_PRESRULES

/* testing ietf-presrules */
void test_ietf_presrules()
{
	txc_context_t* context;
	char *presrules = 0;
	
	/* create our XCAP context */
	TXC_CONTEXT_CREATE(context);

	printf("---\nTEST IETF-PRESRULES\n---\n");

	/* Initialize the context */
	TEST_XXXX_CONTEXT_INIT(context);

	presrules = txc_ietf_presrules_serialize(context);
	printf("IETF pres-rules: \n%s\n***\n", presrules);
	TSK_FREE(presrules);

	/* free the context*/
	TXC_CONTEXT_SAFE_FREE(context);
}

#endif