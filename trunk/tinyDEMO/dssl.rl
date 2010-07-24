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
#include "dssl.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_ragel_state.h"

#include <string.h>

#define DEBUG_PARSER 0

#define set_level(_lv) if(opt) opt->lv = _lv;

char* replace_param(char* pivot, int index_1, int index_2, tsk_params_L_t* params)
{
	char* pname = tsk_null;
	char* ret = tsk_null;
	const tsk_param_t* p;
	//tsk_size_t len = tsk_strlen(pivot);
	//tsk_bool_t parenthesis = tsk_false;
	int i1, i2;
	
	/* whether there are parenthesis around the param */
	if((i1 = tsk_strindexOf((pivot + index_1), tsk_strlen((pivot + index_1)), "(")) == 2
		&& (i2 = tsk_strindexOf((pivot + index_1 + i1), tsk_strlen((pivot + index_1 + i1)), ")")) != -1)
	{
		pname = tsk_strndup((pivot + index_1 + i1 + 1), (i2 - i1 + 1));
		index_2 = index_1 + i1 + i2 + 1;
	}
	else{
		pname = tsk_strndup((pivot + index_1 + 2/*$$*/), (index_2 - index_1 - 2));
	}

	if((p = tsk_params_get_param_by_name(params, pname))){
		tsk_strncat(&ret, pivot, index_1);
		tsk_strncat(&ret, p->value, tsk_strlen(p->value));
		tsk_strncat(&ret, (pivot + index_2), (tsk_strlen(pivot) - index_2));
	}
	else{
		TSK_DEBUG_WARN("Failed to find param [%s]", pname);
	}

	TSK_FREE(pname);
	return ret;
}

tsk_bool_t next_not_(const char* p, const char* pe, char c)
{
	if((p && pe) && (pe-p) >=2){
		return (*p == c && *(p + 1) == c) ? tsk_false : tsk_true;
	}
	return tsk_true;
}
#define next_not_hyphens(p, pe) next_not_(p, pe, '-')
#define next_not_arobases(p, pe) next_not_(p, pe, '@')
#define next_not_percents(p, pe) next_not_(p, pe, '%')
#define next_not_redirs(p, pe) next_not_(p, pe, '>')

