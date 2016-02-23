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
#include "internals/DSUtils.h"
#include "internals/DSCaptureUtils.h"
#include <amvideo.h>
#include <uuids.h>
#include <mtype.h>

#if defined (_WIN32_WCE)
#include <atlbase.h>
#include <atlstr.h>
#else
#include <atlconv.h>
#endif
#include <iostream>
#include <assert.h>

#include "tsk_debug.h"

#if defined (_WIN32_WCE)
#	include "internals/wince/cpropertybag.h"
#endif

HRESULT enumerateCaptureDevices(const std::string &prefix, std::vector<VideoGrabberName> *names)
{
    HRESULT hr = S_OK;

#ifdef _WIN32_WCE

    // FIXME: use FindNextDevice to query all devices
    HANDLE	handle = NULL;
    DEVMGR_DEVICE_INFORMATION di;

    TCHAR pwzName[MAX_PATH];
    memset(pwzName,NULL,MAX_PATH);

    GUID guidCamera = { 0xCB998A05, 0x122C, 0x4166, 0x84, 0x6A,
                        0x93, 0x3E, 0x4D, 0x7E, 0x3C, 0x86
                      }; // http://msdn.microsoft.com/en-us/library/aa918757.aspx

    di.dwSize = sizeof(di);

    for( int i=0; ; i++) {
        if(0 == i) {
            /* 1st time */
            handle = FindFirstDevice( DeviceSearchByGuid, &guidCamera, &di );
            if(!handle || !di.hDevice) {
                hr = ( HRESULT_FROM_WIN32( GetLastError() ));
                goto bail;
            }
        }
        else if(handle) {
            /* 2nd or 3rd time */
            BOOL ret = FindNextDevice(handle, &di);
            if(!ret || !di.hDevice) {
                /* No 2nd or 3rd camera ==> do not return error*/
                goto bail;
            }
        }
        else {
            assert(0);
        }

        StringCchCopy( pwzName, MAX_PATH, di.szDeviceName );

        /* from LPWSTR to LPSTR */
        char mbstr_name[MAX_PATH];
        memset(mbstr_name,NULL,MAX_PATH);
        wcstombs(mbstr_name, pwzName, MAX_PATH);

        VideoGrabberName grabberName(std::string((const char*)mbstr_name), std::string((const char*)mbstr_name));
        names->push_back(grabberName);
    }

bail:
    /* close */
    if(handle) {
        FindClose( handle );
    }

#else
    ICreateDevEnum *deviceEnum;
    IEnumMoniker *enumerator;
    IMoniker *moniker;

    // Create the System Device Enumerator
    hr = COCREATE(CLSID_SystemDeviceEnum, IID_ICreateDevEnum, deviceEnum);
    if (FAILED(hr)) {
        goto bail;
    }

    // Ask for a device enumerator
    hr = deviceEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &enumerator, INCLUDE_CATEGORY_FLAG);
    if (FAILED(hr)) {
        goto bail;
    }

    // hr = S_FALSE and enumerator is NULL if there is no device to enumerate
    if (!enumerator) {
        goto bail;
    }

    USES_CONVERSION;

    while (enumerator->Next(1, &moniker, NULL) == S_OK) {
        // Get the properties bag for each device
        IPropertyBag *propBag;
        hr = moniker->BindToStorage(0, 0, IID_IPropertyBag, reinterpret_cast<void**>(&propBag));
        if (FAILED(hr)) {
            SAFE_RELEASE(moniker);
            continue;
        }

        std::string name;
        std::string description;

        VARIANT varName;
        VariantInit(&varName);
        VARIANT varDescription;
        VariantInit(&varDescription);

        // Find the device path (uniqueness is guaranteed)
        hr = propBag->Read(L"DevicePath", &varName, 0);
        if (SUCCEEDED(hr)) {
            if (prefix != "") {
                name = prefix + ":";
            }
            name = name + std::string(W2A(varName.bstrVal));
        }

        // Find friendly name or the description
        hr = propBag->Read(L"FriendlyName", &varDescription, 0);
        if (SUCCEEDED(hr)) {
            description = std::string(W2A(varDescription.bstrVal));
        }
        else {
            hr = propBag->Read(L"Description", &varDescription, 0);
            if (SUCCEEDED(hr)) {
                description = std::string(W2A(varDescription.bstrVal));
            }
        }

        hr = VariantClear(&varName);
        hr = VariantClear(&varDescription);

        SAFE_RELEASE(propBag);
        SAFE_RELEASE(moniker);

        // Add it to the list
        if (name != "") {
            VideoGrabberName grabberName(name, description);
            names->push_back(grabberName);
        }
    }

