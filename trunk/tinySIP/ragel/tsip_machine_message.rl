/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
/**@file tsip_machine_message.rl
 * @brief Ragel file.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
%%{
	machine tsip_machine_message;

	message_body = OCTET*;
	SIP_Version = ("SIP"i "/" DIGIT+ "." DIGIT+) >tag %parse_sipversion;
	
	message_header = any+ >tag :>CRLF %parse_header;

	# SIP RESPONSE
	Reason_Phrase = ( reserved | unreserved | escaped | UTF8_NONASCII | UTF8_CONT | SP | HTAB )*;
	Status_Code = DIGIT{3};
	Status_Line = SIP_Version SP Status_Code SP Reason_Phrase CRLF;
	Response = Status_Line message_header* CRLF message_body?;


	# SIP REQUEST
	Method = alpha+ >tag %parse_method;
	Request_URI = any+ >tag %parse_requesturi;
	Request_Line = Method :>SP Request_URI :>SP SIP_Version :>CRLF;
	Request = Request_Line message_header* :>CRLF <:message_body? >tag %parse_body;

	# SIP MESSAGE
	SIP_message = (Response | Request) @eob;

}%%