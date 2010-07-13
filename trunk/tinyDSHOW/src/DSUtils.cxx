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
#include <tinydshow/DSUtils.h>

#include <atlbase.h>
#include <atlstr.h>

IPin *GetPin(IBaseFilter *filter, PIN_DIRECTION direction)
{
	IEnumPins	*enumPins = NULL;
	IPin		*pin = NULL;

	HRESULT hr = filter->EnumPins(&enumPins);
	if(!enumPins){
		return NULL;
	}

	for(;;){
		ULONG fetched = 0;
		PIN_DIRECTION pinDir = PIN_DIRECTION(-1);
		pin = NULL;

		if (FAILED(enumPins->Next(1, &pin, &fetched))){
			enumPins->Release();
			return NULL;
		}

		if (fetched == 1 && pin){
			pin->QueryDirection(&pinDir);
			if(pinDir == direction){
				break;
			}
			pin->Release();
		}
	}

	enumPins->Release();
	return pin;
}

HRESULT ConnectFilters(IGraphBuilder *graphBuilder, IBaseFilter *source, IBaseFilter *destination, AM_MEDIA_TYPE *mediaType)
{
	HRESULT hr;

	IPin *outPin = GetPin(source, PINDIR_OUTPUT);
	IPin *inPin = GetPin(destination, PINDIR_INPUT);

	if (mediaType != NULL){
		hr = graphBuilder->ConnectDirect(outPin, inPin, mediaType);
	}
	else{
		hr = graphBuilder->Connect(outPin, inPin);
	}

	SAFE_RELEASE(outPin);
	SAFE_RELEASE(inPin);

	return hr;
}

HRESULT DisconnectFilters(IGraphBuilder *graphBuilder, IBaseFilter *source, IBaseFilter *destination)
{
	HRESULT hr;

	IPin *outPin = GetPin(source, PINDIR_OUTPUT);
	IPin *inPin = GetPin(destination, PINDIR_INPUT);

	if (inPin){
		hr = graphBuilder->Disconnect(inPin);
	}

	if (outPin){
		hr = graphBuilder->Disconnect(outPin);
	}

	SAFE_RELEASE(outPin);
	SAFE_RELEASE(inPin);

	return hr;
}

bool DisconnectAllFilters(IGraphBuilder *graphBuilder)
{
	CComPtr<IEnumFilters> filterEnum = NULL;
	CComPtr<IBaseFilter> currentFilter = NULL;
	ULONG fetched;
	HRESULT hr;

	hr = graphBuilder->EnumFilters(&filterEnum);
	if (FAILED(hr)) return false;

	while(filterEnum->Next(1, &currentFilter, &fetched) == S_OK){
		hr = DisconnectFilters(graphBuilder, currentFilter, currentFilter);
	}

	filterEnum.Release();
	return true;
}

bool RemoveAllFilters(IGraphBuilder *graphBuilder)
{
	CComPtr<IEnumFilters> filterEnum = NULL;
	CComPtr<IBaseFilter> currentFilter = NULL;
	ULONG fetched;
	HRESULT hr;

	hr = graphBuilder->EnumFilters(&filterEnum);
	if (FAILED(hr)) return false;

	while(filterEnum->Next(1, &currentFilter, &fetched) == S_OK){
		hr = graphBuilder->RemoveFilter(currentFilter);
		if (FAILED(hr)){
			filterEnum.Release();
			return false;
		}
		currentFilter.Release();
		filterEnum->Reset();
	}

	filterEnum.Release();
	return true;
}
