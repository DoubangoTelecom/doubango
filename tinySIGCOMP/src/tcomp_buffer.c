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

/**@file tcomp_buffer.c
 * @brief SigComp Buffer.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#include "tcomp_buffer.h"
#include "tsk_binaryutils.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <string.h>

/** SigComp buffer.
*/
typedef struct tcomp_buffer_s {
    TSK_DECLARE_OBJECT;

    tsk_size_t		size;			/**< The size of the buffer */
    uint8_t*	lpbuffer;		/**< Pointer to the buffer */
    tsk_size_t		index_bytes;	/**< Bytes (8bit size) cursor */
    tsk_size_t		index_bits;		/**< Bits (1-bit size) cursor */
    unsigned	owner:1;		/**< Indicates whether we are the owner of the buffer or not (external buffer) */
    uint8_t		P_BIT;			/**< P-BIT controller. */
}
tcomp_buffer_t;


tcomp_buffer_handle_t* tcomp_buffer_create(const void* data, tsk_size_t len)
{
    tcomp_buffer_t* buffer;
    if((buffer = tsk_object_new(tcomp_buffer_def_t))) {
        buffer->owner = tsk_true;
        // The P-bit controls the order in which bits are passed from the dispatcher to the INPUT instructions.
        buffer->P_BIT = TCOMP_P_BIT_MSB_TO_LSB;
        if(data && len) {
            tcomp_buffer_appendBuff(buffer, data, len);
        }
    }
    return buffer;
}

tcomp_buffer_handle_t* tcomp_buffer_create_null()
{
    return tcomp_buffer_create(tsk_null, 0);
}

/**Compares two sigomp buffers.
* @param handle1 First handle to compare.
* @param handle2 Second handle to compare.
* @retval @a tsk_true if the two handles are equals and @a tsk_false otherwise.
*/
tsk_bool_t tcomp_buffer_equals(const tcomp_buffer_handle_t* handle1, const tcomp_buffer_handle_t* handle2)
{
    if( tcomp_buffer_getSize(handle1) == tcomp_buffer_getSize(handle2) ) {
        return tcomp_buffer_startsWith(handle1, handle2);
    }

    return tsk_false;
}


/**Checks if the first internal buffer starts with the second handle internal buffer.
* @param handle1 First handle
* @param handle2 Second handle
* @retval Returns @a tsk_true if the first internal buffer starts with the second handle internal buffer and @a tsk_false otherwise.
*/
tsk_bool_t tcomp_buffer_startsWith(const tcomp_buffer_handle_t* handle1, const tcomp_buffer_handle_t* handle2) /*const*/
{
    tsk_size_t i;
    tcomp_buffer_t* buffer1 = (tcomp_buffer_t*)handle1;
    tcomp_buffer_t* buffer2 = (tcomp_buffer_t*)handle2;

    if(buffer1->size < buffer2->size) {
        return tsk_false;
    }

    for(i = 0; i< buffer2->size; i++) {
        if(buffer1->lpbuffer[i] != buffer2->lpbuffer[i]) {
            return tsk_false;
        }
    }
    return tsk_true;
}

/**Gets a readonly pointer to the internal buffer.
* @param handle The handle for which to get the internal buffer.
* @param position Position pointer
* @retval Pointer to the internal buffer.
*/
const uint8_t* tcomp_buffer_getReadOnlyBufferAtPos(const tcomp_buffer_handle_t* handle, tsk_size_t position)/*const*/
{
    if(handle) {
        return (((tcomp_buffer_t*)handle)->lpbuffer + position);
    }
    else {
        TSK_DEBUG_ERROR("Null SigComp handle");
    }

    return tsk_null;
}

/**Gets a read/write pointer to the internal buffer.
* @param handle The handle for which to get the internal buffer.
* @param position Position pointer
* @retval Pointer to the internal buffer.
*/
uint8_t* tcomp_buffer_getBufferAtPos(const tcomp_buffer_handle_t* handle, tsk_size_t position)
{
    if(handle) {
        if(position && ((tcomp_buffer_t*)handle)->size <= position) {
            TSK_DEBUG_ERROR("%u <= %u", ((tcomp_buffer_t*)handle)->size, position);
            return tsk_null;
        }
        return (((tcomp_buffer_t*)handle)->lpbuffer + position);
    }
    else {
        TSK_DEBUG_ERROR("Null SigComp handle");
    }

    return tsk_null;
}


