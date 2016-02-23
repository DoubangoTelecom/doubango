#if defined(_WIN32_WCE)

#include <ddraw.h>
#include <internals/wince/DSSampleGrabber.h>
#include <initguid.h>


// {38589364-71FD-4641-B426-E443DB023568}
TDSHOW_DEFINE_GUID(CLSID_SampleGrabber,
                   0x38589364, 0x71fd, 0x4641, 0xb4, 0x26, 0xe4, 0x43, 0xdb, 0x2, 0x35, 0x68);

#define RGB565_MASK_RED				0xF800
#define RGB565_MASK_GREEN           0x07E0
#define RGB565_MASK_BLUE            0x001F

DSSampleGrabber::DSSampleGrabber(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr)
    :CTransInPlaceFilter (tszName, punk, CLSID_SampleGrabber, phr)
{
#define FPS_INPUT  30
#define FPS_OUTPUT 5

    this->m_rtFrameLength = (10000000)/FPS_OUTPUT;

    this->m_inputFps = FPS_INPUT;
    this->m_outputFps = FPS_OUTPUT;

    this->m_iFrameNumber = 0;
    this->m_progress = 0;
    this->m_bProcessFrame = true;

    this->callback = NULL;
    this->m_rgb24 = NULL;

    m_cRef = 0;
}

DSSampleGrabber::~DSSampleGrabber()
{
    this->callback = NULL;
    if(this->m_rgb24) {
        delete[]this->m_rgb24;
        this->m_rgb24 = NULL;
    }
}

HRESULT DSSampleGrabber::SetFps(int inputFps, int outputFps)
{
    if (inputFps <= 0 || outputFps <= 0) {
        return E_FAIL;
    }

    // Stop prcessing
    this->m_bProcessFrame = false;

    if (inputFps < outputFps) {
        this->m_inputFps = this->m_outputFps = inputFps;
    }
    else {
        this->m_outputFps = outputFps;
        this->m_inputFps = inputFps;
    }

    // Restart processing
    this->m_iFrameNumber = 0;
    this->m_progress = 0;
    this->m_bProcessFrame = true;

    return S_OK;
}

HRESULT DSSampleGrabber::Transform(IMediaSample *pSample)
{
    BYTE *pData = NULL;
    HRESULT hr = S_OK;
    HRESULT ret = S_FALSE;

    if (!this->m_bProcessFrame) {
        return S_FALSE;
    }

    // Get pointer to the video buffer data
    if ( FAILED(pSample->GetPointer(&pData)) ) {
        ret = E_FAIL;
        goto bail;
    }

    pSample->SetTime(NULL, NULL);

    // Drop frame?
    if (this->m_iFrameNumber == 0) {
        ret = S_OK;
    }
    else if (this->m_progress >= this->m_inputFps) {
        this->m_progress -= this->m_inputFps;
        ret = S_OK;
    }

    // Mark frame as accepted
    if (ret == S_OK) {
        // Set TRUE on every sample for uncompressed frames
        pSample->SetSyncPoint(TRUE);

        long Size = pSample->GetSize();
        if ( this->callback ) {
            LONGLONG start, end;
            WORD *rgb565 = (WORD*)pData;

            for(int i = 0, i24 = 0, i565 = 0; i< (Size/2); i++, i24+=3, i565+=1) {
                BYTE *p24 = (this->m_rgb24+i24);
                WORD val565 = *(rgb565 + i565);

                // extract RGB
                p24[2] = (val565 & RGB565_MASK_RED) >> 11;
                p24[1] = (val565 & RGB565_MASK_GREEN) >> 5;
                p24[0] = (val565 & RGB565_MASK_BLUE);

                // amplify the image
                p24[2] <<= 3;
                p24[1] <<= 2;
                p24[0] <<= 3;
            }

            pSample->GetMediaTime(&start, &end);
            this->callback->BufferCB( (double)start, this->m_rgb24, ((Size >> 1) * 3));
        }
    }

    this->m_progress += this->m_outputFps;
    this->m_iFrameNumber++;

bail:
    SAFE_DELETE_ARRAY( pData );
    SAFE_RELEASE(pSample);

    return ret;
}

HRESULT DSSampleGrabber::CheckInputType(const CMediaType* mtIn)
{
    VIDEOINFO *video;
    if ( !IsEqualGUID( *mtIn->Subtype(), MEDIASUBTYPE_RGB565 ) || !(video=(VIDEOINFO *)mtIn->Format()) ) {
        return E_FAIL;
    }

    return S_OK;
}

STDMETHODIMP DSSampleGrabber::SetCallback( DSISampleGrabberCB* callback_ )
{
    if (!callback_) {
        return E_FAIL;
    }

    this->callback = callback_;
    return S_OK;
}

HRESULT DSSampleGrabber::SetSize(int width, int height)
{
    ZeroMemory(&this->mt, sizeof(CMediaType));

    VIDEOINFO *pvi = (VIDEOINFO *)this->mt.AllocFormatBuffer(sizeof(VIDEOINFO));
    if (NULL == pvi) {
        return E_OUTOFMEMORY;
    }

    ZeroMemory(pvi, sizeof(VIDEOINFO));

    pvi->bmiHeader.biCompression	= BI_RGB;
    pvi->bmiHeader.biBitCount		= 24;
    pvi->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
    pvi->bmiHeader.biWidth			= width;
    pvi->bmiHeader.biHeight			= height;
    pvi->bmiHeader.biPlanes			= 1;
    pvi->bmiHeader.biSizeImage		= GetBitmapSize(&pvi->bmiHeader);
    pvi->bmiHeader.biClrImportant	= 0;

    // Frame rate
    pvi->AvgTimePerFrame			= 10000000/this->m_outputFps;

    SetRectEmpty(&(pvi->rcSource));	// we want the whole image area rendered.
    SetRectEmpty(&(pvi->rcTarget));	// no particular destination rectangle

    this->mt.SetType(&MEDIATYPE_Video);
    this->mt.SetFormatType(&FORMAT_VideoInfo);
    this->mt.SetTemporalCompression(FALSE);

    this->mt.SetSubtype(&MEDIASUBTYPE_RGB24);
    this->mt.SetSampleSize(pvi->bmiHeader.biSizeImage);

    this->m_rgb24 = new BYTE[pvi->bmiHeader.biSizeImage];

    return S_OK;
}

#endif /* _WIN32_WCE */
