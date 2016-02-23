// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#include "DuplicationManager.h"

#include "tinymedia/tmedia_producer.h"

#include <Mfapi.h>

static inline HRESULT CopyRGBb32DownTop(
    BYTE*       pDst,
    const BYTE* pSrc,
    INT       dwWidthDstPixels,
    INT       dwWidthSrcPixels,
    INT       dwHeightPixels
);
//
// Constructor sets up references / variables
//
DUPLICATIONMANAGER::DUPLICATIONMANAGER() : m_DeskDupl(nullptr),
    m_AcquiredDesktopImage(nullptr),
    m_MetaDataBuffer(nullptr),
    m_MetaDataSize(0),
    m_OutputNumber(0),
    m_Device(nullptr),
    m_DeviceContext(nullptr),
    m_BufferPtr(nullptr),
    m_BufferSize(0)
{
    RtlZeroMemory(&m_OutputDesc, sizeof(m_OutputDesc));
}

//
// Destructor simply calls CleanRefs to destroy everything
//
DUPLICATIONMANAGER::~DUPLICATIONMANAGER()
{
    if (m_DeskDupl) {
        m_DeskDupl->Release();
        m_DeskDupl = nullptr;
    }

    if (m_AcquiredDesktopImage) {
        m_AcquiredDesktopImage->Release();
        m_AcquiredDesktopImage = nullptr;
    }

    if (m_MetaDataBuffer) {
        delete [] m_MetaDataBuffer;
        m_MetaDataBuffer = nullptr;
    }

    if (m_DeviceContext) {
        m_DeviceContext->Release();
        m_DeviceContext = nullptr;
    }

    if (m_Device) {
        m_Device->Release();
        m_Device = nullptr;
    }

    if (m_BufferPtr) {
        VirtualFree(m_BufferPtr, 0, MEM_RELEASE);
        m_BufferPtr = nullptr;
    }
}

//
// Initialize duplication interfaces
//
DUPL_RETURN DUPLICATIONMANAGER::InitDupl(_In_ ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, UINT Output)
{
    m_OutputNumber = Output;

    // Take a reference on the device
    m_Device = Device;
    m_Device->AddRef();

    m_DeviceContext = DeviceContext;
    m_DeviceContext->AddRef();

    // Get DXGI device
    IDXGIDevice* DxgiDevice = nullptr;
    HRESULT hr = m_Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&DxgiDevice));
    if (FAILED(hr)) {
        return ProcessFailure(nullptr, L"Failed to QI for DXGI Device", L"Error", hr);
    }

    // Get DXGI adapter
    IDXGIAdapter* DxgiAdapter = nullptr;
    hr = DxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&DxgiAdapter));
    DxgiDevice->Release();
    DxgiDevice = nullptr;
    if (FAILED(hr)) {
        return ProcessFailure(m_Device, L"Failed to get parent DXGI Adapter", L"Error", hr, SystemTransitionsExpectedErrors);
    }

    // Get output
    IDXGIOutput* DxgiOutput = nullptr;
    hr = DxgiAdapter->EnumOutputs(Output, &DxgiOutput);
    DxgiAdapter->Release();
    DxgiAdapter = nullptr;
    if (FAILED(hr)) {
        return ProcessFailure(m_Device, L"Failed to get specified output in DUPLICATIONMANAGER", L"Error", hr, EnumOutputsExpectedErrors);
    }

    DxgiOutput->GetDesc(&m_OutputDesc);

    // QI for Output 1
    IDXGIOutput1* DxgiOutput1 = nullptr;
    hr = DxgiOutput->QueryInterface(__uuidof(DxgiOutput1), reinterpret_cast<void**>(&DxgiOutput1));
    DxgiOutput->Release();
    DxgiOutput = nullptr;
    if (FAILED(hr)) {
        return ProcessFailure(nullptr, L"Failed to QI for DxgiOutput1 in DUPLICATIONMANAGER", L"Error", hr);
    }

    // Create desktop duplication
    hr = DxgiOutput1->DuplicateOutput(m_Device, &m_DeskDupl);
    DxgiOutput1->Release();
    DxgiOutput1 = nullptr;
    if (FAILED(hr)) {
        if (hr == DXGI_ERROR_NOT_CURRENTLY_AVAILABLE) {
            MessageBoxW(nullptr, L"There is already the maximum number of applications using the Desktop Duplication API running, please close one of those applications and then try again.", L"Error", MB_OK);
            return DUPL_RETURN_ERROR_UNEXPECTED;
        }
        return ProcessFailure(m_Device, L"Failed to get duplicate output in DUPLICATIONMANAGER", L"Error", hr, CreateDuplicationExpectedErrors);
    }

    return DUPL_RETURN_SUCCESS;
}