/** Gets the internal buffer size
* @retval The size of the internal buffer
*/
tsk_size_t tcomp_buffer_getSize(const tcomp_buffer_handle_t* handle) /*const*/
{
    if(handle) {
        return ((tcomp_buffer_t*)handle)->size;
    }
    else {
        TSK_DEBUG_ERROR("Null SigComp handle");
    }

    return 0;
}

/**Gets the remainning bits.
* @param handle The handle for which to get the remaining bits.
*/
tsk_size_t tcomp_buffer_getRemainingBits(const tcomp_buffer_handle_t* handle) /*const*/
{
    if(handle) {
        tcomp_buffer_t* buffer = (tcomp_buffer_t*)handle;
        tsk_ssize_t result = ((buffer->size * 8) - ((buffer->index_bytes * 8) + buffer->index_bits));
        return (result < 0) ? 0: result;
    }
    else {
        TSK_DEBUG_ERROR("Null SigComp handle");
    }

    return 0;
}

/**Reads @a size bytes.
* @param handle The handle for which to read bytes.
* @param length Number of bytes to read.
* @retval Pointer to the resulting buffer.
*/
uint8_t* tcomp_buffer_readBytes(tcomp_buffer_handle_t* handle, tsk_size_t length)
{
    if(handle) {
        tcomp_buffer_t* buffer = (tcomp_buffer_t*)handle;
        tsk_size_t old_index;

        if((buffer->index_bytes + length) > (buffer->size)) {
            return tsk_null;
        }

        old_index = buffer->index_bytes;
        buffer->index_bytes += length;

        return tcomp_buffer_getBufferAtPos(handle, old_index);
    }
    else {
        TSK_DEBUG_ERROR("Null SigComp handle");
    }

    return tsk_null;
}

/**Reads the internal buffer from LSB to MSB as per RFC 3320 subclause 8.2.
* @param handle The SigComp handle holding the internal buffer to read.
* @param length The length of the buffer to read.
* @retval All bits as a 2-bytes integer value
*/
uint32_t tcomp_buffer_readLsbToMsb(tcomp_buffer_handle_t* handle, tsk_size_t length)
{
    // UDV Memory is always MSB first
    // MSB  <-- LSB
    // FIXME: use mask
    if(handle) {
        tcomp_buffer_t* buffer = (tcomp_buffer_t*)handle;
        uint8_t pos = 0;
        char* end;
        uint32_t result_val = 0;
        char result_str[16];
        memset(result_str, 0, 16);
        while(pos < length) {
            result_str[pos++] = (buffer->lpbuffer[buffer->index_bytes]
                                 &(1 << (buffer->index_bits))) ? '1' : '0';
            if(++buffer->index_bits == 8) {
                buffer->index_bytes++;
                buffer->index_bits = 0;
            }
        }

        end = (result_str+length);
        result_val = (uint32_t)strtol(result_str, &end, 2);

        return result_val;
    }
    else {
        TSK_DEBUG_ERROR("Null SigComp handle");
    }

    return 0;
}

/**Reads the internal buffer from MSB to LSB as per RFC 3320 subclause 8.2.
* @param handle The SigComp handle holding the internal buffer to read.
* @param length The length of the buffer to read.
* @retval All bits as a 2-bytes integer value
*/
uint32_t tcomp_buffer_readMsbToLsb(tcomp_buffer_handle_t* handle, tsk_size_t length)
{
    // UDV Memory is always MSB first
    // MSB  --> LSB
    // FIXME: use mask
    if(handle) {
        tcomp_buffer_t* buffer = (tcomp_buffer_t*)handle;
        uint8_t pos = 0;
        char* end;
        uint32_t result_val = 0;
        char result_str[16];
        memset(result_str, 0, 16);

        while(pos < length) {
            result_str[pos++] = (buffer->lpbuffer[buffer->index_bytes]
                                 &(128 >> (buffer->index_bits))) ? '1' : '0';
            if(++buffer->index_bits == 8) {
                buffer->index_bytes++;
                buffer->index_bits = 0;
            }
        }

        end = (result_str + length);
        result_val = (uint32_t)strtol(result_str, &end, 2);

        return result_val;
    }
    else {
        TSK_DEBUG_ERROR("Null SigComp handle");
    }

    return 0;
}

