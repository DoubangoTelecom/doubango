#ifndef _DOUBS_SCREEN_AUTHENTICATION_H_
#define _DOUBS_SCREEN_AUTHENTICATION_H_

#include "doubs_config.h"
#include "ui_authentication.h"

#include <Screen.h>

class ScreenAuthentication : public Screen
{
public:
	ScreenAuthentication();
	~ScreenAuthentication();

	static Screen* CreateScreen();
	static SCREEN_TYPE_T getType();

private:
	Ui::FormAuthentication ui;
};

#endif /* _DOUBS_SCREEN_AUTHENTICATION_H_ */