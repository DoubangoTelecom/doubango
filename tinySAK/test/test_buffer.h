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
#ifndef _TEST_BUFFER_H_
#define _TEST_BUFFER_H_

void test_buffer()
{
    tsk_buffer_t *buffer = tsk_buffer_create_null();

    tsk_buffer_append(buffer, "Diop", strlen("Diop"));
    tsk_buffer_append(buffer, " ", strlen(" "));
    tsk_buffer_append(buffer, "Mamadou", strlen("Mamadou"));
    tsk_buffer_append(buffer, "\r\n", strlen("\r\n"));

    printf("1. Buffer=%s", TSK_BUFFER_TO_STRING(buffer));

    tsk_buffer_append_2(buffer, "val1=[%s] and val2=[%d]r\n", "value1", 12);
    tsk_buffer_append_2(buffer, "val3=[%s] and val4=[%s]r\n", "458888554778555LL", "1254852");

    printf("2. Buffer=%s", TSK_BUFFER_TO_STRING(buffer));

    TSK_OBJECT_SAFE_FREE(buffer);
}

#endif /* _TEST_BUFFER_H_ */
