#ifndef _DOUBANGO_API_ENGINE_H_
#define _DOUBANGO_API_ENGINE_H_

#include "pref.h"
#include "api_errors.h"

PREF_NAMESPACE_START

DOUBANGO_API_C ERR engine_initialize(void);
DOUBANGO_API_C ERR engine_deinitialize(void);

DOUBANGO_API_C ERR engine_stack_create(int stack_id);
DOUBANGO_API_C ERR engine_stack_run(int stack_id);
DOUBANGO_API_C ERR engine_stack_shutdown(int stack_id);
DOUBANGO_API_C ERR engine_stack_shutdown_all(void);

void* engine_stack_find(int stack_id);

PREF_NAMESPACE_END

#endif /* _DOUBANGO_API_ENGINE_H_ */