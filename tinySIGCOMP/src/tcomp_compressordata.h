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

/**@file tcomp_compressordata.h
 * @brief  SigComp compressor data.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#ifndef TCOMP_COMPRESSOR_DATA_H
#define TCOMP_COMPRESSOR_DATA_H

#include "tinysigcomp_config.h"
#include "tcomp_buffer.h"

TCOMP_BEGIN_DECLS

typedef void tcomp_compressordata_t;

typedef void (*tcomp_xxx_freeGhostState)(tcomp_compressordata_t *data);
typedef void (*tcomp_xxx_ackGhost)(tcomp_compressordata_t *data, const tcomp_buffer_handle_t *stateid);

#define TCOMP_DECLARE_COMPRESSORDATA \
	void *compressorData; \
	unsigned compressorData_isStream; \
	tcomp_xxx_freeGhostState freeGhostState; \
	tcomp_xxx_ackGhost ackGhost

//#include "tcomp_state.h"
//#include "tcomp_buffer.h"
//
//#include "tsk_object.h"
//#include "tsk_safeobj.h"
//
//#define TCOMP_COMPRESSORDATA_CREATE(isStream)		tsk_object_new(tsk_compressordata_def_t, isStream)
//
//typedef struct tcomp_compressordata_s
//{
//	TSK_DECLARE_OBJECT;
//
//	tcomp_state_t *ghostState;
//	unsigned isStream:1;
//
//	TSK_DECLARE_SAFEOBJ;
//}
//tcomp_compressordata_t;
//
//void tcomp_compressordata_ackGhost(tcomp_compressordata_t *compdata, const tcomp_buffer_handle_t *stateid);
//void tcomp_compressordata_freeGhostState(tcomp_compressordata_t *compdata);
//
//TINYSIGCOMP_GEXTERN const void *tcomp_compressordata_def_t;

TCOMP_END_DECLS

#endif /* TCOMP_COMPRESSOR_DATA_H */
