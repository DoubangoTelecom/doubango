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
#include "Xcap.h"

#include "Common.h"

unsigned XcapStack::count = 0;

/* === ANSI-C functions (local use) === */
static int stack_callback(const thttp_event_t *httpevent);

/* =================================== XCAP Event ==================================== */
typedef enum twrap_xcap_step_type_e
{
	txst_name,
	txst_pos,
	txst_att,
	txst_pos_n_att,
	txst_ns
}
twrap_xcap_step_type_t;

typedef struct twrap_xcap_step_s
{
	TSK_DECLARE_OBJECT;

	twrap_xcap_step_type_t type;
	char* qname;
	char* att_qname;
	char* att_value;
	unsigned pos;
	struct{
		char* prefix;
		char* value;
	} ns;
}
twrap_xcap_step_t;

static tsk_object_t* twrap_xcap_step_ctor(tsk_object_t * self, va_list * app)
{
	twrap_xcap_step_t *step = (twrap_xcap_step_t *)self;
	if(step){
	}
	return self;
}

static tsk_object_t* twrap_xcap_step_dtor(tsk_object_t * self)
{ 
	twrap_xcap_step_t *step = (twrap_xcap_step_t *)self;
	if(step){
		TSK_FREE(step->qname);
		TSK_FREE(step->att_qname);
		TSK_FREE(step->att_value);
		TSK_FREE(step->ns.prefix);
		TSK_FREE(step->ns.value);
	}

	return self;
}

static const tsk_object_def_t twrap_xcap_step_def_s = 
{
	sizeof(twrap_xcap_step_t),
	twrap_xcap_step_ctor, 
	twrap_xcap_step_dtor,
	tsk_null, 
};
const tsk_object_def_t *twrap_xcap_step_def_t = &twrap_xcap_step_def_s;

twrap_xcap_step_t* twrap_xcap_step_create(twrap_xcap_step_type_t type){
	twrap_xcap_step_t* step;
	if((step = (twrap_xcap_step_t*)tsk_object_new(twrap_xcap_step_def_t))){
		step->type = type;
	}
	return step;
}

XcapSelector::XcapSelector(XcapStack* stack)
: auid(tsk_null)
{
	if(stack){
		this->stack_handle = tsk_object_ref(stack->getHandle());
	}
	this->steps = tsk_list_create();
}


XcapSelector* XcapSelector::setAUID(const char* auid)
{
	tsk_strupdate(&this->auid, auid);
	return this;
}

XcapSelector* XcapSelector::setName(const char* qname)
{
	twrap_xcap_step_t* step;
	if((step = twrap_xcap_step_create(txst_name))){
		step->qname = tsk_strdup(qname);
		tsk_list_push_back_data(this->steps, (void**)&step);
	}
	return this;
}

XcapSelector* XcapSelector::setAttribute(const char* qname, const char* att_qname, const char* att_value)
{
	twrap_xcap_step_t* step;
	if((step = twrap_xcap_step_create(txst_att))){
		step->qname = tsk_strdup(qname);
		step->att_qname = tsk_strdup(att_qname);
		step->att_value = tsk_strdup(att_value);
		tsk_list_push_back_data(this->steps, (void**)&step);
	}
	return this;
}

XcapSelector* XcapSelector::setPos(const char* qname, unsigned pos)
{
	twrap_xcap_step_t* step;
	if((step = twrap_xcap_step_create(txst_pos))){
		step->qname = tsk_strdup(qname);
		step->pos = pos;
		tsk_list_push_back_data(this->steps, (void**)&step);
	}
	return this;
}

XcapSelector* XcapSelector::setPosAttribute(const char* qname, unsigned pos, const char* att_qname, const char* att_value)
{
	twrap_xcap_step_t* step;
	if((step = twrap_xcap_step_create(txst_pos))){
		step->qname = tsk_strdup(qname);
		step->pos = pos;
		step->att_qname = tsk_strdup(att_qname);
		step->att_value = tsk_strdup(att_value);
		tsk_list_push_back_data(this->steps, (void**)&step);
	}
	return this;
}

