/*
 * Copyright (C) 2012-2014 Mamadou DIOP
 * Copyright (C) 2012-2014 Doubango Telecom <http://www.doubango.org>.
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
#include "tnet_ice_event.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

static tsk_object_t* tnet_ice_event_ctor(tsk_object_t * self, va_list * app)
{
    tnet_ice_event_t *e = self;
    if(e) {

    }
    return self;
}
static tsk_object_t* tnet_ice_event_dtor(tsk_object_t * self)
{
    tnet_ice_event_t *e = self;
    if(e) {
        TSK_SAFE_FREE(e->phrase);
        TSK_OBJECT_SAFE_FREE(e->action);
        e->ctx = tsk_null; // not the owner (const)
    }

    return self;
}
static const tsk_object_def_t tnet_ice_event_def_s = {
    sizeof(tnet_ice_event_t),
    tnet_ice_event_ctor,
    tnet_ice_event_dtor,
    tsk_null,
};
const tsk_object_def_t *tnet_ice_event_def_t = &tnet_ice_event_def_s;


tnet_ice_event_t* tnet_ice_event_create(const struct tnet_ice_ctx_s* ctx, tnet_ice_event_type_t type, const char* phrase, const void* userdata)
{
    tnet_ice_event_t* e;

    if((e = tsk_object_new(tnet_ice_event_def_t))) {
        e->ctx = ctx;
        e->type = type;
        e->phrase = tsk_strdup(phrase);
        e->userdata = userdata;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create ICE event");
    }

    return e;
}

int tnet_ice_event_set_action(tnet_ice_event_t* self, struct tnet_ice_action_s* action)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->type = tnet_ice_event_type_action;
    TSK_OBJECT_SAFE_FREE(self->action);
    if(action) {
        self->action = tsk_object_ref(action);
    }
    return 0;
}
