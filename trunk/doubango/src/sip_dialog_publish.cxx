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
#include "sip_dialog_publish.h"
#include "api_stack.h"

#include <sofia-sip/sip_protos.h>

PREF_NAMESPACE_START

/* sip_dialog_publish constructor*/
sip_dialog_publish::sip_dialog_publish(stack* stk)
:sip_dialog(stk),sm_ctx(*this)
{
}

/* sip_dialog_options destructor */
sip_dialog_publish::~sip_dialog_publish()
{
}


/* start */
ERR sip_dialog_publish::Start()
{
	this->sm_ctx.enterStartState();
	
	this->handle = nua_handle(this->stk->get_nua(), this->stk->get_home(),
	SIPTAG_TO_STR(this->stk->get_realm()), TAG_END());

	if(!this->handle)
	{
		return ERR_GLOBAL_FAILURE;
	}

	return this->sendPublish();
}

/* stop */
ERR sip_dialog_publish::Stop()
{
	ERR err = (this->state_current == SS_PUBLISH_ESTABLISHED) ? 
		this->sendUnpublish() : this->sendCancel();
	return err;
}

/* state changed */
void sip_dialog_publish::OnStateChanged(SIP_STATE state)
{
	sip_dialog::OnStateChanged(state);
#if 1
	if(this->get_terminated())
	{
		printf("PUBLISH terminated\n");
	}
#endif
}

/* sip method name */
inline const char* sip_dialog_publish::get_sipmethod()const
{
	return "PUBLISH";
}

/* returns true if terminated and false otherwise*/
inline bool sip_dialog_publish::get_terminated()const
{
	return (this->state_current == SS_PUBLISH_TERMINATED);
}

/* send SIP PUBLISH request */
ERR sip_dialog_publish::sendPublish()
{
	if(!this->handle) return ERR_SIP_DIALOG_UNKNOWN;

	bool open = true;
	const char* note = "my dummy note(FIXME)";
	char* xmlnote = su_sprintf(NULL, "<note>%s</note>\n", note);

	sip_payload_t *pl = sip_payload_format(NULL, 
						 "<?xml version='1.0' encoding='UTF-8'?>\n"
						 "<presence xmlns='urn:ietf:params:xml:ns:cpim-pidf'\n"
						 "          entity='%s'>\n"
						 "  <tuple id='%s'>\n"
						 "    <status><basic>%s</basic></status>\n"
						 "%s"
						 "  </tuple>\n"
						 "</presence>\n",
						 this->stk->get_public_id(), "FIXME", 
						 open ? "open" : "closed", 
						 xmlnote ? xmlnote : "");

	nua_publish(this->handle, SIPTAG_PAYLOAD(pl),
		NUTAG_M_USERNAME("FIXME"),
		SIPTAG_PRIVACY_STR(this->stk->get_privacy()),
		SIPTAG_FROM_STR(this->stk->get_public_id()),
		SIPTAG_TO_STR(this->stk->get_public_id()),
		NUTAG_M_FEATURES("expires=200"),// FIXME
		SIPTAG_EVENT_STR("presence"),
		TAG_IF(pl, SIPTAG_CONTENT_TYPE_STR("application/cpim-pidf+xml")), 
		TAG_END());

	this->sm_ctx.sm_publishSent();
	su_free(NULL, pl);

	return ERR_SUCCESS;
}

/* send SIP UNPUBLISH request*/
ERR sip_dialog_publish::sendUnpublish()
{
	if(this->handle)
	{
		nua_publish(this->handle, 
			NUTAG_M_FEATURES("expires=0"),// FIXME
			TAG_END());
		this->sm_ctx.sm_unpublishSent();

		return ERR_SUCCESS;
	}
	else return ERR_SIP_DIALOG_UNKNOWN;
}

/* cancel request */
ERR sip_dialog_publish::sendCancel()
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
void sip_dialog_publish::dialog_callback(nua_event_t _event,
			       int status, char const *phrase,
			       nua_t *nua, nua_magic_t *magic,
			       nua_handle_t *nh, nua_hmagic_t *hmagic,
			       sip_t const *sip,
			       tagi_t tags[])
{
	switch(_event)
	{
	case nua_r_publish:
	case nua_r_unpublish:
		{
			if(status <200) this->sm_ctx.sm_1xx_response();
			else if(status <300) this->sm_ctx.sm_2xx_response( (_event==nua_r_unpublish) );
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