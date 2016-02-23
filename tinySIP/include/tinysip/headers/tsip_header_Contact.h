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

/**@file tsip_header_Contact.h
 * @brief SIP header 'Contact' as per RFC 3261 subclause 20.1.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_CONTACT_H_
#define _TSIP_HEADER_CONTACT_H_

#include "tinysip_config.h"
#include "tinysip/tsip_uri.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS


////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'Contact'.
///
/// @par ABNF: Contact	= 	( "Contact" / "m" ) HCOLON ( STAR / (contact-param *(COMMA contact-param)))
/// contact-param	= 	(name-addr / addr-spec) *( SEMI contact-params)
/// contact-params	= 	c-p-q / c-p-expires / feature-param / temp-gruu / pub-gruu / c-p-instance / contact-extension
/// c-p-q	= 	"q" EQUAL qvalue
/// c-p-expires	= 	"expires" EQUAL delta-seconds
/// feature-param	= 	enc-feature-tag [ EQUAL   LDQUOT   (tag-value-list   /   string-value)   RDQUOT ]
/// enc-feature-tag	= 	base-tags / other-tags
/// base-tags	= 	"audio" / "automata" / "class" / "duplex" / "data" / "control" / "mobility" / "description" / "events" / "priority" / "methods" / "schemes" / "application" / "video" / "language" / "type" / "isfocus" / "actor" / "text" / "extensions"
/// other-tags	= 	"+"   ftag-name
/// ftag-name	= 	ALPHA *( ALPHA / DIGIT / "!" / "'" / "." / "-" / "%" )
/// tag-value-list	= 	tag-value *("," tag-value)
/// tag-value	= 	["!"] (token-nobang / boolean / numeric)
/// token-nobang	= 	1*( alphanum / "-" / "." / "%" / "*" / "_" / "+" / "`" / "'" / "~" )
/// boolean	= 	"TRUE" / "FALSE"
/// numeric	= 	"#" numeric-relation number
/// numeric-relation	= 	">=" / "<=" / "=" / (number ":")
/// number	= 	[ "+" / "-" ] 1*DIGIT ["." 0*DIGIT]
/// string-value	= 	"<" *(qdtext-no-abkt / quoted-pair ) ">"
/// qdtext-no-abkt	= 	LWS / %x21 / %x23-3B / %x3D / %x3F-5B / %x5D-7E / UTF8-NONASCII
/// temp-gruu	= 	"temp-gruu" EQUAL LDQUOT *( qdtext / quoted-pair ) RDQUOT
/// pub-gruu	= 	"pub-gruu" EQUAL LDQUOT *( qdtext / quoted-pair ) RDQUOT
/// c-p-instance	= 	"+sip.instance" EQUAL LDQUOT "<" instance-val ">" RDQUOT
/// instance-val	= 	*uric
/// contact-extension	= 	generic-param
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Contact_s {
    TSIP_DECLARE_HEADER;

    char *display_name;
    tsip_uri_t *uri;
    int64_t expires;
}
tsip_header_Contact_t;
typedef tsk_list_t tsip_header_Contacts_L_t;

TINYSIP_API tsip_header_Contact_t* tsip_header_Contact_create();

TINYSIP_API tsip_header_Contacts_L_t *tsip_header_Contact_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_Contact_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_CONTACT_H_ */

