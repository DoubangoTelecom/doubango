/* Copyright (C) 2011-2013 Doubango Telecom <http://www.doubango.org>
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
*/
#ifndef PLUGIN_DSHOW_DSCAPTUREUTILS_H
#define PLUGIN_DSHOW_DSCAPTUREUTILS_H

#include "plugin_dshow_config.h"
#include "internals/DSCaptureFormat.h"
#include "internals/VideoGrabberName.h"

#include <vector>

// --------------------------------------------------------------------------------

#ifdef INCLUDE_VFW_DEVICES
#define INCLUDE_CATEGORY_FLAG	0
#else
#define INCLUDE_CATEGORY_FLAG	CDEF_DEVMON_FILTER | CDEF_DEVMON_PNP_DEVICE
#endif

// --------------------------------------------------------------------------------

/**
* \brief	Fills in a vector with VideoGrabberName instances constructed from the video capture devices.
* \param	A pointer to the device vector to append
* \return	An HRESULT value
*/
HRESULT enumerateCaptureDevices(const std::string &prefix, std::vector<VideoGrabberName> *names);

/**
* \brief	Fills in a vector with VideoGrabberName instances constructed from the video capture devices.
* \param	A constant string containing a device path
* \param	A pointer to the filter that will contains the filter created or NULL if not the device is not found
* \return	An HRESULT value
*/
HRESULT createSourceFilter(std::string *devicePath, IBaseFilter **sourceFilter);

/**
* \brief	Fills in a vector with DSCaptureFormat instances constructed from the given video capture device.
* \param	An instance of a capture device
* \param	A pointer to the format vector to append
* \return	An HRESULT value
*/
HRESULT getSupportedFormats(IBaseFilter *sourceFilter, std::vector<DSCaptureFormat> *formats);

#endif
