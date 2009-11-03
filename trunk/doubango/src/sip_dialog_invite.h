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

#ifndef __DOUBANGO_DIALOG_INVITE_SM_H__
#define __DOUBANGO_DIALOG_INVITE_SM_H__

#include "pref.h"
#include "sip_dialog.h"
#include "sm_dialog_invite_sm.h"

PREF_NAMESPACE_START

/* INVITE */
class sip_dialog_invite : public sip_dialog
{
public:
	sip_dialog_invite(stack* stk);
	~sip_dialog_invite();

private:
	sip_dialog_inviteContext sm_ctx;
};

PREF_NAMESPACE_END

#endif /* __DOUBANGO_DIALOG_INVITE_SM_H__ */