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
#ifndef TINYWRAP_XCAP_H
#define TINYWRAP_XCAP_H

#include "tinyxcap.h"

class XcapStack;

typedef tsk_list_t twrap_xcap_steps_L_t;

//
//	XcapSelector
//
class XcapSelector
{
public:
	XcapSelector(XcapStack* stack);
	virtual ~XcapSelector();

public: /* API functions */
	XcapSelector* setAUID(const char* auid);
	XcapSelector* setName(const char* qname);
	XcapSelector* setAttribute(const char* qname, const char* att_qname, const char* att_value);
	XcapSelector* setPos(const char* qname, unsigned pos);
	XcapSelector* setPosAttribute(const char* qname, unsigned pos, const char* att_qname, const char* att_value);
	XcapSelector* setNamespace(const char* prefix, const char* value);
	
	char* getString();// %newobject
	void reset();

private:
	txcap_stack_handle_t* stack_handle;
	char* auid;
	twrap_xcap_steps_L_t* steps;
};

//
//	XcapMessage
//
class XcapMessage
{
public:
	XcapMessage();
#if !defined(SWIG)
	XcapMessage(const thttp_message_t *httpmessage);
#endif
	virtual ~XcapMessage();
	
	short getCode() const;
	const char* getPhrase() const;

	char* getXcapHeaderValue(const char* name, unsigned index = 0);
	char* getXcapHeaderParamValue(const char* name, const char* param, unsigned index = 0);
	unsigned getXcapContentLength();
	unsigned getXcapContent(void* output, unsigned maxsize);

private:
	const thttp_message_t *httpmessage;
};

//
//	XcapEvent
//
class XcapEvent
{
public:
#if !defined(SWIG)
	XcapEvent(const thttp_event_t *httpevent);
#endif
	virtual ~XcapEvent();	
	thttp_event_type_t getType();
	const XcapMessage* getXcapMessage() const;

private:
	const thttp_event_t *httpevent;
	const XcapMessage* httpmessage;
};


//
//	XcapCallback
//
class XcapCallback
{
public:
	XcapCallback();
	virtual ~XcapCallback();

	virtual int onEvent(const XcapEvent* e)const { return -1; }
};


//
//	XcapStack
//
class XcapStack
{
public:
	XcapStack(XcapCallback* callback, const char* xui, const char* password, const char* xcap_root);
	virtual ~XcapStack();

public: /* API functions */
	bool registerAUID(const char* id, const char* mime_type, const char* ns, const char* document_name, bool is_global);
	bool start();
	bool setCredentials(const char* xui, const char* password);
	bool setXcapRoot(const char* xcap_root);
	bool setLocalIP(const char* ip);
	bool setLocalPort(unsigned port);
	bool addHeader(const char* name, const char* value);
	bool removeHeader(const char* name);
	bool setTimeout(unsigned timeout);

	bool getDocument(const char* url);
	bool getElement(const char* url);
	bool getAttribute(const char* url);

	bool deleteDocument(const char* url);
	bool deleteElement(const char* url);
	bool deleteAttribute(const char* url);

	bool putDocument(const char* url, const void* payload, unsigned len, const char* contentType);
	bool putElement(const char* url, const void* payload, unsigned len);
	bool putAttribute(const char* url, const void* payload, unsigned len);

	bool stop();	

public: /* Public helper function */
#if !defined(SWIG)
	txcap_stack_handle_t* getHandle(){
		return this->handle;
	}
	XcapCallback* getCallback()const{
		return this->callback;
	}
#endif

private:
	txcap_stack_handle_t* handle;
	XcapCallback* callback;

	static unsigned count;
};


#endif /* TINYWRAP_XCAP_H */
