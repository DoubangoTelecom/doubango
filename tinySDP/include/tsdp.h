/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
/**@file tsdp.h
 * @brief SDP (RFC 4566) implementations with both MMTel and PoC extensions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSDP_TSDP_H
#define TINYSDP_TSDP_H

#include "tinysdp_config.h"

#include "tinysdp/tsdp_message.h"

TSDP_BEGIN_DECLS

#define TSDP_CTX_CREATE()	tsk_object_new(tsdp_ctx_def_t)

typedef void tsdp_ctx_handle_t;

TINYSDP_API const tsdp_message_t* tsdp_ctx_local_get_sdp(const tsdp_ctx_handle_t* self);
TINYSDP_API int tsdp_ctx_local_create_sdp(tsdp_ctx_handle_t* self, const tsdp_message_t* local);
TINYSDP_API int tsdp_ctx_local_create_sdp_2(tsdp_ctx_handle_t* self, const char* sdp, tsk_size_t size);
TINYSDP_API int tsdp_ctx_local_add_headers(tsdp_ctx_handle_t* self, ...);
TINYSDP_API int tsdp_ctx_local_add_media(tsdp_ctx_handle_t* self, const tsdp_header_M_t* media);
TINYSDP_API int tsdp_ctx_local_add_media_2(tsdp_ctx_handle_t* self, const char* media, uint32_t port, const char* proto, ...);

TINYSDP_API const tsdp_message_t* tsdp_ctx_remote_get_sdp(const tsdp_ctx_handle_t* self);

TINYSDP_API const tsdp_message_t* tsdp_ctx_negotiated_get_sdp(const tsdp_ctx_handle_t* self);


// 3GPP TS 24.610 Communication HOLD
TINYSDP_API int tsdp_ctx_hold(tsdp_ctx_handle_t* self, const char* media);
TINYSDP_API int tsdp_ctx_resume(tsdp_ctx_handle_t* self, const char* media);

TINYSDP_GEXTERN const tsk_object_def_t *tsdp_ctx_def_t;

TSDP_END_DECLS


#endif /* TINYSDP_TSDP_H */
