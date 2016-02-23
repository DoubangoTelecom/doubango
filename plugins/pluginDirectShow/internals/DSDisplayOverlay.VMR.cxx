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
#if defined(VMR)

#include "internals/DSDisplayOverlay.h"
#include "internals/DSDisplayGraph.h"
#include "internals/DSUtils.h"
#include "../../resource.h"

using namespace std;

#define ALPHA_VALUE_START	0.8f
#define ALPHA_VALUE_STOP	0.0f


// Hack to get module of the current code
// Only works with Microsoft Linker and could break in the future
EXTERN_C IMAGE_DOS_HEADER __ImageBase;


DSDisplayOverlay::DSDisplayOverlay()
{
    this->window = NULL;
    this->hdcBmp = NULL;
    this->hbmOld = NULL;
}

DSDisplayOverlay::~DSDisplayOverlay()
{
}

void DSDisplayOverlay::attach(HWND parent, DSDisplayGraph *graph)
{
    HRESULT hr;

    // Gets the handle of the parent and the graph
    this->window = parent;
    this->displayGraph = graph;

    if (this->window) {
        // Hack to get module of the current code
        TCHAR *modulePath = (TCHAR *) calloc(255, sizeof(TCHAR));
        GetModuleFileName((HINSTANCE)&__ImageBase, modulePath, 255);
        HMODULE module = GetModuleHandle(modulePath);
        delete[] modulePath;
        if (!module) {
            cout << "Failed to get current module";
            return;
        }

        HBITMAP bitmap = LoadBitmap(module, MAKEINTRESOURCE(IDB_BITMAP_OVERLAY));
        if (!bitmap) {
            cout << "Failed to load overlay bitmap" << endl;
            return;
        }

        RECT rect;
        hr = GetWindowRect(this->window, &rect);
        if (FAILED(hr)) {
            cout << "Failed to get window size" << endl;
            return;
        }

        BITMAP bm;
        HDC hdc = GetDC(this->window);
        this->hdcBmp = CreateCompatibleDC(hdc);
        ReleaseDC(this->window, hdc);

        GetObject(bitmap, sizeof(bm), &bm);
        this->hbmOld= (HBITMAP) SelectObject(this->hdcBmp, bitmap);

        ZeroMemory(&this->alphaBitmap, sizeof(VMRALPHABITMAP));
        this->alphaBitmap.dwFlags = VMRBITMAP_HDC | VMRBITMAP_SRCCOLORKEY;
        this->alphaBitmap.hdc = this->hdcBmp;
        this->alphaBitmap.clrSrcKey = 0x00FF00FF;
        // Source rectangle
        this->alphaBitmap.rSrc.left = 0;
        this->alphaBitmap.rSrc.top = 0;
        this->alphaBitmap.rSrc.right = bm.bmWidth;
        this->alphaBitmap.rSrc.bottom = bm.bmHeight;
        // Destination rectangle
        this->alphaBitmap.rDest.left = (rect.right - rect.left - bm.bmWidth) / 2.0;
        this->alphaBitmap.rDest.top = (rect.bottom - rect.top - bm.bmHeight) / 2.0;
        this->alphaBitmap.rDest.right = this->alphaBitmap.rDest.left + bm.bmWidth;
        this->alphaBitmap.rDest.bottom = this->alphaBitmap.rDest.top + bm.bmHeight;
        this->alphaBitmap.rDest.left /= (rect.right - rect.left);
        this->alphaBitmap.rDest.top /= (rect.bottom - rect.top);
        this->alphaBitmap.rDest.right /= (rect.right - rect.left);
        this->alphaBitmap.rDest.bottom /= (rect.bottom - rect.top);
        // Alpha value for start
        this->alphaBitmap.fAlpha = ALPHA_VALUE_START;

    }
}

void DSDisplayOverlay::detach()
{
    // Clean up
    DeleteObject(SelectObject(this->hdcBmp, this->hbmOld));
    DeleteDC(this->hdcBmp);

    this->hdcBmp = NULL;
    this->hbmOld = NULL;
    this->displayGraph = NULL;
    this->window = NULL;
}

void DSDisplayOverlay::show(int value)
{
    // Store the ticks to count down
    this->ticks = value;

    // Compute alpha value decrement
    this->alphaStep = (this->ticks > 0) ? ((ALPHA_VALUE_START - ALPHA_VALUE_STOP) / this->ticks) : 0;
    this->alphaBitmap.fAlpha = ALPHA_VALUE_START;

    this->internalUpdate();
}

void DSDisplayOverlay::update()
{
    if (this->displayGraph && (this->ticks > 0)) {
        this->ticks--;

        // Be sure alpha is in 0.0 .. 1.0 range.
        float value = this->alphaBitmap.fAlpha;
        value -= this->alphaStep;
        this->alphaBitmap.fAlpha = (value >= 0.0f) ? value : 0.0f;

        this->internalUpdate();
    }
}

void DSDisplayOverlay::internalUpdate()
{
    HRESULT hr;

    if (this->ticks > 0) {
        this->alphaBitmap.dwFlags = VMRBITMAP_HDC | VMRBITMAP_SRCCOLORKEY;
    }
    else {
        this->alphaBitmap.dwFlags = VMRBITMAP_DISABLE;
    }

    hr = this->displayGraph->getMixerBitmap()->SetAlphaBitmap(&this->alphaBitmap);
    if (FAILED(hr)) {
        cout << "Failed to mix overylay (" << hr << ")" << endl;
        return;
    }
}

#endif
