/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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

/**@file tipsec_common.c
 * @brief IPSec common functions.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tipsec_common.h"

#include "tsk_debug.h"

#if !HAVE_IPSEC_VISTA && !HAVE_IPSEC_XP && !HAVE_IPSEC_RACOON

int tipsec_start(tipsec_context_t* ctx)
{
	TSK_DEBUG_ERROR("No IPSec implementation found.");
	return -1;
}

int tipsec_set_local(tipsec_context_t* ctx, const char* addr_local, const char* addr_remote, tipsec_port_t port_uc, tipsec_port_t port_us)
{
	TSK_DEBUG_ERROR("No IPSec implementation found.");
	return -1;
}

int tipsec_set_remote(tipsec_context_t* ctx, tipsec_spi_t spi_pc, tipsec_spi_t spi_ps, tipsec_port_t port_pc, tipsec_port_t port_ps)
{
	TSK_DEBUG_ERROR("No IPSec implementation found.");
	return -1;
}

int tipsec_stop(tipsec_context_t* ctx)
{
	TSK_DEBUG_ERROR("No IPSec implementation found.");
	return -1;
}

#endif