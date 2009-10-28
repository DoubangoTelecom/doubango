#ifndef _LIBDOUBS_SCREEN_H
#define _LIBDOUBS_SCREEN_H

#include "libdoubs_config.h"
#include <QtGui/QWidget>

class Screen;

/* function pointer ... */
typedef Screen* (*ScreenCreatorFunc)();

/* screen type */
typedef enum SCREEN_TYPE_E
{
	SCT_AUTHENTICATION,
	SCT_CONTACTS,
	SCT_HISTORY
}
SCREEN_TYPE_T;

class LIBDOUBS_API Screen : public QWidget
{
	Q_OBJECT

public:
	Screen(SCREEN_TYPE_T type, QWidget *parent = 0, Qt::WFlags flags = 0);
	~Screen();

public:
	inline SCREEN_TYPE_T getType() const{ return this->type; }

protected:
	SCREEN_TYPE_T type;
};

#endif /* _LIBDOUBS_SCREEN_H */