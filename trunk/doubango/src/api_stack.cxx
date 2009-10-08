#include "api_stack.h"

PREF_NAMESPACE_START

/* stack constructor */
stack::stack(int _id, nua_callback_f callback): id(_id), initialized(false)
{
	// initialize memory handling
	if( su_home_init(this->home) ) return;

	// initialize root object
	this->root = su_root_create(this);
	if(!this->root)
	{
		su_home_deinit(this->home);
		return;
	}

	// create NUA stack
	this->nua = nua_create(this->root, callback, this,
                             /* tags as necessary ...*/
                             TAG_NULL());
	if (!this->nua)
	{
		su_root_destroy(this->root);
		su_home_deinit(this->home);
		return;
	}

	// set necessary parameters
    nua_set_params(this->nua,
                    /* tags as necessary ...*/
                    TAG_NULL());

	// enter main loop for processing of messages
    su_root_run(this->root);

	this->initialized = true;
}

/* stack destructor */
stack::~stack()
{
	if(this->initialized)
	{
		// destroy NUA stack
		nua_destroy(this->nua);

		// deinit root object
		su_root_destroy(this->root);

		// deinitialize memory handling 
		su_home_deinit(this->home);
	}
}

/* gets the stack id */
inline int stack::get_id()const 
{ 
	return this->id; 
}

/* returns true if initialized, false otherwise */
inline bool stack::get_initialized() const
{
	return this->initialized;
}

PREF_NAMESPACE_END