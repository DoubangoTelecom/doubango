/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
#ifndef TINYDSHOW_DUTILS_H
#define TINYDSHOW_DUTILS_H

#include <tinydshow_config.h>

#include <strmif.h>

// --------------------------------------------------------------------------------

#define SAFE_RELEASE(x) if ((x)) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE_PTR(x) if ((x)) { delete (x); (x) = NULL; }
#define SAFE_DELETE_ARRAY(x) if ((x)) { delete[] (x); (x) = NULL; }

#define DS_NANOS_TO_100NS(NANOS) (((LONGLONG)(NANOS)) / 100ui64)
#define DS_MICROS_TO_100NS(MICROS) (((LONGLONG)(MICROS)) * 10ui64)
#define DS_MILLIS_TO_100NS(MILLIS) (((LONGLONG)(MILLIS)) * 10000ui64)
#define DS_SECONDS_TO_100NS(SEC) (((LONGLONG)(SEC)) * 10000000ui64)
//#define DS_100NS_TO_SECONDS(NANOS) (((LONGLONG)(NANOS)) / 10000000ui64)  

//#define SECONDS_TO_100NS(s) (LONGLONG)(10000000/(s))
//#define SECONDS_FROM_100NS(ns) (10000000/(ns))

#define COCREATE(cls, iid, target) \
	CoCreateInstance(cls, NULL, CLSCTX_INPROC_SERVER, iid, reinterpret_cast<void**>(&target))
#define QUERY(source, iid, target) \
	source->QueryInterface(iid, reinterpret_cast<void**>(&target))

// --------------------------------------------------------------------------------


#define FILTER_WEBCAM					_T("WEBCAM")
#define FILTER_FRAMERATE				_T("TDSHOW_FRAMERATE")
#define FILTER_OUTPUT					_T("TDSHOW_OUTPUT")
#define	FITLER_SAMPLE_GRABBER			_T("SAMPLE_GRABBER")
#define FILTER_AVI_DECOMPRESSOR			_T("AVI_DECOMPRESSOR")

#define FILTER_COLORSPACE_CONVERTOR		_T("COLORSPACE_CONVERTOR")
#define FILTER_NULL_RENDERER			_T("NULL_RENDERER")
#define FILTER_VIDEO_RENDERER			_T("VIDEO_RENDERER")
#define FILTER_VIDEO_MIXING_RENDERER	_T("VIDEO_MIXING_RENDERER")
#define FILTER_COLOR_CONVERTOR_565		_T("COLOR_CONVERTOR_565")

// --------------------------------------------------------------------------------

HWND GetMainWindow();

bool IsMainThread();

IPin *GetPin(IBaseFilter *pFilter, PIN_DIRECTION dir);

HRESULT ConnectFilters(IGraphBuilder *graphBuilder, IBaseFilter *source, IBaseFilter *destination, AM_MEDIA_TYPE *mediaType = NULL);

HRESULT DisconnectFilters(IGraphBuilder *graphBuilder, IBaseFilter *source, IBaseFilter *destination);

bool DisconnectAllFilters(IGraphBuilder *graphBuilder);

bool RemoveAllFilters(IGraphBuilder *graphBuilder);

int createOnUIThead(HWND hWnd, void** ppRet, bool display);

#endif /* TINYDSHOW_DUTILS_H */
