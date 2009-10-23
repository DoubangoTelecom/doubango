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

#include "api_engine.h"
#include "api_stack.h"

/* TODO: add mutex */
#include <algorithm>
#include <functional>
#include <list>
#include <assert.h>

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
		(*iter)->shutdown();
        //delete(*iter);
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
	stack* stk = new stack(stack_id, engine_callback);
	if(stk->get_initialized())
	{
		__stacks.push_back(stk);
	}
	else
	{
		delete stk;
		return ERR_STACK_NOT_INITIALIZED;
	}

	return ERR_SUCCESS;
}

/* runs the stack with the specified id */
ERR engine_stack_run(int stack_id)
{
	if(!__initialized) return ERR_ENGINE_NOT_INITIALIZED;

	stack* stk = (stack*)engine_stack_find(stack_id);
	if(stk)
	{
		return stk->run();
	}
	else
	{
		return ERR_STACK_NOT_FOUND;
	}
}

/* shutdown downs the stack with the specified id */
ERR engine_stack_shutdown(int stack_id)
{
	if(!__initialized) return ERR_ENGINE_NOT_INITIALIZED;

	stack* stk = (stack*)engine_stack_find(stack_id);
	if(stk)
	{
		return stk->shutdown();
	}
	else
	{
		return ERR_STACK_NOT_FOUND;
	}
}

/* shutdown downs all stacks */
ERR engine_stack_shutdown_all(void)
{
	if(!__initialized) return ERR_ENGINE_NOT_INITIALIZED;

	return ERR_NOT_IMPLEMENTED;
}

/* find stack by id*/
void* engine_stack_find(int stack_id)
{
	if(!__initialized) return NULL;

	std::list<PSTACK>::iterator iter = __stacks.begin();
	iter = std::find_if( iter, __stacks.end(), std::bind2nd( pred_stack_find_by_id(), stack_id ) );
	if(iter != __stacks.end())
	{
		return *iter;
	}
	return NULL;
}

/* engine callback*/
void engine_callback(nua_event_t event, int status, char const *phrase,
                  nua_t *nua, nua_magic_t *magic, nua_handle_t *nh,
                  nua_hmagic_t *hmagic, sip_t const *sip, tagi_t tags[])
{
	((stack*)magic)->callback_handle(event, status, phrase, nua, magic,
                  nh, hmagic, sip, tags);
}

#undef PSTACK

PREF_NAMESPACE_END