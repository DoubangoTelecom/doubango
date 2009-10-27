#ifndef _LIBDOUBS_SCREENMGR_H
#define _LIBDOUBS_SCREENMGR_H

#include "libdoubs_config.h"

#include "Screen.h"

#include <QtGui/QMdiArea>

#include <list>

class LIBDOUBS_API ScreenMgr
{
public:
	ScreenMgr();
	~ScreenMgr();

public:
	void setMdiArea(QMdiArea* area);
	const Screen* getSreen(SCREEN_TYPE_T type) const;
	inline void addScreen(const Screen* screen);
	inline void removeScreen(const Screen* screen);
	inline void removeScreen(SCREEN_TYPE_T type);

private:
	static inline std::list<const Screen*> getScreens() { return ScreenMgr::screens; }

private:
	QMdiArea *mdiArea;
	static std::list<const Screen*> screens;

private:

};

#endif /* _LIBDOUBS_SCREENMGR_H */