/**Discards bits as per RFC 3320 subclause 8.2.
* @param handle SigComp handle holding the internal buffer.
*/
void tcomp_buffer_discardBits(tcomp_buffer_handle_t* handle)
{
    if(handle) {
        tcomp_buffer_t* buffer = (tcomp_buffer_t*)handle;
        if(buffer->index_bits) {
            buffer->index_bits=0;
            buffer->index_bytes++;
        }
    }
    else {
        TSK_DEBUG_ERROR("Null SigComp handle");
    }
}

/**Discards last bytes as per RFC 3320 subclause 8.2.
* @param handle SigComp handle holding the internal buffer.
* @param count The number of bytes to discard.
*/
void tcomp_buffer_discardLastBytes(tcomp_buffer_handle_t* handle, uint32_t count)
{
    if(handle) {
        tcomp_buffer_t* buffer = (tcomp_buffer_t*)handle;
        if(buffer->size > count) {
            buffer->size -= count;
        }
        else {
            tcomp_buffer_freeBuff(handle);
        }
    }
    else {
        TSK_DEBUG_ERROR("Null SigComp handle");
    }
}

/**Allocs the internal buffer.
* @param handle SigComp handle holding the internal buffer to alloc.
* @param size Number of bytes to allocate.
*/
void tcomp_buffer_allocBuff(tcomp_buffer_handle_t* handle, tsk_size_t size)
{
    if(handle) {
        tcomp_buffer_t* buffer = (tcomp_buffer_t*)handle;
        if(!buffer->owner) {
            TSK_DEBUG_ERROR("The SigComp is not the owner of the internal buffer to alloc.");
            return;
        }
        if(!size) {
            TSK_DEBUG_WARN("Cannot allocate zero bytes.");
            return;
        }
        buffer->index_bits = buffer->index_bytes = 0;
        buffer->size = 0;
        if((buffer->lpbuffer = tsk_realloc(buffer->lpbuffer, size))) {
            buffer->size = size;
        }
    }
    else {
        TSK_DEBUG_ERROR("Null SigComp handle");
    }
}

/**Adds a buffer as a reference (not owned).
* @param handle SigComp handle holding the internal buffer.
* @param externalBuff THe external buffer to reference.
* @param size The size of the external buffer.
*/
void tcomp_buffer_referenceBuff(tcomp_buffer_handle_t* handle, uint8_t* externalBuff, tsk_size_t size)
{
    if(handle) {
        tcomp_buffer_t* buffer = (tcomp_buffer_t*)handle;
        if(buffer->size && buffer->owner) {
            TSK_DEBUG_ERROR("The SigComp handle already hold an internal buffer.");
            return;
        }

        buffer->size = size;
        buffer->lpbuffer = externalBuff;
        buffer->index_bytes = 0;
        buffer->index_bits = 0;
        buffer->owner = 0;
    }
    else {
        TSK_DEBUG_ERROR("Null SigComp handle");
    }
}

