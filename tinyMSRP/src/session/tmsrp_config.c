/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou [at) doubango (DOT) org>
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

/**@file tmsrp_media.c
 * @brief MSRP Session config.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinymsrp/session/tmsrp_config.h"

#include "tsk_memory.h"


TINYMSRP_API tmsrp_config_t* tmsrp_config_create()
{
    return tsk_object_new(tmsrp_config_def_t);
}












//=================================================================================================
//	MSRP Session config object definition
//
static void* tmsrp_config_ctor(tsk_object_t * self, va_list * app)
{
    tmsrp_config_t *config = self;
    if(config) {
        config->Failure_Report = tsk_true;
    }
    return self;
}

static void* tmsrp_config_dtor(tsk_object_t * self)
{
    tmsrp_config_t *config = self;
    if(config) {
        TSK_OBJECT_SAFE_FREE(config->From_Path);
        TSK_OBJECT_SAFE_FREE(config->To_Path);
    }

    return self;
}

static const tsk_object_def_t tmsrp_config_def_s = {
    sizeof(tmsrp_config_t),
    tmsrp_config_ctor,
    tmsrp_config_dtor,
    tsk_null,
};
const tsk_object_def_t *tmsrp_config_def_t = &tmsrp_config_def_s;

