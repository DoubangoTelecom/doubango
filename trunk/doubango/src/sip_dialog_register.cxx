/**
* @file
* @author  xxxyyyzzz <imsframework(at)gmail.com>
* @version 1.0
*
* @section LICENSE
*
*	
* This file is part of Open Source Doubango IMS Client Framework project.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*	
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
* @section DESCRIPTION
*
*
*/
#include "sip_dialog_register.h"
#include "api_stack.h"
#include "strutils.h"

PREF_NAMESPACE_START

/* sip_dialog_register constructor*/
sip_dialog_register::sip_dialog_register(stack* stk)
:sip_dialog(stk),sm_ctx(*this), state_current(srs_none)
{
	
}

/* sip_dialog_register destructor */
sip_dialog_register::~sip_dialog_register()
{
	// FIXME: destroy handle
}

/** 
	Initialize default parameters for initial REGISTER request according to
	3GPP TS 24.229 subclause 5.1.1.2.
*/
ERR sip_dialog_register::Start()
{
	this->sm_ctx.enterStartState();
	
	this->handle = nua_handle(this->stk->get_nua(), this->stk->get_home(),
	SIPTAG_FROM_STR(this->stk->get_public_id()),
	SIPTAG_TO_STR(this->stk->get_public_id()),
	//SIPTAG_TO_STR(this->stk->get_realm()),

	NUTAG_M_DISPLAY(this->stk->get_displayname()),
	SIPTAG_PRIVACY_STR(this->stk->get_privacy()),

	SIPTAG_EXPIRES_STR(itoa(this->stk->get_expires()).c_str()),
	NUTAG_OUTBOUND("no-options-keepalive"), 
	NUTAG_OUTBOUND("no-validate"),
	NUTAG_EARLY_IMS(this->stk->get_early_ims()?1:0),
	//NUTAG_KEEPALIVE(0),
	//NUTAG_M_FEATURES("audio;expires=20;+g.3gpp.cs-voice"),
	//NUTAG_M_USERNAME("FIXME"),
    NUTAG_CALLEE_CAPS(0),
	SIPTAG_SUPPORTED_STR("timer, precondition, path, replaces, 100rel, gruu, outbound"),
	SIPTAG_EVENT_STR("registration"),
	SIPTAG_ALLOW_EVENTS_STR("presence"),

	//NUTAG_M_FEATURES("+g.3gpp.smsip;+g.3gpp.cs-voice"),
	//TAG_IF(this->gsma_vs, NUTAG_M_FEATURES("+g.3gpp.cs-voice")),

	TAG_END());
	
	//su_free(NULL, features);

	if(!this->handle)
	{
		return ERR_GLOBAL_FAILURE;
	}

	return this->sendRegister();
}

/* stop */
ERR sip_dialog_register::Stop()
{
	ERR err = (this->state_current == srs_registered) ? 
		this->sendUnregister() : this->sendCancel();
	return err;
}

/* state changed */
void sip_dialog_register::OnStateChanged(sip_state_registration_t state)
{
	this->state_current = state;
#if 0
	switch(state)
	{
		case srs_none: printf("sip_dialog_register: %s\n", "srs_none"); break;
		case srs_trying: printf("sip_dialog_register: %s\n", "srs_trying"); break;
		case srs_authentifying: printf("sip_dialog_register: %s\n", "srs_authentifying"); break;
		case srs_unregistered: printf("sip_dialog_register: %s\n", "srs_unregistered"); break;
		case srs_registered: printf("sip_dialog_register: %s\n", "srs_registered"); break;
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
	return (this->state_current == srs_unregistered);
}

/* send SIP REGISTER request */
ERR sip_dialog_register::sendRegister()
{
	if(!this->handle) return ERR_SIP_DIALOG_UNKNOWN;
	
	/* register */
	nua_register(this->handle, TAG_END());

	/* alert state machine */
	this->sm_ctx.sm_registerSent();

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

			//
			//	Alert all subscriber that the registration state has changed
			//
			this->stk->registrationStateChanged(this->stk->get_id(), this->state_current, status, phrase);
			
			break;
		}

	default:
		{
			this->sm_ctx.sm_xxx_response();
			this->stk->registrationStateChanged(this->stk->get_id(), this->state_current, status, phrase);
			break;
		}
	}
}

PREF_NAMESPACE_END