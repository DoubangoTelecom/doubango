/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
*	
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*	
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
#ifndef TINYWRAP_SAFEOBJECT_H
#define TINYWRAP_SAFEOBJECT_H

#include "tinyWRAP_config.h"

#include "tsk_mutex.h"

class TINYWRAP_API SafeObject
{
public:
	SafeObject();
	virtual ~SafeObject();

/* protected: */
	int Lock()const;
	int UnLock()const;

private:
	tsk_mutex_handle_t *mutex;
};

#endif /* TINYWRAP_SAFEOBJECT_H */
