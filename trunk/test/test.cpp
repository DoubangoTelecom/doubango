// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <doubango.h>

#define STACK_ID	1234

int _tmain(int argc, _TCHAR* argv[])
{
	assert( ERR_SUCCEED(dgo::engine_initialize()) );

	assert( ERR_SUCCEED(dgo::engine_stack_create(STACK_ID)) );
	assert( ERR_SUCCEED(dgo::engine_stack_run(STACK_ID)) );

	assert( ERR_SUCCEED(dgo::engine_deinitialize()) );

	return 0;
}

