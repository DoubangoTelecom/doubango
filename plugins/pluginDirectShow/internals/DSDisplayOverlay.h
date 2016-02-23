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
#ifndef PLUGIN_DSHOW_DSDISPLAYOVERLAY_H
#define PLUGIN_DSHOW_DSDISPLAYOVERLAY_H

#include "plugin_dshow_config.h"
#include <strmif.h>

#if defined(VMR9) || defined(VMR9_WINDOWLESS)
#include <D3D9.h>
#include <D3Dx9.h>
#include <vmr9.h>
#endif

class DSDisplayGraph;

class DSDisplayOverlay
{
public:
    DSDisplayOverlay();
    virtual ~DSDisplayOverlay();

    void attach(HWND parent, DSDisplayGraph *graph);
    void detach();

    void show(int value);
    void update();

private:
    void internalUpdate();

private:
    HWND				 window;

    DSDisplayGraph		*displayGraph;
    int					 ticks;

#if defined(VMR)
    HDC					 hdcBmp;
    HBITMAP              hbmOld;
    VMRALPHABITMAP		 alphaBitmap;
    float				 alphaStep;
#elif defined(VMR9) || defined(VMR9_WINDOWLESS)
    IDirect3D9			*direct3D;
    IDirect3DDevice9	*direct3DDevice;
    IDirect3DSurface9   *direct3DSurface;
    D3DXIMAGE_INFO		 overlayInfo;
    VMR9AlphaBitmap		 alphaBitmap;
    float				 alphaStep;
#endif
};

#endif
