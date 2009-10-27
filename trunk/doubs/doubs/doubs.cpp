/****************************************************************************
		 _             _                             
		| |           | |                            
	  _ | | ___  _   _| | _   ____ ____   ____  ___  
	 / || |/ _ \| | | | || \ / _  |  _ \ / _  |/ _ \ 
	( (_| | |_| | |_| | |_) | ( | | | | ( ( | | |_| |
	 \____|\___/ \____|____/ \_||_|_| |_|\_|| |\___/ 
                                        (_____|

	Copyright (C) 2009 xxxyyyzzz <imsframework(at)gmail.com>

	This file is part of Open Source Doubango IMS Client Framework project.

    DOUBANGO is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
	
    DOUBANGO is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
	
    You should have received a copy of the GNU General Public License
    along with DOUBANGO.
****************************************************************************/
#include "doubs.h"

#include <QMessageBox>

#include <Screen.h>

doubs::doubs(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	setCentralWidget(ui.mdiArea);

	/* slots */
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionOnline, SIGNAL(triggered()), this, SLOT(slotOnline()));
	connect(ui.actionBusy, SIGNAL(triggered()), this, SLOT(slotBusy()));
}

doubs::~doubs()
{

}

/*******************************************
SLOTS
*******************************************/

/* change user status to 'online' */
void doubs::slotOnline()
{
	QMessageBox::information(this, tr("User status"),
            tr("change user status to 'online'"));

	//Screen* s = new Screen(SCT_LOGIN);
}

/* change user status to 'busy' */
void doubs::slotBusy()
{
	QMessageBox::warning(this, tr("User status"),
            tr("change user status to 'busy'"));
}