/* Copyright (C) 2013 Mamadou DIOP
* Copyright (C) 2013 Doubango Telecom <http://www.doubango.org>
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
#include "mf_devices.h"
#include "mf_utils.h"

DeviceList::DeviceList()
: m_ppDevices(NULL)
, m_cDevices(0)
{
	
}

DeviceList::~DeviceList()
{
	Clear();
}

UINT32 DeviceList::Count()const
{
	return m_cDevices;
}

void DeviceList::Clear()
{
	for (UINT32 i = 0; i < m_cDevices; i++) {
        SafeRelease(&m_ppDevices[i]);
    }
    CoTaskMemFree(m_ppDevices);
    m_ppDevices = NULL;

    m_cDevices = 0;
}

HRESULT DeviceList::EnumerateDevices(const GUID& sourceType)
{
	HRESULT hr = S_OK;
    IMFAttributes *pAttributes = NULL;

    Clear();

    // Initialize an attribute store. We will use this to
    // specify the enumeration parameters.

    hr = MFCreateAttributes(&pAttributes, 1);

    // Ask for source type = video capture devices
    if (SUCCEEDED(hr))
    {
        hr = pAttributes->SetGUID(
            MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
            sourceType
            );
    }

    // Enumerate devices.
    if (SUCCEEDED(hr))
    {
        hr = MFEnumDeviceSources(pAttributes, &m_ppDevices, &m_cDevices);
    }

    SafeRelease(&pAttributes);

    return hr;
}

HRESULT DeviceList::GetDeviceAtIndex(UINT32 index, IMFActivate **ppActivate)
{
	if (index >= Count())
	{
        return E_INVALIDARG;
    }

    *ppActivate = m_ppDevices[index];
    (*ppActivate)->AddRef();

    return S_OK;
}

HRESULT DeviceList::GetDeviceBest(IMFActivate **ppActivate, WCHAR *pszName /*= NULL*/)
{
	UINT32 index = 0;
	if(pszName) 
	{
		WCHAR *_pszName = NULL;
		BOOL bFound = FALSE;
		for(UINT32 i = 0; i < Count() && !bFound; ++i)
		{
			if((SUCCEEDED(GetDeviceName(i, &_pszName))))
			{
				if(wcscmp(_pszName, pszName) == 0)
				{
					index = i;
					bFound = TRUE;
					// do not break the loop because we need to free(_pszName)
				}
			}
			if(_pszName)
			{
				CoTaskMemFree(_pszName), _pszName = NULL;
			}
		}
	}
	return GetDeviceAtIndex(index, ppActivate);
}

// The caller must free the memory for the string by calling CoTaskMemFree
HRESULT DeviceList::GetDeviceName(UINT32 index, WCHAR **ppszName)
{
	if (index >= Count())
	{
        return E_INVALIDARG;
    }

    HRESULT hr = S_OK;

    hr = m_ppDevices[index]->GetAllocatedString(
        MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME,
        ppszName,
        NULL
        );

    return hr;
}

HRESULT DeviceListAudio::EnumerateDevices()
{
	// call base class function
	return DeviceList::EnumerateDevices(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_AUDCAP_GUID);
}

HRESULT DeviceListVideo::EnumerateDevices()
{
	// call base class function
	return DeviceList::EnumerateDevices(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
}
