#ifndef _LIBDOUBS_SCREENMGR_H
#define _LIBDOUBS_SCREENMGR_H

#include "libdoubs_config.h"
#include "Screen.h"

#include <QtGui/QMdiArea>

#include <list>

typedef struct ScreenDefinition_s
{
	SCREEN_TYPE_T type;
	ScreenCreatorFunc creatorFunc;
	Screen* screen;

	ScreenDefinition_s()
	{
		screen = 0;
	}
}
ScreenDefinition_t;

class LIBDOUBS_API ScreenMgr
{
public:
	ScreenMgr();
	~ScreenMgr();

public:
	void setMdiArea(QMdiArea* area);
	inline void setCurrentScreen(SCREEN_TYPE_T type);
	inline void registerScreenCreatorFunc(SCREEN_TYPE_T type, ScreenCreatorFunc creatorFunc);

private:

private:
	QMdiArea *mdiArea;
	std::list<ScreenDefinition_t*> screenDefinitions;

private:

};

#endif /* _LIBDOUBS_SCREENMGR_H */