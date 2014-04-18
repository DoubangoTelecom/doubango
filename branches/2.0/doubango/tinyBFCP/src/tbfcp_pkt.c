/* Copyright (C) 2014 Mamadou DIOP.
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
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
#include "tinybfcp/tbfcp_pkt.h"

#include "tsk_debug.h"


//=================================================================================================
//	BFCP Packet object definition
//
static tsk_object_t* tbfcp_pkt_ctor(tsk_object_t * self, va_list * app)
{
    tbfcp_pkt_t *p_pkt = (tbfcp_pkt_t *)self;
    if (p_pkt) {
    }
    return self;
}
static tsk_object_t* tbfcp_pkt_dtor(tsk_object_t * self)
{
    tbfcp_pkt_t *p_pkt = (tbfcp_pkt_t *)self;
    if (p_pkt) {
        TSK_DEBUG_INFO("*** BFCP Packet destroyed ***");

    }

    return self;
}
static int tbfcp_pkt_cmp(const tsk_object_t *_pk1, const tsk_object_t *_pk2)
{
    const tbfcp_pkt_t *pc_pk1 = (const tbfcp_pkt_t *)_pk1;
    const tbfcp_pkt_t *pc_pk2 = (const tbfcp_pkt_t *)_pk2;

    return (int)(pc_pk1-pc_pk2);
}
static const tsk_object_def_t tbfcp_pkt_def_s = {
    sizeof(tbfcp_pkt_t),
    tbfcp_pkt_ctor,
    tbfcp_pkt_dtor,
    tbfcp_pkt_cmp,
};
const tsk_object_def_t *tbfcp_pkt_def_t = &tbfcp_pkt_def_s;
