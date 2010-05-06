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

/**@file tmedia_codec_dummy.c
 * @brief Dummy codecs used for test only.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinymedia/tmedia_codec_dummy.h"

#include "tsk_debug.h"

/**@ingroup tmedia_codec_group
* Creates dummy G.711u codec 
*/
tmedia_codec_dpcmu_t* tmedia_codec_dpcmu_create()
{
	tmedia_codec_dpcmu_t* dpcmu;

	/* creates the codec */
	if(!(dpcmu = tsk_object_new(tmedia_codec_dpcmu_def_t, "G.711u", "Dummy G.711u codec", TMEDIA_CODEC_FORMAT_G711u))){
		TSK_DEBUG_ERROR("Failed to create G.711u codec");
		return tsk_null;
	}

	return dpcmu;
}


//=================================================================================================
//	Dummy G.711u object definition
//
static tsk_object_t* tmedia_codec_dpcmu_ctor(tsk_object_t * self, va_list * app)
{
	tmedia_codec_dpcmu_t *dpcum = self;
	if(dpcum){
		const char* name = va_arg(*app, const char *);
		const char* desc = va_arg(*app, const char *);
		const char* format = va_arg(*app, const char *);

		/* init base */
		tmedia_codec_audio_init(dpcum, name, desc, format); 
		/* init self */
	}
	return self;
}

static tsk_object_t* tmedia_codec_dpcmu_dtor(tsk_object_t * self)
{ 
	tmedia_codec_dpcmu_t *dpcum = self;
	if(dpcum){
		/* deinit base */
		tmedia_codec_audio_deinit(dpcum);
		/* deinit self */
	}

	return self;
}

static int tmedia_codec_dpcmu_cmp(const tsk_object_t *_c1, const tsk_object_t *_c2)
{
	return tmedia_codec_cmp(TMEDIA_CODEC(_c1), TMEDIA_CODEC(_c2));
}

static const tsk_object_def_t tmedia_codec_dpcmu_def_s = 
{
	sizeof(tmedia_codec_dpcmu_t),
	tmedia_codec_dpcmu_ctor, 
	tmedia_codec_dpcmu_dtor,
	tmedia_codec_dpcmu_cmp, 
};
const tsk_object_def_t *tmedia_codec_dpcmu_def_t = &tmedia_codec_dpcmu_def_s;