XcapSelector* XcapSelector::setNamespace(const char* prefix, const char* value)
{
	twrap_xcap_step_t* step;
	if((step = twrap_xcap_step_create(txst_ns))){
		step->ns.prefix = tsk_strdup(prefix);
		step->ns.value = tsk_strdup(value);
		tsk_list_push_back_data(this->steps, (void**)&step);
	}
	return this;
}

/* From tinyXCAP::txcap_selector_get_node_2() */
char* XcapSelector::getString()
{
	char* node = tsk_null;
	char* temp = tsk_null;
	char* _namespace = tsk_null;
	tsk_buffer_t* buffer = tsk_buffer_create_null();
	const tsk_list_item_t* item;
	const twrap_xcap_step_t* step;

	/* Node */
	tsk_list_foreach(item, this->steps){
		step = (twrap_xcap_step_t*)item->data;
		switch(step->type){
			case txst_name:
					if(tsk_buffer_append_2(buffer, "/%s", step->qname)){
						goto bail;
					}
					break;

			case txst_pos:
					tsk_buffer_append_2(buffer, "/%s%%5B%u%%5D", 
						step->att_qname, step->pos);
					break;

			case txst_att:
					tsk_buffer_append_2(buffer, "/%s%%5B@%s=%%22%s%%22%%5D", 
						step->qname, step->att_qname, step->att_value);
					break;
				
			case txst_pos_n_att:
					tsk_buffer_append_2(buffer, "/%s%%5B%u%%5D%%5B@%s=%%22%s%%22%%5D", 
						step->qname, step->pos, step->att_qname, step->att_value);
					break;
				
			case txst_ns:					
					tsk_sprintf(&temp, "%sxmlns(%s=%%22%s%%22)", 
						_namespace?"":"%3F", step->ns.prefix, step->ns.value);
					tsk_strcat(&_namespace, temp);
					TSK_FREE(temp);
					break;
				

		} /* switch */
	} /* for */

	/* append the namespace */
	if(_namespace){
		tsk_buffer_append(buffer, _namespace, (tsk_size_t)tsk_strlen(_namespace));
		TSK_FREE(_namespace);
	}

bail:
	if(TSK_BUFFER_DATA(buffer) && TSK_BUFFER_SIZE(buffer)){
		node = tsk_strndup((const char*)TSK_BUFFER_DATA(buffer), TSK_BUFFER_SIZE(buffer));
	}
	TSK_OBJECT_SAFE_FREE(buffer);


	/* Document */
	if(this->auid){
		char* document;
		if((document = txcap_selector_get_document(this->stack_handle, this->auid))){
			if(node){
				tsk_strcat_2(&document, "/~~/%s%s", this->auid, node);
				TSK_FREE(node);
			}
			return document;
		}
	}
	
	return node;
}

void XcapSelector::reset()
{
	TSK_FREE(this->auid);
	tsk_list_clear_items(this->steps);
}

XcapSelector::~XcapSelector()
{
	this->reset();
	TSK_OBJECT_SAFE_FREE(this->steps);
	
	tsk_object_unref(this->stack_handle);
}

/* =================================== XCAP Message ==================================== */
XcapMessage::XcapMessage() :
httpmessage(tsk_null)
{
}

XcapMessage::XcapMessage(const thttp_message_t *_httpmessage)
{
	this->httpmessage = _httpmessage;
}

XcapMessage::~XcapMessage()
{
}

short XcapMessage::getCode() const
{
	if(this->httpmessage){
		return this->httpmessage->line.response.status_code;
	}
	return 0;
}

const char* XcapMessage::getPhrase() const
{
	if(this->httpmessage){
		return this->httpmessage->line.response.reason_phrase;
	}
	return tsk_null;
}

char* XcapMessage::getXcapHeaderValue(const char* name, unsigned index /*= 0*/)
{
	const thttp_header_t* header;
	if((header = thttp_message_get_headerByName(this->httpmessage, name))){
		return thttp_header_value_tostring(header);
	}
	return tsk_null;
}

