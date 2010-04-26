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

#include "cmd_parser.h"

#include "tsk_string.h"
#include "tsk_debug.h"

#define PAD "  "

/* parse a command line */
tsk_options_L_t* cmd_parse(char* buffer, cmd_type_t* cmd)
{
	tsk_options_L_t* options;
	
	options = cmd_parser_parse(buffer, cmd);

	return options;
}

/* print usage */
void cmd_print_help()
{
	printf("\n\n========================= Usage =========================\n\n");

	printf("Usage:\n");
	printf(PAD"++[command] --[options]\n\n");

	/* Commands */
	printf("Commands:\n--\n");
	printf(PAD"[++audio] or [++a"PAD"%s", "Make audio call\n--\n");
	printf(PAD"[++audiovideo] or [++av]"PAD"%s", "Make audio/video call\n--\n");
	printf(PAD"[++config-file] or [++cf]"PAD"%s", "Load options from config file\n--\n");
	printf(PAD"[++config-session] | [++css]"PAD"%s", "Configure an 3GPP IMS/LTE session\n--\n");
	printf(PAD"[++config-stack] | [++cst]"PAD"%s", "Configure an 3GPP IMS/LTE stack\n--\n");
	printf(PAD"[++exit] | [++e]"PAD"%s", "Exit the application\n--\n");
	printf(PAD"[++file] | [++f]"PAD"%s", "Send a file. The stack must be running (see ++run command). To abort the File transfer, use ++hangup.\n--\n");
	printf(PAD"[++hangup] | [++hp]"PAD"%s", "Hangup any SIP session (unREGISTER, unSUBSCRIBE, unPUBLISH, HangUp Call, Abort ...).\n--\n");
	printf(PAD"[++help] | [++h]"PAD"%s", "Print this help screen\n--\n");
	printf(PAD"[++message] | [++m]"PAD"%s", "Send Pager Mode IM. The stack must be running (see ++run command).\n\n");
	printf(PAD"[++publish] | [++pub]"PAD"%s", "Send PUBLISH message. The stack must be running (see ++run command). To unPUBLISH, use ++hanggup.\n--\n");
	printf(PAD"[++quit] | [++q]"PAD"%s", "Quit the application\n--\n");
	printf(PAD"[++run]"PAD"%s", "Start/Run the 3GPP IMS/LTE stack. Mandatory before starting to do anything.\n--\n");
	printf(PAD"[++sms]"PAD"%s", "Send Binary SMS (RP-DATA). The stack must be running (see ++run command).\n--\n");
	printf(PAD"[++subscribe] | [++sub]"PAD"%s", "Send SUBSCRIBE message. The stack must be running (see ++run command). To unSUBSCRIBE, use ++hangup.\n--\n");
	printf(PAD"[++video] or [++v]"PAD"%s", "Make video call\n--\n");

	printf("\n\n========================= =========================\n\n");
}