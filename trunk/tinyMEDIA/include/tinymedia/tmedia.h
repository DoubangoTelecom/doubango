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

/**@file tmedia.h
 * @brief Media.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYMEDIA_TMEDIA_H
#define TINYMEDIA_TMEDIA_H

#include "tinymedia_config.h"

#include "tsk_params.h"

TMEDIA_BEGIN_DECLS

#define TMEDIA_VA_ARGS(name)		tmedia_def_t, (const char*)name
#define TMEDIA_CREATE(name)			tsk_object_new(name))
#define TMEDIA_CREATE_NULL()		TSDP_HEADER_M_CREATE(tsk_null)

#define TMEDIA(self)		((tmedia_t*)(self))

typedef struct tmedia_s
{
	TSK_DECLARE_OBJECT;

	const struct tmedia_plugin_def_s* plugin;

	char* name;
	uint32_t port;
	char* protocol;
}
tmedia_t;
typedef tsk_list_t tmedias_L_t;

#define TMED_DECLARE_MEDIA tmedia_t media

typedef struct tmedia_plugin_def_s
{
	const tsk_object_def_t* objdef;
	const char* name;

	int (* set_params) (tmedia_t* , const tsk_params_L_t* );

	int	(* start) (tmedia_t* );
	int	(* pause) (tmedia_t* );
	int	(* stop) (tmedia_t* );

	char* (* get_local_offer) (tmedia_t* );
	char* (* get_negotiated_offer) (tmedia_t* );
	int (* set_remote_offer) (tmedia_t* , const char* );
}
tmedia_plugin_def_t;

TINYMEDIA_API int tmedia_init(tmedia_t* self, const char* name);
TINYMEDIA_API int tmedia_deinit(tmedia_t* self);

TINYMEDIA_API int tmedia_plugin_register(const tmedia_plugin_def_t* def);
TINYMEDIA_API tmedia_t* tmedia_factory_create(const char* name);

TINYMEDIA_API int tmedia_set_params(tmedia_t* , const tsk_params_L_t* );

TINYMEDIA_API int tmedia_start(tmedia_t* );
TINYMEDIA_API int tmedia_pause(tmedia_t* );
TINYMEDIA_API int tmedia_stop(tmedia_t* );

TINYMEDIA_API char* tmedia_get_local_offer(tmedia_t* );
TINYMEDIA_API char* tmedia_get_negotiated_offer(tmedia_t* );
TINYMEDIA_API int tmedia_set_remote_offer(tmedia_t* , const char* );

TINYMEDIA_GEXTERN const void *tmedia_def_t;

TMEDIA_END_DECLS


#endif /* TINYMEDIA_TMEDIA_H */
