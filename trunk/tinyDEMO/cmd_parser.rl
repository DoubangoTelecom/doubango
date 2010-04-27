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
#include "cmd_parser.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_ragel_state.h"

#include <string.h>

#define DEBUG_PARSER 0

/* this function will dup if exist, which isn't the case of  tsk_options_add_option_2*/
int add_option(tsk_options_L_t **options, tsk_option_t** option)
{
	if(!*options){
		*options = tsk_list_create();
	}
	if(*option){
		tsk_list_push_back_data(*options, (void**)option);
	}
	return 0;
}

/***********************************
*	Ragel state machine.
*/
%%{
	machine demo_machine_parser_cmd;

	action tag{
		tag_start = p;
#if DEBUG_PARSER
		TSK_DEBUG_INFO("tag=%s", tag_start);
#endif
	}
	
	action create_option{
#if DEBUG_PARSER
		TSK_DEBUG_INFO("create_option");
#endif
		TSK_OBJECT_SAFE_FREE(curr_opt);
		curr_opt = tsk_option_create_null();
	}

	action add_option{
#if DEBUG_PARSER
		TSK_DEBUG_INFO("add_option");
#endif
		add_option(&options, &curr_opt);
	}

	action set_value{
		if(curr_opt){
			TSK_PARSER_SET_STRING(curr_opt->value);
			tsk_strtrim_both(&curr_opt->value);
#if DEBUG_PARSER
			TSK_DEBUG_INFO("set_value %d %s", curr_opt->id, curr_opt->value);
#endif
		}
	}

	action is_comment{
#if DEBUG_PARSER
		TSK_DEBUG_INFO("is_comment");
#endif
		*comment = tsk_true;
		TSK_OBJECT_SAFE_FREE(options);
	}

	#action contains_hyphen{
	#	toto(tag_start)
	#}

	action option_error{
		TSK_PARSER_SET_STRING(temp);
		TSK_DEBUG_ERROR("%s not a valid option.", temp);
	}

	action command_error{
		TSK_PARSER_SET_STRING(temp);
		TSK_DEBUG_ERROR("%s not a valid command.", temp);
	}

	SP = " ";
	LF = "\n";
	CR = "\r";
	comment = SP*<: "#" any*; #%is_comment only if start with comment
	endline = ("\n" | "\r");
	equal = "=";
	hyphens = "-" {2,};
	plusplus = "++";

	command = (("audio"i | "a"i) %{ *cmd = cmd_audio; } |
	("audiovideo"i | "av"i) %{ *cmd = cmd_audiovideo; } |
	("config-session"i | "css"i) %{ *cmd = cmd_config_session; } |
	("config-stack"i | "cst"i) %{ *cmd = cmd_config_stack; } |
	("dump"i | "d"i) %{ *cmd = cmd_dump; } |
	("exit"i | "e"i) %{ *cmd = cmd_exit; } |
	("quit"i | "q"i) %{ *cmd = cmd_quit; } |
	("file"i | "f"i) %{ *cmd = cmd_file; } |
	("hangup"i | "hp"i) %{ *cmd = cmd_hangup; } |
	("help"i | "h"i) %{ *cmd = cmd_help; } |
	("message"i | "m"i) %{ *cmd = cmd_message; } |
	("publish"i | "pub"i) %{ *cmd = cmd_publish; } |
	("register"i | "reg"i) %{ *cmd = cmd_register; } |
	("run"i | "r"i) %{ *cmd = cmd_run; } |
	("scenario"i | "sn"i) %{ *cmd = cmd_scenario; } |
	"sleep"i %{ *cmd = cmd_sleep; } |
	"sms"i %{ *cmd = cmd_sms; } |
	("subscribe"i | "sub"i) %{ *cmd = cmd_subscribe; } |
	("video"i | "v"i) %{ *cmd = cmd_video; }
	)** > 10 |
	(any*) > 0 %command_error;

	key = ("amf"i % { curr_opt->id = opt_amf; } |
	"caps"i % { curr_opt->id = opt_caps; } |
	"dhcpv4"i % { curr_opt->id = opt_dhcpv4; } |
	"dhcpv6"i % { curr_opt->id = opt_dhcpv6; } |
	"dname"i % { curr_opt->id = opt_amf; } |
	"dns-naptr"i % { curr_opt->id = opt_dname; } |
	("expires"i | "xp"i) % { curr_opt->id = opt_expires; } |
	"from"i % { curr_opt->id = opt_from; } |
	"header"i % { curr_opt->id = opt_header; } |
	"impi"i % { curr_opt->id = opt_impi; } |
	"impu"i % { curr_opt->id = opt_impu; } |
	"ipv6"i % { curr_opt->id = opt_ipv6; } |
	"local-ip"i % { curr_opt->id = opt_local_ip; } |
	"local-port"i % { curr_opt->id = opt_local_port; } |	
	"opid"i % { curr_opt->id = opt_opid; } |
	("password"i | "pwd"i) % { curr_opt->id = opt_password; } |
	"path"i % { curr_opt->id = opt_path; } |
	("payload"i | "pay"i) % { curr_opt->id = opt_payload; } |
	"pcscf-ip"i % { curr_opt->id = opt_pcscf_ip; } |
	"pcscf-port"i % { curr_opt->id = opt_pcscf_port; } |	
	"pcscf-trans"i % { curr_opt->id = opt_pcscf_trans; } |
	"realm"i % { curr_opt->id = opt_realm; } |
	"sec"i % { curr_opt->id = opt_sec; } |
	"sid"i % { curr_opt->id = opt_sid; } |
	"sigcomp"i % { curr_opt->id = opt_sigcomp; } |
	"to"i % { curr_opt->id = opt_to; }
	)** > 10 |
	(any*) > 0 %option_error;

	value = any*>tag %set_value;
	option = key>tag:>SP* <:value;

	Command_Line = ( (comment>tag %is_comment | (plusplus command>tag :> SP*)) (hyphens <: (option>1 >create_option %add_option | comment>2))* );

	main := Command_Line :>> (CR | LF)?;
}%%

tsk_options_L_t *cmd_parser_parse(const char *buffer, cmd_type_t* cmd, tsk_bool_t *comment)
{
	int cs = 0;
	const char *p = buffer;
	const char *pe;
	const char *eof;
	tsk_option_t* curr_opt = tsk_null;
	tsk_options_L_t *options = tsk_null;
	char* temp = tsk_null;
	int index;
	size_t size = tsk_strlen(buffer);
	
	const char *tag_start;
	
	/* global vars and initilization (Ragel) */
	%%write data;
	%%write init;
	
	/* default values */
	*cmd = cmd_none;
	*comment = tsk_false;

	/* only parse one line */
	if((index = tsk_strindexOf(p, size, "\n")) != -1){
		pe =  eof = (p + index);
	}
	else{
		pe = eof = p + size;
	}
	
	/* exec */
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse [%s] command-Line.", buffer);
		TSK_OBJECT_SAFE_FREE(options);
		*cmd = cmd_none;
	}
	TSK_OBJECT_SAFE_FREE(curr_opt);
	TSK_FREE(temp);
	
	return options;
}

