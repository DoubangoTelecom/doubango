/* Copyright (C) 2014 Mamadou DIOP.
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
#include "stdafx.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "tinynet.h"
#include "tsk.h"

#define kConfId				1927375685
#define kTransacId			32871
#define kUserId				18177
#define kFloorId			10665
#define kFloorReqId			2
#define kReqStatus			3 // Granted
#define kQueuePosition		0
#define kUserDisplayName	"Mamadou DIOP"
#define kUserUri			"Mamadou DIOP@TEST.COM"
#define kRemoteIP			"192.168.0.28"
#define kRemotePort			5060
#define kLocalIP			"192.168.0.37"
#define kLocalPort			5060

#define kNullOctetStringPtr 0

#define BAIL_IF_ERR(expr) { int _ret_; if ((_ret_) = (expr)) { TSK_DEBUG_ERROR("Error %d", (_ret_)); goto bail; } }

#define LOOP						0

#define RUN_TEST_ALL				0
#define RUN_TEST_PARSER				0
#define RUN_TEST_SESSION			1

#include "test_parser.h"
#include "test_session.h"


#ifdef _WIN32_WCE
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
    tnet_startup();

    do {
        /* Print copyright information */
        printf("Doubango Project\nCopyright (C) 2014 Mamadou Diop \n\n");

#if RUN_TEST_PARSER || RUN_TEST_ALL
        test_parser();
#endif
#if RUN_TEST_SESSION || RUN_TEST_ALL
        test_session();
#endif
    }
    while(LOOP);

    tnet_cleanup();

    getchar();

    return 0;
}

