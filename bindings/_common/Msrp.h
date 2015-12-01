/*
* Copyright (C) 2010-2011 Mamadou Diop.
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
#ifndef TINYWRAP_MSRP_H
#define TINYWRAP_MSRP_H

#include "tinyWRAP_config.h"

#include "tinymsrp.h"

class MsrpSession;

class TINYWRAP_API MsrpMessage
{
public:
	MsrpMessage();
#if !defined(SWIG)
	MsrpMessage(tmsrp_message_t *message);
#endif
	virtual ~MsrpMessage();

	bool isRequest();
	short getCode();
	const char* getPhrase();
	tmsrp_request_type_t getRequestType();
#if defined(SWIG)
	void getByteRange(int64_t* OUTPUT, int64_t* OUTPUT, int64_t* OUTPUT);
#else
	void getByteRange(int64_t* start, int64_t* end, int64_t* total);
#endif
	bool isLastChunck();
	bool isFirstChunck();
	bool isSuccessReport();
	char* getMsrpHeaderValue(const char* name);
	char* getMsrpHeaderParamValue(const char* name, const char* param);
	unsigned getMsrpContentLength();
	unsigned getMsrpContent(void* output, unsigned maxsize);
#if !defined(SWIG)
	const tmsrp_message_t* getWrappedMsrpMessage() { return m_pMessage; }
#endif

private:
	const tmsrp_header_t* getMsrpHeader(const char* name, unsigned index = 0);

private:
	tmsrp_message_t *m_pMessage;
};

class TINYWRAP_API MsrpEvent
{
public:
#if !defined(SWIG)
	MsrpEvent(const tmsrp_event_t *_event);
#endif
	virtual ~MsrpEvent();

	tmsrp_event_type_t getType();
	const MsrpSession* getSipSession();
	const MsrpMessage* getMessage() const;

protected:
	const tmsrp_event_t *_event;
	MsrpMessage* m_pMessage;
};

class TINYWRAP_API MsrpCallback
{
public:
	MsrpCallback() {  }
	virtual ~MsrpCallback() {}
	virtual int OnEvent(const MsrpEvent* e) { return -1; }
};


#if !defined(SWIG)
int twrap_msrp_cb(const tmsrp_event_t* _event);
#endif

#endif /* TINYWRAP_MSRP_H */