char* XcapMessage::getXcapHeaderParamValue(const char* name, const char* pname, unsigned index /*= 0*/)
{
	const thttp_header_t* header;
	if((header = thttp_message_get_headerByName(this->httpmessage, name))){
		const tsk_param_t* param;
		if((param = tsk_params_get_param_by_name(header->params, pname))){
			return tsk_strdup(param->value);
		}
	}
	return tsk_null;
}

unsigned XcapMessage::getXcapContentLength()
{
	if(this->httpmessage && this->httpmessage->Content){
		return this->httpmessage->Content->size;
	}
	return 0;
}

unsigned XcapMessage::getXcapContent(void* output, unsigned maxsize)
{
	unsigned retsize = 0;
	if(output && maxsize && this->httpmessage->Content){
		retsize = (this->httpmessage->Content->size > maxsize) ? maxsize : this->httpmessage->Content->size;
		memcpy(output, this->httpmessage->Content->data, retsize);
	}
	return retsize;
}


/* =================================== XCAP Event ==================================== */
XcapEvent::XcapEvent(const thttp_event_t *_httpevent)
{
	this->httpevent = _httpevent;
	if(_httpevent){
		this->httpmessage = new XcapMessage(_httpevent->message);
	}
	else{
		this->httpmessage = tsk_null;
	}
}

XcapEvent::~XcapEvent()
{
	if(this->httpmessage){
		delete this->httpmessage;
	}
}

thttp_event_type_t XcapEvent::getType()
{
	return this->httpevent->type;
}

const XcapMessage* XcapEvent::getXcapMessage() const
{
	return this->httpmessage;
}




/* =================================== XCAP Callback ==================================== */
XcapCallback::XcapCallback()
{
}

XcapCallback::~XcapCallback()
{
}





/* =================================== XCAP Stack ==================================== */
XcapStack::XcapStack(XcapCallback* _callback, const char* xui, const char* password, const char* xcap_root)
{
	/* Initialize network layer */
	if(XcapStack::count == 0){
		tnet_startup();
	}

	this->callback = _callback;
	this->handle = txcap_stack_create(stack_callback, xui, password, xcap_root,
		TXCAP_STACK_SET_USERDATA(this),
		TXCAP_STACK_SET_NULL());
}

XcapStack::~XcapStack()
{
	TSK_OBJECT_SAFE_FREE(this->handle);

	/* DeInitialize the network layer (only if last stack) */
	if(--XcapStack::count == 0){
		tnet_cleanup();
	}
}

bool XcapStack::registerAUID(const char* id, const char* mime_type, const char* ns, const char* document_name, bool is_global)
{
	txcap_stack_t* stack = (txcap_stack_t*)this->handle;
	if(stack){
		tsk_bool_t _global = is_global?tsk_true:tsk_false; // 32bit <-> 64bit workaround
		return (txcap_auid_register(stack->auids, id, mime_type, ns, document_name, _global) == 0);
	}
	return false;
}

bool XcapStack::start()
{
	return (txcap_stack_start(this->handle) == 0);
}

bool XcapStack::setCredentials(const char* xui, const char* password)
{
	return txcap_stack_set(this->handle,
		TXCAP_STACK_SET_XUI(xui),
		TXCAP_STACK_SET_PASSWORD(password),
		TXCAP_STACK_SET_NULL()) == 0;
}

bool XcapStack::setXcapRoot(const char* xcap_root)
{
	return txcap_stack_set(this->handle,
		TXCAP_STACK_SET_ROOT(xcap_root),
		TXCAP_STACK_SET_NULL()) == 0;
}

bool XcapStack::setLocalIP(const char* ip)
{
	return txcap_stack_set(this->handle,
		TXCAP_STACK_SET_LOCAL_IP(ip),
		TXCAP_STACK_SET_NULL()) == 0;
}

bool XcapStack::setLocalPort(unsigned port)
{
	tsk_istr_t port_str;
	tsk_itoa(port, &port_str);
	return txcap_stack_set(this->handle,
		TXCAP_STACK_SET_LOCAL_PORT(port_str),
		TXCAP_STACK_SET_NULL()) == 0;
}

