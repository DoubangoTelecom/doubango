#ifndef _DOUBANGO_ERRORS_H_
#define _DOUBANGO_ERRORS_H_

#include "pref.h"

PREF_NAMESPACE_START

#define ERR_SUCCEED(code)	(code == PREF_NAMESPACE_VALUE::ERR_SUCCESS)
#define ERR_FAILED(code)	!ERR_SUCCEED(code)

typedef enum tag_ERR
{
	ERR_SUCCESS					= 0x0000,

	ERR_GLOBAL_FAILURE			= -0xFFFF,
	ERR_NOT_IMPLEMENTED,
	ERR_FUNC_OBSOLETE,

	/* ENGINE errors */
	ERR_ENGINE_NOT_INITIALIZED,		/* engine not initialized or initialization failed*/
	ERR_ENGINE_ALREADY_INITIALIZED,	/* engine have been already initialized */

	/* STACK errors */
	ERR_STACK_ALREADY_EXIST,
	ERR_STACK_NOT_INITIALIZED,		/* stack not initialized or initialization failed*/
	ERR_STACK_IS_INVALID,			/* stack is in an invalide state (NULL, destroyed, locked, ...) */
	ERR_STACK_NOT_RUNNING,			/* operation failed because the stack is not running */
}
ERR;

PREF_NAMESPACE_END

#endif /* _DOUBANGO_ERRORS_H_ */