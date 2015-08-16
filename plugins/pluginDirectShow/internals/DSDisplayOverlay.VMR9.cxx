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
#if defined(VMR9) || defined(VMR9_WINDOWLESS)

#define DIRECT3D_VERSION 0x0900

#include <internals/DSDisplayOverlay.h>
#include <internals/DSDisplayGraph.h>
#include <internals/DSUtils.h>

using namespace std;

#define FILENAME			_T("Overlay.png")
#define ALPHA_VALUE_START	0.8f
#define ALPHA_VALUE_STOP	0.0f


DSDisplayOverlay::DSDisplayOverlay()
{
	this->window = NULL;
	this->direct3DDevice = NULL;
	this->direct3DSurface = NULL;

	this->direct3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!this->direct3D)
	{
		cout << "Cannot create Direct3D environment" << endl;
		return;
	}
}

DSDisplayOverlay::~DSDisplayOverlay()
{
	SAFE_RELEASE(this->direct3D);
}

void DSDisplayOverlay::attach(HWND parent, DSDisplayGraph *graph)
{
	HRESULT hr;

	// Gets the handle of the parent and the graph
	this->window = parent;
	this->displayGraph = graph;

	if (this->window)
	{
		D3DPRESENT_PARAMETERS d3dpp; 
		ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;

		hr = this->direct3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			this->window,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&this->direct3DDevice);
		if (FAILED(hr))
		{
			cout << "Cannot create Direct3D device" << endl;
			return;
		}

		ZeroMemory(&this->overlayInfo, sizeof(D3DXIMAGE_INFO));
		hr = D3DXGetImageInfoFromFile(FILENAME, &this->overlayInfo);
		if (FAILED(hr))
		{
			cout << "Cannot stat overlay file" << endl;
			return;
		}

		hr = this->direct3DDevice->CreateOffscreenPlainSurface(
			this->overlayInfo.Width,
			this->overlayInfo.Height,
			D3DFMT_A8R8G8B8,
			D3DPOOL_SYSTEMMEM,
			&this->direct3DSurface,
			NULL);
		if (FAILED(hr))
		{
			cout << "Cannot create Direct3D surface" << endl;
			return;
		}

		D3DCOLOR alphaKey = 0xFF000000;

		hr = D3DXLoadSurfaceFromFile(this->direct3DSurface,
			NULL,
			NULL,
			FILENAME,
			NULL,
			D3DX_FILTER_NONE,
			alphaKey,
			&this->overlayInfo);
		if (FAILED(hr))
		{
			cout << "Cannot load overlay file" << endl;
			return;
		}

		D3DVIEWPORT9 viewport;
		ZeroMemory(&viewport, sizeof(D3DVIEWPORT9));

		hr= this->direct3DDevice->GetViewport(&viewport);
		if (FAILED(hr))
		{
			cout << "Cannot get view port" << endl;
			return;
		}

		ZeroMemory(&this->alphaBitmap, sizeof(VMR9AlphaBitmap));
		this->alphaBitmap.dwFlags = VMR9AlphaBitmap_EntireDDS;
		this->alphaBitmap.hdc = NULL;
		this->alphaBitmap.pDDS = this->direct3DSurface;
		// Source rectangle
		this->alphaBitmap.rSrc.left = 0;
		this->alphaBitmap.rSrc.top = 0;
		this->alphaBitmap.rSrc.right = this->overlayInfo.Width;
		this->alphaBitmap.rSrc.bottom = this->overlayInfo.Height;
		// Destination rectangle
		this->alphaBitmap.rDest.left = (viewport.Width - this->overlayInfo.Width) / 2.0;
		this->alphaBitmap.rDest.top = (viewport.Height - this->overlayInfo.Height) / 2.0;
		this->alphaBitmap.rDest.right = this->alphaBitmap.rDest.left + this->overlayInfo.Width;
		this->alphaBitmap.rDest.bottom = this->alphaBitmap.rDest.top + this->overlayInfo.Height;
		this->alphaBitmap.rDest.left /= viewport.Width;
		this->alphaBitmap.rDest.top /= viewport.Height;
		this->alphaBitmap.rDest.right /= viewport.Width;
		this->alphaBitmap.rDest.bottom /= viewport.Height;
		// Alpha value for start
		this->alphaBitmap.fAlpha = ALPHA_VALUE_START;
	}
}

void DSDisplayOverlay::detach()
{
	SAFE_RELEASE(this->direct3DSurface);
	SAFE_RELEASE(this->direct3DDevice);

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
	if (this->displayGraph && (this->ticks > 0))
	{
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

	if (this->ticks > 0)
	{
		this->alphaBitmap.dwFlags = VMR9AlphaBitmap_EntireDDS;
	}
	else
	{
		this->alphaBitmap.dwFlags = VMR9AlphaBitmap_Disable;
	}

	hr = this->displayGraph->getMixerBitmap()->SetAlphaBitmap(&this->alphaBitmap);
	if (FAILED(hr))
	{
		cout << "Failed to mix overylay (" << hr << ")" << endl;
		return;
	}
}

#endif
