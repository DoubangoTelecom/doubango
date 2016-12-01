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

/**@file tmsrp_media.h
 * @brief MSRP Session config.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef TINYMSRP_CONFIG_H
#define TINYMSRP_CONFIG_H

#include "tinymsrp_config.h"

#include "tinymsrp/headers/tmsrp_header_From-Path.h"
#include "tinymsrp/headers/tmsrp_header_To-Path.h"

#include "tsk_object.h"

TMSRP_BEGIN_DECLS

#ifndef TMSRP_MAX_CHUNK_SIZE
#	define TMSRP_MAX_CHUNK_SIZE				2048
#endif

typedef struct tmsrp_config_s {
    TSK_DECLARE_OBJECT;

    tmsrp_header_To_Path_t* To_Path;
    tmsrp_header_From_Path_t* From_Path;

    tsk_bool_t Failure_Report;
    tsk_bool_t Success_Report;
    tsk_bool_t OMA_Final_Report;
}
tmsrp_config_t;

TINYMSRP_API tmsrp_config_t* tmsrp_config_create();

TINYMSRP_GEXTERN const tsk_object_def_t *tmsrp_config_def_t;

TMSRP_END_DECLS

#endif /* TINYMSRP_CONFIG_H */