bail:
    SAFE_RELEASE(enumerator);
    SAFE_RELEASE(deviceEnum);
#endif

    return hr;
}

HRESULT createSourceFilter(std::string *devicePath, IBaseFilter **sourceFilter)
{
    HRESULT hr;

    IEnumMoniker *enumerator = NULL;
    IMoniker *moniker = NULL;
    bool found = false;

    // Set sourceFilter to null
    SAFE_RELEASE((*sourceFilter));

#if defined( _WIN32_WCE)
    CPropertyBag  pBag;
    HANDLE	handle = NULL;
    DEVMGR_DEVICE_INFORMATION di;
    TCHAR pwzName[MAX_PATH];
    CComVariant varCamName;
    IPersistPropertyBag *propBag = NULL;
    GUID guidCamera = { 0xCB998A05, 0x122C, 0x4166, 0x84, 0x6A,
                        0x93, 0x3E, 0x4D, 0x7E, 0x3C, 0x86
                      }; // http://msdn.microsoft.com/en-us/library/aa918757.aspx

    di.dwSize = sizeof(di);

    for( int i=0; ; i++) {
        if(0 == i) {
            /* 1st time */
            handle = FindFirstDevice( DeviceSearchByGuid, &guidCamera, &di );
            if(!handle || !di.hDevice) {
                hr = ( HRESULT_FROM_WIN32( GetLastError() ));
                goto bail;
            }
        }
        else if(handle) {
            /* 2nd or 3rd time */
            BOOL ret = FindNextDevice(handle, &di);
            if(!ret || !di.hDevice) {
                /* No 2nd or 3rd camera ==> do not return error*/
                goto bail;
            }
        }
        else {
            assert(0);
        }

        StringCchCopy( pwzName, MAX_PATH, di.szDeviceName );

        /* from LPWSTR to LPSTR */
        char mbstr_name[MAX_PATH];
        memset(mbstr_name,NULL,MAX_PATH);
        wcstombs(mbstr_name, pwzName, MAX_PATH);

        if((std::string((const char*)mbstr_name) == (*devicePath)) || ("0" == (*devicePath))) {
            varCamName = pwzName;
            if( varCamName.vt != VT_BSTR ) {
                hr = E_OUTOFMEMORY;
                goto bail;
            }

            // Create Source filter
            hr = COCREATE(CLSID_VideoCapture, IID_IBaseFilter, *sourceFilter);
            if(FAILED(hr)) {
                goto bail;
            }

            // Query PropertyBag
            hr = QUERY((*sourceFilter), IID_IPersistPropertyBag, propBag);
            if(FAILED(hr)) {
                goto bail;
            }

            hr = pBag.Write( L"VCapName", &varCamName );
            if(FAILED(hr)) {
                goto bail;
            }

            hr = propBag->Load( &pBag, NULL );
            if(FAILED(hr)) {
                goto bail;
            }
        }
    }
#else
    ICreateDevEnum *deviceEnum = NULL;
    IPropertyBag *propBag = NULL;

    // Create the System Device Enumerator
    hr = COCREATE(CLSID_SystemDeviceEnum, IID_ICreateDevEnum, deviceEnum);
    if (FAILED(hr)) {
        goto bail;
    }

    // Ask for a device enumerator
    hr = deviceEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &enumerator, INCLUDE_CATEGORY_FLAG);
    if(FAILED(hr)) {
        goto bail;
    }

    // hr = S_FALSE and enumerator is NULL if there is no device to enumerate
    if(!enumerator) {
        goto bail;
    }

    USES_CONVERSION;

    while (!found && (enumerator->Next(1, &moniker, NULL) == S_OK)) {
        // Get the properties bag for each device
        hr = moniker->BindToStorage(0, 0, IID_IPropertyBag, reinterpret_cast<void**>(&propBag));
        if (FAILED(hr)) {
            SAFE_RELEASE(moniker);
            continue;
        }

        std::string name;

        VARIANT varName;
        VariantInit(&varName);

        // Find the device path (uniqueness is guaranteed)
        hr = propBag->Read(L"DevicePath", &varName, 0);
        if (SUCCEEDED(hr)) {
            name = std::string(W2A(varName.bstrVal));
        }

        // Check for device path
        // "Null" means first found
        if ((name == (*devicePath)) ||
                ("Null" == (*devicePath))) {
            hr = moniker->BindToObject(0, 0, IID_IBaseFilter, reinterpret_cast<void**>(&(*sourceFilter)));
            if (SUCCEEDED(hr)) {
                (*devicePath) = name;
                found = true;
            }
        }

        hr = VariantClear(&varName);

        SAFE_RELEASE(propBag);
        SAFE_RELEASE(moniker);
    }
