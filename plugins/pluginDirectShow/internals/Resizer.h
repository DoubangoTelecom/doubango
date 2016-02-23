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
/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 */
#ifndef PLUGIN_DSHOW_RESIZER_H
#define PLUGIN_DSHOW_RESIZER_H

#include "plugin_dshow_config.h"

/*
* StretchC.C
*
* StretchBlt for DIBs
*
* C version of stretch.asm: StretchDIB optimised for AVI.
*
* NOTES
*	- does not handle mirroring in x or y
*	- does not handle pixel translation
*	- will not work in place.
*/


/* Outline:
*
* we select a y-stretching function depending on the ratio (eg 1:N or N:1).
* it copies scanlines from source to destination, duplicating or omitting
* scanlines as necessary to fit the destination. It copies each scanline
* via the X_FUNC function we passed as an argument: this copies one scanline
* duplicating or omitting pixels to fit the destination: we select an X_FUNC
* depending on the bit-depth as well as the x-stretching ratio.
*
* both x and y stretching functions use the following basic model for deciding
* when to insert/omit elements:
*
* 	delta = <larger extent> -1;
*
*      for (number of destination elements) {
*
*		copy one element
*		advance pointer to larger region
*		delta -= <smaller extent>
*		if (delta < 0) {
*			delta += <larger extent>;
*			advance pointer to smaller region
*		}
*	}
*/

#include <streams.h>


void ResizeRGB( BITMAPINFOHEADER *pbiIn,    //Src's BitMapInFoHeader
                const unsigned char * dibBits,    //Src bits
                BITMAPINFOHEADER *pbiOut,
                unsigned char *pFrame,    //Dst bits
                int iNewWidth,            //new W in pixel
                int iNewHeight);           //new H in pixel

#endif //RESIZER_H
