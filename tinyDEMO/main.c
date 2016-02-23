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
#include "main.h"
#include "common.h"

#include "invite.h"
#include "message.h"
#include "options.h"
#include "publish.h"
#include "register.h"
#include "subscribe.h"

#include "tinydav.h" /* Doubango Audio/Video Framework */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define LINE_DELIM "\n \r\n"

/* === global variables === */
ctx_t* ctx = tsk_null;

const char* trim(const char*);
int insert(char* dest, tsk_size_t index, tsk_size_t dest_size, char* src, tsk_size_t src_size);
int update_param(const char* , const char* );
int execute(const cmd_t* );

/* === entry point === */
int main(int argc, char** argv)
{
    char cmdbuf[4096];
    tsk_buffer_t* buffer = tsk_null;
    cmd_t* cmd = tsk_null;
    tsk_bool_t comment = tsk_false;
    int ret;
    int i, index;
    const char* start = tsk_null, *end = tsk_null;

    int a = 32 | 1 | 2;

    /* Copyright */
    printf("Doubango Project (tinyDEMO)\nCopyright (C) 2009 - 2013 Mamadou Diop \n\n");

    /* Initialize Network Layer ==> Mandatory */
    tnet_startup();
    /* Initialize Doubango Audio/Video Framework ==> will register all plugins(codecs and sessions)
    * Not mandatory if you have your own plugins*/
    tdav_init();

    /* Print Usage */
    //cmd_print_help();

    /* create user's ctx */
    if(!(ctx = ctx_create()) || !ctx->stack) {
        TSK_DEBUG_ERROR("Failed to create user's ctx.");
        goto bail;
    }

    /* create new buffer */
    if(!(buffer = tsk_buffer_create_null())) {
        TSK_DEBUG_ERROR("Failed to create new buffer.");
        goto bail;
    }

    /* initial args */
    for(i=1 /* index zero contains the exe path */, index=0; i<argc && argv[i]; i++) {
        if(index) {
            tsk_buffer_append(buffer, " ", 1);
        }
        tsk_buffer_append(buffer, argv[i], tsk_strlen(argv[i]));
    }

    /* If initial args ==> parse it now */
    if(buffer->size) {
        TSK_DEBUG_INFO("Initial command-line: %s", buffer->data);
        goto init_buffer;
    }

    /* always use fgets() instead of gets. gets() is considered to be unsafe.(Android and Mac OS X will warn) */
    while(fgets(cmdbuf, sizeof(cmdbuf), stdin)) {
        TSK_DEBUG_INFO("Command-Line: %s", cmdbuf);
        tsk_buffer_cleanup(buffer); /* cannot read from console while executing scenario */
        tsk_buffer_append(buffer, cmdbuf, tsk_strlen(cmdbuf));
init_buffer:
        start = buffer->data;
        //start = trim(start);
        end = start + buffer->size;
        if(start >= end) {
            TSK_DEBUG_INFO("Empty buffer");
            continue;
        }
parse_buffer:
        TSK_OBJECT_SAFE_FREE(cmd); /* Free old value */
        cmd = cmd_parse(start, (end-start), &comment, ctx->params);
        if(cmd) {
            if(comment || cmd->type == cmd_none) {
                goto nex_line;
            }
        }
        else {
            continue;
        }

        /* Load from scenario file? */
        if(cmd->type == cmd_scenario) {
            FILE* file;
            const opt_t* opt;
            tsk_size_t read = 0;
            tsk_bool_t rm_lf = tsk_false;
            if((opt = opt_get_by_type(cmd->opts, opt_path)) && !tsk_strnullORempty(opt->value)) { /* --path option */
                if((file = fopen(opt->value, "r"))) {
                    memset(cmdbuf, '\0', sizeof(cmdbuf)), cmdbuf[0] = '\n';
                    read = fread(cmdbuf+1, sizeof(uint8_t), sizeof(cmdbuf)-1, file);
                    fclose(file), file = tsk_null;

                    if(read == 0) {
                        TSK_DEBUG_ERROR("[%s] is empty.", opt->value);
                        goto nex_line;
                    }
                    else if(read == sizeof(cmdbuf)-1) {
                        TSK_DEBUG_ERROR("Buffer too short.");

                        goto nex_line;
                    }
                    read++; /* \n */
                    /* repplace all '\' with spaces (easier than handling that in the ragel file) */
                    for(i=0; ((tsk_size_t)i)<read; i++) {
                        if(cmdbuf[i] == '\\') {
                            cmdbuf[i] = ' ';
                            rm_lf = tsk_true;
                        }
                        else if(rm_lf && cmdbuf[i] == '\n') {
                            cmdbuf[i] = ' ';
                            rm_lf = tsk_false;
                        }
                    }
                    cmdbuf[read] = '\n';

                    /* insert embedded scenario */
                    if((index = tsk_strindexOf(start, (end-start), "\n")) == -1) { /* ++sn line */
                        index = buffer->size;
                    }
                    else {
                        index += (start - ((const char*)buffer->data));
                    }

                    if(tsk_buffer_insert(buffer, index, cmdbuf, read)) {
                        continue;
                    }
                    else {
                        start = ((const char*)buffer->data) + index; // because insert use realloc()
                        end = (((const char*)buffer->data) + buffer->size);
                        goto nex_line;
                    }
                }
                else {
                    TSK_DEBUG_ERROR("Failed to open scenario-file [%s].", opt->value);
                    goto nex_line;
                }
                continue;
            }
            else {
                TSK_DEBUG_ERROR("++scenario command must have --path option.");
                continue;
            }
        }

        /* execute current command */
        switch(cmd->type) {
        case cmd_exit:
            TSK_DEBUG_INFO("Exit/Quit");
            goto bail;
        default:
            ret = execute(cmd);
            break;
        }

        /* next line */
nex_line:
        if((index = tsk_strindexOf(start, (end - start), "\n")) !=-1) {
            start += index;
            while((start < end) && isspace(*start)) {
                start ++;
            }
            if((start + 2/*++*/) < end) {
                goto parse_buffer; /* next line */
            }
            else {
                continue; /* wait for new commands */
            }
        }
    } /* while(buffer) */


bail:

    /* Free current command */
    TSK_OBJECT_SAFE_FREE(cmd);
    /* Free buffer */
    TSK_OBJECT_SAFE_FREE(buffer);
    /* Destroy the user's ctx */
    TSK_OBJECT_SAFE_FREE(ctx);
    /* Deinitialize Doubango Audio/Video Framework ==> will unregister all plugins(codecs and sessions)
    * Not mandatory */
    tdav_init();
    /* Uninitilize Network Layer */
    tnet_cleanup();

#if ANDROID
    exit(0);
#endif
    return 0;
}

