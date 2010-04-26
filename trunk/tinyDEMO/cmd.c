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

#include "tsk_string.h"
#include "tsk_debug.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h> /* isspace() */

int cmp_parse_kvp(const char* kvp, char** key, char** value)
{
	return 0;
}

tsk_options_L_t* cmd_parse(char* buffer)
{
	tsk_options_L_t* options  = tsk_list_create();
	//char* cmd_name = strtok(buffer, "--");
	int32_t index = 0;
	size_t len = tsk_strlen(buffer);
	const char* space;

	if(len <= 2){
		TSK_DEBUG_ERROR("Command line too short.");
		goto bail;
	}

	if(buffer[0] != '-' || buffer[1] != '-'){
		TSK_DEBUG_ERROR("Command should start with \"--\".");
		goto bail;
	}

	do{
		while(space && isspace(*space)){ /* remove all spaces */
			space++;
		}
		if(space && ((space - buffer)>2) && (*space++ == '-' && *space++ == '-')){
			
		}
	} while((space = strstr(&buffer[index], " ")))
	
	

	//while(cmd_name){
	//	printf("cmd_name=%s\n", cmd_name);
	//	if((index = tsk_strindexOf(cmd_name, strlen(cmd_name), " ")) == -1){
	//		index = strlen(cmd_name);
	//	}
	//	
	//	/* add command as option */
	//	if(tsk_strniequals(cmd_name, "realm", index)){
	//		tsk_options_add_option(&options, cmd_type_realm, &cmd_name[index+1]);
	//	}
	//	
	//	/* next command */
	//	cmd_name = strtok(&buffer[tsk_strlen(cmd_name)], " --");
	//}

bail:
	return options;
}