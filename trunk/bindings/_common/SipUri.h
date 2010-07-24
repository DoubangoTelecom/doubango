/*
* Copyright (C) 2009-2010 Mamadou Diop.
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
#ifndef TINYWRAP_SIPURI_H
#define TINYWRAP_SIPURI_H

#include "tinysip.h"

class SipUri
{
public:
	SipUri(const char*);
	~SipUri();

public:
	static bool isValid(const char*);

	bool isValid();
	const char* getScheme();
	const char* getHost();
	unsigned short getPort();
	const char* getUserName();
	const char* getPassword();
	const char* getDisplayName();
	const char* getParamValue(const char* pname);

private:
	tsip_uri_t* uri;
};

#endif /* TINYWRAP_SIPURI_H */
