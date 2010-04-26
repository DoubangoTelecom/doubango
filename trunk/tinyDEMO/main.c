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

#include "invite.h"
#include "message.h"
#include "publish.h"
#include "subscribe.h"
#include "cmd.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	char buffer[1024];
	tsk_options_L_t* options = tsk_null;
	const tsk_list_item_t* item;
	const tsk_option_t* option;

	/*while((c = getchar())){
		printf("%c", c);
		if(c == 0x0A){
			printf("finished");
		}
	}*/
	while(gets(buffer)){
		options = cmd_parse(buffer);
		tsk_list_foreach(item, options){
			option = item->data;
			switch(option->id){
				case cmd_type_realm:
					{	
						printf("realm=%s", option->value);
						break;
					}
				case cmd_type_impi:
					{
						printf("impi=%s", option->value);
						break;
					}
				case cmd_type_ipv6:
					{
						printf("ipv6=%s", option->value);
						break;
					}
			}
		}

		TSK_OBJECT_SAFE_FREE(options);
	}

	return 0;
}