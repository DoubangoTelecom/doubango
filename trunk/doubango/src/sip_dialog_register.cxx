/****************************************************************************
			 _             _                             
			| |           | |                            
		  _ | | ___  _   _| | _   ____ ____   ____  ___  
		 / || |/ _ \| | | | || \ / _  |  _ \ / _  |/ _ \ 
		( (_| | |_| | |_| | |_) | ( | | | | ( ( | | |_| |
		 \____|\___/ \____|____/ \_||_|_| |_|\_|| |\___/ 
											(_____|   
	
	Copyright (C) 2009 xxxyyyzzz <imsframework(at)gmail.com>

	This file is part of Open Source Doubango IMS Client Framework project.

    DOUBANGO is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
	
    DOUBANGO is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
	
    You should have received a copy of the GNU General Public License
    along with DOUBANGO.
****************************************************************************/
#include "sip_dialog_register.h"
#include "api_stack.h"

PREF_NAMESPACE_START

/* sip_dialog_register constructor*/
sip_dialog_register::sip_dialog_register(stack* stk)
:sip_dialog(stk),sm_ctx(*this)
{
	
}

/* sip_dialog_register destructor */
sip_dialog_register::~sip_dialog_register()
{
	// FIXME: destroy handle
}

/* start */
ERR sip_dialog_register::Start()
{
	this->sm_ctx.enterStartState();
	
	this->handle = nua_handle(this->stk->get_nua(), this->stk->get_home(),
	SIPTAG_TO_STR(this->stk->get_realm()), TAG_END());

	if(!this->handle)
	{
		return ERR_GLOBAL_FAILURE;
	}

	return this->sendRegister();
}

/* stop */
ERR sip_dialog_register::Stop()
{
	ERR err = (this->state_current == SS_REGISTER_ESTABLISHED) ? 
		this->sendUnregister() : this->sendCancel();
	return err;
}

/* state changed */
void sip_dialog_register::OnStateChanged(SIP_STATE state)
{
	sip_dialog::OnStateChanged(state);
#if 0
	if(this->get_terminated())
	{
		printf("REGISTER terminated\n");
	}
#endif
}

/* sip method name */
inline const char* sip_dialog_register::get_sipmethod()const
{
	return "REGISTER";
}

/* returns true if terminated and false otherwise*/
inline bool sip_dialog_register::get_terminated()const
{
	return (this->state_current == SS_REGISTER_TERMINATED);
}

/* send SIP REGISTER request */
ERR sip_dialog_register::sendRegister()
{
	if(!this->handle) return ERR_SIP_DIALOG_UNKNOWN;

	// FIXME: secure
	bool secure = false;
	char* ims_default_auth = su_sprintf(NULL, "Digest username=\"%s\",realm=\"%s\",nonce=\"\",uri=\"%s:%s\",response=\"\"",
		this->stk->get_private_id(), this->stk->get_realm(), secure?"sips":"sip",this->stk->get_realm());

	nua_register(this->handle,
	SIPTAG_PRIVACY_STR(this->stk->get_privacy()),
	NUTAG_OUTBOUND("no-options-keepalive"), 
	NUTAG_OUTBOUND("no-validate"), 
	NUTAG_KEEPALIVE(0),
	NUTAG_M_FEATURES("audio;expires=200;+g.3gpp.cs-voice"),// FIXME
	NUTAG_M_USERNAME("FIXME"),
	SIPTAG_FROM_STR(this->stk->get_public_id()),
	SIPTAG_TO_STR(this->stk->get_public_id()),
    NUTAG_CALLEE_CAPS(0),
	SIPTAG_SUPPORTED_STR("timer, precondition, path, replaces, 100rel, gruu"),
	SIPTAG_EVENT_STR("registration"),
	SIPTAG_ALLOW_EVENTS_STR("presence"),
	TAG_IF(!this->stk->get_early_ims(), SIPTAG_AUTHORIZATION_STR(ims_default_auth)), /* 3GPP TS 24.229. See section 5.1.1 */

	//NUTAG_M_FEATURES("+g.3gpp.smsip;+g.3gpp.cs-voice"),
	//TAG_IF(this->gsma_vs, NUTAG_M_FEATURES("+g.3gpp.cs-voice")),

	TAG_END());
	this->sm_ctx.sm_registerSent();

	su_free(NULL, ims_default_auth);
	return ERR_SUCCESS;
}

/* send SIP UNREGISTER request*/
ERR sip_dialog_register::sendUnregister()
{
	if(this->handle)
	{
		nua_unregister(this->handle, TAG_END());
		this->sm_ctx.sm_unregisterSent();

		return ERR_SUCCESS;
	}
	else return ERR_SIP_DIALOG_UNKNOWN;
}

/* cancel request */
ERR sip_dialog_register::sendCancel()
{
	if(this->handle)
	{
		nua_cancel(this->handle, TAG_END());
		this->sm_ctx.sm_cancelSent();

		return ERR_SUCCESS;
	}
	else return ERR_SIP_DIALOG_UNKNOWN;
}

/* dialog callback function*/
void sip_dialog_register::dialog_callback(nua_event_t _event,
			       int status, char const *phrase,
			       nua_t *nua, nua_magic_t *magic,
			       nua_handle_t *nh, nua_hmagic_t *hmagic,
			       sip_t const *sip,
			       tagi_t tags[])
{
	switch(_event)
	{
	case nua_r_register:
	case nua_r_unregister:
		{
			if(status <200) this->sm_ctx.sm_1xx_response();
			else if(status <300) this->sm_ctx.sm_2xx_response( (_event==nua_r_unregister) );
			else if(status <400) this->sm_ctx.sm_3xx_response();
			else if(status == 401 || status == 407 || status == 421 || status == 494) 
			{
				this->sm_ctx.sm_401_407_421_494_response();
				this->authenticate(nh, sip);
				this->sm_ctx.sm_authentificationSent();
			}
			else if(status<500) this->sm_ctx.sm_4xx_response();
			else if(status<600) this->sm_ctx.sm_5xx_response();
			else if(status<700) this->sm_ctx.sm_6xx_response();
			else this->sm_ctx.sm_xxx_response();
			break;
		}

	default:
		{
			break;
		}
	}
}

PREF_NAMESPACE_END