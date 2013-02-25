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
/**@file thttp_machine_message.rl
 * @brief Ragel file.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
%%{
	machine thttp_machine_message;

	message_body = any*;
	HTTP_Version = ("HTTP"i "/" DIGIT+ "." DIGIT+)>tag %parse_httpversion;
	
	message_header = any+ >tag :>CRLF %parse_header;
	
	# HTTP RESPONSE
	Reason_Phrase = (( reserved | unreserved | escaped | UTF8_NONASCII | UTF8_CONT | SP | HTAB )*)>tag %parse_reason_phrase;
	Status_Line = HTTP_Version :>SP Status_Code>tag %parse_status_code :>SP Reason_Phrase :>CRLF;
	Response = Status_Line (message_header* :>CRLF);
	
	# HTTP REQUEST
	URI = (any+)>tag %parse_requesturl;
	Request_URI = URI;
	Request_Line = Method>tag %parse_method :>SP Request_URI :>SP HTTP_Version :>CRLF;
	Request = Request_Line (message_header* :>CRLF);

	# HTTP MESSAGE
	HTTP_message = (Response | Request)>1 @eoh message_body?>0;
}%%