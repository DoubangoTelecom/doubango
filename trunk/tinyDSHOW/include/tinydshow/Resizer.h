/*
* Copyright (C) 2009 Mamadou Diop.
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
/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 */
#ifndef RESIZER_H
#define RESIZER_H

#include <tinydshow_config.h>

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

/* stretch proportions */
#define STRETCH_1_1	1
#define STRETCH_1_2	2
#define STRETCH_1_4	3
#define STRETCH_1_N	4
#define STRETCH_N_1	5
#define STRETCH_4_1	6
#define STRETCH_2_1	7

void __stdcall StretchDIB(
		   LPBITMAPINFOHEADER biDst,   //	--> BITMAPINFO of destination
		   LPVOID	lpvDst,		    //	--> to destination bits
		   int	DstX,		    //	Destination origin - x coordinate
		   int	DstY,		    //	Destination origin - y coordinate
		   int	DstXE,		    //	x extent of the BLT
		   int	DstYE,		    //	y extent of the BLT
		   LPBITMAPINFOHEADER biSrc,   //	--> BITMAPINFO of source
		   LPVOID	lpvSrc,		    //	--> to source bits
		   int	SrcX,		    //	Source origin - x coordinate
		   int	SrcY,		    //	Source origin - y coordinate
		   int	SrcXE,		    //	x extent of the BLT
		   int	SrcYE	 	    //	y extent of the BLT
		   );

void ResizeRGB( BITMAPINFOHEADER *pbiIn,    //Src's BitMapInFoHeader
			   const unsigned char * dibBits,    //Src bits
			   BITMAPINFOHEADER *pbiOut,
			   unsigned char *pFrame,    //Dst bits
			   int iNewWidth,            //new W in pixel
			   int iNewHeight);           //new H in pixel

/*
* an X_FUNC is a function that copies one scanline, stretching or shrinking it
* to fit a destination scanline. Pick an X_FUNC depending on
* bitdepth and stretch ratio (1:1, 1:2, 1:4, 1:N, N:1, 4:1, 2:1)
*
* the x_fract argument is the delta fraction: it is a representation
* of the smaller extent (whichever that is) as a fraction of the larger,
* and is used when stretching or shrinking to advance the pointer to the
* smaller scanline every (fract) pixels of the larger.
* Thus if we are expanding 1:8, x_fract will be 1/8, we will advance the
* source pointer once every 8 pixels, and thus copy each source pixel to
* 8 dest pixels. Note that if shrinking 8:1, x_fract will still be 1/8
* and we will use it to control advancement of the dest pointer.
* the fraction is multiplied by 65536.
*/
typedef void (*X_FUNC) (LPBYTE lpSrc,
						LPBYTE lpDst,
						int SrcXE,
						int DstXE,
						int x_fract);


void X_Stretch_1_1_8Bits(LPBYTE lpSrc, LPBYTE lpDst, int SrcXE, int DstXE, int x_fract);
void X_Stretch_1_2_8Bits(LPBYTE lpSrc, LPBYTE lpDst, int SrcXE, int DstXE, int x_fract);
void X_Stretch_1_4_8Bits(LPBYTE lpSrc, LPBYTE lpDst, int SrcXE, int DstXE, int x_fract);
void X_Stretch_1_N_8Bits(LPBYTE lpSrc, LPBYTE lpDst, int SrcXE, int DstXE, int x_fract);
void X_Stretch_N_1_8Bits(LPBYTE lpSrc, LPBYTE lpDst, int SrcXE, int DstXE, int x_fract);

void X_Stretch_1_1_16Bits(LPBYTE lpSrc, LPBYTE lpDst, int SrcXE, int DstXE, int x_fract);
void X_Stretch_1_2_16Bits(LPBYTE lpSrc, LPBYTE lpDst, int SrcXE, int DstXE, int x_fract);
void X_Stretch_1_N_16Bits(LPBYTE lpSrc, LPBYTE lpDst, int SrcXE, int DstXE, int x_fract);
void X_Stretch_N_1_16Bits(LPBYTE lpSrc, LPBYTE lpDst, int SrcXE, int DstXE, int x_fract);

void X_Stretch_1_1_24Bits(LPBYTE lpSrc, LPBYTE lpDst, int SrcXE, int DstXE, int x_fract);
void X_Stretch_1_N_24Bits(LPBYTE lpSrc, LPBYTE lpDst, int SrcXE, int DstXE, int x_fract);
void X_Stretch_N_1_24Bits(LPBYTE lpSrc, LPBYTE lpDst, int SrcXE, int DstXE, int x_fract);

void X_Stretch_1_1_32Bits(LPBYTE lpSrc, LPBYTE lpDst, int SrcXE, int DstXE, int x_fract);
void X_Stretch_1_N_32Bits(LPBYTE lpSrc, LPBYTE lpDst, int SrcXE, int DstXE, int x_fract);
void X_Stretch_N_1_32Bits(LPBYTE lpSrc, LPBYTE lpDst, int SrcXE, int DstXE, int x_fract);


/*
* Y_Stretch_* functions copy DstYE scanlines (using
* an X_FUNC to copy each scanline) omitting or duplicating scanlines to
* fit the destination extent. Pick a Y_ depending on the ratio
* (1:N, N:1...)
*/

void Y_Stretch_1_N(LPBYTE lpSrc, LPBYTE lpDst, int SrcXE,int SrcYE, int DstXE,
				   int DstYE, int SrcWidth, int DstWidth, int x_fract,
				   X_FUNC x_func, int nBits);

void Y_Stretch_N_1(LPBYTE lpSrc, LPBYTE lpDst, int SrcXE,int SrcYE, int DstXE,
				   int DstYE, int SrcWidth, int DstWidth, int x_fract,
				   X_FUNC x_func);

/*
* special case y-stretch functions for 1:2 in both dimensions for 8 and 16 bits
* takes no X_FUNC arg. Will do entire stretch.
*/
void Stretch_1_2_8Bits(LPBYTE lpSrc, LPBYTE lpDst, int SrcXE,int SrcYE, int DstXE,
					   int DstYE, int SrcWidth, int DstWidth, int x_fract);


void Stretch_1_2_16Bits(LPBYTE lpSrc, LPBYTE lpDst, int SrcXE,int SrcYE, int DstXE,
						int DstYE, int SrcWidth, int DstWidth, int x_fract);

/* straight copy of one scanline of count bytes */
void X_CopyScanline(LPBYTE lpSrc, LPBYTE lpDst, int count);

#endif //RESIZER_H
