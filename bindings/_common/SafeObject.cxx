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
#include "SafeObject.h"

SafeObject::SafeObject()
{
	this->mutex = tsk_mutex_create();
}

int SafeObject::Lock()const
{
	return tsk_mutex_lock(this->mutex);
}

int SafeObject::UnLock()const
{
	return tsk_mutex_unlock(this->mutex);
}

SafeObject::~SafeObject()
{
	tsk_mutex_destroy(&this->mutex);
}