bool XcapStack::addHeader(const char* name, const char* value)
{
	return txcap_stack_set(this->handle,
		TXCAP_STACK_SET_HEADER(name, value),
		TXCAP_STACK_SET_NULL()) == 0;
}

bool XcapStack::removeHeader(const char* name)
{
	return txcap_stack_set(this->handle,
		TXCAP_STACK_UNSET_HEADER(name),
		TXCAP_STACK_SET_NULL()) == 0;
}

bool XcapStack::setTimeout(unsigned timeout)
{
	tsk_istr_t timeout_str;
	tsk_itoa(timeout, &timeout_str);
	return txcap_stack_set(this->handle,
		TXCAP_STACK_SET_TIMEOUT(timeout_str),
		TXCAP_STACK_SET_NULL()) == 0;
}

bool XcapStack::getDocument(const char* url)
{
	return txcap_action_fetch_document(this->handle,
			TXCAP_ACTION_SET_REQUEST_URI(url),
			TXCAP_ACTION_SET_NULL()) == 0;
}

bool XcapStack::getElement(const char* url)
{
	return txcap_action_fetch_element(this->handle,
			TXCAP_ACTION_SET_REQUEST_URI(url),
			TXCAP_ACTION_SET_NULL()) == 0;
}

bool XcapStack::getAttribute(const char* url)
{
	return txcap_action_fetch_attribute(this->handle,
			TXCAP_ACTION_SET_REQUEST_URI(url),
			TXCAP_ACTION_SET_NULL()) == 0;
}

bool XcapStack::deleteDocument(const char* url)
{
	return txcap_action_delete_document(this->handle,
			TXCAP_ACTION_SET_REQUEST_URI(url),
			TXCAP_ACTION_SET_NULL()) == 0;
}

bool XcapStack::deleteElement(const char* url)
{
	return txcap_action_delete_element(this->handle,
			TXCAP_ACTION_SET_REQUEST_URI(url),
			TXCAP_ACTION_SET_NULL()) == 0;
}

bool XcapStack::deleteAttribute(const char* url)
{
	return txcap_action_delete_attribute(this->handle,
			TXCAP_ACTION_SET_REQUEST_URI(url),
			TXCAP_ACTION_SET_NULL()) == 0;
}


bool XcapStack::putDocument(const char* url, const void* payload, unsigned len, const char* contentType)
{
	return txcap_action_create_document(this->handle,
			TXCAP_ACTION_SET_REQUEST_URI(url),
			TXCAP_ACTION_SET_PAYLOAD(payload, len),
			TXCAP_ACTION_SET_HEADER("Content-Type", contentType),
			TXCAP_ACTION_SET_NULL()) == 0;
}

bool XcapStack::putElement(const char* url, const void* payload, unsigned len)
{
	return txcap_action_create_element(this->handle,
			TXCAP_ACTION_SET_REQUEST_URI(url),
			TXCAP_ACTION_SET_PAYLOAD(payload, len),
			TXCAP_ACTION_SET_NULL()) == 0;
}

bool XcapStack::putAttribute(const char* url, const void* payload, unsigned len)
{
	return txcap_action_create_attribute(this->handle,
			TXCAP_ACTION_SET_REQUEST_URI(url),
			TXCAP_ACTION_SET_PAYLOAD(payload, len),
			TXCAP_ACTION_SET_NULL()) == 0;
}


bool XcapStack::stop()
{
	return (txcap_stack_stop(this->handle) == 0);
}


int stack_callback(const thttp_event_t *httpevent)
{
	const XcapStack* stack = tsk_null;
	XcapEvent* e = tsk_null;

	const txcap_stack_handle_t* stack_handle = thttp_session_get_userdata(httpevent->session);
	if(!stack_handle || !(stack = dyn_cast<const XcapStack*>((const XcapStack*)stack_handle))){
		TSK_DEBUG_ERROR("Invalid user data");
		return -1;
	}

	if(stack->getCallback()){
		if((e = new XcapEvent(httpevent))){
			stack->getCallback()->onEvent(e);
			delete e;
		}
	}
	return 0;
}



