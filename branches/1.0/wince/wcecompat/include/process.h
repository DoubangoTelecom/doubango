/*  wcecompat: Windows CE C Runtime Library "compatibility" library.
 *
 *  Copyright (C) 2001-2002 Essemer Pty Ltd.  All rights reserved.
 *  http://www.essemer.com.au/
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef __wcecompat__PROCESS_H__
#define __wcecompat__PROCESS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

_CRTIMP uintptr_t __cdecl _beginthread (void( *start_address )( void * ),
   unsigned stack_size, void *arglist );
#define beginthread _beginthread

_CRTIMP uintptr_t __cdecl _beginthreadex( 
   void *security,
   unsigned stack_size,
   unsigned ( *start_address )( void * ),
   void *arglist,
   unsigned initflag,
   unsigned *thrdaddr);
#define beginthreadex _beginthreadex

_CRTIMP void __cdecl _endthread (void);
#define endthread _endthread

_CRTIMP void __cdecl _endthreadex (unsigned retval);
#define endthreadex _endthreadex

int __cdecl _getpid(void);
#define getpid _getpid


#ifdef __cplusplus
}
#endif


#endif // __wcecompat__PROCESS_H__
