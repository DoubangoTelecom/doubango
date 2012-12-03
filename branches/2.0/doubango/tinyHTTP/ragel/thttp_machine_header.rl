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
/**@file thttp_machine_headers.rl.
 * @brief Ragel file.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */

/*== Header pasrsing machine. Supports both full-length and compact mode. */
%%{
	machine thttp_machine_header;
	
	Accept = "Accept"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Accept;
	Accept_Charset = "Accept-Charset"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Accept_Charset;
	Accept_Encoding = "Accept-Encoding"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Accept_Encoding;
	Accept_Language = "Accept-Language"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Accept_Language;
	Allow = "Allow"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Allow;
	Authorization = "Authorization"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Authorization;
	Cache_Control = "Cache-Control"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Cache_Control;
	Connection = "Connection"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Connection;
	Content_Encoding = ("Content-Encoding"i | "e") SP* HCOLON SP*<: any* :>CRLF  @parse_header_Content_Encoding;
	Content_Language = "Content-Language"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Content_Language;
	Content_Length = "Content-Length"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Content_Length;
	Content_Location = "Content-Location"i  SP* HCOLON SP*<: any* :>CRLF  @parse_header_Content_Location;
	Content_MD5 = "Content-MD5"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Content_MD5;
	Content_Range = "Content-Range"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Content_Range;
	Content_Type = ("Content-Type"i | "c") SP* HCOLON SP*<: any* :>CRLF  @parse_header_Content_Type;
	Date = "Date"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Date;
	ETag = "ETag"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_ETag;
	Expect = "Expect"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Expect;
	Expires = "Expires"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Expires;
	From = "From"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_From;
	Host = "Host"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Host;
	If_Match = "If-Match"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_If_Match;
	If_Modified_Since = "If-Modified-Since"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_If_Modified_Since;
	If_None_Match = "If-None-Match"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_If_None_Match;
	If_Range = "If-Range"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_If_Range;
	If_Unmodified_Since = "If-Unmodified-Since"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_If_Unmodified_Since;
	Last_Modified = "Last-Modified"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Last_Modified;
	Max_Forwards = "Max-Forwards"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Max_Forwards; 
	Pragma = "Pragma"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Pragma;
	Proxy_Authenticate = "Proxy-Authenticate"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Proxy_Authenticate;
	Proxy_Authorization = "Proxy-Authorization"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Proxy_Authorization;
	Range = "Range"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Range;
	Referer = "Referer"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Referer;
	Transfer_Encoding = "Transfer-Encoding"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Transfer_Encoding;
	TE = "TE"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_TE;
	Trailer = "Trailer"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Trailer;
	Upgrade = "Upgrade"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Upgrade;
	User_Agent = "User-Agent"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_User_Agent;
	Via = "Via"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Via;
	Warning = "Warning"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Warning;
	WWW_Authenticate = "WWW-Authenticate"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_WWW_Authenticate;
	
	# WebSocket
	Sec_WebSocket_Accept = "Sec-WebSocket-Accept"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Sec_WebSocket_Accept;
	Sec_WebSocket_Key = "Sec-WebSocket-Key"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Sec_WebSocket_Key;
	Sec_WebSocket_Protocol = "Sec-WebSocket-Protocol"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Sec_WebSocket_Protocol;
	Sec_WebSocket_Version = "Sec-WebSocket-Version"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Sec_WebSocket_Version;
	

	######
	extension_header = (token) SP* HCOLON SP*<: any* :>CRLF  @parse_header_extension_header;
	
	general_header = Cache_Control | Connection | Date | Pragma | Trailer | Transfer_Encoding | Upgrade | Via | Warning;
	auth_header = Authorization | WWW_Authenticate | Proxy_Authorization | Proxy_Authenticate;
	request_header = Accept | Accept_Charset | Accept_Encoding | Accept_Language | Expect | From | Host | If_Match | If_Modified_Since | If_None_Match | If_Range | If_Unmodified_Since | Max_Forwards | Range | Referer | TE | User_Agent;
	entity_header = Allow | Content_Encoding | Content_Language | Content_Length | Content_Location | Content_MD5 | Content_Range | Content_Type | ETag | Expires | Last_Modified;
	ws_header = Sec_WebSocket_Accept | Sec_WebSocket_Key | Sec_WebSocket_Protocol | Sec_WebSocket_Version;

	HEADER = (general_header | auth_header | request_header | entity_header | ws_header)@1 | extension_header@0;
}%%