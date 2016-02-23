/* Copyright (C) 2011 Doubango Telecom <http://www.doubango.org>
* Copyright (C) 2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango(dot)org>
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

/**@file tsip_dialog_info.h
 * @brief SIP dialog INFO.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_DIALOG_INFO_H
#define TINYSIP_DIALOG_INFO_H

#include "tinysip_config.h"
#include "tinysip/dialogs/tsip_dialog.h"

TSIP_BEGIN_DECLS

/* Forward declaration */
struct tsip_message_s;

#define TSIP_DIALOG_INFO(self)							((tsip_dialog_info_t*)(self))

typedef struct tsip_dialog_info {
    TSIP_DECLARE_DIALOG;
    /**< Last incoming message. */
    struct tsip_message_s* last_iMessage;
}
tsip_dialog_info_t;

tsip_dialog_info_t* tsip_dialog_info_create(const tsip_ssession_handle_t* ss);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_dialog_info_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_DIALOG_INFO_H */
