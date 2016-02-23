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
#if !defined(VMR) && !defined(VMR9) && !defined(VMR9_WINDOWLESS)

#include "internals/DSDisplayOverlay.h"
#include "internals/DSDisplayGraph.h"

#include <iostream>

using namespace std;

DSDisplayOverlay::DSDisplayOverlay()
{
}

DSDisplayOverlay::~DSDisplayOverlay()
{
}

void DSDisplayOverlay::attach(HWND parent, DSDisplayGraph *graph)
{
    this->displayGraph = graph;
}

void DSDisplayOverlay::detach()
{
    this->displayGraph = NULL;
}

void DSDisplayOverlay::show(int value)
{
    // Store the ticks to count down
    this->ticks = value;

    this->internalUpdate();
}

void DSDisplayOverlay::update()
{
    if (this->displayGraph && (this->ticks > 0)) {
        this->ticks--;
        this->internalUpdate();
    }
}

void DSDisplayOverlay::internalUpdate()
{
    this->displayGraph->getSourceFilter()->showOverlay(this->ticks);
}

#endif
