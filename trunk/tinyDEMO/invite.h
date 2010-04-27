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
#if !defined(TINYDEMO_INVITE_H)
#define TINYDEMO_INVITE_H

#include "demo_config.h"

#include "tsk_options.h"

_BEGIN_DECLS

/* Forward declarations */
struct tsip_event_s;
enum cmd_type_e;

int invite_handle_event(const struct tsip_event_s *sipevent);
int invite_handle_cmd(enum cmd_type_e cmd, const tsk_options_L_t* options);

_END_DECLS

#endif /* TINYDEMO_INVITE_H */