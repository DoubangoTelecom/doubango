/* Copyright (C) 2014-2015 Mamadou DIOP
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
#pragma once

#if defined(_WIN32_WCE)

#include "plugin_dshow_config.h"

#include <streams.h>
#include <math.h>

class DSNullFilter : public CTransInPlaceFilter
{
public:

    DECLARE_IUNKNOWN;

    HRESULT Transform(IMediaSample *pSample);
    HRESULT CheckInputType(const CMediaType *mtIn);

    // Constructor
    DSNullFilter( LPUNKNOWN punk, HRESULT *phr );
};

#endif /* _WIN32_WCE */
