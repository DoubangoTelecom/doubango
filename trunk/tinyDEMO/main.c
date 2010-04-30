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
#include "publish.h"
#include "register.h"
#include "subscribe.h"

#include <stdio.h>
#include <string.h>

#define LINE_DELIM "\n \r\n"

/* === global variables === */
ctx_t* ctx = tsk_null;

const char* trim(const char*);
int insert(char* dest, size_t index, size_t dest_size, char* src, size_t src_size);
int update_param(const char* , const char* );
int execute(const cmd_t* );

/* === entry point === */
int main(int argc, char* argv[])
{
	char buffer[4096];
	cmd_t* cmd = tsk_null;
	tsk_bool_t comment = tsk_false;
	int ret;
	int i, index;
	const char* start = tsk_null, *end = tsk_null;
	
	/* Copyright */
	printf("Doubango Project\nCopyright (C) 2009 - 2010 Mamadou Diop \n\n");
	/* Initialize Network Layer ==> Mandatory */
	tnet_startup();
	/* Print Usage */
	//cmd_print_help();

	/* create user's ctx */
	if(!(ctx = ctx_create())){
		TSK_DEBUG_ERROR("Failed to create user's ctx.");
		goto bail;
	}

	/* reset the buffer */
	memset(buffer, '\0', sizeof(buffer));
	/* initial args */
	for(i=1 /* index zero contains the exe path */, index=0; i<argc; i++){
		if(index){
			buffer[index++] = ' ';
		}
		memcpy(&buffer[index], argv[i], tsk_strlen(argv[i]));
		index += tsk_strlen(argv[i]);
	}
	
	/* If initial args ==> parse it now */
	if(index){
		TSK_DEBUG_INFO("Initial command-line: %s", buffer);
		goto init_buffer;
	}

	while(gets(buffer)){
init_buffer:
		start = buffer;
		start = trim(start);
		end = start + tsk_strlen(start);
		if(start >= end){
			TSK_DEBUG_INFO("Empty buffer");
			continue;
		}
parse_buffer:
		TSK_OBJECT_SAFE_FREE(cmd); /* Free old value */
		cmd = cmd_parse(start, &comment, ctx->params);
		if(cmd){
			if(comment || cmd->type == cmd_none){
				goto nex_line;
			}
		}
		else{
			continue;
		}

		/* Load from scenario file? */
		if(cmd->type == cmd_scenario){
			FILE* file;
			const opt_t* opt;
			size_t read = 0;
			tsk_bool_t rm_lf = tsk_false;
			static char temp[2048];
			if((opt = opt_get_by_type(cmd->opts, opt_path)) && !tsk_strnullORempty(opt->value)){ /* --path option */
				if((file = fopen(opt->value, "r"))){
					memset(temp, '\0', sizeof(temp)), temp[0] = '\n';
					read = fread(temp+1, sizeof(uint8_t), sizeof(temp)-1, file);
					fclose(file), file = tsk_null;

					if(read == 0){
						TSK_DEBUG_ERROR("[%s] is empty.", opt->value);
						continue;
					}
					else if(read == sizeof(temp)-1){
						TSK_DEBUG_ERROR("Buffer too short.");
						
						continue;
					}
					read++; /* \n */
					/* repplace all '\' with spaces (easier than handling that in the ragel file) */
					for(i=0; ((size_t)i)<read; i++){
						if(temp[i] == '\\'){
							temp[i] = ' ';
							rm_lf = tsk_true;
						}
						else if(rm_lf && temp[i] == '\n'){
							temp[i] = ' ';
							rm_lf = tsk_false;
						}
					}
					
					/* insert scenario */
					if(insert(buffer, (end - start), sizeof(buffer), temp, read)){
						continue;
					}
					else{
						end += read;
						goto nex_line;
					}
				}
				else{
					TSK_DEBUG_ERROR("Failed to open scenario-file [%s].", opt->value);
				}
				continue;
			}
			else{
				TSK_DEBUG_ERROR("++scenario command must have --path option.");
				continue;
			}
		}
		
		/* execute current command */
		switch(cmd->type){
			case cmd_exit:
					TSK_DEBUG_INFO("Exit/Quit");
					goto bail;
			default:
				tsk_safeobj_lock(ctx);
				ret = execute(cmd);
				tsk_safeobj_unlock(ctx);
				break;
		}

		/* next line */
nex_line:
		if((index = tsk_strindexOf(start, (end - start), "\n")) !=-1){
			start += index;
			while((start < end) && isspace(*start)){
				start ++;
			}
			if((start + 2/*++*/) < end){
				goto parse_buffer; /* next line */
			}
			else{
				continue; /* wait for new commands */
			}
		}
	} /* while(buffer) */


bail:
	
	/* Free current command */
	TSK_OBJECT_SAFE_FREE(cmd);
	/* Destroy the user's ctx */
	TSK_OBJECT_SAFE_FREE(ctx);
	/* Uninitilize Network Layer */
	tnet_cleanup();

	getchar();

	return 0;
}