/***********************************
*	Ragel state machine.
*/
%%{
	machine demo_machine_parser_cmd;

	action tag{
		tag_start = p;
#if DEBUG_PARSER
		// - TSK_DEBUG_INFO("tag=%s", tag_start);
#endif
	}
	
	action create_option{
#if DEBUG_PARSER
		TSK_DEBUG_INFO("create_option");
#endif
		TSK_OBJECT_SAFE_FREE(opt);
		opt = opt_create_null();
	}

	action add_option{
#if DEBUG_PARSER
		TSK_DEBUG_INFO("add_option");
#endif
		tsk_list_push_back_data(cmd->opts, (void**)&opt);
	}

	action set_opt_value{
		if(opt){
			int index_1, index_2;
			char* newval = tsk_null;
			TSK_PARSER_SET_STRING(opt->value);
			/* trim both: left and right */
			tsk_strtrim_both(&opt->value);
			/* replace params */
replace:
			if((index_1 = tsk_strindexOf(opt->value, tsk_strlen(opt->value), "$$")) != -1){
				if((index_2 = tsk_strindexOf((opt->value + index_1), tsk_strlen((opt->value + index_1)), " ")) != -1){
					newval = replace_param(opt->value, index_1, (index_1 + index_2), params);
				}
				else{
					newval = replace_param(opt->value, index_1, tsk_strlen(opt->value), params);
				}
				if(newval){
					TSK_FREE(opt->value);
					opt->value = newval;

					/* again */
					goto replace;
				}
			}
#if DEBUG_PARSER
			TSK_DEBUG_INFO("set_opt_value [%d] '%s'", opt->type, opt->value);
#endif
		}
	}

	action set_sidparam_value{
		TSK_PARSER_SET_STRING(cmd->sidparam);
#if DEBUG_PARSER
			TSK_DEBUG_INFO("set_sidparam_value '%s'", cmd->sidparam);
#endif
	}

	action create_param{
#if DEBUG_PARSER
		TSK_DEBUG_INFO("create_param");
#endif
		TSK_OBJECT_SAFE_FREE(param);
		param = tsk_param_create_null();
	}

	action add_param{
#if DEBUG_PARSER
		TSK_DEBUG_INFO("add_param");
#endif
		tsk_params_add_param_2(&params, param);
		TSK_OBJECT_SAFE_FREE(param);
	}

	action set_param_value{
		if(param){
			TSK_PARSER_SET_STRING(param->value);
			/* trim both: left and right */
			tsk_strtrim_both(&param->value);
#if DEBUG_PARSER
			TSK_DEBUG_INFO("set_param_value [%s] '%s'", param->name, param->value);
#endif
		}
	}

	action set_param_name{
		if(param){
			TSK_PARSER_SET_STRING(param->name);
		}
	}

	action is_comment{
#if DEBUG_PARSER
		TSK_PARSER_SET_STRING(temp);
		TSK_DEBUG_INFO("is_comment '%s'", temp);
#endif
		// *comment = tsk_true;
		// TSK_OBJECT_SAFE_FREE(opts);
	}

	action next_not_reserved{
		next_not_hyphens(p, pe) 
			&& next_not_arobases(p, pe)
			&& next_not_percents(p, pe)
			&& next_not_redirs(p, pe)
			&& (p && *p != '\n')
			&& (p && *p != '#')
	}

	action next_not_newline{
		(p && *p != '\n')
	}	

	action option_error{
		TSK_PARSER_SET_STRING(temp);
		TSK_DEBUG_ERROR("'%s' is not a valid option.", temp);
	}

	action command_error{
		TSK_PARSER_SET_STRING(temp);
		TSK_DEBUG_ERROR("'%s' is not a valid command.", temp);
	}

	action level_error{
		TSK_PARSER_SET_STRING(temp);
		TSK_DEBUG_ERROR("'%s' is not a valid level.", temp);
	}

	SP = " ";
	LF = "\n";
	CR = "\r";
	endline = ("\n" | "\r");
	equal = "=";
	hyphens = "-" {2,};
	plusplus = "+" {2,};
	percents = "%" {2,};
	redirs = ">" {2,};
	sharp = "#";
	arobases = "@" {2,};

	command = (("audio"i | "a"i) %{ cmd->type = cmd_audio; } |
	("audiovideo"i | "av"i) %{ cmd->type = cmd_audiovideo; } |
	("config-session"i | "css"i) %{ cmd->type = cmd_config_session; } |
	("config-stack"i | "cst"i) %{ cmd->type = cmd_config_stack; } |
	("dtmf"i) %{ cmd->type = cmd_dtmf; } |
	("dump"i | "d"i) %{ cmd->type = cmd_dump; } |
	("ect"i) %{ cmd->type = cmd_ect; } |
	("exit"i | "e"i | "quit"i | "q"i) %{ cmd->type = cmd_exit; } |
	("file"i | "f"i) %{ cmd->type = cmd_file; } |
	("hangup"i | "hu"i) %{ cmd->type = cmd_hangup; } |
	("help"i | "h"i) %{ cmd->type = cmd_help; } |
	("hold"i | "ho"i) %{ cmd->type = cmd_hold; } |
	("large-message"i | "lm"i) %{ cmd->type = cmd_large_message; } |
	("message"i | "m"i) %{ cmd->type = cmd_message; } |
	("options"i | "opt"i) %{ cmd->type = cmd_options; } |
	("publish"i | "pub"i) %{ cmd->type = cmd_publish; } |
	("register"i | "reg"i) %{ cmd->type = cmd_register; } |
	("resume"i | "res"i) %{ cmd->type = cmd_resume; } |
	("run"i | "r"i) %{ cmd->type = cmd_run; } |
	("scenario"i | "sn"i) %{ cmd->type = cmd_scenario; } |
	"sleep"i %{ cmd->type = cmd_sleep; } |
	"sms"i %{ cmd->type = cmd_sms; } |
	"stop"i %{ cmd->type = cmd_stop; } |
	("subscribe"i | "sub"i) %{ cmd->type = cmd_subscribe; } |
	("video"i | "v"i) %{ cmd->type = cmd_video; }
	)** >10 |
	(any*) >tag >0 %command_error;

	key = ("amf"i % { opt->type = opt_amf; } |
	"caps"i % { opt->type = opt_caps; } |
	"dhcpv4"i % { opt->type = opt_dhcpv4; } |
	"dhcpv6"i % { opt->type = opt_dhcpv6; } |
	"dname"i % { opt->type = opt_amf; } |
	"dns-naptr"i % { opt->type = opt_dname; } |
	"event"i % { opt->type = opt_event; } |
	("expires"i | "xp"i) % { opt->type = opt_expires; } |
	"from"i % { opt->type = opt_from; } |
	"header"i % { opt->type = opt_header; } |
	"impi"i % { opt->type = opt_impi; } |
	"impu"i % { opt->type = opt_impu; } |
	"ipv6"i % { opt->type = opt_ipv6; } |
	"local-ip"i % { opt->type = opt_local_ip; } |
	"local-port"i % { opt->type = opt_local_port; } |	
	"opid"i % { opt->type = opt_opid; } |
	("password"i | "pwd"i) % { opt->type = opt_password; } |
	"path"i % { opt->type = opt_path; } |
	("payload"i | "pay"i) % { opt->type = opt_payload; } |
	"pcscf-ip"i % { opt->type = opt_pcscf_ip; } |
	"pcscf-port"i % { opt->type = opt_pcscf_port; } |	
	"pcscf-trans"i % { opt->type = opt_pcscf_trans; } |
	"realm"i % { opt->type = opt_realm; } |
	"sec"i % { opt->type = opt_sec; } |
	"sid"i % { opt->type = opt_sid; } |
	"sigcomp-id"i % { opt->type = opt_sigcomp_id; } |
	"silent"i % { opt->type = opt_silent; } |
	"smsc"i % { opt->type = opt_smsc; } |
	"stun-ip"i % { opt->type = opt_stun_ip; } |
	"stun-pwd"i % { opt->type = opt_stun_pwd; } |
	"stun-port"i % { opt->type = opt_stun_port; } |
	"stun-usr"i % { opt->type = opt_stun_usr; } |
	"to"i % { opt->type = opt_to; }
	)** >10 |
	(any*) >tag >0 %option_error;

	level_stack = ("stack"i | "st"i);
	level_session = ("session"i | "ss"i);
	level_action= ("action"i | "request"i | "a"i | "r"i);
	level = ( (level_stack %{ set_level(lv_stack); } | level_session %{ set_level(lv_session); } | level_action %{ set_level(lv_action); })** > 10 
		| (any*)>tag >0 %level_error ) :> space*;
	value = (any when next_not_reserved)*>tag;
	option = key>tag:>SP* <:value %set_opt_value (arobases level)?;
	
	pname = (any when next_not_newline)*>tag %set_param_name;
	pvalue = value;
	param = pname :>(space | '\n')+ <:pvalue %set_param_value;

	main := |*
				(plusplus command) >100 { };
				(hyphens option>create_option %add_option) >99 { };
				(percents param>create_param %add_param) >98 { };
				(redirs "("?<: value %set_sidparam_value :>")"?) >97 { };
				(sharp (any when next_not_newline)* >tag %is_comment) >96 { };
				any >0 { };
			*|;
}%%

cmd_t* dssl_parse(const char *buffer, tsk_size_t size, tsk_bool_t *comment, tsk_params_L_t* params)
{
	int cs = 0;
	const char *p = buffer;
	const char *pe;
	const char *eof;
	opt_t* opt = tsk_null;
	tsk_param_t* param = tsk_null;
	cmd_t *cmd = cmd_create_null();
	char* temp = tsk_null;
	int index;

	const char *ts = tsk_null, *te = tsk_null;
	int act = 0;
	
	const char *tag_start = tsk_null;
	
	/* global vars and initilization (Ragel) */
	%%write data;
	%%write init;
	
	/* default values */
	*comment = tsk_false;

	/* only parse one line */
	if((index = tsk_strindexOf(p, size, "\n")) != -1){
		pe =  eof = (p + index + 1/*\n*/);
	}
	else{
		pe = eof = p + size;
	}
	
	/* exec */
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse [%s] command-Line.", p);
		TSK_OBJECT_SAFE_FREE(cmd);
	}
	TSK_OBJECT_SAFE_FREE(opt);
	TSK_OBJECT_SAFE_FREE(param);
	TSK_FREE(temp);
	
	return cmd;
}

