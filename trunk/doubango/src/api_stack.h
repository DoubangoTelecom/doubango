#ifndef __DOUBANGO_STACK_H__
#define  __DOUBANGO_STACK_H__

#include "pref.h"
#include <sofia-sip/nua.h>

PREF_NAMESPACE_START

class DOUBANGO_API stack
{
public:
	stack(int id, nua_callback_f callback);
	~stack();
	
	inline int get_id() const;
	inline bool get_initialized() const;

private:
	int id;
	bool initialized;

	su_home_t      home[1];  /* memory home */
	su_root_t      *root;    /* root object */
	nua_t          *nua;     /* NUA stack object */
};

PREF_NAMESPACE_END

#endif /* __DOUBANGO_STACK_H__ */