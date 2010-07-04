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

/**@file tdav_codec_amr.c
 * @brief AMR-NB and AMR-WB codecs.
 * RTP payloader/depayloader are based on RFC 4867
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/codecs/amr/tdav_codec_amr.h"

#include "tsk_debug.h"

#if HAVE_OPENCORE_AMR

/* ============ Common ================= */
int tdav_codec_amr_init(tdav_codec_amr_t* self, tdav_codec_amr_type_t type, tdav_codec_amr_mode_t mode);
int tdav_codec_amr_deinit(tdav_codec_amr_t* self);


/* ============ AMR-NB Plugin interface ================= 
	The AMR codec was originally developed and standardized by the
	European Telecommunications Standards Institute (ETSI) for GSM
	cellular systems.  It is now chosen by the Third Generation
	Partnership Project (3GPP) as the mandatory codec for third
	generation (3G) cellular systems [1].

	The AMR codec is a multi-mode codec that supports eight narrow band
	speech encoding modes with bit rates between 4.75 and 12.2 kbps.  The
	sampling frequency used in AMR is 8000 Hz and the speech encoding is
	performed on 20 ms speech frames.  Therefore, each encoded AMR speech
	frame represents 160 samples of the original speech.

	Among the eight AMR encoding modes, three are already separately
	adopted as standards of their own.  Particularly, the 6.7 kbps mode
	is adopted as PDC-EFR [18], the 7.4 kbps mode as IS-641 codec in TDMA
	[17], and the 12.2 kbps mode as GSM-EFR [16].
*/

#define tdav_codec_amrnb_fmtp_set tsk_null

int tdav_codec_amrnb_open(tmedia_codec_t* self)
{
	tdav_codec_amrnb_t* amrnb = (tdav_codec_amrnb_t*)self;

	return 0;
}

int tdav_codec_amrnb_close(tmedia_codec_t* self)
{
	tdav_codec_amrnb_t* amrnb = (tdav_codec_amrnb_t*)self;

	return 0;
}

tsk_size_t tdav_codec_amrnb_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data)
{	
	tdav_codec_amrnb_t* amrnb = (tdav_codec_amrnb_t*)self;
	
	if(!self || !in_data || !in_size || !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	
	return 0;
}

tsk_size_t tdav_codec_amrnb_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, const tsk_object_t* proto_hdr)
{
	tdav_codec_amrnb_t* amrnb = (tdav_codec_amrnb_t*)self;

	if(!self || !in_data || !in_size || !out_data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	return 0;
}

char* tdav_codec_amrnb_fmtp_get(const tmedia_codec_t* codec)
{
	return tsk_null;
}

tsk_bool_t tdav_codec_amrnb_fmtp_match(const tmedia_codec_t* codec, const char* fmtp)
{	
	return tsk_true;
}


//
//	G.711u Plugin definition
//

/* constructor */
static tsk_object_t* tdav_codec_amrnb_ctor(tsk_object_t * self, va_list * app)
{
	tdav_codec_amrnb_t *amrnb = self;
	if(amrnb){
		/* init base: called by tmedia_codec_create() */
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_codec_amrnb_dtor(tsk_object_t * self)
{ 
	tdav_codec_amrnb_t *amrnb = self;
	if(amrnb){
		/* deinit base */
		tmedia_codec_audio_deinit(amrnb);
		/* deinit self */
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_amrnb_def_s = 
{
	sizeof(tdav_codec_amrnb_t),
	tdav_codec_amrnb_ctor, 
	tdav_codec_amrnb_dtor,
	tmedia_codec_cmp, 
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_amrnb_plugin_def_s = 
{
	&tdav_codec_amrnb_def_s,

	tmedia_audio,
	"AMR",
	"AMR Narrow Band - Bandwidth Efficient",
	TMEDIA_CODEC_FORMAT_AMR_NARROWBAND_BE,
	tsk_true,
	8000, // rate
	
	{ /* audio */
		1, // channels
		20 // ptime
	},

	/* video */
	{0},

	tdav_codec_amrnb_open,
	tdav_codec_amrnb_close,
	tdav_codec_amrnb_encode,
	tdav_codec_amrnb_decode,
	tdav_codec_amrnb_fmtp_match,
	tdav_codec_amrnb_fmtp_get,
	tdav_codec_amrnb_fmtp_set
};
const tmedia_codec_plugin_def_t *tdav_codec_amrnb_plugin_def_t = &tdav_codec_amrnb_plugin_def_s;










//
// Common functions
//

int tdav_codec_amr_init(tdav_codec_amr_t* self, tdav_codec_amr_type_t type, tdav_codec_amr_mode_t mode)
{
	if(self){
		self->type = type;
		self->mode = mode;

		return 0;
	}
	else{
		TSK_DEBUG_ERROR("Invalid Parameter");
		return -1;
	}
}

int tdav_codec_amr_deinit(tdav_codec_amr_t* self)
{
	if(self){
		

		return 0;
	}
	else{
		TSK_DEBUG_ERROR("Invalid Parameter");
		return -1;
	}
}


#endif /* HAVE_OPENCORE_AMR */
