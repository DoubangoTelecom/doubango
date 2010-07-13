/*
* Copyright (C) 2009-2010 Mamadou Diop.
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
#ifndef DSCAPTUREFORMAT_H
#define DSCAPTUREFORMAT_H

#include <tinydshow_config.h>
#include <strmif.h>


class DSCaptureFormat
{
public:
	DSCaptureFormat(int w, int h, int f, GUID c) : width(w), height(h), fps(f), chroma(c) {};
	virtual ~DSCaptureFormat() {};

	int getWidth() { return this->width; };
	int getHeight() { return this->height; };
	int getFramerate() { return this->fps; };
	GUID getChroma() { return this->chroma; };

	int getMatchScore(int w, int h);
	bool isRGB();

private:
	int width;
	int height;
	int fps;
	GUID chroma;
};


#endif
