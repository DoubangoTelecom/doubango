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
/**@file tsdp.c
 * @brief SDP (RFC 4566) implementations with both MMTel and PoC extensions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tsdp.h"

#include "tinysdp/parsers/tsdp_parser_message.h"

// Defined in tsdp_message.c
extern int __pred_find_media_by_name(const tsk_list_item_t *item, const void *name);


typedef struct tsdp_ctx_s {
    TSK_DECLARE_OBJECT;

    tsdp_message_t* local;
    tsdp_message_t* remote;
    tsdp_message_t* negotiated;
}
tsdp_ctx_t;

const tsdp_message_t* tsdp_ctx_local_get_sdp(const tsdp_ctx_handle_t* self)
{
    const tsdp_ctx_t* ctx = self;

    if(ctx) {
        return ctx->local;
    }
    else {
        return tsk_null;
    }
}

int tsdp_ctx_local_create_sdp(tsdp_ctx_handle_t* self, const tsdp_message_t* local)
{
    tsdp_ctx_t* ctx = self;
    tsdp_message_t* newsdp;

    if(!ctx || !local) {
        return -1;
    }

    // set new local sdp
    if((newsdp = tsdp_message_clone(local))) {
        TSK_OBJECT_SAFE_FREE(ctx->local);
        ctx->local = newsdp;
        return 0;
    }
    else {
        return -2;
    }
}

int tsdp_ctx_local_create_sdp_2(tsdp_ctx_handle_t* self, const char* sdp, tsk_size_t size)
{
    tsdp_ctx_t* ctx = self;
    tsdp_message_t* newsdp;

    if(!ctx || !sdp || !size) {
        return -1;
    }

    if((newsdp = tsdp_message_parse(sdp, size))) {
        TSK_OBJECT_SAFE_FREE(ctx->local);
        ctx->local = newsdp;
        return 0;
    }
    else {
        return -2;
    }
}

int tsdp_ctx_local_add_headers(tsdp_ctx_handle_t* self, ...)
{
    tsdp_ctx_t* ctx = self;
    const tsk_object_def_t* objdef;
    tsdp_header_t *header;
    va_list ap;

    if(!ctx || !ctx->local) {
        return -1;
    }

    va_start(ap, self);
    while((objdef = va_arg(ap, const tsk_object_def_t*))) {
        if((header = tsk_object_new_2(objdef, &ap))) {
            tsdp_message_add_header(ctx->local, header);
            TSK_OBJECT_SAFE_FREE(header);
        }
    }
    va_end(ap);

    return 0;
}

int tsdp_ctx_local_add_media(tsdp_ctx_handle_t* self, const tsdp_header_M_t* media)
{
    tsdp_ctx_t* ctx = self;

    if(!ctx || !media) {
        return -1;
    }

    if(ctx->local) {
        return tsdp_message_add_header(ctx->local, TSDP_HEADER(media));
    }
    else {
        return -2;
    }
}

int tsdp_ctx_local_add_media_2(tsdp_ctx_handle_t* self, const char* media, uint32_t port, const char* proto, ...)
{
    tsdp_ctx_t* ctx = self;
    va_list ap;

    if(!ctx || !media || !proto) {
        return -1;
    }

    if(ctx->local) {
        int ret;
        va_start(ap, proto);
        ret = tsdp_message_add_media_2(ctx->local, media, port, proto, &ap);
        va_end(ap);
        return ret;
    }
    else {
        return -2;
    }
}

const tsdp_message_t* tsdp_ctx_remote_get_sdp(const tsdp_ctx_handle_t* self)
{
    const tsdp_ctx_t* ctx = self;

    if(ctx) {
        return ctx->remote;
    }
    else {
        return tsk_null;
    }
}

const tsdp_message_t* tsdp_ctx_negotiated_get_sdp(const tsdp_ctx_handle_t* self)
{
    const tsdp_ctx_t* ctx = self;

    if(ctx) {
        return ctx->negotiated;
    }
    else {
        return tsk_null;
    }
}















//=================================================================================================
//	Sdp ctx object definition
//
static void* tsdp_ctx_create(void * self, va_list * app)
{
    tsdp_ctx_t *ctx = self;
    if(ctx) {
    }
    return self;
}

static void* tsdp_ctx_destroy(void * self)
{
    tsdp_ctx_t *ctx = self;
    if(ctx) {
        TSK_OBJECT_SAFE_FREE(ctx->local);
        TSK_OBJECT_SAFE_FREE(ctx->remote);
        TSK_OBJECT_SAFE_FREE(ctx->negotiated);
    }
    return self;
}

static const tsk_object_def_t tsdp_ctx_def_s = {
    sizeof(tsdp_ctx_t),
    tsdp_ctx_create,
    tsdp_ctx_destroy,
    tsk_null,
};
const tsk_object_def_t *tsdp_ctx_def_t = &tsdp_ctx_def_s;
