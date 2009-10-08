#include "api_engine.h"

/* TODO: add mutex */
#include <algorithm>
#include <functional>
#include <list>

#include <sofia-sip/su.h>

#define PSTACK stack*

PREF_NAMESPACE_START

/* engine callback declaration */
static void engine_callback(nua_event_t   event,
                  int           status,
                  char const   *phrase,
                  nua_t        *nua,
                  nua_magic_t  *magic,
                  nua_handle_t *nh,
                  nua_hmagic_t *hmagic,
                  sip_t const  *sip,
                  tagi_t        tags[]);

/* global variable holding engine initialization state */
static bool __initialized = false;

/* global variable holding all stacks */
static std::list<PSTACK> __stacks;

/* predicate: find stack by id */
struct pred_stack_find_by_id: public std::binary_function< PSTACK, int, bool > {
	bool operator () ( const PSTACK s, const int &id ) const {
		return s->get_id() == id;
	}
};


/* initialize engine (system utilities) */
ERR engine_initialize()
{
	if(__initialized) return ERR_ENGINE_ALREADY_INITIALIZED;

	if( !su_init() )
	{
		__initialized = true;
		return ERR_SUCCESS;
	}

	return ERR_GLOBAL_FAILURE;
}

/* deinitialize engine (system utilities) */
ERR engine_deinitialize()
{
	if(!__initialized) return ERR_ENGINE_NOT_INITIALIZED;

	std::list<PSTACK>::iterator iter = __stacks.begin();
	for ( ; iter!=__stacks.end(); iter++ ) 
	{
        delete(*iter);
	} 
    __stacks.clear(); 

	su_deinit();
	
	__initialized = false;
	return ERR_SUCCESS;
}

/* creates a stack with the specified id */
ERR engine_stack_create(int stack_id)
{
	if(!__initialized) return ERR_ENGINE_NOT_INITIALIZED;

	if(engine_stack_find(stack_id))
	{
		return ERR_STACK_ALREADY_EXIST;
	}
	stack* sk = new stack(stack_id, engine_callback);
	if(sk->get_initialized())
	{
		__stacks.push_back(sk);
	}
	else
	{
		delete sk;
		return ERR_STACK_NOT_INITIALIZED;
	}

	return ERR_SUCCESS;
}

/* runs the stack with the specified id */
ERR engine_stack_run(int stack_id)
{
	if(!__initialized) return ERR_ENGINE_NOT_INITIALIZED;

	return ERR_NOT_IMPLEMENTED;
}

/* shutdown downs the stack with the specified id */
ERR engine_stack_shutdown(int stack_id)
{
	if(!__initialized) return ERR_ENGINE_NOT_INITIALIZED;

	return ERR_NOT_IMPLEMENTED;
}

/* shutdown downs all stacks */
ERR engine_stack_shutdown_all(void)
{
	if(!__initialized) return ERR_ENGINE_NOT_INITIALIZED;

	return ERR_NOT_IMPLEMENTED;
}

/* find stack by id*/
stack* engine_stack_find(int stack_id)
{
	std::list<PSTACK>::iterator iter = __stacks.begin();
	iter = std::find_if( iter, __stacks.end(), std::bind2nd( pred_stack_find_by_id(), stack_id ) );
	if(iter != __stacks.end())
	{
		return *iter;
	}
	return NULL;
}

/* engine callback*/
void engine_callback(nua_event_t   event,
                  int           status,
                  char const   *phrase,
                  nua_t        *nua,
                  nua_magic_t  *magic,
                  nua_handle_t *nh,
                  nua_hmagic_t *hmagic,
                  sip_t const  *sip,
                  tagi_t        tags[])
{

}

#undef PSTACK

PREF_NAMESPACE_END