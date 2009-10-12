/*
	** Copyright (C) 2009 Doubango Telecom <imsframework(at)gmail.com> 

	This file is part of 'Doubango IMS Client Framework'.

    'Doubango IMS Client Framework' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'Doubango IMS Client Framework' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'Doubango IMS Client Framework'.  If not, see <http://www.gnu.org/licenses/>.

	** For commercial use, you must purchase a proprietary license from Doubango Telecom.
*/

/* Copyright (C) 2009 Doubango Telecom
*
* Authors:	F0076RTY <imsframework@gmail.com>
*
*/
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

	nua_register(this->handle,
	SIPTAG_PRIVACY_STR(this->stk->get_privacy()),
	NUTAG_OUTBOUND("no-options-keepalive"), 
	NUTAG_OUTBOUND("no-validate"), 
	NUTAG_KEEPALIVE(0),
	SIPTAG_EXPIRES_STR("20"),
	//NUTAG_M_USERNAME("doubango"),
	SIPTAG_FROM_STR(this->stk->get_public_id()),
	SIPTAG_TO_STR(this->stk->get_public_id()),
    NUTAG_M_FEATURES("audio"),
    NUTAG_CALLEE_CAPS(0),
	SIPTAG_SUPPORTED_STR("timer, precondition, path, replaces, 100rel, gruu"),


	//NUTAG_M_FEATURES("+g.3gpp.smsip;+g.3gpp.cs-voice"),
	//TAG_IF(this->gsma_vs, NUTAG_M_FEATURES("+g.3gpp.cs-voice")),

	TAG_END());
	this->sm_ctx.sm_registerSent();

	return ERR_SUCCESS;
}

/* stop */
ERR sip_dialog_register::Stop()
{
	if(this->handle)
	{
		nua_unregister(this->handle, TAG_END());
		this->sm_ctx.sm_unregisterSent();

		return ERR_SUCCESS;
	}
	else return ERR_SIP_DIALOG_UNKNOWN;
}

/* state changed */
void sip_dialog_register::OnStateChanged(SIP_STATE state)
{
	sip_dialog::OnStateChanged(state);
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
			else if(status <300) this->sm_ctx.sm_2xx_response();
			else if(status == 401 || status == 407 || status == 421 || status == 494) this->sm_ctx.sm_401_407_421_494_response();
			else this->sm_ctx.sm_unsupported_response();

			break;
		}

	default:
		{
			break;
		}
	}
}

PREF_NAMESPACE_END