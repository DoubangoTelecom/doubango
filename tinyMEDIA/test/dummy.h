/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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
#ifndef TEST_MEDIA_DUMMY_H
#define TEST_MEDIA_DUMMY_H

#include "tinymedia/tmedia.h"

#define DUMMY(self)	((dummy_t*)(self))

typedef struct dummy_s {
    TMED_DECLARE_MEDIA;

    char* local_sdp;
    char* remote_sdp;
    char* negotiated_sdp;
}
dummy_t;

const tsk_object_def_t *dummy_def_t;
const tmedia_plugin_def_t *dummy_plugin_def_t;


#endif /* TEST_MEDIA_DUMMY_H */
