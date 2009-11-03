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
#ifndef _DOUBANGO_SIP_STATES_H_
#define _DOUBANGO_SIP_STATES_H_

#include "pref.h"

PREF_NAMESPACE_START

/**
	SIP registration states
*/
typedef enum sip_state_registration_e
{
	srs_none,
	srs_trying,
	srs_authentifying,
	srs_unregistered,
	srs_registered
}
sip_state_registration_t;

typedef enum tag_SIP_STATE
{
	SS_UNKNOWN_UNKNOWN,

	/* REGISTER */
	SS_REGISTER_UNKNOWN,
	SS_REGISTER_INITIALIZED,
	SS_REGISTER_TRYING,
	SS_REGISTER_AUTHENTIFYING,
	SS_REGISTER_ESTABLISHED,
	SS_REGISTER_TERMINATED,

	/* PUBLISH */
	SS_PUBLISH_UNKNOWN,
	SS_PUBLISH_INITIALIZED,
	SS_PUBLISH_TRYING,
	SS_PUBLISH_AUTHENTIFYING,
	SS_PUBLISH_ESTABLISHED,
	SS_PUBLISH_TERMINATED,

	/* SUBSCRIBE */
	SS_SUBSCRIBE_UNKNOWN,
	SS_SUBSCRIBE_INITIALIZED,
	SS_SUBSCRIBE_TRYING,
	SS_SUBSCRIBE_AUTHENTIFYING,
	SS_SUBSCRIBE_ESTABLISHED,
	SS_SUBSCRIBE_NOTIFIED,
	SS_SUBSCRIBE_TERMINATED,

	/* MESSAGE */
	SS_MESSAGE_UNKNOWN,
	SS_MESSAGE_INITIALIZED,
	SS_MESSAGE_TRYING,
	SS_MESSAGE_AUTHENTIFYING,
	SS_MESSAGE_TERMINATED,
}
SIP_STATE;

PREF_NAMESPACE_END

#endif /* _DOUBANGO_SIP_STATES_H_ */