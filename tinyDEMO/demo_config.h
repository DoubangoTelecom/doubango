/*
* Copyright (C) 2009 Mamadou Diop.
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
#if !defined(TINYDEMO_CONFIG_H)
#define TINYDEMO_CONFIG_H

#ifdef __SYMBIAN32__
#undef _WIN32 /* Because of WINSCW */
#endif

/* Guards against C++ name mangling  */
#ifdef __cplusplus
#	define _BEGIN_DECLS extern "C" {
#	define _END_DECLS }
#else
#	define _BEGIN_DECLS
#	define _END_DECLS
#endif

#ifdef _MSC_VER
#	define _CRT_SECURE_NO_WARNINGS
#	pragma warning( disable : 4996 )
#endif

/* use SIP OPTIONS and "rport" to hack  the AOR
* usefult on some emulator (such as Android) with non-routable IP addresses */
#define HACK_AOR 0

#endif /* TINYDEMO_CONFIG_H */
