/* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
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

#ifndef _TINYSAK_PLUGIN_H_
#define _TINYSAK_PLUGIN_H_

#include "tinysak_config.h"
#include "tsk_object.h"

TSK_BEGIN_DECLS

typedef enum tsk_plugin_def_type_e
{
	tsk_plugin_def_type_none = 0,
	tsk_plugin_def_type_consumer = (1 << 0),
	tsk_plugin_def_type_producer = (1 << 1),
	tsk_plugin_def_type_session = (1 << 2),
	tsk_plugin_def_type_codec = (1 << 3),
	tsk_plugin_def_type_resampler = (1 << 4),
	tsk_plugin_def_type_jb = (1 << 5),
	tsk_plugin_def_type_denoiser = (1 << 6),
	tsk_plugin_def_type_converter = (1 << 7),
	tsk_plugin_def_type_ipsec = (1 << 8),
	tsk_plugin_def_type_all = (~0)
}
tsk_plugin_def_type_t;

typedef enum tsk_plugin_def_media_type_e
{
	tsk_plugin_def_media_type_none = 0,
	tsk_plugin_def_media_type_audio = (1 << 0),
	tsk_plugin_def_media_type_video = (1 << 1),
	tsk_plugin_def_media_type_screencast = (1 << 2),
	tsk_plugin_def_media_type_all = (~0)
}
tsk_plugin_def_media_type_t;

typedef void tsk_plugin_handle_t;
typedef void tsk_plugin_symbol_t;
typedef const void* tsk_plugin_def_ptr_const_t;

TINYSAK_API struct tsk_plugin_s* tsk_plugin_create(const char* path);
TINYSAK_API tsk_plugin_def_ptr_const_t tsk_plugin_get_def(struct tsk_plugin_s* self, tsk_plugin_def_type_t type, tsk_plugin_def_media_type_t media_type);
TINYSAK_API tsk_plugin_def_ptr_const_t tsk_plugin_get_def_2(struct tsk_plugin_s* self, tsk_plugin_def_type_t type, tsk_plugin_def_media_type_t media_type, tsk_size_t index);
TINYSAK_API tsk_plugin_symbol_t* tsk_plugin_get_symbol(struct tsk_plugin_s* self, const char* symbol_name);
TINYSAK_API tsk_bool_t tsk_plugin_file_exist(const char* path);

TSK_END_DECLS

#endif /* _TINYSAK_PLUGIN_H_ */
