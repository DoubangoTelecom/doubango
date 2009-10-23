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

#ifndef __DOUBANGO_PREF_H__
#define __DOUBANGO_PREF_H__

#define PREF_NAMESPACE_USE		1
#define PREF_NAMESPACE_VALUE	dgo

/* doubango api funtions export */
#if defined(WIN32) || defined(__SYMBIAN32__)
#	ifdef DOUBANGO_EXPORTS
#	define DOUBANGO_API __declspec(dllexport)
#	define DOUBANGO_API_C extern "C" DOUBANGO_API
#	else
#	define DOUBANGO_API __declspec(dllimport)
#	define DOUBANGO_API_C
#	endif
#else
#	define DOUBANGO_API
#	define DOUBANGO_API_C
#endif

/* disable warnings */
#ifdef WIN32
#pragma warning( disable : 4132 4100 4127 4152 4355 4996 4251)
#endif

/* namespace definition*/
#if PREF_NAMESPACE_USE 
#	define PREF_NAMESPACE_START namespace PREF_NAMESPACE_VALUE {
#	define PREF_NAMESPACE_END	} 
#else
#	define PREF_NAMESPACE_START
#	define PREF_NAMESPACE_END
#endif

#endif /* __DOUBANGO_PREF_H__ */