//
// Retrieves mouse info and write it into PtrInfo
//
DUPL_RETURN DUPLICATIONMANAGER::GetMouse(_Inout_ PTR_INFO* PtrInfo, _In_ DXGI_OUTDUPL_FRAME_INFO* FrameInfo, INT OffsetX, INT OffsetY)
{
    // A non-zero mouse update timestamp indicates that there is a mouse position update and optionally a shape change
    if (FrameInfo->LastMouseUpdateTime.QuadPart == 0) {
        return DUPL_RETURN_SUCCESS;
    }

    bool UpdatePosition = true;

    // Make sure we don't update pointer position wrongly
    // If pointer is invisible, make sure we did not get an update from another output that the last time that said pointer
    // was visible, if so, don't set it to invisible or update.
    if (!FrameInfo->PointerPosition.Visible && (PtrInfo->WhoUpdatedPositionLast != m_OutputNumber)) {
        UpdatePosition = false;
    }

    // If two outputs both say they have a visible, only update if new update has newer timestamp
    if (FrameInfo->PointerPosition.Visible && PtrInfo->Visible && (PtrInfo->WhoUpdatedPositionLast != m_OutputNumber) && (PtrInfo->LastTimeStamp.QuadPart > FrameInfo->LastMouseUpdateTime.QuadPart)) {
        UpdatePosition = false;
    }

    // Update position
    if (UpdatePosition) {
        PtrInfo->Position.x = FrameInfo->PointerPosition.Position.x + m_OutputDesc.DesktopCoordinates.left - OffsetX;
        PtrInfo->Position.y = FrameInfo->PointerPosition.Position.y + m_OutputDesc.DesktopCoordinates.top - OffsetY;
        PtrInfo->WhoUpdatedPositionLast = m_OutputNumber;
        PtrInfo->LastTimeStamp = FrameInfo->LastMouseUpdateTime;
        PtrInfo->Visible = FrameInfo->PointerPosition.Visible != 0;
    }

    // No new shape
    if (FrameInfo->PointerShapeBufferSize == 0) {
        return DUPL_RETURN_SUCCESS;
    }

    // Old buffer too small
    if (FrameInfo->PointerShapeBufferSize > PtrInfo->BufferSize) {
        if (PtrInfo->PtrShapeBuffer) {
            delete [] PtrInfo->PtrShapeBuffer;
            PtrInfo->PtrShapeBuffer = nullptr;
        }
        PtrInfo->PtrShapeBuffer = new (std::nothrow) BYTE[FrameInfo->PointerShapeBufferSize];
        if (!PtrInfo->PtrShapeBuffer) {
            PtrInfo->BufferSize = 0;
            return ProcessFailure(nullptr, L"Failed to allocate memory for pointer shape in DUPLICATIONMANAGER", L"Error", E_OUTOFMEMORY);
        }

        // Update buffer size
        PtrInfo->BufferSize = FrameInfo->PointerShapeBufferSize;
    }

    // Get shape
    UINT BufferSizeRequired;
    HRESULT hr = m_DeskDupl->GetFramePointerShape(FrameInfo->PointerShapeBufferSize, reinterpret_cast<VOID*>(PtrInfo->PtrShapeBuffer), &BufferSizeRequired, &(PtrInfo->ShapeInfo));
    if (FAILED(hr)) {
        delete [] PtrInfo->PtrShapeBuffer;
        PtrInfo->PtrShapeBuffer = nullptr;
        PtrInfo->BufferSize = 0;
        return ProcessFailure(m_Device, L"Failed to get frame pointer shape in DUPLICATIONMANAGER", L"Error", hr, FrameInfoExpectedErrors);
    }

    return DUPL_RETURN_SUCCESS;
}