/**Appends data to our internal buffer.
* @param handle The handle to the SigComp buffer.
* @param data Data to append to our internal buffer.
* @param size The size of the data
* @retval @a tsk_true if succeed an @a tsk_false otherwise.
*/
tsk_bool_t tcomp_buffer_appendBuff(tcomp_buffer_handle_t* handle, const void* data, tsk_size_t size)
{
    if(handle) {
        tcomp_buffer_t* buffer = (tcomp_buffer_t*)handle;
        tsk_size_t oldSize = buffer->size;
        tsk_size_t newSize = (oldSize + size);
        {
            // realloc buffer
            if(!buffer->size) {
                buffer->lpbuffer = (uint8_t*)tsk_calloc(1, newSize);
            }
            else {
                buffer->lpbuffer = (uint8_t*)tsk_realloc(buffer->lpbuffer, newSize);
            }
        }

        if(!buffer->lpbuffer) {
            return tsk_false;
        }

        if(data) {
            memcpy((buffer->lpbuffer+oldSize), data, size);
        }
        else {
            memset((buffer->lpbuffer+oldSize), 0, size);
        }

        buffer->size = newSize;
        return tsk_true;
    }
    else {
        TSK_DEBUG_ERROR("Null SigComp handle");
    }

    return tsk_false;
}

/**Removes @a size bytes from the internal buffer.
* @param handle SigComp handle holding the internal buffer from which to remove bytes.
* @param pos The starting position from which to start removing bytes
* @param size The number of bytes to remove
* @retval @a tsk_true if succeed an @a tsk_false otherwise.
*/
tsk_bool_t tcomp_buffer_removeBuff(tcomp_buffer_handle_t* handle, tsk_size_t pos, tsk_size_t size)
{
    if(handle) {
        tcomp_buffer_t* buffer = (tcomp_buffer_t*)handle;
        tsk_size_t oldSize, newSize;

        if(((pos + size) > buffer->size)) {
            size = (buffer->size - pos);
        }
        memcpy((buffer->lpbuffer + pos), (buffer->lpbuffer + pos + size), (buffer->size - (pos + size)));

        oldSize = buffer->size;
        newSize = (oldSize - size);
        {
            if(!(buffer->size)) {
                buffer->lpbuffer = (uint8_t*)tsk_calloc(1, newSize);
            }
            else {
                buffer->lpbuffer = (uint8_t*)tsk_realloc(buffer->lpbuffer, newSize);
            }
        }
        if(buffer->lpbuffer) {
            buffer->size = newSize;
            return tsk_true;
        }
        return tsk_false;
    }
    else {
        TSK_DEBUG_ERROR("Null SigComp handle");
    }

    return tsk_false;
}

/**Free the internal buffer.
* @param handle SigComp handle holding the internal buffer to free.
*/
void tcomp_buffer_freeBuff(tcomp_buffer_handle_t* handle)
{
    if(handle) {
        tcomp_buffer_t* buffer = (tcomp_buffer_t*)handle;
        if(buffer->lpbuffer && buffer->size && buffer->owner) {
            tsk_free((void**)&(buffer->lpbuffer));
        }
        buffer->size = buffer->index_bytes = buffer->index_bits = 0;
    }
    else {
        TSK_DEBUG_ERROR("Null SigComp handle");
    }
}

/**Gets the bytes cursor position.
* @param handle SigComp handle holding the internal buffer.
* @retval The cursor position.
*/
tsk_size_t* tcomp_buffer_getIndexBytes(const tcomp_buffer_handle_t* handle)
{
    if(handle) {
        return &(((tcomp_buffer_t*)handle)->index_bytes);
    }
    else {
        TSK_DEBUG_ERROR("Null SigComp handle");
    }

    return 0;
}

/**Gets the bits cursor position.
* @param handle SigComp handle holding the internal buffer.
* @retval The cursor position.
*/
tsk_size_t* tcomp_buffer_getIndexBits(const tcomp_buffer_handle_t* handle)
{
    if(handle) {
        return &(((tcomp_buffer_t*)handle)->index_bits);
    }
    else {
        TSK_DEBUG_ERROR("Null SigComp handle");
    }

    return tsk_null;
}

/**Gets the P-bit controller value.
* The P-bit controls the order in which bits are passed from the dispatcher to the INPUT instructions.  If set to 0, it indicates that
* the bits within an individual byte are passed to the INPUT instructions in MSB to LSB order.  If it is set to 1, the bits are
* passed in LSB to MSB order.
* @param handle SigComp handle holding the internal buffer.
* @retval The P-Bit value.
*/
uint8_t* tcomp_buffer_getP_BIT(const tcomp_buffer_handle_t* handle)
{
    if(handle) {
        return &(((tcomp_buffer_t*)handle)->P_BIT);
    }
    else {
        TSK_DEBUG_ERROR("Null SigComp handle");
    }

    return tsk_null;
}

