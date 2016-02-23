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
#ifndef _TEST_PARAMS_H_
#define _TEST_PARAMS_H_


void test_params()
{
    tsk_buffer_t *buffer = tsk_buffer_create_null();
    tsk_params_L_t *params = tsk_list_create();

    //
    //
    //
    tsk_param_t *param1 = tsk_params_parse_param("mamadou=diop", strlen("mamadou=diop"));
    tsk_param_t *param2 = tsk_params_parse_param("project=doubango", strlen("project=doubango"));
    tsk_param_t *param3 = tsk_params_parse_param("license=gpl", strlen("license=gpl"));

    tsk_list_push_back_data(params, (void**)&param1);
    tsk_list_push_back_data(params, (void**)&param2);
    tsk_list_push_back_data(params, (void**)&param3);

    tsk_params_tostring(params, ',', buffer);
    TSK_DEBUG_INFO("Buffer=[%s]", TSK_BUFFER_TO_STRING(buffer));

    tsk_buffer_cleanup(buffer);
    TSK_OBJECT_SAFE_FREE(params);


    //
    //
    //
    params = tsk_params_fromstring("octet-align=1; mode-set=0,1,2,3,4,5,6,7; mode-change-period=1; mode-change-capability=2; mode-change-neighbor=0", ';', tsk_true);
    tsk_params_tostring(params, ';', buffer);
    TSK_DEBUG_INFO("Buffer=[%s]", TSK_BUFFER_TO_STRING(buffer));
    tsk_buffer_cleanup(buffer);
    TSK_OBJECT_SAFE_FREE(params);

    params = tsk_params_fromstring("QCIF=1;CIF=2;MaxBR=4200", ';', tsk_true);
    tsk_params_tostring(params, ';', buffer);
    TSK_DEBUG_INFO("Buffer=[%s]", TSK_BUFFER_TO_STRING(buffer));
    tsk_buffer_cleanup(buffer);
    TSK_OBJECT_SAFE_FREE(params);

    TSK_OBJECT_SAFE_FREE(buffer);
}


#endif /*_TEST_PARAMS_H_ */
