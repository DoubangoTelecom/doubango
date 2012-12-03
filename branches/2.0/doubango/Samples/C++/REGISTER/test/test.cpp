// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <assert.h>
#include <api_engine.h>
#include <api_stack.h>

#define STACK_ID	1234

#ifdef WIN32
#	include <windows.h>
#endif

/* Event listening using a static method */
void OnRegistrationStateChanged(int stack_id, dgo::sip_state_registration_t state, int sipcode, const char* sipdesc)
{
	/* check if it's our stack */
	if(stack_id != STACK_ID) return;

	switch(state)
	{
		case dgo::srs_none: printf("Test_OnRegistrationStateChanged: %s(%s)\n", "srs_none", sipdesc); break;
		case dgo::srs_trying: printf("Test_OnRegistrationStateChanged: %s(%s)\n", "srs_trying", sipdesc); break;
		case dgo::srs_authentifying: printf("Test_OnRegistrationStateChanged: %s(%s)\n", "srs_authentifying", sipdesc); break;
		case dgo::srs_unregistered: printf("Test_OnRegistrationStateChanged: %s(%s)\n", "srs_unregistered", sipdesc); break;
		case dgo::srs_registered: printf("Test_OnRegistrationStateChanged: %s(%s)\n", "srs_registered", sipdesc); break;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	/* MUST call this function to initialize the engine befor using the first stack */
	assert( ERR_SUCCEED(dgo::engine_initialize()) );

	/* create the stack */
	dgo::stack* stack = new dgo::stack(STACK_ID);

	/* Events registration */
	stack->registrationStateChanged.connect(&OnRegistrationStateChanged);

	/* check that the stack has been successfuly initialized */
	assert(stack->get_initialized());

	/* MUST: Initialize mandatory parameters */
	stack->set_public_id("sip:bob@ims.inexbee.com");
	stack->set_private_id("bob@ims.inexbee.com");

	stack->set_pcscf("192.168.0.14");
	stack->set_pcscf_port(4060);
	stack->set_realm("ims.inexbee.com");

	/* Not mandatory but must be set before starting */
	stack->set_sigcomp(true);

	/* run stack */
	assert( ERR_SUCCEED(stack->run()) );

	/* set other optional parameters */
	stack->set_displayname("Doubango");
	stack->set_privacy("none");
	stack->set_password("bob");
	stack->set_early_ims(false);
	stack->set_expires(10);

	/* register */
	stack->sip_register();

	/* wait */
#ifdef WIN32
	Sleep(500000);
#else
	getchar();
#endif
	
	/* Now it's time to unregister */
	stack->sip_unregister();

	/* wait */
	getchar();

	/* destroy the engine and unregister all identities*/
	assert( ERR_SUCCEED(dgo::engine_deinitialize()) );

	return 0;
}