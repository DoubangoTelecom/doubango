/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

#include "stdafx.h"

#include "tsk.h"
#include "tinysms.h"

char* tohex(uint8_t* ptr, size_t size)
{
    char* ret = tsk_null;
    size_t i;
    for(i=0; i<size; i++) {
        tsk_strcat_2(&ret, "%.2X", *ptr++);
    }
    return ret;
}

void printhex(const char* what, uint8_t* ptr, size_t size)
{
    size_t i;
    printf("%s", what);
    for(i=0; i<size; i++) {
        printf("%.2X", *ptr++);
    }
    printf("\n");
}

tsk_bool_t bin_equals(const uint8_t* b1, const uint8_t* b2, size_t size)
{
    size_t i;

    // we assume that sizeof(b1)==sizeof(b2)
    if(!b1 || !b2 || !size) {
        return tsk_false;
    }
    for(i=0; i<size; i++) {
        if(b1[i] != b2[i]) {
            return tsk_false;
        }
    }
    return tsk_true;
}


#include "test_packing.h"
#include "test_tpdu.h"
#include "test_rpdu.h"


#define RUN_TEST_LOOP		1

#define RUN_TEST_ALL		0
#define RUN_TEST_PACKING	0
#define RUN_TEST_TPDU		0
#define RUN_TEST_RPDU		1


#ifdef _WIN32_WCE
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
#if RUN_TEST_LOOP
    for(;;)
#endif
    {
        /* Print copyright information */
        printf("Doubango Project\nCopyright (C) 2009 - 2010 Mamadou Diop \n\n");

#if RUN_TEST_ALL  || RUN_TEST_PACKING
        test_packing();
#endif

#if RUN_TEST_ALL  || RUN_TEST_TPDU
        test_tpdu();
#endif

#if RUN_TEST_ALL  || RUN_TEST_RPDU
        test_rpdu();
#endif

    }
}