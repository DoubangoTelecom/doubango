/*
* Copyright (C) 2012-2015 Doubango Telecom <http://www.doubango.org>
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

/**@file tmedia_imageattr.c
 * @brief 'image-attr' parser as per RFC 6236
 */
#include "tinymedia/tmedia_imageattr.h"

#include "tsk_ragel_state.h"
#include "tsk_debug.h"

#include <stdlib.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tmedia_machine_imageattr;

	# Includes
	include tmedia_machine_utils "./ragel/tmedia_machine_utils.rl";

	action tag{
		tag_start = p;
	}

	action is_send{
		sets = &self->send.sets[0];
		sets_count = &self->send.count;
		*sets_count = 0;
	}

	action is_recv{
		sets = &self->recv.sets[0];
		sets_count = &self->recv.count;
		*sets_count = 0;
	}

	action is_xrange{
		xyrange = (*sets_count < TMEDIA_imageattr_ARRAY_MAX_SIZE) ? &sets[*sets_count].xrange : tsk_null;
	}
	action is_yrange{
		xyrange = (*sets_count < TMEDIA_imageattr_ARRAY_MAX_SIZE) ? &sets[*sets_count].yrange : tsk_null;
	}
	action set_parsed{
		if(*sets_count < TMEDIA_imageattr_ARRAY_MAX_SIZE) (*sets_count)++;
	}
	action set_is_not_range{
		if(xyrange) xyrange->is_range = 0;
	}
	action set_is_range{
		if(xyrange) xyrange->is_range = 1;
	}
	action parse_xyvalue_range_start{
		if(xyrange)xyrange->range.start = atoi(tag_start);
	}
	action parse_xyvalue_range_step{
		if(xyrange)xyrange->range.step = atoi(tag_start);
	}
	action parse_xyvalue_range_end{
		if(xyrange)xyrange->range.end = atoi(tag_start);
	}
	action parse_xyvalue_array_value{
		if(xyrange && xyrange->array.count < TMEDIA_imageattr_ARRAY_MAX_SIZE){
			xyrange->array.values[xyrange->array.count++] = atoi(tag_start);
		}
	}
	action set_parse_qvalue{
		if(*sets_count < TMEDIA_imageattr_ARRAY_MAX_SIZE){
			sets[*sets_count].qvalue = atof(tag_start);
		}
	}

	action srange_is_array{
		if(srange) srange->is_range = 0;
	}
	action srange_is_not_array{
		if(srange) srange->is_range = 1;
	}
	action start_srange{
		srange = (*sets_count < TMEDIA_imageattr_ARRAY_MAX_SIZE) ? &sets[*sets_count].srange : tsk_null;
	}
	action parse_srange_array_value{
		if(srange && srange->array.count < TMEDIA_imageattr_ARRAY_MAX_SIZE){
			srange->array.values[srange->array.count++] = atof(tag_start);
		}
	}
	action parse_srange_range_start{
		if(srange) srange->range.start = atof(tag_start);
	}
	action parse_srange_range_end{
		if(srange) srange->range.end = atof(tag_start);
	}

	action parse_prange_start{
		if(*sets_count < TMEDIA_imageattr_ARRAY_MAX_SIZE) sets[*sets_count].prange.start = atof(tag_start);
	}
	action parse_prange_end{
		if(*sets_count < TMEDIA_imageattr_ARRAY_MAX_SIZE) sets[*sets_count].prange.end = atof(tag_start), sets[*sets_count].prange.is_present = 1;
	}

	PT = DIGIT+ | "*";
	onetonine = "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
	xyvalue = (onetonine DIGIT{,5});
	step = xyvalue;
	xyrange = ( "[" xyvalue>tag %parse_xyvalue_range_start ":" ( step>tag %parse_xyvalue_range_step ":" )? xyvalue>tag %parse_xyvalue_range_end "]" )%set_is_range | ( "[" xyvalue>tag %parse_xyvalue_array_value ( "," xyvalue>tag %parse_xyvalue_array_value )+ "]" )%set_is_not_range | xyvalue%set_is_not_range>tag %parse_xyvalue_array_value;
	spvalue = ( "0" "." onetonine DIGIT{,3} ) | ( onetonine "." DIGIT{1,4} );
	srange = (( "[" spvalue>tag %parse_srange_array_value ( "," spvalue>tag %parse_srange_array_value )+ "]" )%srange_is_array | ( "[" spvalue>tag %parse_srange_range_start "-" spvalue>tag %parse_srange_range_end "]" )%srange_is_not_array | spvalue>tag %parse_srange_array_value %srange_is_array)>start_srange;
	prange = ( "[" spvalue>tag %parse_prange_start "-" spvalue>tag %parse_prange_end "]" );
	_qvalue = (( "0" "." DIGIT{1,2} ) | ( "1" "." "0"{1,2} ))>tag %set_parse_qvalue; #qvalue collision
	key_value = ( "sar="i srange ) | ( "par="i prange ) | ( "q="i _qvalue );
	set = ("[" "x="i xyrange>is_xrange "," "y="i xyrange>is_yrange ( "," key_value )* "]")%set_parsed;
	attr_list = ( set ( WSP+ set )* ) | "*";
	imageattr = ( WSP* ( "send"i%is_send | "recv"i%is_recv ) WSP+ attr_list ){1,2};

	# Entry point
	main:= imageattr;
}%%

static int tmedia_imageattr_reset(tmedia_imageattr_xt* self)
{
	if(self){
		tsk_size_t i;
		memset(self, 0, sizeof(*self));
		for(i = 0; i < TMEDIA_imageattr_ARRAY_MAX_SIZE; ++i){
			self->send.sets[i].qvalue = 0.5;
			self->recv.sets[i].qvalue = 0.5;
		}
		return 0;
	}
	TSK_DEBUG_ERROR("Invalid parameter");
	return -1;
}

int tmedia_imageattr_parse(tmedia_imageattr_xt* self, const void* in_data, tsk_size_t in_size)
{
	int cs = 0;
	const char *p = in_data;
	const char *pe = p + in_size;
	const char *eof = pe;
	
	const char *tag_start = tsk_null;

	tmedia_imageattr_set_xt* sets = tsk_null;
	tsk_size_t* sets_count = tsk_null;
	tmedia_imageattr_xyrange_xt* xyrange = tsk_null;
	tmedia_imageattr_srange_xt* srange = tsk_null;

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write data;
	(void)(eof);
	(void)(tmedia_machine_imageattr_first_final);
	(void)(tmedia_machine_imageattr_error);
	(void)(tmedia_machine_imageattr_en_main);
	%%write init;
	tmedia_imageattr_reset(self);
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()

	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Parsing failed to parse image-attr=%s", (char*)in_data);
		return -1;
	}
	
	return 0;
}