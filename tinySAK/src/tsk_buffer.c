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

/**@file tsk_buffer.c
 * @brief Buffer manager.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tsk_buffer.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

#if defined(_WIN32_WCE)
#	include "tsk_string.h" /* tsk_strlen() */
#endif


#include <stdio.h>
#include <string.h>

/**@defgroup tsk_buffer_group Buffer management.
*/

#if defined(_MSC_VER) || TSK_UNDER_WINDOWS
#	define vsnprintf	_vsnprintf
#endif

/**@ingroup tsk_buffer_group
* Creates new buffer.
* @param data A pointer to the data to copy into the newly created buffer.
* @param size The size of the data to copy.
* @retval A new buffer object.
* @sa @ref tsk_buffer_create_null
*/
tsk_buffer_t* tsk_buffer_create(const void* data, tsk_size_t size)
{
    return (tsk_buffer_t*)tsk_object_new(tsk_buffer_def_t, data, size);
}

/**@ingroup tsk_buffer_group
* Creates a new empty buffer.
* @retval A new empty buffer object.
* @sa tsk_buffer_create.
*/
tsk_buffer_t* tsk_buffer_create_null()
{
    return tsk_buffer_create(tsk_null, 0);
}

/**@ingroup tsk_buffer_group
* Appends new data to the buffer.
* @param self The buffer to append to. The buffer should be created using @ref tsk_buffer_create or @ref tsk_buffer_create_null.
* @param format A string with embedded tag to be substituted.
* @param ... List of parameters.
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref tsk_buffer_append.
*
* @code
* tsk_buffer_t* buffer = tsk_buffer_create_null();
* tsk_buffer_append_2(buffer, "str1=%s, c1=%c and val1=%x", "str1", 'c', 0x2554);
* printf(TSK_BUFFER_TO_STRING(buffer));
* TSK_OBJECT_SAFE_FREE(buffer);
* @endcode
*/
int tsk_buffer_append_2(tsk_buffer_t* self, const char* format, ...)
{
    /*
     * I suppose that sizeof(char) = 1-byte
     */
    int len = 0;
    va_list ap;
    char *buffer;
    tsk_size_t oldsize;
    va_list ap2;

    if(!self) {
        return -1;
    }

    oldsize = self->size;
    buffer = (char*)TSK_BUFFER_DATA(self);

    /* initialize variable arguments (needed for 64bit platforms where vsnprintf will change the va_list) */
    va_start(ap, format);
    va_start(ap2, format);

    /* compute destination len for windows mobile
    */
#if defined(_WIN32_WCE)
    {
        int n;
        len = (tsk_strlen(format)*2);
        buffer = (char*)tsk_realloc(buffer, (oldsize+len));
        for(;;) {
            if( (n = vsnprintf((char*)(buffer + oldsize), len, format, ap)) >= 0 && (n<=len) ) {
                len = n;
                break;
            }
            else {
                len += 10;
                buffer = (char*)tsk_realloc(buffer, (oldsize+len));
            }
        }
    }
#else
    len = vsnprintf(tsk_null, 0, format, ap);
    buffer = (char*)tsk_realloc(buffer, oldsize+len+1);
    vsnprintf((buffer + oldsize), len
#if !defined(_MSC_VER) || defined(__GNUC__)
              +1
#endif
              , format, ap2);
#endif

    /* reset variable arguments */
    va_end(ap);
    va_end(ap2);

    self->data = buffer;
    self->size = (oldsize+len);

    return 0;
}

/**@ingroup tsk_buffer_group
* Appends data to the buffer.
* @param self The buffer to append to. The buffer should be created using @ref tsk_buffer_create or @ref tsk_buffer_create_null.
* @param data The data to append to the buffer.
* @param size The size of the @a data to append.
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref tsk_buffer_append_2.
*
* @code
* tsk_buffer_t* buffer = tsk_buffer_create_null();
* tsk_buffer_append(buffer, "doubango", tsk_strlen("doubango"));
* printf(TSK_BUFFER_TO_STRING(buffer));
* TSK_OBJECT_SAFE_FREE(buffer);
* @endcode
*/
int tsk_buffer_append(tsk_buffer_t* self, const void* data, tsk_size_t size)
{
    if(self && size) {
        tsk_size_t oldsize = self->size;
        tsk_size_t newsize = oldsize + size;

        if(oldsize) {
            self->data = tsk_realloc(self->data, newsize);
        }
        else {
            self->data = tsk_calloc(size, sizeof(uint8_t));
        }

        if(self->data) {
            if(data) {
                memcpy((void*)(TSK_BUFFER_TO_U8(self) + oldsize), data, size);
            }
            self->size = newsize;
            return 0;
        }
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
    }
    return -1;
}

