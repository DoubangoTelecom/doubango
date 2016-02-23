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

/**@file tcomp_compressor.h
 * @brief  SigComp Deflate compressor(Compressor data).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#ifndef TCOMP_COMPRESSORDEFLATE_DATA_H
#define TCOMP_COMPRESSORDEFLATE_DATA_H

#include "tinysigcomp_config.h"
#include "tcomp_params.h"
#include "tcomp_state.h"
#include "tcomp_compressordata.h"

#if HAS_ZLIB
#	include <zlib.h>
#else
#	include "zlib.h"
#endif

TCOMP_BEGIN_DECLS

//============================================================//
#define USE_DICTS_FOR_COMPRESSION				0

#define Z_DEFAULT_WINDOW_BITS 10 /* 1024*/


#define DEFLATE_DECOMPRESSION_PTR_INDEX			70
#if USE_DICTS_FOR_COMPRESSION
#	define DEFLATE_UDVM_CIRCULAR_START_INDEX	701
#else
#	define DEFLATE_UDVM_CIRCULAR_START_INDEX	630
#endif


#define DEFLATE_SIP_DICT_ONLY					0x00
#define DEFLATE_PRES_DICT_ONLY					0x01
#define DEFLATE_SIP_PRES_DICTS					0x02
#define DEFLATE_NO_DICT							0x03
#define DEFLATE_FIXME_DICT						DEFLATE_NO_DICT



#define DEFLATE_BYTECODE_DESTINATION_START		320
#define DEFLATE_BYTECODE_DESTINATION_CODE		0x04 // 320
#if USE_DICTS_FOR_COMPRESSION
#	define DEFLATE_BYTECODE_LEN					381
#else
#	define DEFLATE_BYTECODE_LEN					310
#endif
//==========================================================//


#if USE_DICTS_FOR_COMPRESSION
#define DEFLATEDATA_DEFLATE_BYTECODE \
	"\x0f\x86\x7a\xa2\xbd\x8d\x05\xa2\xbd\x00\x03\x00\x04\x00\x05\x00\x06\x00\x07\x00\x08\x00" \
	"\x09\x00\x0a\x01\x0b\x01\x0d\x01\x0f\x01\x11\x02\x13\x02\x17\x02\x1b\x02\x1f\x03\x23\x03" \
	"\x2b\x03\x33\x03\x3b\x04\xa0\x43\x04\xa0\x53\x04\xa0\x63\x04\xa0\x73\x05\xa0\x83\x05\xa0" \
	"\xa3\x05\xa0\xc3\x05\xa0\xe3\x00\xa1\x02\x00\x01\x00\x02\x00\x03\x00\x04\x01\x05\x01\x07" \
	"\x02\x09\x02\x0d\x03\x11\x03\x19\x04\x21\x04\x31\x05\xa0\x41\x05\xa0\x61\x06\xa0\x81\x06" \
	"\xa0\xc1\x07\xa1\x01\x07\xa1\x81\x08\xa2\x01\x08\xa3\x01\x09\xa4\x01\x09\xa6\x01\x0a\xa8" \
	"\x01\x0a\xac\x01\x0b\xb0\x01\x0b\xb8\x01\x0c\x80\x20\x01\x0c\x80\x30\x01\x0d\x80\x40\x01" \
	"\x0d\x80\x60\x01\x1c\x08\xa1\x34\xa0\xde\x0e\xa0\x42\xc1\x36\x06\x21\x86\x1a\x04\xc1\x3a" \
	"\xa0\x0c\xa0\x1e\xa0\x30\xa0\x47\x0f\xa1\x3a\x04\xa6\xfb\x80\xe5\x07\x80\xdf\xe5\x80\xe6" \
	"\x00\x16\xa0\x2c\x0f\xa1\x3a\x04\xa6\xd9\x80\x42\x29\x80\x7d\x0b\x80\xb3\x00\x16\xa0\x1a" \
	"\x0f\xa1\x3a\x07\xa6\xfb\x80\xe5\x07\x80\xdf\xe5\x80\xe6\x06\x80\xd9\x42\x80\x29\x7d\xab" \
	"\xb3\x1d\x03\x22\xa0\x89\x1e\x20\xa0\x68\x04\x07\x00\x17\x80\x40\x11\x01\x30\xa0\xbf\x00" \
	"\x00\xa0\xc0\xa0\xc7\x80\x40\x29\x01\xa1\x90\xa1\xff\xa0\x90\x17\x50\x80\x40\x11\xa0\x0b" \
	"\xa0\x49\xa0\x15\x22\x21\x01\x13\x21\x01\x23\x16\x9f\xcf\x08\x10\x04\x12\x50\x04\x22\x1d" \
	"\x51\x22\xa0\x49\x06\x12\x51\x1e\x20\xa0\x41\x01\x05\x00\x1f\x2f\x08\x10\x04\x12\x50\x04" \
	"\x26\x1d\x53\x26\xa0\x31\x06\x14\x53\x0e\x20\x63\x14\x54\x52\x23\x22\x50\x52\x16\x9f\x9b" \
	"\x0e\x2a\xa4\x86\x0f\x38\x04\xc1\x36\x86\xa1\xec\x06\x0d\x38\xc1\x34\x2c\x23\x2a\xa1\x38" \
	"\xc1\x36\x86\xa1\xec\x06\x00"
