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
#include "cmd.h"

#include "dssl.h"

#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"

//#define PAD "  "

static int pred_find_opt_by_type(const tsk_list_item_t *item, const void *type);

/* parse a command line */
cmd_t* cmd_parse(const char* buffer, tsk_size_t size, tsk_bool_t *comment, tsk_params_L_t* params)
{
    return dssl_parse(buffer, size, comment, params);
}

///* print usage */
void cmd_print_help()
{
    printf("\n\n========================= Usage =========================\n\n");
    printf("Please refer to the Programmer's Guide at\n http://www.doubango.org/\n\n");

//
//	printf("Usage:\n");
//	printf(PAD"++[command] --[opts]\n\n");
//
//	/* Commands */
//	printf("Commands:\n--\n");
//	printf(PAD"[++audio] or [++a"PAD"%s", "Make audio call\n--\n");
//	printf(PAD"[++audiovideo] or [++av]"PAD"%s", "Make audio/video call\n--\n");
//	printf(PAD"[++config-file] or [++cf]"PAD"%s", "Load opts from config file\n--\n");
//	printf(PAD"[++config-session] | [++css]"PAD"%s", "Configure an 3GPP IMS/LTE session\n--\n");
//	printf(PAD"[++config-stack] | [++cst]"PAD"%s", "Configure an 3GPP IMS/LTE stack\n--\n");
//	printf(PAD"[++exit] | [++e]"PAD"%s", "Exit the application\n--\n");
//	printf(PAD"[++file] | [++f]"PAD"%s", "Send a file. The stack must be running (see ++run command). To abort the File transfer, use ++hangup.\n--\n");
//	printf(PAD"[++hangup] | [++hp]"PAD"%s", "Hangup any SIP session (unREGISTER, unSUBSCRIBE, unPUBLISH, HangUp Call, Abort ...).\n--\n");
//	printf(PAD"[++help] | [++h]"PAD"%s", "Print this help screen\n--\n");
//	printf(PAD"[++message] | [++m]"PAD"%s", "Send Pager Mode IM. The stack must be running (see ++run command).\n\n");
//	printf(PAD"[++publish] | [++pub]"PAD"%s", "Send PUBLISH message. The stack must be running (see ++run command). To unPUBLISH, use ++hanggup.\n--\n");
//	printf(PAD"[++quit] | [++q]"PAD"%s", "Quit the application\n--\n");
//	printf(PAD"[++run]"PAD"%s", "Start/Run the 3GPP IMS/LTE stack. Mandatory before starting to do anything.\n--\n");
//	printf(PAD"[++sms]"PAD"%s", "Send Binary SMS (RP-DATA). The stack must be running (see ++run command).\n--\n");
//	printf(PAD"[++subscribe] | [++sub]"PAD"%s", "Send SUBSCRIBE message. The stack must be running (see ++run command). To unSUBSCRIBE, use ++hangup.\n--\n");
//	printf(PAD"[++video] or [++v]"PAD"%s", "Make video call\n--\n");
//
//	printf("\n\n========================= =========================\n\n");
}

cmd_t* cmd_create(cmd_type_t type)
{
    return tsk_object_new(cmd_def_t, type);
}

static tsk_object_t* cmd_ctor(tsk_object_t * self, va_list * app)
{
    cmd_t *cmd = self;
    if(cmd) {
        cmd->type = va_arg(*app, cmd_type_t);
        cmd->opts = tsk_list_create();
    }
    return self;
}

static tsk_object_t* cmd_dtor(tsk_object_t * self)
{
    cmd_t *cmd = self;
    if(cmd) {
        TSK_OBJECT_SAFE_FREE(cmd->opts);
        TSK_FREE(cmd->sidparam);
    }

    return self;
}

static const tsk_object_def_t cmd_def_s = {
    sizeof(cmd_t),
    cmd_ctor,
    cmd_dtor,
    tsk_null,
};
const tsk_object_def_t *cmd_def_t = &cmd_def_s;


opt_t* opt_create(opt_type_t type, lv_t level, const char* value)
{
    return tsk_object_new(opt_def_t, type, level, value);
}

const opt_t* opt_get_by_type(const opts_L_t* opts, opt_type_t type)
{
    const tsk_list_item_t* item;
    if((item = tsk_list_find_item_by_pred(opts, pred_find_opt_by_type, &type))) {
        return item->data;
    }
    return tsk_null;
}

static int pred_find_opt_by_type(const tsk_list_item_t *item, const void *type)
{
    if(item && item->data) {
        opt_t *opt = item->data;
        return (opt->type - *((opt_type_t*)type));
    }
    return -1;
}

static tsk_object_t* opt_ctor(tsk_object_t * self, va_list * app)
{
    opt_t *opt = self;
    if(opt) {
        opt->type = va_arg(*app, opt_type_t);
        opt->lv = va_arg(*app, lv_t);
        opt->value = tsk_strdup(va_arg(*app, const char*));
    }
    return self;
}

static tsk_object_t* opt_dtor(tsk_object_t * self)
{
    opt_t *opt = self;
    if(opt) {
        TSK_FREE(opt->value);
    }

    return self;
}

static const tsk_object_def_t opt_def_s = {
    sizeof(opt_t),
    opt_ctor,
    opt_dtor,
    tsk_null,
};
const tsk_object_def_t *opt_def_t = &opt_def_s;
