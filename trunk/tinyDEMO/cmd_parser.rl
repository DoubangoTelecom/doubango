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

/***********************************
*	Ragel state machine.
*/
%%{
	machine demo_machine_parser_cmd;

	action tag{
		tag_start = p;
	}
	
	action create_option{
		TSK_OBJECT_SAFE_FREE(curr_opt);
		curr_opt = tsk_option_create_null();
	}

	action add_option{
		tsk_options_add_option_2(&options, curr_opt);
		TSK_OBJECT_SAFE_FREE(curr_opt);
	}

	action set_value{
		if(curr_opt){
			TSK_PARSER_SET_STRING(curr_opt->value);
		}
	}

	action option_error{
		TSK_PARSER_SET_STRING(temp);
		TSK_DEBUG_ERROR("%s not a valid option.", temp);
	}

	action command_error{
		TSK_PARSER_SET_STRING(temp);
		TSK_DEBUG_ERROR("%s not a valid command.", temp);
	}

	equal = "=";
	hyphens = "--";
	plusplus = "++";

	command = (("audio" | "a") %{ *cmd = cmd_audio; } |
	("audiovideo" | "av") %{ *cmd = cmd_audiovideo; } |
	("config-file" | "cf") %{ *cmd = cmd_config_file; } |
	("config-session" | "css") %{ *cmd = cmd_config_session; } |
	("config-stack" | "cst") %{ *cmd = cmd_config_stack; } |
	("exit" | "e") %{ *cmd = cmd_exit; } |
	("quit" | "q") %{ *cmd = cmd_quit; } |
	("file" | "f") %{ *cmd = cmd_file; } |
	("hangup" | "hp") %{ *cmd = cmd_hangup; } |
	("help" | "h") %{ *cmd = cmd_help; } |
	("message" | "m") %{ *cmd = cmd_message; } |
	("publish" | "pub") %{ *cmd = cmd_publish; } |
	("register" | "reg") %{ *cmd = cmd_register; } |
	("run" | "r") %{ *cmd = cmd_run; } |
	"sms" %{ *cmd = cmd_sms; } |
	("subscribe" | "sub") %{ *cmd = cmd_subscribe; } |
	("video" | "v") %{ *cmd = cmd_video; }
	)** > 10 |
	(any*) > 0 %command_error;

	key = ("amf" % { curr_opt->id = opt_amf; } |
	"caps" % { curr_opt->id = opt_caps; } |
	"dhcpv4" % { curr_opt->id = opt_dhcpv4; } |
	"dhcpv6" % { curr_opt->id = opt_dhcpv6; } |
	"dname" % { curr_opt->id = opt_amf; } |
	"dns-naptr" % { curr_opt->id = opt_dname; } |
	"from" % { curr_opt->id = opt_from; } |
	"header" % { curr_opt->id = opt_header; } |
	"impi" % { curr_opt->id = opt_impi; } |
	"impu" % { curr_opt->id = opt_impu; } |
	"ipv6" % { curr_opt->id = opt_ipv6; } |
	"local-ip" % { curr_opt->id = opt_local_ip; } |
	"local-port" % { curr_opt->id = opt_local_port; } |	
	"opid" % { curr_opt->id = opt_opid; } |
	"password" % { curr_opt->id = opt_password; } |
	"pcscf-ip" % { curr_opt->id = opt_pcscf_ip; } |
	"pcscf-port" % { curr_opt->id = opt_pcscf_port; } |	
	"pcscf-trans" % { curr_opt->id = opt_pcscf_trans; } |
	"realm" % { curr_opt->id = opt_realm; } |
	"sid" % { curr_opt->id = opt_sid; } |
	"sigcomp" % { curr_opt->id = opt_sigcomp; } |
	"to" % { curr_opt->id = opt_to; }
	)** > 10 |
	(any*) > 0 %option_error;

	value = any*>tag %set_value;
	option = hyphens <: key>tag :>space* :>(equal | space)* <:value;


	Command_Line = plusplus command>tag :> space* (option>create_option %add_option :>space*)*;

	main := Command_Line;
}%%

tsk_options_L_t *cmd_parser_parse(const char *line, cmd_type_t* cmd)
{
	int cs = 0;
	const char *p = line;
	const char *pe = p + tsk_strlen(p);
	const char *eof = pe;
	tsk_option_t* curr_opt = tsk_null;
	tsk_options_L_t *options = tsk_null;
	char* temp = tsk_null;
	
	const char *tag_start;
	
	/* global vars and initilization (Ragel) */
	%%write data;
	%%write init;
	
	/* default values */
	*cmd = cmd_none;
	
	/* exec */
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse [%s] command-Line.", line);
		TSK_OBJECT_SAFE_FREE(options);
	}
	TSK_OBJECT_SAFE_FREE(curr_opt);
	TSK_FREE(temp);
	
	return options;
}

