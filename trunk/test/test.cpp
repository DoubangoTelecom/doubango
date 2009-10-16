// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <doubango.h>

#ifdef WIN32
#include <windows.h>
#endif

#define STACK_ID	1234

int _tmain(int argc, _TCHAR* argv[])
{
	unsigned int sub_reg_dialog_id;

	assert( ERR_SUCCEED(dgo::engine_initialize()) );

	assert( ERR_SUCCEED(dgo::engine_stack_create(STACK_ID)) );
	assert( ERR_SUCCEED(dgo::engine_stack_run(STACK_ID)) );
	
	//Sleep(1000);
	
	assert( ERR_SUCCEED(dgo::auth_set_early_ims(STACK_ID, false)) );
	assert( ERR_SUCCEED(dgo::auth_set(STACK_ID, "sip:doubango@wonderland.net", "doubango@wonderland.net", "doubango", "wonderland.net" )) );
	assert( ERR_SUCCEED(dgo::auth_set_privacy(STACK_ID, "none")) );
	assert( ERR_SUCCEED(dgo::auth_set_displayname(STACK_ID, "My display Name")) );
	assert( ERR_SUCCEED(dgo::network_set_pcscf(STACK_ID, "192.168.0.14", 4060)) );

	assert( ERR_SUCCEED(dgo::sip_register(STACK_ID)) );
	Sleep(5000);
	assert( ERR_SUCCEED(dgo::sip_subscribe(STACK_ID, "sip:doubango@wonderland.net", "reg", "application/reginfo+xml", false, &sub_reg_dialog_id)) );
	assert( ERR_SUCCEED(dgo::sip_publish(STACK_ID)) );
	assert( ERR_SUCCEED(dgo::sip_message(STACK_ID, "sip:toto@wonderland.net", "text/plain", "test")) );
	Sleep(50000);
	assert( ERR_SUCCEED(dgo::sip_unregister(STACK_ID)) );
	Sleep(5000);

	assert( ERR_SUCCEED(dgo::engine_deinitialize()) );

	Sleep(5000);
	return 0;
}

