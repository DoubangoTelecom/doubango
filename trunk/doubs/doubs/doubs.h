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
#ifndef DOUBS_H
#define DOUBS_H

#include <QtGui/QMainWindow>
#include "ui_doubs.h"

class doubs : public QMainWindow
{
	Q_OBJECT

public:
	doubs(QWidget *parent = 0, Qt::WFlags flags = 0);
	~doubs();

private slots:
	void slotOnline();
	void slotBusy();

private:
	Ui::doubsClass ui;
};

#endif // DOUBS_H
