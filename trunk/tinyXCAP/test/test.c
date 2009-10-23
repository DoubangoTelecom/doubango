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

#include "stdafx.h"
#include "xdm_api.h"

/*
* These AUID are supported: 
- xcap-caps
- resource-lists
- rls-services
- pres-rules
- directory
- org.openmobilealliance.conv-history
- org.openmobilealliance.pres-rules
- org.openmobilealliance.xdm-directory
- org.openmobilealliance.deferred-list
- org.openmobilealliance.pres-content
- org.openmobilealliance.groups

* Both HTTPS and HTTP are supported
* Automatic Url encoding/decoding
*/

#include <stdarg.h>

#include "test_selector.h"
#include "test_xcap-caps.h"
#include "test_rlist.h"
#include "test_oma-directory.h"
#include "test_oma-pres-cont.h"
#include "test_rls.h"
#include "test_xcap.h"
#include "test_gsma_rcs.h"

/* http://curl.haxx.se/libcurl/c/libcurl-tutorial.html */

int _tmain(int argc, _TCHAR* argv[])
{
#if 0		//uncomment to test memory leaks
	while(1)
#endif
	{
		/* test xcap selector */
		//test_selector();

		/* test xcap-caps */
		//test_xcap_caps();

		/* test resource-lists */
		//test_rlist();

		/* test resource-lists 2 */
		//test_rlist2();

		/* test oma xcap-directory*/
		//test_oma_directory();

		/* test oma-pres-content */
		//test_oma_pres_cont();

		/* test rls-services */
		//test_rls_services();

		/* test xcap */
		test_xcap();

		/* test gsma rcs */
		//test_gsma_rcs();
	}

	getchar();

	return 0;
}

