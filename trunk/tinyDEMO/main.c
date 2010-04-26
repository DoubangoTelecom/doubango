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
context_t* context = tsk_null;

/* === entry point === */
int main(int argc, char* argv[])
{
	char buffer[1024];
	tsk_options_L_t* options = tsk_null;
	cmd_type_t cmd;
	int ret;

	/* Copyright */
	printf("Doubango Project\nCopyright (C) 2009 - 2010 Mamadou Diop \n\n");
	/* Print Usage */
	cmd_print_help();

	/* create user's context */
	if(!(context = tsk_object_new(context_def_t))){
		TSK_DEBUG_ERROR("Failed to create user's context.");
		goto bail;
	}

//	const tsk_list_item_t* item;
//	const tsk_option_t* option;

	/*while((c = getchar())){
		printf("%c", c);
		if(c == 0x0A){
			printf("finished");
		}
	}*/
	while(gets(buffer)){
		char* a = tsk_strdup(buffer);
		char* line = strtok(buffer, LINE_DELIM);
		while(line){
			//options = cmd_parse("++config-stack --realm ims.inexbee.com --impi mamadou@ims.inexbee.com --impu sip:mamadou@ims.inexbee.com");
			if(!(options = cmd_parse(line/*"++config-stack --realm ims.inexbee.com --impi mamadou@ims.inexbee.com --impu sip:mamadou@ims.inexbee.com"*/, &cmd)) && cmd == cmd_none){
				goto next;
			}
			
			/* lock */
			tsk_safeobj_lock(context);

			switch(cmd){
				case cmd_audio:
					{
						TSK_DEBUG_INFO("command=audio");
						break;
					}
				case cmd_config_file:
					{
						TSK_DEBUG_INFO("command=config-file");
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
						ret = stack_config(options);
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
				case cmd_help:
					{
						TSK_DEBUG_INFO("command=help");
						cmd_print_help();
						break;
					}
				case cmd_message:
					{
						TSK_DEBUG_INFO("command=message");
						break;
					}
				case cmd_publish:
					{
						TSK_DEBUG_INFO("command=publish");
						break;
					}
				case cmd_quit:
					{
						TSK_DEBUG_INFO("command=quit");
						goto bail;
						break;
					}
				case cmd_register:
					{
						TSK_DEBUG_INFO("command=register");
						ret = register_handle_cmd(context, cmd, options);
						break;
					}
				case cmd_run:
					{
						TSK_DEBUG_INFO("command=run");
						stack_run(options);
						break;
					}
				case cmd_sms:
					{
						TSK_DEBUG_INFO("command=sms");
						break;
					}
				case cmd_subscribe:
					{
						TSK_DEBUG_INFO("command=subscribe");
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
		
			/* unlock */
			tsk_safeobj_unlock(context);

next:
			line = strtok (tsk_null, LINE_DELIM);
			TSK_OBJECT_SAFE_FREE(options);
		} /* while(line) */
	} /* while(buffer) */


bail:

	/* Destroy the user's context */
	TSK_OBJECT_SAFE_FREE(context);

	return 0;
}
