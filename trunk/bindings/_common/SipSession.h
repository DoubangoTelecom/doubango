/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@doubango.org>
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
#ifndef TINYWRAP_SIPSESSION_H
#define TINYWRAP_SIPSESSION_H

#include "tinysip.h"

class SipStack;

class SipSession
{
public:
	SipSession(SipStack* Stack);
	virtual ~SipSession();

public:
	bool addHeader(const char* name, const char* value);
	bool removeHeader(const char* name);
	bool addCaps(const char* name, const char* value);
	bool addCaps(const char* name);
	bool removeCaps(const char* name);
	bool setExpires(unsigned expires);
	bool setFromUri(const char* fromUri);
	bool setToUri(const char* toUri);

#if defined(SWIG)
	/* http://www.swig.org/Doc1.3/Library.html#Library_carrays
	* 8.3.2 Passing binary data
	*/
	
	%apply (const char *str, unsigned len) { (const char *str, unsigned len) };
#endif
	bool setPayload(const char *str, unsigned len);
	
protected:
	tsip_ssession_handle_t* handle;
};

#endif /* TINYWRAP_SIPSESSION_H */