#endif

bail:
#ifdef _WIN32_WCE
    if(handle) {
        FindClose(handle);
    }
#else
    SAFE_RELEASE(deviceEnum);
#endif
    SAFE_RELEASE(moniker);
    SAFE_RELEASE(enumerator);
    SAFE_RELEASE(propBag);

    return hr;
}

HRESULT getSupportedFormats(IBaseFilter *sourceFilter, std::vector<DSCaptureFormat> *formats)
{
    HRESULT hr = E_FAIL;
    IPin *pinOut = NULL;
    IAMStreamConfig *streamConfig = NULL;
    AM_MEDIA_TYPE *mediaType = NULL;
    int count, size;

    // Check source filter pointer
    if (!sourceFilter) {
        goto bail;
    }

    pinOut = GetPin(sourceFilter, PINDIR_OUTPUT);
    if(!pinOut) {
        goto bail;
    }

    // Retrieve the stream config interface
    hr = QUERY(pinOut, IID_IAMStreamConfig, streamConfig);
    if (FAILED(hr)) {
        goto bail;
    }

    // Get the number of capabilities
    hr = streamConfig->GetNumberOfCapabilities(&count, &size);
    if (FAILED(hr)) {
        goto bail;
    }

    hr = streamConfig->GetFormat(&mediaType);
    if (FAILED(hr)) {
        goto bail;
    }

    // Iterate through the formats
    for (int i = 0; i < count; i++) {
        VIDEO_STREAM_CONFIG_CAPS streamConfigCaps;

        hr = streamConfig->GetStreamCaps(i, &mediaType, reinterpret_cast<BYTE*>(&streamConfigCaps));

        if (FAILED(hr)) {
            TSK_DEBUG_ERROR("Failed to get Stream caps");
            break;
        }

        if (streamConfigCaps.guid == FORMAT_VideoInfo) {
            VIDEOINFOHEADER* vih = reinterpret_cast<VIDEOINFOHEADER*>(mediaType->pbFormat);
            BITMAPINFOHEADER* bih = &vih->bmiHeader;

            int width = abs(bih->biWidth);
            int height = abs(bih->biHeight);
            int fps = (int) ((float)(vih->AvgTimePerFrame)/10000.f);
            GUID chroma = mediaType->subtype;

            // Add format to the list
            DSCaptureFormat format(width, height, fps, chroma);
            formats->push_back(format);
        }

        DeleteMediaType(mediaType);
    }

bail:
    SAFE_RELEASE(streamConfig);
    SAFE_RELEASE(pinOut);

    return hr;
}
