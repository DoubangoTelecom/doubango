/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tsdp_header_O.h
 * @brief SDP "o=" header (Origin).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *
 * @date Created: Oat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSDP_HEADER_O_H_
#define _TSDP_HEADER_O_H_

#include "tinysdp_config.h"
#include "tinysdp/headers/tsdp_header.h"

TSDP_BEGIN_DECLS


#define TSDP_HEADER_O_VA_ARGS(username, sess_id, sess_version, nettype, addrtype, addr)			tsdp_header_O_def_t, (const char*)username, (uint32_t)sess_id, (uint32_t)sess_version, (const char*)nettype, (const char*)addrtype, (const char*)addr
#define TSDP_HEADER_O_VA_ARGS_DEFAULT(username, nettype, addrtype, addr)						TSDP_HEADER_O_VA_ARGS(username, TSDP_HEADER_O_SESS_ID_DEFAULT, TSDP_HEADER_O_SESS_VERSION_DEFAULT, nettype, addrtype, addr)

#define TSDP_HEADER_O_SESS_ID_DEFAULT		123456
#define TSDP_HEADER_O_SESS_VERSION_DEFAULT	678901

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SDP "o=" header (Origin).
///  The "o=" field gives the originator of the session (her username and
///     the address of the user's host) plus a session identifier and version number.
///
/// @par ABNF : u=username SP
/// sess-id SP sess-version SP nettype SP addrtype SP unicast-address
///
/// username	=  	non-ws-string 
/// sess-id	=  	1*DIGIT
/// sess-version	=  	1*DIGIT
/// nettype	=  	token 
/// addrtype	=  	token
/// unicast-address = FQDN
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsdp_header_O_s
{	
	TSDP_DECLARE_HEADER;

	/** <username> is the user's login on the originating host, or it is "-"
	if the originating host does not support the concept of user IDs.
	The <username> MUST NOT contain spaces.*/
	char* username;
	/** <sess-id>, <nettype>, <addrtype>, and <unicast-address> forms a
	globally unique identifier for the session.  The method of
	<sess-id> allocation is up to the creating tool, but it has been
	suggested that a Network Time Protocol (NTP) format timestamp be
	used to ensure uniqueness*/
	uint32_t sess_id;
	/** <sess-version> is a version number for this session description.  Its
	usage is up to the creating tool, so long as <sess-version> is
	increased when a modification is made to the session data.  Again,
	it is RECOMMENDED that an NTP format timestamp is used.*/
	uint32_t sess_version;
	/** <nettype> is a text string giving the type of network.  Initially
	"IN" is defined to have the meaning "Internet", but other values
	MAY be registered in the future (see Section 8 of RFC 4566)*/
	char* nettype;
	/**<addrtype> is a text string giving the type of the address that
	follows.  Initially "IP4" and "IP6" are defined, but other values
	MAY be registered in the future (see Section 8 of RFC 4566)*/
	char* addrtype;
	/** <unicast-address> is the address of the machine from which the
	session was created.  For an address type of IP4, this is either
	the fully qualified domain name of the machine or the dotted-
	decimal representation of the IP version 4 address of the machine.
	For an address type of IP6, this is either the fully qualified
	domain name of the machine or the compressed textual
	representation of the IP version 6 address of the machine.  For
	both IP4 and IP6, the fully qualified domain name is the form that
	SHOULD be given unless this is unavailable, in which case the
	globally unique address MAY be substituted.  A local IP address
	MUST NOT be used in any context where the SDP description might
	leave the scope in which the address is meaningful (for example, a
	local address MUST NOT be included in an application-level
	referral that might leave the scope)*/
	char* addr;
}
tsdp_header_O_t;

TINYSDP_API tsdp_header_O_t* tsdp_header_O_create(const char* username, uint32_t sess_id, uint32_t sess_version, const char* nettype, const char* addrtype, const char* addr);
TINYSDP_API tsdp_header_O_t* tsdp_header_O_create_null();
TINYSDP_API tsdp_header_O_t* tsdp_header_O_create_default(const char* username, const char* nettype, const char* addrtype, const char* addr);

TINYSDP_API tsdp_header_O_t *tsdp_header_O_parse(const char *data, tsk_size_t size);

TINYSDP_GEXTERN const tsk_object_def_t *tsdp_header_O_def_t;

TSDP_END_DECLS

#endif /* _TSDP_HEADER_O_H_ */