#else
#define DEFLATEDATA_DEFLATE_BYTECODE \
	"\x0f\x86\x7a\xa2\x76\x8d\x05\xa2\x76\x00\x03\x00\x04\x00\x05\x00\x06\x00\x07\x00\x08\x00" \
	"\x09\x00\x0a\x01\x0b\x01\x0d\x01\x0f\x01\x11\x02\x13\x02\x17\x02\x1b\x02\x1f\x03\x23\x03" \
	"\x2b\x03\x33\x03\x3b\x04\xa0\x43\x04\xa0\x53\x04\xa0\x63\x04\xa0\x73\x05\xa0\x83\x05\xa0" \
	"\xa3\x05\xa0\xc3\x05\xa0\xe3\x00\xa1\x02\x00\x01\x00\x02\x00\x03\x00\x04\x01\x05\x01\x07" \
	"\x02\x09\x02\x0d\x03\x11\x03\x19\x04\x21\x04\x31\x05\xa0\x41\x05\xa0\x61\x06\xa0\x81\x06" \
	"\xa0\xc1\x07\xa1\x01\x07\xa1\x81\x08\xa2\x01\x08\xa3\x01\x09\xa4\x01\x09\xa6\x01\x0a\xa8" \
	"\x01\x0a\xac\x01\x0b\xb0\x01\x0b\xb8\x01\x0c\x80\x20\x01\x0c\x80\x30\x01\x0d\x80\x40\x01" \
	"\x0d\x80\x60\x01\x1c\x06\xa1\x34\xa0\x97\x0e\xa0\x42\xc1\x36\x06\x21\x86\x1d\x03\x22\xa0" \
	"\x89\x1e\x20\xa0\x68\x04\x07\x00\x17\x80\x40\x11\x01\x30\xa0\xbf\x00\x00\xa0\xc0\xa0\xc7" \
	"\x80\x40\x29\x01\xa1\x90\xa1\xff\xa0\x90\x17\x50\x80\x40\x11\xa0\x0b\xa0\x49\xa0\x15\x22" \
	"\x21\x01\x13\x21\x01\x23\x16\x9f\xcf\x08\x10\x04\x12\x50\x04\x22\x1d\x51\x22\xa0\x49\x06" \
	"\x12\x51\x1e\x20\xa0\x41\x01\x05\x00\x1f\x2f\x08\x10\x04\x12\x50\x04\x26\x1d\x53\x26\xa0" \
	"\x31\x06\x14\x53\x0e\x20\x63\x14\x54\x52\x23\x22\x50\x52\x16\x9f\x9b\x0e\x2a\xa4\x86\x0f" \
	"\x38\x04\xc1\x36\x86\xa1\xec\x06\x0d\x38\xc1\x34\x2c\x23\x2a\xa1\x38\xc1\x36\x86\xa1\xec" \
	"\x06\x00"

#endif /* USE_DICTS_FOR_COMPRESSION */


typedef struct tcomp_deflateStream_s {
    z_stream zs;
    unsigned dataWaitingAck:1;
    unsigned stateful:1;
}
tcomp_deflateStream_t;

tsk_bool_t tcomp_deflateStream_end(tcomp_deflateStream_t *stream);
tsk_bool_t tcomp_deflateStream_copy(tcomp_deflateStream_t *stream, tcomp_deflateStream_t *source);


typedef struct tcomp_deflatedata_s {
    TSK_DECLARE_OBJECT;

    tcomp_deflateStream_t stream_1;
    tcomp_deflateStream_t stream_acked;
    int zLevel;
    int zWindowBits;

    tsk_bool_t isInitialized;

    uint32_t ghost_copy_offset;
    tcomp_state_t *ghostState;

    tsk_bool_t isStream;

    tsk_bool_t useOnlyACKedStates;

    TSK_DECLARE_SAFEOBJ;
}
tcomp_deflatedata_t;

tcomp_deflatedata_t* tcomp_deflatedata_create(tsk_bool_t isStream, tsk_bool_t useOnlyACKedStates);

void tcomp_deflatedata_freeGhostState(tcomp_compressordata_t *deflatedata);
void tcomp_deflatedata_ackGhost(tcomp_compressordata_t *deflatedata, const tcomp_buffer_handle_t *stateid);
void tcomp_deflatedata_createGhost(tcomp_deflatedata_t *deflatedata, uint32_t state_len, tcomp_params_t *params);
void tcomp_deflatedata_updateGhost(tcomp_deflatedata_t *deflatedata, const uint8_t *input_ptr, tsk_size_t input_size);
uint32_t* tcomp_deflatedata_getGhostCopyOffset(tcomp_deflatedata_t *deflatedata);

tsk_bool_t tcomp_deflatedata_zReset(tcomp_deflatedata_t *deflatedata);
tsk_bool_t tcomp_deflatedata_zCompress(tcomp_deflatedata_t *deflatedata, const void* in, tsk_size_t inLen, void* out, tsk_size_t* outLen, tsk_bool_t *stateChanged);

int tcomp_deflatedata_zGetWindowBits(tcomp_deflatedata_t *deflatedata);
void tcomp_deflatedata_zSetWindowBits(tcomp_deflatedata_t *deflatedata, int windowSize);

tsk_bool_t tcomp_deflatedata_isStateful(tcomp_deflatedata_t *deflatedata);
tsk_bool_t tcomp_deflatedata_zInit(tcomp_deflatedata_t *deflatedata);
tsk_bool_t tcomp_deflatedata_zUnInit(tcomp_deflatedata_t *deflatedata);

TINYSIGCOMP_GEXTERN const tsk_object_def_t *tcomp_deflatedata_def_t;

TCOMP_END_DECLS

#endif /* TCOMP_COMPRESSORDEFLATE_DATA_H */
