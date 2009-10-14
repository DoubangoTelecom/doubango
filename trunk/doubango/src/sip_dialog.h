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

#ifndef __DOUBANGO_DIALOG_SM_H__
#define __DOUBANGO_DIALOG_SM_H__

#include "pref.h"
#include "api_sip_states.h"
#include "api_errors.h"

#include <sofia-sip/nua_tag.h>
#include <sofia-sip/nua.h>

PREF_NAMESPACE_START

class stack;

class DOUBANGO_API sip_dialog
{
public:
	sip_dialog(stack* stk);
	~sip_dialog();
	
	inline nua_handle_t* get_handle() const{ return this->handle; }
	inline void set_handle(nua_handle_t* h) { this->handle = h; }

	inline SIP_STATE get_state_current() const{ return this->state_current; }
	inline void set_state_current(SIP_STATE s) { this->state_current = s; }

	virtual ERR Start() = 0;
	virtual ERR Stop() = 0;
	virtual void OnStateChanged(SIP_STATE state);
	virtual inline const char* get_sipmethod()const = 0;
	virtual inline bool get_terminated()const = 0;
	virtual void dialog_callback(nua_event_t event,
			       int status, char const *phrase,
			       nua_t *nua, nua_magic_t *magic,
			       nua_handle_t *nh, nua_hmagic_t *hmagic,
			       sip_t const *sip,
			       tagi_t tags[]) = 0;


protected:
	void authenticate(nua_handle_t *nh, sip_t const *sip);

protected:
	nua_handle_t* handle;
	stack* stk;
	SIP_STATE state_current;
};

PREF_NAMESPACE_END

#endif /* __DOUBANGO_DIALOG_SM_H__ */