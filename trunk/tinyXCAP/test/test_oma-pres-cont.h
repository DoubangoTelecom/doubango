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
#ifndef _TEST_OMA_PRESCONTENT_H_
#define _TEST_OMA_PRESCONTENT_H_
// http://www.openmobilealliance.org/technical/release_program/docs/PresenceSIMPLE/V2_0-20081223-C/OMA-TS-Presence_SIMPLE_Content_XDM-V1_0-20081223-C.pdf  subclause C.1.1
	const char* omaprescont_str0 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
								"<content xmlns=\"urn:oma:xml:prs:pres-content\">"
								"<mime-type>image/png</mime-type>"
								"<encoding>base64</encoding>"
								"<description>Attending OMA Meeting</description>"
								"<data>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</data>"
								"</content>";

/* test OMA directory (urn:oma:xml:xdm:xcap-directory) */
void test_oma_pres_cont(const char* buffer, size_t size)
{
	txc_oma_prescont_t* omaprescont = 0;
	
	printf("\n---\nTEST OMA PRES-CONTENT\n---\n");
	{
		/* create oma-prescontent */
		omaprescont = txc_oma_prescont_create(buffer, size);

		/* dump */
		printf("\ndump pres-content\n");
		if(omaprescont)
		{
			printf("mime-type: \"%s\" encoding: \"%s\" description: \"%s\" data: \"%s\"\n", 
				omaprescont->mime_type, omaprescont->encoding, omaprescont->description, omaprescont->data);
		}
		/* free omaprescontent */
		txc_oma_prescont_free(&omaprescont);
	}
}
#endif /* _TEST_OMA_PRESCONTENT_H_ */