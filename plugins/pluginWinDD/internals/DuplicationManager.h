// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#ifndef _DUPLICATIONMANAGER_H_
#define _DUPLICATIONMANAGER_H_

#include "CommonTypes.h"

//
// Handles the task of duplicating an output.
//
class DUPLICATIONMANAGER
{
public:
    DUPLICATIONMANAGER();
    ~DUPLICATIONMANAGER();
    _Success_(*Timeout == false && return == DUPL_RETURN_SUCCESS) DUPL_RETURN GetFrame(_Out_ FRAME_DATA* Data, _Out_ bool* Timeout);
    DUPL_RETURN DoneWithFrame();
    DUPL_RETURN InitDupl(_In_ ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, UINT Output);
    DUPL_RETURN GetMouse(_Inout_ PTR_INFO* PtrInfo, _In_ DXGI_OUTDUPL_FRAME_INFO* FrameInfo, INT OffsetX, INT OffsetY);
    void GetOutputDesc(_Out_ DXGI_OUTPUT_DESC* DescPtr);
    HRESULT SendData(struct tmedia_producer_s* pProducer, FRAME_DATA* FrameData);

private:

    // vars
    IDXGIOutputDuplication* m_DeskDupl;
    ID3D11Texture2D* m_AcquiredDesktopImage;
    _Field_size_bytes_(m_MetaDataSize) BYTE* m_MetaDataBuffer;
    UINT m_MetaDataSize;
    UINT m_OutputNumber;
    DXGI_OUTPUT_DESC m_OutputDesc;
    ID3D11Device* m_Device;
    ID3D11DeviceContext* m_DeviceContext;
    BYTE* m_BufferPtr;
    UINT m_BufferSize;
};

#endif
