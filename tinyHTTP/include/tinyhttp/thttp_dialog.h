/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file thttp_dialog.h
 * @brief HTTP Dialog.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef THTTP_DIALOG_H
#define THTTP_DIALOG_H

#include "tinyhttp_config.h"

#include "tsk_fsm.h"
#include "tsk_list.h"
#include "tsk_buffer.h"

THTTP_BEGIN_DECLS

struct thttp_message_s;

typedef uint64_t thttp_dialog_id_t;

typedef struct thttp_dialog_s {
    TSK_DECLARE_OBJECT;

    thttp_dialog_id_t id;
    uint64_t timestamp;

    tsk_fsm_t* fsm;

    tsk_buffer_t* buf;

    struct thttp_session_s* session;
    struct thttp_action_s* action;
    tsk_bool_t answered;
}
thttp_dialog_t;

typedef tsk_list_t thttp_dialogs_L_t;

TINYHTTP_API int thttp_dialog_fsm_act(thttp_dialog_t* self, tsk_fsm_action_id , const struct thttp_message_s* , const struct thttp_action_s*);
TINYHTTP_API thttp_dialog_t* thttp_dialog_new(struct thttp_session_s* session);
thttp_dialog_t* thttp_dialog_get_oldest(thttp_dialogs_L_t* dialogs);

TINYHTTP_GEXTERN const tsk_object_def_t *thttp_dialog_def_t;

THTTP_END_DECLS

#endif /* THTTP_DIALOG_H */