//
// Get next frame and write it into Data
//
_Success_(*Timeout == false && return == DUPL_RETURN_SUCCESS)
DUPL_RETURN DUPLICATIONMANAGER::GetFrame(_Out_ FRAME_DATA* Data, _Out_ bool* Timeout)
{
    IDXGIResource* DesktopResource = nullptr;
    DXGI_OUTDUPL_FRAME_INFO FrameInfo;

    // Get new frame
    HRESULT hr = m_DeskDupl->AcquireNextFrame(500, &FrameInfo, &DesktopResource);
    if (hr == DXGI_ERROR_WAIT_TIMEOUT) {
        *Timeout = true;
        return DUPL_RETURN_SUCCESS;
    }
    *Timeout = false;

    if (FAILED(hr)) {
        return ProcessFailure(m_Device, L"Failed to acquire next frame in DUPLICATIONMANAGER", L"Error", hr, FrameInfoExpectedErrors);
    }

    // If still holding old frame, destroy it
    if (m_AcquiredDesktopImage) {
        m_AcquiredDesktopImage->Release();
        m_AcquiredDesktopImage = nullptr;
    }

    // QI for IDXGIResource
    hr = DesktopResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&m_AcquiredDesktopImage));
    DesktopResource->Release();
    DesktopResource = nullptr;
    if (FAILED(hr)) {
        return ProcessFailure(nullptr, L"Failed to QI for ID3D11Texture2D from acquired IDXGIResource in DUPLICATIONMANAGER", L"Error", hr);
    }

    // Get metadata
    if (FrameInfo.TotalMetadataBufferSize) {
        // Old buffer too small
        if (FrameInfo.TotalMetadataBufferSize > m_MetaDataSize) {
            if (m_MetaDataBuffer) {
                delete [] m_MetaDataBuffer;
                m_MetaDataBuffer = nullptr;
            }
            m_MetaDataBuffer = new (std::nothrow) BYTE[FrameInfo.TotalMetadataBufferSize];
            if (!m_MetaDataBuffer) {
                m_MetaDataSize = 0;
                Data->MoveCount = 0;
                Data->DirtyCount = 0;
                return ProcessFailure(nullptr, L"Failed to allocate memory for metadata in DUPLICATIONMANAGER", L"Error", E_OUTOFMEMORY);
            }
            m_MetaDataSize = FrameInfo.TotalMetadataBufferSize;
        }

        UINT BufSize = FrameInfo.TotalMetadataBufferSize;

        // Get move rectangles
        hr = m_DeskDupl->GetFrameMoveRects(BufSize, reinterpret_cast<DXGI_OUTDUPL_MOVE_RECT*>(m_MetaDataBuffer), &BufSize);
        if (FAILED(hr)) {
            Data->MoveCount = 0;
            Data->DirtyCount = 0;
            return ProcessFailure(nullptr, L"Failed to get frame move rects in DUPLICATIONMANAGER", L"Error", hr, FrameInfoExpectedErrors);
        }
        Data->MoveCount = BufSize / sizeof(DXGI_OUTDUPL_MOVE_RECT);

        BYTE* DirtyRects = m_MetaDataBuffer + BufSize;
        BufSize = FrameInfo.TotalMetadataBufferSize - BufSize;

        // Get dirty rectangles
        hr = m_DeskDupl->GetFrameDirtyRects(BufSize, reinterpret_cast<RECT*>(DirtyRects), &BufSize);
        if (FAILED(hr)) {
            Data->MoveCount = 0;
            Data->DirtyCount = 0;
            return ProcessFailure(nullptr, L"Failed to get frame dirty rects in DUPLICATIONMANAGER", L"Error", hr, FrameInfoExpectedErrors);
        }
        Data->DirtyCount = BufSize / sizeof(RECT);

        Data->MetaData = m_MetaDataBuffer;
    }

    Data->Frame = m_AcquiredDesktopImage;
    Data->FrameInfo = FrameInfo;

    return DUPL_RETURN_SUCCESS;
}

//
// Release frame
//
DUPL_RETURN DUPLICATIONMANAGER::DoneWithFrame()
{
    HRESULT hr = m_DeskDupl->ReleaseFrame();
    if (FAILED(hr)) {
        return ProcessFailure(m_Device, L"Failed to release frame in DUPLICATIONMANAGER", L"Error", hr, FrameInfoExpectedErrors);
    }

    if (m_AcquiredDesktopImage) {
        m_AcquiredDesktopImage->Release();
        m_AcquiredDesktopImage = nullptr;
    }

    return DUPL_RETURN_SUCCESS;
}

//
// Gets output desc into DescPtr
//
void DUPLICATIONMANAGER::GetOutputDesc(_Out_ DXGI_OUTPUT_DESC* DescPtr)
{
    *DescPtr = m_OutputDesc;
}


