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
#include "tinyrtp/rtcp/trtp_rtcp_report_sr.h"


//=================================================================================================
//	RTCP Sender Report (SR) Packet object definition
//
static tsk_object_t* trtp_rtcp_report_sr_ctor(tsk_object_t * self, va_list * app)
{
	trtp_rtcp_report_sr_t *packet = self;
	if(packet){
	}
	return self;
}

static tsk_object_t* trtp_rtcp_report_sr_dtor(tsk_object_t * self)
{ 
	trtp_rtcp_report_sr_t *packet = self;
	if(packet){
		// deinit self
		// deinit base
		trtp_rtcp_packet_deinit(TRTP_RTCP_PACKET(packet));
	}

	return self;
}

static const tsk_object_def_t trtp_rtcp_report_sr_def_s = 
{
	sizeof(trtp_rtcp_report_sr_t),
	trtp_rtcp_report_sr_ctor, 
	trtp_rtcp_report_sr_dtor,
	tsk_null, 
};
const tsk_object_def_t *trtp_rtcp_report_sr_def_t = &trtp_rtcp_report_sr_def_s;
