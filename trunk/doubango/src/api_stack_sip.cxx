#include "api_stack.h"
#include "sip_dialog_register.h"
#include "sip_dialog_message.h"
//#include<sofia-sip/auth_module.h>

PREF_NAMESPACE_START

/* SIP REGISTER*/
ERR stack::sip_register()
{	
	GET_DIALOG_BY_SIPMETHOD(dlg, "REGISTER");
	if(dlg){
		return ((sip_dialog_register*)dlg)->Start();
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
		return ((sip_dialog_register*)dlg)->Stop();
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
	

PREF_NAMESPACE_END