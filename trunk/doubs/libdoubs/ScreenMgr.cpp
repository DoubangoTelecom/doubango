#include "ScreenMgr.h"

#include <algorithm>

/* predicate to find screen by type */
struct pred_screen_find_by_type: public std::binary_function< const Screen*, SCREEN_TYPE_T, bool > {
	bool operator () ( const Screen* screen, SCREEN_TYPE_T type ) const {
		return screen->getType() == type;
	}
};

/* predicate to find screen by type */
struct pred_screendef_find_by_type: public std::binary_function< const ScreenDefinition_t*, SCREEN_TYPE_T, bool > {
	bool operator () ( const ScreenDefinition_t* screendef, SCREEN_TYPE_T type ) const {
		return screendef->type == type;
	}
};

#define DELETE_SCREEN_DEF(screenDef)\
	if((screenDef)){\
		if((screenDef)->screen) delete ((screenDef)->screen); \
		delete (screenDef); \
	}

/* constructor */
ScreenMgr::ScreenMgr()
{
	this->mdiArea = 0;
}

/* destructor */
ScreenMgr::~ScreenMgr()
{
	std::list<ScreenDefinition_t*>::iterator it;
	for ( it=this->screenDefinitions.begin() ; it != this->screenDefinitions.end(); it++ )
	{
		DELETE_SCREEN_DEF(*it);
	}
}

/* set mdi area */
void ScreenMgr::setMdiArea(QMdiArea* area)
{
	this->mdiArea = area;
}

/* set current screen */
inline void ScreenMgr::setCurrentScreen(SCREEN_TYPE_T type)
{
	std::list<ScreenDefinition_t*>::iterator iter = std::find_if( this->screenDefinitions.begin(), this->screenDefinitions.end(), std::bind2nd(pred_screendef_find_by_type(), type) );
	if(iter == this->screenDefinitions.end())
	{	/* not registered ==> FIXME: print error message */
	}

	if(!(*iter)->screen)
	{	//* create creen NULL */
		(*iter)->screen = (*iter)->creatorFunc();
		(*iter)->screen->setParent(this->mdiArea);
	}
	(*iter)->screen->showMaximized();
}

/* register screen creator function */
void ScreenMgr::registerScreenCreatorFunc(SCREEN_TYPE_T type, ScreenCreatorFunc creatorFunc)
{
	if(std::find_if( this->screenDefinitions.begin(), this->screenDefinitions.end(), std::bind2nd( pred_screendef_find_by_type(), type ) ) 
		!= this->screenDefinitions.end())
	{
		return;
	}
	ScreenDefinition_t* screenDef = new ScreenDefinition_t();
	screenDef->creatorFunc = creatorFunc;
	screenDef->type = type;

	this->screenDefinitions.push_back(screenDef);
}