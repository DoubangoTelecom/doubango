/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
/**@file tsip_machine_utils.rl
 * @brief Ragel file.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
%%{

	machine tmsrp_machine_utils;
	
	OCTET = "0x"[0-9A-Fa-f]+;
	CHAR = 0x01..0x7f;
	VCHAR = 0x21..0x7e;
	ALPHA = 0x41..0x5a | 0x61..0x7a;
	DIGIT = 0x30..0x39;
	CTL = 0x00..0x1f | 0x7f;
	HTAB = "\t";
	LF = "\n";
	CR = "\r";
	SP = " ";
	DQUOTE = "\"";
	BIT = "0" | "1";
	HEXDIG = DIGIT | "A"i | "B"i | "C"i | "D"i | "E"i | "F"i;
	CRLF = CR LF;
	WSP = SP | HTAB;
	LWSP = ( WSP | ( CRLF WSP ) )*;
	LWS = ( WSP* CRLF )? WSP+;
	SWS = LWS?;
	EQUAL = SWS "=" SWS;
	LHEX = DIGIT | 0x61..0x66;
	HCOLON = ( SP | HTAB )* ":" SWS;
	separators = "(" | ")" | "<" | ">" | "@" | "," | ";" | ":" | "\\" | DQUOTE | "/" | "[" | "]" | "?" | "=" | "{" | "}" | SP | HTAB;
	STAR = SWS "*" SWS;
	SLASH = SWS "/" SWS;
	LPAREN = SWS "(" SWS;
	RPAREN = SWS ")" SWS;
	COMMA = SWS "," SWS;
	SEMI = SWS ";" SWS;
	COLON = SWS ":" SWS;
	LAQUOT = SWS "<";
	LDQUOT = SWS DQUOTE;
	RAQUOT = ">" SWS;
	RDQUOT = DQUOTE SWS;
	UTF8_CONT = 0x80..0xbf;
    ##### FIXME: UTF8_NONASCII up to 2bytes will fail on Android
	UTF8_NONASCII = ( 0x80..0xff );
	#UTF8_NONASCII = ( 0xc0..0xdf UTF8_CONT ) | ( 0xe0..0xef UTF8_CONT{2} ) | ( 0xf0..0xf7 UTF8_CONT{3} ) | ( 0xf8..0xfb UTF8_CONT{4} ) | ( 0xfc..0xfd UTF8_CONT{5} );	ctext = 0x21..0x27 | 0x2a..0x5b | 0x5d..0x7e | UTF8_NONASCII | LWS;
	qvalue = ( "0" ( "." DIGIT{,3} )? ) | ( "1" ( "." "0"{,3} )? );
	alphanum = ALPHA | DIGIT;
	token = ( alphanum | "-" | "." | "!" | "%" | "*" | "_" | "+" | "`" | "'" | "~" )+;
	ietf_token = token;
	x_token = "x-"i token;
	iana_token = token;
	token_nodot = ( alphanum | "-" | "!" | "%" | "*" | "_" | "+" | "`" | "'" | "~" )+;
	word = ( alphanum | "-" | "." | "!" | "%" | "*" | "_" | "+" | "`" | "'" | "~" | "(" | ")" | "<" | ">" | ":" | "\\" | DQUOTE | "/" | "[" | "]" | "?" | "{" | "}" )+;
	domainlabel = alphanum | ( alphanum ( alphanum | "-" )* alphanum );
	toplabel = ALPHA | ( ALPHA ( alphanum | "-" )* alphanum );
	hostname = ( domainlabel "." )* toplabel "."?;
	IPv4address = DIGIT{1,3} "." DIGIT{1,3} "." DIGIT{1,3} "." DIGIT{1,3};
	hex4 = HEXDIG{1,4};
	hexseq = hex4 ( ":" hex4 )*;
	hexpart = hexseq | ( hexseq "::" hexseq? ) | ( "::" hexseq? );
	IPv6address = hexpart ( ":" IPv4address )?;
	IPv6reference = "[" IPv6address "]";
	host = hostname | IPv4address | IPv6reference;
	qdtext = LWS | "!" | 0x23..0x5b | 0x5d..0x7e | UTF8_NONASCII;
	quoted_pair = "\\" ( 0x00..0x09 | 0x0b..0x0c | 0x0e..0x7f );
	quoted_string = SWS DQUOTE ( qdtext | quoted_pair )* DQUOTE;
	gen_value = token | host | quoted_string;
	generic_param = token ( EQUAL gen_value )?;
	accept_param = ( "q"i EQUAL qvalue ) | generic_param;
	mark = "-" | "_" | "." | "!" | "~" | "*" | "'" | "(" | ")";
	unreserved = alphanum | mark;
	escaped = "%" HEXDIG HEXDIG;
	user_unreserved = "&" | "=" | "+" | "$" | "," | ";" | "?" | "/";
	user = ( unreserved | escaped | user_unreserved )+;
	visual_separator = "-" | "." | "(" | ")";
	phonedigit = DIGIT | visual_separator?;
	global_number_digits = "+" phonedigit* DIGIT phonedigit*;
	param_unreserved = "[" | "]" | "/" | ":" | "&" | "+" | "$";
	pct_encoded = "%" HEXDIG HEXDIG;
	paramchar = param_unreserved | unreserved | pct_encoded;
	pname = paramchar+;
	pvalue = paramchar+;
	parameter = ";" pname ( "=" pvalue )?;
	port = DIGIT+;
	
	gen_delims	=  	":" | "/" | "?" | "#" | "[" | "]" | "@";
	sub_delims	= 	"!" | "$" | "&" | "'" | "(" | ")" | "*" | "+" | "," | ";" | "=";
	utf8text = (HTAB | 0x20..0x7e | UTF8_NONASCII)*;
	UPALPHA	=  	0x41..0x5A;
	hname = ALPHA *token;
	hval = utf8text;
	ident_char = alphanum | "." | "-" | "+" | "%" | "=";
	ident = alphanum ident_char{3,31};
}%%
