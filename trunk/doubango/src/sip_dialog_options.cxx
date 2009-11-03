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
#include "sip_dialog_options.h"

PREF_NAMESPACE_START

/* sip_dialog_message constructor*/
sip_dialog_options::sip_dialog_options(stack* stk)
:sip_dialog(stk),sm_ctx(*this)
{
}

/* sip_dialog_options destructor */
sip_dialog_options::~sip_dialog_options()
{
}

PREF_NAMESPACE_END