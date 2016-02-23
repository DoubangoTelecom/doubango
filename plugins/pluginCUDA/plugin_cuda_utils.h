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
#ifndef PLUGIN_CUDA_UTILS_H
#define PLUGIN_CUDA_UTILS_H

#include "plugin_cuda_config.h"

#include <Windows.h>

#undef CHECK_HR
// In CHECK_HR(x) When (x) is a function it will be executed twice when used in "TSK_DEBUG_ERROR(x)" and "If(x)"
#define CHECK_HR(x) { HRESULT __hr__ = (x); if (FAILED(__hr__)) { TSK_DEBUG_ERROR("Operation Failed (%08x)", __hr__); goto bail; } }

#undef SafeRelease
#define SafeRelease(ppT) \
{ \
    if (*ppT) \
    { \
        (*ppT)->Release(); \
        *ppT = NULL; \
    } \
}

class CudaUtils
{
public:
    static HRESULT Startup();
    static HRESULT Shutdown();
    static bool IsH264Supported();
    static int ConvertSMVer2Cores(int nMajor, int nMinor);
    static int GetMaxGflopsDeviceId();

private:
    static bool g_bStarted;
    static bool g_bH264Checked;
    static bool g_bH264Supported;
    static int g_nCores;
};

#endif/* PLUGIN_CUDA_UTILS_H */
