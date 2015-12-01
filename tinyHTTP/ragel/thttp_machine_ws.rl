/*
* Copyright (C) 2010-2015 Mamadou Diop.
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
*/

%%{
	machine thttp_machine_ws;

	extension_param = token ( "=" ( token | quoted_string ) )?;
	NZDIGIT = "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
	version = DIGIT | ( NZDIGIT DIGIT ) | ( "1" DIGIT DIGIT ) | ( "2" DIGIT DIGIT );
	base64_character = ALPHA | DIGIT | "+" | "/";
	base64_data = base64_character{4};
	base64_padding = ( base64_character{2} "==" ) | ( base64_character{3} "=" );
	base64_value_non_empty = ( base64_data+ base64_padding? ) | base64_padding;
	Sec_WebSocket_Key = base64_value_non_empty;
	registered_token = token;
	extension_token = registered_token;
	extension = extension_token ( ";" extension_param )*;
	extension_list = extension ( COMMA extension )*;
	Sec_WebSocket_Extensions = extension_list;
	Sec_WebSocket_Protocol_Client = token ( COMMA token )*;
	Sec_WebSocket_Version_Client = version;
	Sec_WebSocket_Accept = base64_value_non_empty;
	Sec_WebSocket_Protocol_Server = token;
	Sec_WebSocket_Version_Server = version ( COMMA version )*;
	Sec_WebSocket_Version = version ( COMMA version )*;
	Sec_WebSocket_Protocol = token ( COMMA token )*;

}%%