/**@ingroup tsk_buffer_group
* Reallocates the buffer.
* @param self The buffer to realloc.
* @param size The new size.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tsk_buffer_realloc(tsk_buffer_t* self, tsk_size_t size)
{
    if(self) {
        if(size == 0) {
            return tsk_buffer_cleanup(self);
        }

        if(self->size == 0) { // first time?
            self->data = tsk_calloc(size, sizeof(uint8_t));
        }
        else if(self->size != size) { // only realloc if different sizes
            self->data = tsk_realloc(self->data, size);
        }

        self->size = size;
        return 0;
    }
    return -1;
}

/**@ingroup tsk_buffer_group
* Removes a chunck of data from the buffer.
* @param self The buffer from which to remove the chunck.
* @param position The chunck start position.
* @param size The size of the chunck.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tsk_buffer_remove(tsk_buffer_t* self, tsk_size_t position, tsk_size_t size)
{
    if(self && self->data && size) {
        if((position == 0) && ((position + size) >= self->size)) { /* Very common case. */
            return tsk_buffer_cleanup(self);
        }
        else if((position + size) < self->size) {
            memcpy(((uint8_t*)self->data) + position, ((uint8_t*)self->data) + position + size,
                   self->size-(position+size));
            return tsk_buffer_realloc(self, (self->size-size));
        }
    }
    return -1;
}

/**@ingroup tsk_buffer_group
* Inserts a chunck of data into the buffer.
* @param self The buffer to insert to.
* @param position The starting position to insert to.
* @param data A pointer to the chunck to insert.
* @param size The size of the chunck.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tsk_buffer_insert(tsk_buffer_t* self, tsk_size_t position, const void* data, tsk_size_t size)
{
    if(self && size) {
        int ret;
        tsk_size_t tomove;

        if(position > self->size) {
            TSK_DEBUG_ERROR("Invalid parameter");
            return -2;
        }

        tomove = (self->size - position);

        if((ret = tsk_buffer_realloc(self, (self->size + size)))) {
            return ret;
        }
        memmove(((uint8_t*)self->data) + position + size, ((uint8_t*)self->data) + position,
                tomove/*self->size - (position + size)*/);


        if(data) {
            memcpy(((uint8_t*)self->data) + position, data, size);
        }
        else {
            memset(((uint8_t*)self->data) + position, 0, size);
        }

        return 0;
    }
    return -1;
}

int tsk_buffer_copy(tsk_buffer_t* self, tsk_size_t start, const void* data, tsk_size_t size)
{
    int ret = 0;
    if(!self || !data || !size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // realloc the buffer to match the overral size
    if(self->size != (start + size) && (ret = tsk_buffer_realloc(self, (start + size)))) {
        TSK_DEBUG_ERROR("failed to realloc the buffer");
        return ret;
    }

    memcpy(((uint8_t*)self->data) + start, data, size);
    return ret;
}

/**@ingroup tsk_buffer_group
* Cleanups the internal data and reset the size.
* @param self The buffer holding the internal data to free.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tsk_buffer_cleanup(tsk_buffer_t* self)
{
    if(self && self->data) {
        tsk_free(&(self->data));
        self->size = 0;
    }
    return 0;
}

/**@ingroup tsk_buffer_group
* Takes the ownership of the @a data. If the destination buffer had data, then it will
* be cleaned up.
* @param self The buffer
* @param data
* @param size
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tsk_buffer_takeownership(tsk_buffer_t* self, void** data, tsk_size_t size)
{
    if(!self || !data || !*data || !size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(self->data) {
        tsk_free(&(self->data));
    }
    self->data = *data;
    self->size = size;
    *data = tsk_null;

    return 0;
}












//=================================================================================================
//	Buffer object definition
//
static tsk_object_t* tsk_buffer_ctor(tsk_object_t * self, va_list * app)
{
    tsk_buffer_t *buffer = (tsk_buffer_t *)self;
    const void *data = va_arg(*app, const void *);
    tsk_size_t size = va_arg(*app, tsk_size_t);

    if (size) {
        buffer->data = tsk_calloc((size+1), sizeof(uint8_t));
        if (data) {
            memcpy(buffer->data, data, size);
        }
        buffer->size = size;
    }
    return self;
}

static tsk_object_t* tsk_buffer_dtor(tsk_object_t * self)
{
    tsk_buffer_t *buffer = (tsk_buffer_t *)self;
    if(buffer) {
        TSK_FREE(buffer->data);
        buffer->size = 0;
    }

    return self;
}

static const tsk_object_def_t tsk_buffer_def_s = {
    sizeof(tsk_buffer_t),
    tsk_buffer_ctor,
    tsk_buffer_dtor,
    tsk_null,
};
const tsk_object_def_t *tsk_buffer_def_t = &tsk_buffer_def_s;

