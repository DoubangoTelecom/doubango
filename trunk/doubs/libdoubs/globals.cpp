#include "globals.h"

ScreenMgr* globals::screenMgr;

/* init globals */
void globals::init()
{
	/* screen manager */
	globals::screenMgr = 0;
}

/* get screen manager */
ScreenMgr* globals::getScreenMgr()
{
	if(!globals::screenMgr) globals::screenMgr = new ScreenMgr();
	return globals::screenMgr;
}