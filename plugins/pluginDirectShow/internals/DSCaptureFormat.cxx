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
#include "internals/DSCaptureFormat.h"
#include <uuids.h>


int DSCaptureFormat::getMatchScore(int w, int h)
{
    int factor;

    if ((w == width) && (h = height)) {
        factor = 100;
    }
    else if ((w > this->width) && (h > this->height)) {
        factor = 0;
    }
    else {
        factor = (50 * w) / this->width + (50 * h) / this->height;
    }

    if (isRGB()) {
        factor *= 2;
    }

    return factor;
}

bool DSCaptureFormat::isRGB()
{
    // Order used is optimized for most used RGB types
    if (IsEqualGUID(this->chroma, MEDIASUBTYPE_RGB32)) {
        return true;
    }
    if (IsEqualGUID(this->chroma, MEDIASUBTYPE_RGB24)) {
        return true;
    }
    if (IsEqualGUID(this->chroma, MEDIASUBTYPE_RGB565)) {
        return true;
    }
    if (IsEqualGUID(this->chroma, MEDIASUBTYPE_RGB555)) {
        return true;
    }
    if (IsEqualGUID(this->chroma, MEDIASUBTYPE_RGB8)) {
        return true;
    }
    if (IsEqualGUID(this->chroma, MEDIASUBTYPE_RGB4)) {
        return true;
    }
    if (IsEqualGUID(this->chroma, MEDIASUBTYPE_RGB1)) {
        return true;
    }
#ifndef _WIN32_WCE
    if (IsEqualGUID(this->chroma, MEDIASUBTYPE_ARGB32)) {
        return true;
    }
    if (IsEqualGUID(this->chroma, MEDIASUBTYPE_ARGB4444)) {
        return true;
    }
    if (IsEqualGUID(this->chroma, MEDIASUBTYPE_ARGB1555)) {
        return true;
    }
#endif

    return false;
}
