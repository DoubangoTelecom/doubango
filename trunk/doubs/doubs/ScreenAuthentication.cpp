#include "ScreenAuthentication.h"

/* Screen constructor */
ScreenAuthentication::ScreenAuthentication()
:Screen(ScreenAuthentication::getType())
{
	ui.setupUi(this);
}

/* Screen destructor */
ScreenAuthentication::~ScreenAuthentication()
{
}

/* get authentication screen type */
SCREEN_TYPE_T ScreenAuthentication::getType()
{
	return SCT_AUTHENTICATION;
}

/* function to create authentication screen */
Screen* ScreenAuthentication::CreateScreen()
{
	return new ScreenAuthentication();
}

#undef SCREEN_TYPE