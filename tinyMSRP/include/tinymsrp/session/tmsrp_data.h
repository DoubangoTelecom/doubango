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

/**@file tmsrp_data.h
 * @brief MSRP input/output data.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef TINYMSRP_DATA_H
#define TINYMSRP_DATA_H

#include "tinymsrp_config.h"

#include "tinymsrp/tmsrp_message.h"

#include "tsk_buffer.h"

TMSRP_BEGIN_DECLS

#define TMSRP_DATA(self)					((tmsrp_data_t*)(self))
#define TMSRP_DATA_IS_OUTGOING(self)		(TMSRP_DATA(self)->outgoing)


typedef struct tmsrp_data_s {
    TSK_DECLARE_OBJECT;

    tsk_bool_t outgoing;
    tsk_bool_t isOK;

    char* id;
    char* ctype;
    char* wctype;
}
tmsrp_data_t;

#define TMSRP_DECLARE_DATA tmsrp_data_t data
typedef tsk_list_t tmsrp_datas_L_t;

typedef struct tmsrp_data_in_s {
    TMSRP_DECLARE_DATA;

    tsk_buffer_t* buffer;
}
tmsrp_data_in_t;

int tmsrp_data_in_put(tmsrp_data_in_t* self, const void* pdata, tsk_size_t size);
tmsrp_message_t* tmsrp_data_in_get(tmsrp_data_in_t* self);

typedef struct tmsrp_data_out_s {
    TMSRP_DECLARE_DATA;

    FILE* file;
    tsk_buffer_t* message;
    tsk_size_t size; // File/message size
}
tmsrp_data_out_t;

tmsrp_data_in_t* tmsrp_data_in_create();
tmsrp_data_out_t* tmsrp_data_out_create(const void* pdata, tsk_size_t size);
tmsrp_data_out_t* tmsrp_data_out_file_create(const char* filepath);

tsk_buffer_t* tmsrp_data_out_get(tmsrp_data_out_t* self);

TINYMSRP_GEXTERN const tsk_object_def_t *tmsrp_data_in_def_t;
TINYMSRP_GEXTERN const tsk_object_def_t *tmsrp_data_out_def_t;

TMSRP_END_DECLS

#endif /* TINYMSRP_DATA_H */
