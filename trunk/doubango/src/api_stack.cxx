/**
* @file
* @author  xxxyyyzzz <imsframework(at)gmail.com>
* @version 1.0
*
* @section LICENSE
*
*	
* This file is part of Open Source Doubango IMS Client Framework project.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*	
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
* @section DESCRIPTION
*
*
*/
#include <assert.h>
#include "api_stack.h"
#include "api_engine.h"

#include <sofia-sip/auth_module.h>
#include <sofia-sip/su.h>

#define DEFAULT_USER_AGENT "IM-client/OMA1.0"

PREF_NAMESPACE_START


/* global callback shared between all stacks*/
static void shared_callback(nua_event_t $event, int status, char const *phrase,
                  nua_t *nua, nua_magic_t *magic, nua_handle_t *nh,
                  nua_hmagic_t *hmagic, sip_t const *sip, tagi_t tags[])
{
	GET_DIALOG_BY_HANDLE(((stack*)magic), dlg, nh);
	if(dlg)
	{
		dlg->dialog_callback($event, status, phrase, nua, magic,
			       nh, hmagic, sip, tags);
	}
	else
	{
	}
	//((stack*)magic)->callback_handle(event, status, phrase, nua, magic,
    //              nh, hmagic, sip, tags);
}

/**
	Stack constructor

	@param stack_id stack identifier
	@param stack_callback stack callback function
*/
stack::stack(int stack_id)
{
	/* check that no stack has the same id */
	assert(!engine_stack_find(stack_id));

	// initialize
	this->id = stack_id;
	
	this->initialized = false;
	this->thread = NULL;
	this->nua = NULL;
	this->root = NULL;
	this->running = false;

	/* authentication */
	displayname = NULL, public_id = NULL, private_id = NULL, realm = NULL, 
		password = NULL, privacy = NULL, early_ims = false;

	/* Network */
	pcscf = NULL, pcscf_port = 5060;

	/* Features */
	smsip = true, sigcomp = false, oma_large_msg = true, oma_sip_im = true, gsma_is = true, gsma_vs = true;

	/* Params */
	expires = IMS_DEFAULT_EXIPRES;

	// initialize memory handling
	if( su_home_init(this->home) ) return;

	/* add stack to the engine list */
	if(!ERR_SUCCEED(engine_push_stack(this))) return;

	this->initialized = true;
}

/**
	stack destructor 
*/
stack::~stack()
{
	if(this->thread) CloseHandle(this->thread);
}

/** 
	Runs main loop for processing of messages

	@return returns ERR_SUCCESS if succeed and stack error code otherwise
*/
inline ERR stack::run()
{
	ERR error = ERR_SUCCESS;

	if(this->initialized && !this->running && !this->thread)
	{
#ifdef WIN32 // FIXME: use pthread_cond + pthread_create
		DWORD threadId;
		this->thread = CreateThread(NULL, 0, ThreadRunner, this, 0, &threadId);
		for(int i=0; i<5; i++)
		{
			if(this->running) break;
			Sleep(1000);
		}	
#else
#	error "must be implemented"
#endif
	}

	if(this->initialized && this->running)
	{
		/* 
			According to 3GPP TS 24.229 subcluse 5.1.1.1B when using an UE without ISIM or USIM we shall
			assume that public identity, private identity and realm are provisioned.
			I added the P-CSCF because without this information it's impossible to register
		*/
		error = 
			!this->public_id ? ERR_PARAMS_INVALID_PUBLIC_ID :
			(!this->private_id ? ERR_PARAMS_INVALID_PRIVATE_ID :
			((!this->pcscf || !this->pcscf_port) ? ERR_PARAMS_INVALID_PCSCF:
			(!this->realm ? ERR_PARAMS_INVALID_REALM : error)));
		if(!ERR_SUCCEED(error)) goto bail;
		
		/* Proxy-CSCF */
		char* pcscf_ip_port = su_sprintf(NULL, "sip:%s:%d", this->pcscf, this->pcscf_port);
		nua_set_params(nua, NUTAG_PROXY(pcscf_ip_port), NUTAG_OUTBOUND("no-validate"), TAG_NULL());
		su_free(NULL, pcscf_ip_port);

		/* Public id */
		nua_set_params(nua, NUTAG_IMPU(this->public_id), TAG_NULL());

		/* Private id */
		nua_set_params(nua, NUTAG_IMPI(this->private_id), TAG_NULL());

		/* Realm: a home network domain name to address the SIP REGISTER request to */
		nua_set_params(nua, NUTAG_REALM(this->realm), TAG_NULL());
	}
bail:
	if(!ERR_SUCCEED(error) && this->running)
	{
		this->shutdown();
	}

	return error;
}

/**
	shutdown the stack 

	@return
*/
inline ERR stack::shutdown()
{
	if(this->running)
	{
		if(this->root)
		{
			su_root_break(this->root);
			return ERR_SUCCESS;
		}
		else
		{
			return ERR_GLOBAL_FAILURE;
		}
	}
	else
	{
		return ERR_STACK_NOT_RUNNING;
	}
}

/** 
	event loop processing 
*/
void stack::loop()
{
	// initialize root object
	this->root = su_root_create(this);
	if(!this->root)
	{
		su_home_deinit(this->home);
		return;
	}

	// create NUA stack
	this->nua = nua_create(this->root, shared_callback, this,
							 NUTAG_USER_AGENT(DEFAULT_USER_AGENT),
							 NUTAG_PROXY("sip:127.0.0.1:5060"),
                             NUTAG_SIGCOMP_ENABLED(this->sigcomp),

                             TAG_NULL());
	if (!this->nua)
	{
		su_root_destroy(this->root);
		su_home_deinit(this->home);
		return;
	}

	// FIXME
	this->running = true;

	// enter main loop for processing of messages
    su_root_run(this->root);

	// Release resources
	if(this->nua) {
		nua_shutdown(this->nua);
		//nua_destroy(this->nua);
		this->nua = NULL;
	}
	if(this->root) {
		su_root_destroy(this->root);
		this->root = NULL;
	}
	if(this->home) {
		su_home_deinit(this->home);
	}

	this->running = false;
}

#ifdef WIN32
/**
	static threads runner
*/
DWORD WINAPI stack::ThreadRunner( void* magic ) { 
	((stack*)magic)->loop();

	return 0;
} 
#endif


PREF_NAMESPACE_END