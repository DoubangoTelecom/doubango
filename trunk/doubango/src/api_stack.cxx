#include "api_stack.h"

#define DEFAULT_USER_AGENT "IM-client/OMA1.0"

PREF_NAMESPACE_START

/* stack constructor */
stack::stack(int _id, nua_callback_f _callback)
{
	if(!_callback) return;

	// initialize
	this->id = _id;
	this->callback = _callback;
	
	this->initialized = false;
	this->thread = NULL;
	this->nua = NULL;
	this->root = NULL;
	this->running = false;

	/* authentication */
	displayname = NULL, public_id = NULL, private_id = NULL, realm = NULL, 
		password = NULL, privacy = NULL, pcscf_ip = NULL, pcscf_port = 5060;

	/* Features */
	smsip = true, oma_large_msg = true, oma_sip_im = true, gsma_is = true, gsma_vs = true;

	// initialize memory handling
	if( su_home_init(this->home) ) return;

	this->initialized = true;
}

/* stack destructor */
stack::~stack()
{
	if(this->thread) CloseHandle(this->thread);
}

/* run main loop for processing of messages */
inline ERR stack::run()
{
	if(this->initialized && !this->running && !this->thread)
	{
#ifdef WIN32
		DWORD threadId;
		this->thread = CreateThread(NULL, 0, ThreadRunner, this, 0, &threadId);
		for(int i=0; i<5; i++)
		{
			if(this->running)
			{
				return ERR_SUCCESS;
			}
			Sleep(1000);
		}
		
		return ERR_STACK_NOT_RUNNING;
#else
#	error "must be implemented"
#endif
	}

	return (!this->initialized ? ERR_STACK_NOT_INITIALIZED : (this->running? ERR_STACK_ALREADY_EXIST : ERR_GLOBAL_FAILURE));
}

/* shutdown the stack */
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

/* event loop processing */
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
	this->nua = nua_create(this->root, this->callback, this,
							 NUTAG_USER_AGENT(DEFAULT_USER_AGENT),
							 NUTAG_PROXY("sip:127.0.0.1:5060"),
                             /* tags as necessary ...*/
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
/* static threads runner*/
DWORD WINAPI stack::ThreadRunner( void* magic ) { 
	((stack*)magic)->loop();

	return 0;
} 
#endif

/* handle callback event*/
void stack::callback_handle(nua_event_t _event,
			       int status, char const *phrase,
			       nua_t *nua, nua_magic_t *magic,
			       nua_handle_t *nh, nua_hmagic_t *hmagic,
			       sip_t const *sip,
			       tagi_t tags[])
{
	GET_DIALOG_BY_HANDLE(dlg, nh);
	if(dlg)
	{
		dlg->dialog_callback(_event, status, phrase, nua, magic,
			       nh, hmagic, sip, tags);
	}
	else
	{
	}
}

PREF_NAMESPACE_END