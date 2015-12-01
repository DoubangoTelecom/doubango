/*Copyright (C) 2013 Doubango Telecom <http://www.doubango.org>
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
*/
#pragma once

#define rtSafeDelete(ptr) if((ptr)) delete (ptr), (ptr) = nullptr;

#if COM_VISIBLE
#	define OVERRIDE_FUNC(fun) 
#	define VISIBLE_VOID_PTR Platform::String^
#	define VISIBLE_VOID_PTR_2_VOID_PTR(ptr) rtString::toUtf8((ptr)).data()
#else
#	define OVERRIDE_FUNC(fun) fun
#	define VISIBLE_VOID_PTR Platform::IntPtr
#	define VISIBLE_VOID_PTR_2_VOID_PTR(ptr) (void*)((ptr))
#endif
	