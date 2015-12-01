/* Copyright (C) 2013 Mamadou DIOP
* Copyright (C) 2013 Doubango Telecom <http://www.doubango.org>
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
#ifndef PLUGIN_WASAPI_UTILS_H
#define PLUGIN_WASAPI_UTILS_H

#include "plugin_wasapi_config.h"

#include <Windows.h>

#undef SafeRelease
#define SafeRelease(ppT) \
{ \
    if (*ppT) \
    { \
        (*ppT)->Release(); \
        *ppT = NULL; \
    } \
} 

#undef CHECK_HR
// In CHECK_HR(x) When (x) is a function it will be executed twice when used in "TSK_DEBUG_ERROR(x)" and "If(x)"
#define CHECK_HR(x) { HRESULT __hr__ = (x); if (FAILED(__hr__)) { TSK_DEBUG_ERROR("Operation Failed (%08x)", __hr__); goto bail; } }

#define PLUGIN_WASAPI_ERROR(hr) WASAPIUtils::PrintError(__FILE__, __FUNCTION__, __LINE__, (hr))

class WASAPIUtils
{
public:
	static HRESULT Startup();
	static HRESULT Shutdown();
	static void PrintError(const char* pcFileName, const char* pcFuncName, unsigned nLineNumber, HRESULT hr);

private:
	static bool g_bStarted;
};

#endif /* PLUGIN_WASAPI_UTILS_H */