const char* trim(const char* str)
{
    while(isspace(*str)) {
        str++;
    }
    return str;
}

int update_param(const char* pname, const char* pvalue)
{
    if(ctx->params && pname) {
        const tsk_param_t* param;
        if((param = tsk_params_get_param_by_name(ctx->params, pname))) {
            tsk_strupdate(&TSK_PARAM(param)->value, pvalue);
            return 0;
        }
        else {
            TSK_DEBUG_INFO("[%s] parameter does not exist", pname);
        }
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
    }
    return -1;
}

int execute(const cmd_t* cmd)
{
    int ret = 0;
    tsip_ssession_id_t sid;
    tsk_istr_t istr;

    if(!cmd) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(ctx);

    switch(cmd->type) {
    case cmd_audio:
    case cmd_audiovideo: {
        TSK_DEBUG_INFO("command=audio/video");
        if((sid = invite_handle_cmd(cmd->type, cmd->opts)) != TSIP_SSESSION_INVALID_ID) {
            if(cmd->sidparam) {
                tsk_itoa(sid, &istr);
                update_param(cmd->sidparam, istr);
            }
        }
        break;
    }
    case cmd_config_session: {
        TSK_DEBUG_INFO("command=config-session");
        break;
    }
    case cmd_config_stack: {
        TSK_DEBUG_INFO("command=config-satck");
        ret = stack_config(cmd->opts);
        break;
    }
    case cmd_dtmf: {
        const opt_t* opt;
        TSK_DEBUG_INFO("command=dtmf");
        if(!(opt = opt_get_by_type(cmd->opts, opt_sid)) || tsk_strnullORempty(opt->value)) { /* --sid option */
            TSK_DEBUG_ERROR("++dtmf command need --sid option");
            break;
        }
        if(!(opt = opt_get_by_type(cmd->opts, opt_event)) || tsk_strnullORempty(opt->value)) { /* --event option */
            TSK_DEBUG_ERROR("++dtmf command need --event option");
            break;
        }
        invite_handle_cmd(cmd->type, cmd->opts);
        break;
    }
    case cmd_dump: {
        TSK_DEBUG_INFO("command=dump");
        ret = stack_dump();
        break;
    }
    case cmd_ect: {
        const opt_t* opt;
        TSK_DEBUG_INFO("command=ect");
        if((opt = opt_get_by_type(cmd->opts, opt_sid)) && !tsk_strnullORempty(opt->value)) {
            TSK_DEBUG_ERROR("++ect command need --sid option");
            ret = -1;
            break;
        }
        if((opt = opt_get_by_type(cmd->opts, opt_to)) && !tsk_strnullORempty(opt->value)) {
            TSK_DEBUG_ERROR("++ect command need --to option");
            ret = -1;
            break;
        }
        invite_handle_cmd(cmd->type, cmd->opts);
        break;
    }
    case cmd_exit: {
        TSK_DEBUG_INFO("command=exit");
        goto bail;
        break;
    }
    case cmd_file: {
        const opt_t* opt;
        TSK_DEBUG_INFO("command=file");
        if(!(opt = opt_get_by_type(cmd->opts, opt_path)) || tsk_strnullORempty(opt->value)) {
            TSK_DEBUG_ERROR("++file command need --path option");
            break;
        }
        if((sid = invite_handle_cmd(cmd->type, cmd->opts)) != TSIP_SSESSION_INVALID_ID) {
            if(cmd->sidparam) {
                tsk_itoa(sid, &istr);
                update_param(cmd->sidparam, istr);
            }
        }
        break;
    }
    case cmd_hangup: {
        const opt_t* opt;
        TSK_DEBUG_INFO("command=hangup");
        if((opt = opt_get_by_type(cmd->opts, opt_sid)) && !tsk_strnullORempty(opt->value)) { /* --sid option */
            ret = session_hangup(tsk_atoll(opt->value));
        }
        else {
            TSK_DEBUG_ERROR("++hangup command need --sid option");
            ret = -1;
        }
        break;
    }
    case cmd_help: {
        TSK_DEBUG_INFO("command=help");
        cmd_print_help();
        break;
    }
    case cmd_hold: {
        const opt_t* opt;
        TSK_DEBUG_INFO("command=hold");
        if((opt = opt_get_by_type(cmd->opts, opt_sid)) && !tsk_strnullORempty(opt->value)) { /* --sid option */
            invite_handle_cmd(cmd->type, cmd->opts);
        }
        else {
            TSK_DEBUG_ERROR("++hold command need --sid option");
            ret = -1;
        }
        break;
    }
    case cmd_message: {
        TSK_DEBUG_INFO("command=message");
        if((sid = message_handle_cmd(cmd->type, cmd->opts)) != TSIP_SSESSION_INVALID_ID) {
            if(cmd->sidparam) {
                tsk_itoa(sid, &istr);
                update_param(cmd->sidparam, istr);
            }
        }
        break;
    }

    case cmd_options: {
        TSK_DEBUG_INFO("command=options");
        if((sid = options_handle_cmd(cmd->type, cmd->opts)) != TSIP_SSESSION_INVALID_ID) {
            if(cmd->sidparam) {
                tsk_itoa(sid, &istr);
                update_param(cmd->sidparam, istr);
            }
        }
        break;
    }
    case cmd_publish: {
        TSK_DEBUG_INFO("command=publish");
        if((sid = publish_handle_cmd(cmd->type, cmd->opts)) != TSIP_SSESSION_INVALID_ID) {
            if(cmd->sidparam) {
                tsk_itoa(sid, &istr);
                update_param(cmd->sidparam, istr);
            }
        }
        break;
    }
    case cmd_register: {
        TSK_DEBUG_INFO("command=register");
        if((sid = register_handle_cmd(cmd->type, cmd->opts)) != TSIP_SSESSION_INVALID_ID) {
            if(cmd->sidparam) {
                tsk_itoa(sid, &istr);
                update_param(cmd->sidparam, istr);
            }
        }
        break;
    }
    case cmd_resume: {
        const opt_t* opt;
        TSK_DEBUG_INFO("command=resume");
        if((opt = opt_get_by_type(cmd->opts, opt_sid)) && !tsk_strnullORempty(opt->value)) { /* --sid option */
            invite_handle_cmd(cmd->type, cmd->opts);
        }
        else {
            TSK_DEBUG_ERROR("++resume command need --sid option");
            ret = -1;
        }
        break;
    }
    case cmd_run: {
        TSK_DEBUG_INFO("command=run");
        ret = stack_run(cmd->opts);
        break;
    }
    case cmd_scenario: {
        TSK_DEBUG_INFO("command=scenario");
        break;
    }
    case cmd_sleep: {
        const opt_t* opt;
        double seconds;

        tsk_safeobj_unlock(ctx); /* beacuse of callback function */

        if((opt = opt_get_by_type(cmd->opts, opt_sec)) && !tsk_strnullORempty(opt->value)) { /* --sec option */
            seconds = strtod(opt->value, tsk_null); /* strtod() is better than atof() */
            if(seconds<=0) {
                printf("\n==== Press ENTER to continue...\n");
                getchar();
            }
            else {
                TSK_DEBUG_INFO("Sleeping %f seconds", seconds);
                tsk_thread_sleep((uint64_t)(seconds * 1000));
            }
        }
        else {
            TSK_DEBUG_WARN("++sleep need --sec option.");
        }
        return 0; /* bail: will unlock again */
    }
    case cmd_sms: {
        TSK_DEBUG_INFO("command=sms");
        if((sid = message_handle_cmd(cmd->type, cmd->opts)) != TSIP_SSESSION_INVALID_ID) {
            if(cmd->sidparam) {
                tsk_itoa(sid, &istr);
                update_param(cmd->sidparam, istr);
            }
        }
        break;
    }
    case cmd_stop: {
        TSK_DEBUG_INFO("command=stop");
        tsip_stack_stop(ctx->stack);
        break;
    }
    case cmd_subscribe: {
        TSK_DEBUG_INFO("command=subscribe");
        if((sid = subscribe_handle_cmd(cmd->type, cmd->opts)) != TSIP_SSESSION_INVALID_ID) {
            if(cmd->sidparam) {
                tsk_itoa(sid, &istr);
                update_param(cmd->sidparam, istr);
            }
        }
        break;
    }
    case cmd_video: {
        TSK_DEBUG_INFO("command=video");
        break;
    }
    default: {
        TSK_DEBUG_ERROR("%d not a valid command.", cmd);
        break;
    }
    }

bail:
    tsk_safeobj_unlock(ctx);

    return ret;
}