const char* trim(const char* str)
{
	while(isspace(*str)){
		str++;
	}
	return str;
}

int insert(char* dest, size_t index, size_t dest_size, char* src, size_t src_size)
{
	if(!dest || !dest_size || !src || !src_size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if((tsk_strlen(dest) + src_size + 1) >= dest_size){
		TSK_DEBUG_ERROR("Destination too short");
		return -3;
	}

	/* move old data */
	memmove((dest + index + src_size), (dest + index), src_size);
	/* copy new data */
	memcpy((dest + index), src, src_size);

	return 0;
}

int update_param(const char* pname, const char* pvalue)
{
	if(ctx->params && pname){
		const tsk_param_t* param;
		if((param = tsk_params_get_param_by_name(ctx->params, pname))){
			tsk_strupdate(&TSK_PARAM(param)->value, pvalue);
			return 0;
		}
		else{
			TSK_DEBUG_INFO("[%s] parameter does not exist", pname);
		}
	}
	else{
		TSK_DEBUG_ERROR("Invalid parameter");
	}
	return -1;
}

int execute(const cmd_t* cmd)
{
	int ret = 0;
	tsip_ssession_id_t sid;
	tsk_istr_t istr;

	if(!cmd){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	switch(cmd->type){
		case cmd_audio:
			{
				TSK_DEBUG_INFO("command=audio");
				break;
			}
		case cmd_config_session:
			{
				TSK_DEBUG_INFO("command=config-session");
				break;
			}
		case cmd_config_stack:
			{
				TSK_DEBUG_INFO("command=config-satck");
				ret = stack_config(cmd->opts);
				break;
			}
		case cmd_dump:
			{
				TSK_DEBUG_INFO("command=dump");
				ret = stack_dump();
				break;
			}
		case cmd_exit:
			{
				TSK_DEBUG_INFO("command=exit");
				goto bail;
				break;
			}
		case cmd_file:
			{
				TSK_DEBUG_INFO("command=file");
				break;
			}
		case cmd_hangup:
			{
				const opt_t* opt;
				TSK_DEBUG_INFO("command=hangup");
				if((opt = opt_get_by_type(cmd->opts, opt_sid)) && !tsk_strnullORempty(opt->value)){ /* --sid option */
					ret = session_hangup(tsk_atoll(opt->value));
				}
				else{
					TSK_DEBUG_ERROR("++hangup command need --sid option");
					ret = -1;
				}
				break;
			}
		case cmd_help:
			{
				TSK_DEBUG_INFO("command=help");
				cmd_print_help();
				break;
			}
		case cmd_message:
			{
				TSK_DEBUG_INFO("command=message");
				if((sid = message_handle_cmd(cmd->type, cmd->opts)) != TSIP_SSESSION_INVALID_ID){
					if(cmd->sidparam){
						tsk_itoa(sid, &istr);
						update_param(cmd->sidparam, istr);
					}
				}
				break;
			}
		case cmd_publish:
			{
				TSK_DEBUG_INFO("command=publish");
				break;
			}
		case cmd_register:
			{
				TSK_DEBUG_INFO("command=register");
				if((sid = register_handle_cmd(cmd->type, cmd->opts)) != TSIP_SSESSION_INVALID_ID){
					if(cmd->sidparam){
						tsk_itoa(sid, &istr);
						update_param(cmd->sidparam, istr);
					}
				}
				break;
			}
		case cmd_run:
			{
				TSK_DEBUG_INFO("command=run");
				ret = stack_run(cmd->opts);
				break;
			}
		case cmd_scenario:
			{
				TSK_DEBUG_INFO("command=scenario");
				break;
			}
		case cmd_sleep:
			{
				const opt_t* opt;
				double seconds;
				if((opt = opt_get_by_type(cmd->opts, opt_sec)) && !tsk_strnullORempty(opt->value)){ /* --sec option */
					seconds = atof(opt->value);
					TSK_DEBUG_INFO("Sleeping %f seconds", seconds);
					tsk_thread_sleep((uint64_t)(seconds*1000));
				}
				else{
					TSK_DEBUG_WARN("++sleep need --sec option.");
				}
				break;
			}
		case cmd_sms:
			{
				TSK_DEBUG_INFO("command=sms");
				break;
			}
		case cmd_stop:
			{
				TSK_DEBUG_INFO("command=stop");
				tsip_stack_stop(ctx->stack);
				break;
			}
		case cmd_subscribe:
			{
				TSK_DEBUG_INFO("command=subscribe");
				if((sid = subscribe_handle_cmd(cmd->type, cmd->opts)) != TSIP_SSESSION_INVALID_ID){
					if(cmd->sidparam){
						tsk_itoa(sid, &istr);
						update_param(cmd->sidparam, istr);
					}
				}
				break;
			}
		case cmd_video:
			{
				TSK_DEBUG_INFO("command=video");
				break;
			}
		default:
			{
				TSK_DEBUG_ERROR("%d not a valid command.", cmd);
				break;
			}
	}

bail:
	return ret;
}
