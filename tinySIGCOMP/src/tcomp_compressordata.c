///*
//* Copyright (C) 2010-2011 Mamadou Diop.
//*
//* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
//*
//* This file is part of Open Source Doubango Framework.
//*
//* DOUBANGO is free software: you can redistribute it and/or modify
//* it under the terms of the GNU General Public License as published by
//* the Free Software Foundation, either version 3 of the License, or
//* (at your option) any later version.
//*
//* DOUBANGO is distributed in the hope that it will be useful,
//* but WITHOUT ANY WARRANTY; without even the implied warranty of
//* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//* GNU General Public License for more details.
//*
//* You should have received a copy of the GNU General Public License
//* along with DOUBANGO.
//*
//*/
//
///**@file tcomp_compressordata.c
// * @brief  SigComp compressor data.
// *
// * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
// *
//
// */
//#include "tcomp_compressordata.h"
//
//#include "tsk_debug.h"
//
//#include <assert.h>
//
//
///**@ingroup tcomp_compressordata_group
//*/
//void tcomp_compressordata_ackGhost(tcomp_compressordata_t *compdata, const tcomp_buffer_handle_t *stateid)
//{
//	if(compdata)
//	{
//		tsk_safeobj_lock(compdata);
//		assert(0);
//		tsk_safeobj_unlock(compdata);
//	}
//	else
//	{
//		TSK_DEBUG_ERROR("NULL compressor data.");
//	}
//}
//
///**@ingroup tcomp_compressordata_group
//*/
//void tcomp_compressordata_freeGhostState(tcomp_compressordata_t *compdata)
//{
//	if(compdata)
//	{
//		tsk_safeobj_lock(compdata);
//		assert(0);
//		tsk_safeobj_unlock(compdata);
//	}
//	else
//	{
//		TSK_DEBUG_ERROR("NULL compressor data.");
//	}
//}
//
//
//
//
//
//
//
//
//
////========================================================
////	SigComp compressor data object definition
////
//static void* tcomp_compressordata_create(void * self, va_list * app)
//{
//	tcomp_compressordata_t *compdata = self;
//	if(compdata)
//	{
//		/* Initialize safeobject */
//		tsk_safeobj_init(compdata);
//
//		compdata->isStream = va_arg(*app, int);
//	}
//
//	return self;
//}
//
//static void* tcomp_compressordata_destroy(void *self)
//{
//	tcomp_compressordata_t *compdata = self;
//	if(compdata)
//	{
//		/* Deinitialize safeobject */
//		tsk_safeobj_deinit(compdata);
//
//		TSK_OBJECT_SAFE_FREE(compdata->ghostState);
//	}
//
//	return self;
//}
//
//static const tsk_object_def_t tsk_compressordata_def_s =
//{
//	sizeof(tcomp_compressordata_t),
//	tcomp_compressordata_create,
//	tcomp_compressordata_destroy,
//	0
//};
//const void *tcomp_compressordata_def_t = &tsk_compressordata_def_s;
