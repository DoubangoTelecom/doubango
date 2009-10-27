#include "ScreenMgr.h"

#include <algorithm>

/* predicate to find screen by type */
struct pred_screen_find_by_type: public std::binary_function< const Screen*, SCREEN_TYPE_T, bool > {
	bool operator () ( const Screen* screen, SCREEN_TYPE_T type ) const {
		return screen->getType() == type;
	}
};

/* constructor */
ScreenMgr::ScreenMgr()
{
	this->mdiArea = 0;
}

/* destructor */
ScreenMgr::~ScreenMgr()
{
}

/* set mdi area */
void ScreenMgr::setMdiArea(QMdiArea* area)
{
	this->mdiArea = area;
}

/* find screen by type */
const Screen* ScreenMgr::getSreen(SCREEN_TYPE_T type) const
{
	std::list<const Screen*>::iterator iter = ScreenMgr::getScreens().begin();
	iter = std::find_if( iter, ScreenMgr::getScreens().end(), std::bind2nd( pred_screen_find_by_type(), type ) );
	if(iter != ScreenMgr::getScreens().end())
	{
		return *iter;
	}

	return 0;
}

/* add a new screen */
/* scrren will be added only no other sccreen has the same type */
inline void ScreenMgr::addScreen(const Screen* screen)
{
	if(!this->getSreen(screen->getType()))
	{
		ScreenMgr::getScreens().push_back(screen);
	}
}

/* remove screen */
inline void ScreenMgr::removeScreen(const Screen* screen)
{
	std::remove_if(ScreenMgr::getScreens().begin(), ScreenMgr::getScreens().end(), std::bind2nd(pred_screen_find_by_type(), screen->getType()));
}

/* remove screen */
inline void ScreenMgr::removeScreen(SCREEN_TYPE_T type)
{
	std::remove_if(ScreenMgr::getScreens().begin(), ScreenMgr::getScreens().end(), std::bind2nd(pred_screen_find_by_type(), type));
}