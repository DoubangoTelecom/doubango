#include "globals.h"

/* globals */
ScreenMgr* globals::screenMgr = 0;

/* get screen manager */
ScreenMgr* globals::getScreenMgr()
{
	if(!globals::screenMgr) globals::screenMgr = new ScreenMgr();
	return globals::screenMgr;
}