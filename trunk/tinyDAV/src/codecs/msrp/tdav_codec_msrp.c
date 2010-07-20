/*
* Copyright (C) 2009-2010 Mamadou Diop.
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

/**@file tdav_codec_msrp.c
 * @brief The Message Session Relay Protocol (MSRP) fake codec.
 * Used for both Message (RFC 4975) and file transfer (RFC 5547).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/codecs/msrp/tdav_codec_msrp.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

/* ============ MSRP Plugin interface ================= */

#define tdav_codec_msrp_fmtp_get tsk_null
#define tdav_codec_msrp_fmtp_set tsk_null

tsk_size_t tdav_codec_msrp_fmtp_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
	return 0;
}

tsk_size_t tdav_codec_msrp_fmtp_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
	return 0;
}

tsk_bool_t tdav_codec_msrp_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{	/* always match */
	return tsk_true;
}


//
//	MSRP Plugin definition
//

/* constructor */
static tsk_object_t* tdav_codec_msrp_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_msrp_t *msrp = self;
	if(msrp){
		/* init base: called by tmedia_codec_create() */
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_msrp_dtor(tsk_object_t * self)
{ 
	tdav_codec_msrp_t *msrp = self;
	if(msrp){
		/* deinit base */
		tmedia_codec_msrp_deinit(msrp);
		/* deinit self */
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_msrp_def_s = 
{
	sizeof(tdav_codec_msrp_t),
	tdav_codec_msrp_ctor, 
	tdav_codec_msrp_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_msrp_plugin_def_s = 
{
	&tdav_codec_msrp_def_s,

	tmedia_msrp,
	"message",
	"MSRP fake codec",
	TMEDIA_CODEC_FORMAT_MSRP,
	tsk_false,
	0, // rate
	
	 /* audio */
	{0},

	/* video */
	{0},

	tsk_null, // open
	tsk_null, // close
	tdav_codec_msrp_fmtp_encode,
	tdav_codec_msrp_fmtp_decode,
	tdav_codec_msrp_fmtp_match,
	tdav_codec_msrp_fmtp_get,
	tdav_codec_msrp_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_msrp_plugin_def_t = &tdav_codec_msrp_plugin_def_s;
