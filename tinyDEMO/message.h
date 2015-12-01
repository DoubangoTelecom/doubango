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
#if !defined(TINYDEMO_MESSAGE_H)
#define TINYDEMO_MESSAGE_H

#include "demo_config.h"

#include "common.h"

_BEGIN_DECLS

int message_handle_event(const tsip_event_t *_event);
tsip_ssession_id_t message_handle_cmd(cmd_type_t cmd, const opts_L_t* opts);

_END_DECLS

#endif /* TINYDEMO_MESSAGE_H */
