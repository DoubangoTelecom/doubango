/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tipsec_racoon.c
 * @brief IPSec racoon functions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tipsec_racoon.h"

#include "tsk_debug.h"

/**@defgroup tipsec_racoon_group IPSec racoon implementation (IPSec-tools).
* Supported algo: <b>hmac-md5-96</b> and <b>hmac-sha-1-96</b>.<br>
* Supported ealg: <b>des-ede3-cbc</b>, <b>aes-cbc</b> and <b>null</b>.<br>
* Supported mode: <b>tunnel</b> and <b>transport</b>.<br>
* Supported proto: <b>ah</b> and <b>esp</b>.<br>
* Supported IP proto: <b>tcp</b> and <b>udp</b>.<br>
* Supported IP version: <b>IPv4</b> and <b>IPv6</b>.
*/
#if HAVE_IPSEC_RACOON

/**@ingroup tipsec_racoon_group
*/
int tipsec_start(tipsec_context_t* ctx)
{
	TSK_DEBUG_ERROR("No IPSec implementation found.");
	return -1;
}

/**@ingroup tipsec_racoon_group
*/
int tipsec_set_local(tipsec_context_t* ctx, const char* addr_local, const char* addr_remote, tipsec_port_t port_uc, tipsec_port_t port_us)
{
	TSK_DEBUG_ERROR("No IPSec implementation found.");
	return -1;
}

/**@ingroup tipsec_racoon_group
*/
int tipsec_set_remote(tipsec_context_t* ctx, tipsec_spi_t spi_pc, tipsec_spi_t spi_ps, tipsec_port_t port_pc, tipsec_port_t port_ps, tipsec_lifetime_t lifetime)
{
	TSK_DEBUG_ERROR("No IPSec implementation found.");
	return -1;
}

/**@ingroup tipsec_racoon_group
*/
int tipsec_stop(tipsec_context_t* ctx)
{
	TSK_DEBUG_ERROR("No IPSec implementation found.");
	return -1;
}



//=================================================================================================
//	IPSec context object definition
//
static tsk_object_t* tipsec_context_ctor(tsk_object_t * self, va_list * app)
{
	tipsec_context_t *context = self;
	if(context){
	}
bail:
	return self;
}

static tsk_object_t* tipsec_context_dtor(tsk_object_t * self)
{ 
	tipsec_context_t *context = self;
	if(context){
	}
	
	return self;
}

static int tipsec_context_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	return-1;
}

static const tsk_object_def_t tipsec_context_def_s = 
{
sizeof(tipsec_context_t),
tipsec_context_ctor, 
tipsec_context_dtor,
tipsec_context_cmp, 
};


const void *tipsec_context_def_t = &tipsec_context_def_s;


#endif /* HAVE_IPSEC_RACOON */
