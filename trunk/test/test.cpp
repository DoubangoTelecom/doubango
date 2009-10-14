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
	assert( ERR_SUCCEED(dgo::engine_initialize()) );

	assert( ERR_SUCCEED(dgo::engine_stack_create(STACK_ID)) );
	assert( ERR_SUCCEED(dgo::engine_stack_run(STACK_ID)) );
	
	//Sleep(1000);
	
	assert( ERR_SUCCEED(dgo::auth_set_early_ims(STACK_ID, false)) );
	assert( ERR_SUCCEED(dgo::auth_set(STACK_ID, "sip:doubango@wonderland.net", "doubango@wonderland.net", "doubango", "wonderland.net" )) );
	assert( ERR_SUCCEED(dgo::auth_set_privacy(STACK_ID, "none")) );
	assert( ERR_SUCCEED(dgo::auth_set_displayname(STACK_ID, "My display Name")) );
	assert( ERR_SUCCEED(dgo::network_set_pcscf(STACK_ID, "192.168.0.14", 5060)) );

	assert( ERR_SUCCEED(dgo::sip_register(STACK_ID)) );
	Sleep(1000);
	//assert( ERR_SUCCEED(dgo::sip_message(STACK_ID, "sip:toto@wonderland.net", "text/plain", "test")) );
	Sleep(50000);
	assert( ERR_SUCCEED(dgo::sip_unregister(STACK_ID)) );
	Sleep(5000);

	assert( ERR_SUCCEED(dgo::engine_deinitialize()) );

	Sleep(5000);
	//http://sofia-sip.sourceforge.net/refdocs/iptsec/index.html
	// FIXME:http://209.85.229.132/search?q=cache:8_OFeYfRS9EJ:fisheye.freeswitch.org/browse/~raw,r%3D14993/FreeSWITCH/src/mod/endpoints/mod_sofia/sofia_reg.c+nua_unregister&cd=7&hl=en&ct=clnk
	return 0;
}