HRESULT DUPLICATIONMANAGER::SendData(struct tmedia_producer_s* pProducer, FRAME_DATA* FrameData)
{
    HRESULT hr = E_FAIL;
    D3D11_TEXTURE2D_DESC CopyBufferDesc = {0};
    D3D11_TEXTURE2D_DESC FullDesc;
    DXGI_MAPPED_RECT MappedSurface;
    D3D11_BOX Box;
    UINT BuffSize;

    ID3D11Texture2D* CopyBuffer = nullptr;
    IDXGISurface* CopySurface = nullptr;
    ID3D11Device* Device = nullptr;

    FrameData->Frame->GetDesc(&FullDesc);

    CopyBufferDesc.Width = FullDesc.Width;
    CopyBufferDesc.Height = FullDesc.Height;
    CopyBufferDesc.MipLevels = 1;
    CopyBufferDesc.ArraySize = 1;
    CopyBufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    CopyBufferDesc.SampleDesc.Count = 1;
    CopyBufferDesc.SampleDesc.Quality = 0;
    CopyBufferDesc.Usage = D3D11_USAGE_STAGING;
    CopyBufferDesc.BindFlags = 0;
    CopyBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    CopyBufferDesc.MiscFlags = 0;

    FrameData->Frame->GetDevice(&Device);
    if (!Device) {
        hr = E_POINTER;
        ProcessFailure(m_Device, L"Failed to get device", L"Error", hr, SystemTransitionsExpectedErrors);
        goto bail;
    }

    hr = Device->CreateTexture2D(&CopyBufferDesc, nullptr, &CopyBuffer);
    if (FAILED(hr)) {
        ProcessFailure(m_Device, L"Failed creating staging texture for pointer", L"Error", hr, SystemTransitionsExpectedErrors);
        goto bail;
    }

    Box.left = 0;
    Box.top = 0;
    Box.right = CopyBufferDesc.Width;
    Box.bottom = CopyBufferDesc.Height;
    Box.front = 0;
    Box.back = 1;
    m_DeviceContext->CopySubresourceRegion(CopyBuffer, 0, 0, 0, 0, FrameData->Frame, 0, &Box);

    hr = CopyBuffer->QueryInterface(__uuidof(IDXGISurface), (void **)&CopySurface);
    if (FAILED(hr)) {
        ProcessFailure(nullptr, L"Failed to QI staging texture into IDXGISurface for pointer", L"Error", hr, SystemTransitionsExpectedErrors);
        goto bail;
    }

    BuffSize = CopyBufferDesc.Width * CopyBufferDesc.Height * 4;
    if (m_BufferSize < BuffSize) {
        if (m_BufferPtr) {
            VirtualFree(m_BufferPtr, 0, MEM_RELEASE);
            m_BufferSize = 0;
        }
        if (!(m_BufferPtr = (BYTE*)VirtualAlloc(NULL, BuffSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE))) {
            ProcessFailure(Device, L"Failed to allocate memory", L"Error", hr, SystemTransitionsExpectedErrors);
            goto bail;
        }
        m_BufferSize = BuffSize;
    }

    hr = CopySurface->Map(&MappedSurface, DXGI_MAP_READ); // *** MAP *** //
    if (FAILED(hr)) {
        ProcessFailure(Device, L"Failed to map surface for pointer", L"Error", hr, SystemTransitionsExpectedErrors);
        goto bail;
    }

    pProducer->video.width = CopyBufferDesc.Width;
    pProducer->video.height = CopyBufferDesc.Height;

#if 0
    hr = MFCopyImage(
             m_BufferPtr,
             (LONG)(CopyBufferDesc.Width << 2),
             (BYTE*)MappedSurface.pBits,
             (LONG)MappedSurface.Pitch,
             (DWORD)(CopyBufferDesc.Width << 2),
             (DWORD)CopyBufferDesc.Height
         );
#else;
    hr = CopyRGBb32DownTop(
             m_BufferPtr,
             MappedSurface.pBits,
             CopyBufferDesc.Width,
             (MappedSurface.Pitch >> 2), // Bytes -> Pixels
             CopyBufferDesc.Height);
#endif

    pProducer->enc_cb.callback(pProducer->enc_cb.callback_data, m_BufferPtr, BuffSize);

    CopySurface->Unmap(); // *** UNMAP *** //

bail:
    if (CopyBuffer) {
        CopyBuffer->Release();
    }
    if (CopySurface) {
        CopySurface->Release();
    }
    if (Device) {
        Device->Release();
    }
    return hr;
}

// For RGB32:
// Direct3D -> Top-Down
// Video Processor -> Down-Top
static inline HRESULT CopyRGBb32DownTop(
    BYTE*       pDst,
    const BYTE* pSrc,
    INT       dwWidthDstPixels,
    INT       dwWidthSrcPixels,
    INT       dwHeightPixels
)
{
    RGBQUAD *pSrcPixel = &((RGBQUAD*)pSrc)[(dwWidthSrcPixels * dwHeightPixels) - dwWidthSrcPixels];
    RGBQUAD *pDestPixel = &((RGBQUAD*)pDst)[0];

    register INT x;
    register INT y;

    for (y = dwHeightPixels; y > 0; --y) {
        for (x = 0; x < dwWidthDstPixels; ++x) {
            pDestPixel[x] = pSrcPixel[x];
        }
        pDestPixel += dwWidthDstPixels;
        pSrcPixel -= dwWidthSrcPixels;
    }
    return S_OK;
}