/**Creates a random HASH number.
*/
uint64_t tcomp_buffer_createHash(const void *data, tsk_size_t len)
{
    if(!data || !len) {
        TSK_DEBUG_ERROR("Null data.");
        return 0;
    }
    {
#define PRIME_1		500237
#define PRIME_2		700241
        uint64_t hash = 0;
        uint8_t* strid = (uint8_t*)data;

        /* Generate Hash code from id */
        {
            uint64_t b = PRIME_1, a = PRIME_2;
            tsk_size_t i;
            for(i = 0; i < len; strid++, i++) {
                hash = hash * a + (*strid);
                a = a * b;
            }
        }
        return hash;

#undef PRIME_1
#undef PRIME_2
    }
}

/**Prints the internal buffer.
* @param handle SigComp handle holding the internal buffer to print.
* @param size The number of bytes to print.
*/
void tcomp_buffer_nprint(const tcomp_buffer_handle_t* handle, tsk_ssize_t size)
{
#if defined(_DEBUG) || defined(DEBUG)

    if(handle) {
        tsk_size_t i, tsk_size_to_print;
        tcomp_buffer_t* buffer = (tcomp_buffer_t*)handle;
        tsk_size_to_print = (size<0) ? buffer->size : size;

        if( !tsk_size_to_print || !buffer->lpbuffer) {
            return;
        }

        for(i = 0; i < tsk_size_to_print; i+=2) {
            char s[10];
            uint32_t value;
            memset(s, 0, 10);

            if((i+1) == tsk_size_to_print) {
                // last 2-byte lay in 1byte
                value = buffer->lpbuffer[i];
#if 0
                sprintf_s(s, 10, i?"%0.2x":"0x%0.2x", value);
#else
                sprintf(s, i ? "%0.2x" : "0x%0.2x", value);
#endif
            }
            else {
                uint8_t *b_ptr = tcomp_buffer_getBufferAtPos(handle, i);
                value = TSK_BINARY_GET_2BYTES(b_ptr);
#if 0
                sprintf_s(s, 10, i?"%0.4x":"0x%0.4x", value);
#else
                sprintf(s, i?"%0.4x":"0x%0.4x", value);
#endif
            }
            printf("%s ", s);
        }
        printf("\n");
    }
    else {
        TSK_DEBUG_ERROR("Null SigComp handle");
    }
#endif
}

/**Resets a sigcomp buffer.
* @param handle Handle holding the internal buffer to reset.
*/
void tcomp_buffer_reset(tcomp_buffer_handle_t* handle)
{
    if(handle) {
        tcomp_buffer_t* buffer = (tcomp_buffer_t*)handle;

        buffer->index_bytes = 0;
        buffer->index_bits = 0;
        if(buffer->lpbuffer) {
            memset(buffer->lpbuffer, 0, buffer->size);
        }
    }
    else {
        TSK_DEBUG_ERROR("Null SigComp handle");
    }
}










//========================================================
//	SigComp buffer object definition
//
static tsk_object_t* tcomp_buffer_ctor(tsk_object_t *self, va_list * app)
{
    tcomp_buffer_t* buffer = self;
    if(buffer) {
    }
    return self;
}

static tsk_object_t* tcomp_buffer_dtor(tsk_object_t *self)
{
    tcomp_buffer_t* buffer = self;
    if(buffer) {
        tcomp_buffer_freeBuff(buffer);
    }
    else {
        TSK_DEBUG_ERROR("Null SigComp handle.");
    }

    return self;
}

static const tsk_object_def_t tcomp_buffer_def_s = {
    sizeof(tcomp_buffer_t),
    tcomp_buffer_ctor,
    tcomp_buffer_dtor,
    tsk_null
};
const tsk_object_def_t *tcomp_buffer_def_t = &tcomp_buffer_def_s;

