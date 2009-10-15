#include "api_stack.h"
#include "sip_dialog_register.h"
#include "sip_dialog_message.h"
#include "sip_dialog_publish.h"

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

PREF_NAMESPACE_END