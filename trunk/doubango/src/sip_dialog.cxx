///****************************************************************************
//			 _             _                             
//			| |           | |                            
//		  _ | | ___  _   _| | _   ____ ____   ____  ___  
//		 / || |/ _ \| | | | || \ / _  |  _ \ / _  |/ _ \ 
//		( (_| | |_| | |_| | |_) | ( | | | | ( ( | | |_| |
//		 \____|\___/ \____|____/ \_||_|_| |_|\_|| |\___/ 
//											(_____|   
//	
//	Copyright (C) 2009 xxxyyyzzz <imsframework(at)gmail.com>
//
//	This file is part of Open Source Doubango IMS Client Framework project.
//
//    DOUBANGO is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//	
//    DOUBANGO is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//	
//    You should have received a copy of the GNU General Public License
//    along with DOUBANGO.
//****************************************************************************/
#include "sip_dialog.h"
#include "api_stack.h"

#include <nua_stack.h>
#include <sofia-sip/sip_hclasses.h>
#include <sofia-sip/msg_header.h>

PREF_NAMESPACE_START

unsigned int sip_dialog::dialog_counter = 0;

/* sip_dialog constructor */
sip_dialog::sip_dialog(stack* _stk)
{
	this->handle = NULL;
	this->stk = _stk;
	this->dialog_id = (++sip_dialog::dialog_counter);
}

/* sip_dialog destructor*/
sip_dialog::~sip_dialog()
{
}

/* called when dialog state change*/
void sip_dialog::OnStateChanged(SIP_STATE state)
{
	this->state_current = state;
	SU_DEBUG_3(("%s::OnStateChanged ==> %d\n", this->get_sipmethod(), state));
}

/* authenticate the supplied request*/
void sip_dialog::authenticate(nua_handle_t *nh, sip_t const *sip)
{
	const char* realm = sip->sip_www_authenticate?
		msg_params_find(sip->sip_www_authenticate->au_params, "realm="): (sip->sip_proxy_authenticate ? msg_params_find(sip->sip_proxy_authenticate->au_params, "realm="): "UNKNOWN");
	const char* scheme = sip->sip_www_authenticate? 
		sip->sip_www_authenticate->au_scheme: (sip->sip_proxy_authenticate ? sip->sip_proxy_authenticate->au_scheme: "UNKNOWN");

	char* authstring = su_sprintf(NULL, "%s:%s:%s:%s", 
	scheme, realm, this->stk->get_private_id(), this->stk->get_password());

	nua_authenticate(nh, NUTAG_AUTH(authstring), TAG_END());

	su_free(NULL, authstring);
}

PREF_NAMESPACE_END