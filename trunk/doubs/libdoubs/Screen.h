#ifndef _LIBDOUBS_SCREEN_H
#define _LIBDOUBS_SCREEN_H

#include "libdoubs_config.h"

/* screen type */
typedef enum SCREEN_TYPE_E
{
	SCT_LOGIN,
	SCT_CONTACTS,
	SCT_HISTORY
}
SCREEN_TYPE_T;

class LIBDOUBS_API Screen
{
public:
	Screen(SCREEN_TYPE_T type);
	~Screen();

public:
	inline SCREEN_TYPE_T getType() const{ return this->type; }

protected:
	SCREEN_TYPE_T type;
};

#endif /* _LIBDOUBS_SCREEN_H */