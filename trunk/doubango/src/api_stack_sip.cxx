///****************************************************************************
//			 _             _                             
//			| |           | |                            
//		  _ | | ___  _   _| | _   ____ ____   ____  ___  
//		 / || |/ _ \| | | | || \ / _  |  _ \ / _  |/ _ \ 
//		( (_| | |_| | |_| | |_) | ( | | | | ( ( | | |_| |
//		 \____|\___/ \____|____/ \_||_|_| |_|\_|| |\___/ 
//											(_____|   
//	
//	Copyright (C) 2009 xxxyyyzzz <imsframework(at)gmail.com>
//
//	This file is part of Open Source Doubango IMS Client Framework project.
//
//    DOUBANGO is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//	
//    DOUBANGO is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//	
//    You should have received a copy of the GNU General Public License
//    along with DOUBANGO.
//****************************************************************************/
#include "api_stack.h"
#include "sip_dialog_register.h"
#include "sip_dialog_message.h"
#include "sip_dialog_publish.h"
#include "sip_dialog_subscribe.h"

PREF_NAMESPACE_START

/* SIP REGISTER*/
ERR stack::sip_register()
{	
	GET_DIALOG_BY_SIPMETHOD(dlg, "REGISTER");
	if(dlg){
		return dlg->Start();
	}
	else{
		sip_dialog_register* dlg_register = new sip_dialog_register(this);
		this->dialogs.push_back(dlg_register);
		return dlg_register->Start();
	}
}

/* SIP UNREGISTER */
ERR stack::sip_unregister()
{	
	GET_DIALOG_BY_SIPMETHOD(dlg, "REGISTER");
	if(dlg){
		return dlg->Stop();
	}
	else return ERR_SIP_DIALOG_NOT_FOUND;
}

/* SIP MESSAGE */
ERR stack::sip_message(const char* dest_address, const char* content_type, const char* content)
{
	sip_dialog_message* dlg_message = new sip_dialog_message(this, dest_address, content_type, content);
	this->dialogs.push_back(dlg_message);
	return dlg_message->Start();
}
	
/* SIP PUBLISH */
ERR stack::sip_publish()
{
	GET_DIALOG_BY_SIPMETHOD(dlg, "PUBLISH");
	if(dlg){
		return dlg->Start();
	}
	else{
		sip_dialog_publish* dlg_publish = new sip_dialog_publish(this);
		this->dialogs.push_back(dlg_publish);
		return dlg_publish->Start();
	}
}

/* SIP UNPUBLISH*/
ERR stack::sip_unpublish()
{
	GET_DIALOG_BY_SIPMETHOD(dlg, "PUBLISH");
	if(dlg){
		return dlg->Stop();
	}
	else return ERR_SIP_DIALOG_NOT_FOUND;
}

/* SIP SUBSCRIBE */
ERR stack::sip_subscribe(const char* dest_address, const char* eventpackg, const char* allow, int eventlist, unsigned int* dialog_id)
{
	sip_dialog_subscribe* dlg_subscribe = new sip_dialog_subscribe(this, dest_address, eventpackg, allow, eventlist?true:false);
	*dialog_id = dlg_subscribe->get_dialog_id();
	this->dialogs.push_back(dlg_subscribe);
	return dlg_subscribe->Start();
}

/* SIP UNSUBSCRIBE*/
ERR stack::sip_unsubscribe(unsigned int dialog_id)
{
	GET_DIALOG_BY_ID(dlg, dialog_id);
	if(dlg){
		return dlg->Stop();
	}
	else return ERR_SIP_DIALOG_NOT_FOUND;
}

PREF_NAMESPACE_END