/*
* Copyright (C) 2010-2011 Mamadou Diop.
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
#include "internals/DSDisplay.h"
#include "internals/DSUtils.h"

#include "tsk_list.h"
#include "tsk_debug.h"

#include <string>

using namespace std;

#define USE_OVERLAY				0
#define OVERLAY_TIMEOUT			3
#define WM_GRAPHNOTIFY			WM_APP + 1

#define FSCREEN_MIN_IDEAL_WIDTH 352
#define FSCREEN_MIN_IDEAL_HEIGHT 288

typedef struct tdshow_display_s {
    TSK_DECLARE_OBJECT;

    HWND hwnd;
    DSDisplay* display;
}
tdshow_display_t;
typedef tsk_list_t tdshow_displays_L_t;
const tsk_object_def_t *tdshow_display_def_t;

// Static list to find which display is link to a given hWnd
static tdshow_displays_L_t* __directshow__Displays = tsk_null;

/*== Predicate function to find tdshow_display_t object by HWND. */
static int __pred_find_display_by_hwnd(const tsk_list_item_t *item, const void *hWnd)
{
    if(item && item->data) {
        const tdshow_display_t *display = (const tdshow_display_t *)item->data;
        int ret = 0;
        tsk_subsat_int32_ptr(display->hwnd, *((HWND*)hWnd), &ret);
        return ret;
    }
    return -1;
}

// C Callback that dispatch event to the right display
static LRESULT CALLBACK __directshow__WndProcWindow(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = FALSE;
    BOOL resultSet = FALSE;

    if(__directshow__Displays) {
        tsk_list_lock(__directshow__Displays);

        const tdshow_display_t *display =  (const tdshow_display_t *)tsk_list_find_object_by_pred(__directshow__Displays, __pred_find_display_by_hwnd, &hWnd);
        if((resultSet = (display && display->display))) {
            result = display->display->handleEvents(hWnd, uMsg, wParam, lParam);
        }

        tsk_list_unlock(__directshow__Displays);
    }

    return resultSet ? result : DefWindowProc(hWnd, uMsg, wParam, lParam);
}


DSDisplay::DSDisplay(HRESULT *hr)
{
    this->window = NULL;
    this->parentWindowProc = NULL;
    this->hooked = false;
    this->fullscreen = false;
    this->bPluginFirefox = false;
    this->top = 0;
    this->left = 0;
    this->width = this->imgWidth = 176;
    this->height = this->imgHeight = 144;
    this->fps = 15;

    this->graph = new DSDisplayGraph(hr);
    if (FAILED(*hr)) {
        return;
    }
#if USE_OVERLAY
    this->overlay = new DSDisplayOverlay();
#else
    this->overlay = NULL;
#endif

    this->graph->getVideoWindow()->put_Visible(OAFALSE);
}

DSDisplay::~DSDisplay()
{
    this->unhook();

    SAFE_DELETE_PTR(this->overlay);
    SAFE_DELETE_PTR(this->graph);
}

void DSDisplay::start()
{
    if (!this->graph->isRunning()) {
        this->hook();
    }
    if (!this->graph->isRunning() || this->graph->isPaused()) {
        this->graph->start();
    }
    this->graph->getVideoWindow()->put_Visible(OATRUE);
}

void DSDisplay::pause()
{
    this->graph->pause();
}

void DSDisplay::stop()
{
    if (this->graph->isRunning()) {
        this->setFullscreen(false);

        this->graph->stop();
        this->unhook();
    }
}

void DSDisplay::attach(INT64 parent)
{
    this->attach((void*)parent);
}

void DSDisplay::attach(void *parent)
{
    // Don't reattach if this is the same parent
    if (this->isAttached() && parent) {
        HWND hwnd = reinterpret_cast<HWND>(parent);
        if (hwnd != this->window) {
            this->detach();
        }
    }

    // Gets the handle of the parent
    this->window = reinterpret_cast<HWND>(parent);
    // Hook to the parent WindowProc
    this->hook();

#if USE_OVERLAY
    // Allows the overlay to initialize
    this->overlay->attach(this->window, this->graph);
#endif
}

void DSDisplay::detach(void *parent)
{
    // The detach action is only valid and if this is the same parent
    if (parent) {
        HWND hwnd = reinterpret_cast<HWND>(parent);
        if (hwnd == this->window) {
            this->detach();
        }
    }
}

void DSDisplay::detach()
{
    if (!this->isAttached()) {
        return;
    }

#if USE_OVERLAY
    // Clean up overlay
    this->overlay->detach();
#endif

    // Unhook from the parent WindowProc
    this->unhook();

    // Set the handle of the parent to NULL
    this->window = NULL;
}

bool DSDisplay::isAttached()
{
    return (this->window != NULL);
}

int DSDisplay::getWidth()
{
    return this->width;
}

int DSDisplay::getHeight()
{
    return this->height;
}

void DSDisplay::setSize(int w, int h)
{
    //this->width = w;
    //this->height = h;

    if (!this->fullscreen) {
        this->graph->setImageFormat(w, h);
        if(this->hooked) {
#if 0
#if defined(VMR9_WINDOWLESS)
            RECT rc;
            SetRect(&rc, 0, 0, w, h);
            this->graph->getWindowlessControl()->SetVideoPosition(&rc, &rc);
#else
            this->graph->getVideoWindow()->SetWindowPosition(0, 0, this->width , this->height);
#endif
#endif
        }
    }
}

void DSDisplay::applyRatio(RECT rect)
{
    long w = rect.right - rect.left;
    long h = rect.bottom - rect.top;
    float ratio = ((float)this->imgWidth/(float)this->imgHeight);
    // (w/h)=ratio =>
    // 1) h=w/ratio
    // and
    // 2) w=h*ratio
    this->width = (int)(w/ratio) > h ? (int)(h * ratio) : w;
    this->height = (int)(this->width/ratio) > h ? h : (int)(this->width/ratio);
    this->left = ((w - this->width) >> 1);
    this->top = ((h - this->height) >> 1);
}

bool DSDisplay::isFullscreen()
{
#if defined(VMR9_WINDOWLESS)
    // TODO
#else
    long result;
    HRESULT hr = this->graph->getVideoWindow()->get_FullScreenMode(&result);
    if (SUCCEEDED(hr)) {
        this->fullscreen = (result == OATRUE);
    }
    else {
        TSK_DEBUG_ERROR("get_FullScreenMode failed with %ld", hr);
        this->fullscreen = FALSE;
    }
#endif
    return this->fullscreen;
}

void DSDisplay::setFullscreen(bool value)
{
    if(!this->canFullscreen()) {
        TSK_DEBUG_WARN("Cannot fullscreen");
        return;
    }

    HRESULT hr;

#if defined(VMR9_WINDOWLESS)
    // TODO
#else
    if (this->isFullscreen() == value) {
        return;
    }

    hr = this->graph->getVideoWindow()->put_FullScreenMode(value ? OATRUE : OAFALSE);
    if (SUCCEEDED(hr)) {
        this->fullscreen = value;
#if USE_OVERLAY
        this->overlay->show(this->fullscreen ? (OVERLAY_TIMEOUT * this->graph->getDisplayFps()) : 0);
#endif
    }
    else {
        TSK_DEBUG_ERROR("put_FullScreenMode failed with %ld", hr);
    }
#endif
}

void DSDisplay::setPluginFirefox(bool value)
{
    bPluginFirefox = value;
}

bool DSDisplay::canFullscreen()
{
#if defined(VMR9_WINDOWLESS)
    // TODO
#else
    if(this->graph) {
        UINT image_w, image_h;

        if( this->graph->getImageFormat(image_w, image_h) ) {
            //this->graph->getVideoWindow()->GetMinIdealImageSize(&ideal_w, &ideal_h);
            return (((long)image_w >= FSCREEN_MIN_IDEAL_WIDTH) && ((long)image_h >= FSCREEN_MIN_IDEAL_HEIGHT));
        }
    }
#endif
    return false;
}

void DSDisplay::setFps(int fps_)
{
    this->fps = fps_;
    this->graph->setDisplayFps(fps_);
}


// w and h are the size of the buffer not the display
void DSDisplay::handleVideoFrame(const void* data, int w, int h)
{
    if (this->graph->isRunning()) {
        // The graph will take care of changing the source filter if needed
        // in case of dimension change or anything else...
        this->graph->handleFrame(data, w, h);
        if(this->imgWidth != w || this->imgHeight != h) {
            this->imgWidth = w;
            this->imgHeight = h;
            if(this->window) {
                SendMessage(this->window, WM_SIZE, SIZE_RESTORED, MAKELPARAM(this->width , this->height));
            }
        }
#if USE_OVERLAY
        this->overlay->update();
#endif
    }
}

LRESULT DSDisplay::handleEvents(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg) {
    case WM_CREATE:
    case WM_SIZE:
    case WM_MOVE: {
        RECT rect = {0};
        GetWindowRect(hWnd, &rect);
        applyRatio(rect);

#if defined(VMR9_WINDOWLESS)
        this->graph->getWindowlessControl()->SetVideoPosition(&rect, &rect);
#else
        this->graph->getVideoWindow()->SetWindowPosition(this->left, this->top, this->width , this->height);
#endif
    }
    break;

    case WM_LBUTTONDBLCLK:
        if(this->canFullscreen()) {
            this->setFullscreen(true);
        }
        break;

    case WM_FULLSCREEN_SET:
        if(this->canFullscreen()) {
            this->setFullscreen(!this->isFullscreen());
        }
        break;

    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_KEYDOWN:
        if(this->isFullscreen()) {
#if USE_OVERLAY
            // Re-Show overlay
            this->overlay->show(OVERLAY_TIMEOUT * this->graph->getDisplayFps());
#endif
        }
        break;

    case WM_CHAR:
    case WM_KEYUP:
        if(this->isFullscreen() && (wParam == 0x1B || wParam == VK_ESCAPE)) {
            // escape
            this->setFullscreen(false);
        }

        break;

    case WM_GRAPHNOTIFY: {
        long evCode;
        LONG_PTR param1, param2;
        HRESULT hr;
        while (hr = this->graph->getMediaEvent()->GetEvent(&evCode, &param1, &param2, 0), SUCCEEDED(hr)) {
            hr = this->graph->getMediaEvent()->FreeEventParams(evCode, param1, param2);

            switch(evCode) {
            case EC_FULLSCREEN_LOST:
#if USE_OVERLAY
                this->overlay->show(0);
#endif
                break;
            case EC_COMPLETE:
            case EC_USERABORT:
            default:
                break;
            }
        }
    }
    break;

#if defined(VMR9_WINDOWLESS)
    case WM_DISPLAYCHANGE: {
        this->graph->getWindowlessControl()->DisplayModeChanged();
    }
    break;
    case WM_PAINT: {
        RECT rect = {0};
        GetWindowRect(hWnd, &rect);

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        this->graph->getWindowlessControl()->RepaintVideo(hWnd, hdc);

        EndPaint(hWnd, &ps);
    }
    break;
#endif

    }

    return bPluginFirefox ? DefWindowProc(hWnd, uMsg, wParam, lParam) : CallWindowProc(this->parentWindowProc, hWnd, uMsg, wParam, lParam);
}

void DSDisplay::hook()
{
    HRESULT hr;

    if (!this->window) {
        return;
    }

    if(this->hooked) {
        return;
    }
    this->hooked = TRUE;

    bool lock = (__directshow__Displays != NULL);

    if(lock) {
        tsk_list_lock(__directshow__Displays);
    }
    {
        // Gets the parent Window procedure
#if defined(_WIN32_WCE)
        // Workaround for bug in SetWindowLong, call twice the API
        //this->parentWindowProc = (WNDPROC)SetWindowLong( this->window, GWL_WNDPROC, (LONG) __directshow__WndProcWindow );
        //this->parentWindowProc = (WNDPROC)SetWindowLong( this->window, GWL_WNDPROC, (LONG) __directshow__WndProcWindow );
        //__directshow__Displays[this->window] = this;
#else
        this->parentWindowProc = (WNDPROC) SetWindowLongPtr(this->window, GWLP_WNDPROC, (LONG_PTR) __directshow__WndProcWindow);
        // Add this instance to the callback map
        tsk_object_new(tdshow_display_def_t, this->window, this);
#endif
    }
    if(lock) {
        tsk_list_unlock(__directshow__Displays);
    }

    RECT rect;
    GetWindowRect(this->window, &rect);
    applyRatio(rect);

#if defined(VMR9_WINDOWLESS)
    rect.left = 0;
    rect.top = 0;
    rect.right = this->width;
    rect.bottom = this->height;

    // TODO : Review
    hr = this->graph->getWindowlessControl()->SetVideoClippingWindow(this->window);
    hr = this->graph->getWindowlessControl()->SetBorderColor(RGB(0, 0, 128));
    hr = this->graph->getWindowlessControl()->SetVideoPosition(NULL, &rect);
#else
    // TODO : Review the order
    hr = this->graph->getVideoWindow()->put_Owner((OAHWND) this->window);
    hr = this->graph->getVideoWindow()->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    hr = this->graph->getVideoWindow()->SetWindowPosition(this->left, this->top, this->width, this->height);
    hr = this->graph->getVideoWindow()->put_MessageDrain((OAHWND) this->window);
    hr = this->graph->getVideoWindow()->put_Visible(OATRUE);
#endif

    hr = this->graph->getMediaEvent()->SetNotifyWindow((OAHWND) this->window, WM_GRAPHNOTIFY, 0);
}

void DSDisplay::unhook()
{
    HRESULT hr;

    if(!this->window) {
        return;
    }

    if(!this->hooked) {
        return;
    }

    hr = this->graph->getMediaEvent()->SetNotifyWindow(NULL, WM_GRAPHNOTIFY, 0);

#if defined(VMR9_WINDOWLESS)
    // TODO : Review
    hr = this->graph->getWindowlessControl()->SetVideoClippingWindow(NULL);
#else
    // TODO : Review the order
    hr = this->graph->getVideoWindow()->put_Visible(OAFALSE);
    hr = this->graph->getVideoWindow()->put_MessageDrain((OAHWND) NULL);
    hr = this->graph->getVideoWindow()->put_Owner((OAHWND) NULL);
    hr = this->graph->getVideoWindow()->put_AutoShow(OAFALSE);
#endif

    bool lock = (__directshow__Displays != NULL);
    if(lock) {
        tsk_list_lock(__directshow__Displays);
    }
    {
        // Remove this instance from the callback map
        tsk_list_remove_item_by_pred(__directshow__Displays, __pred_find_display_by_hwnd, &this->window);
        // Restore parent Window procedure
#if defined(_WIN32_WCE)
        // Workaround for bug in SetWindowLong, call twice the API
        //this->parentWindowProc = (WNDPROC)SetWindowLong( this->window, GWL_WNDPROC, (LONG) this->parentWindowProc );
        //this->parentWindowProc = (WNDPROC)SetWindowLong( this->window, GWL_WNDPROC, (LONG) this->parentWindowProc );
#else
        SetWindowLongPtr(this->window, GWLP_WNDPROC, (LONG_PTR) this->parentWindowProc);
#endif
    }
    if(lock) {
        tsk_list_unlock(__directshow__Displays);
    }

    this->hooked = FALSE;
}











//=================================================================================================
//	String object definition
//
static tsk_object_t* tdshow_display_ctor(tsk_object_t * self, va_list * app)
{
    tdshow_display_t *display = (tdshow_display_t *)self;

    if(display) {
        display->hwnd = va_arg(*app, HWND);
        display->display = va_arg(*app, DSDisplay*);

        if(!__directshow__Displays) {
            __directshow__Displays = tsk_list_create();
        }
        tsk_list_push_back_data(__directshow__Displays, (void**)&display);
    }

    return self;
}

static tsk_object_t* tdshow_display_dtor(tsk_object_t * self)
{
    tdshow_display_t *display = (tdshow_display_t *)self;
    if(display) {
        if(__directshow__Displays) {
            tsk_list_remove_item_by_data(__directshow__Displays, display);
            //if(TSK_LIST_IS_EMPTY(__directshow__Displays)){
            //	TSK_OBJECT_SAFE_FREE(__directshow__Displays);
            //}
        }
    }

    return self;
}

static int tdshow_display_cmp(const tsk_object_t *_d1, const tsk_object_t *_d2)
{
    const tdshow_display_t *d1 = (const tdshow_display_t *)_d1;
    const tdshow_display_t *d2 = (const tdshow_display_t *)_d2;

    if(d1 && d2) {
        int ret = 0;
        tsk_subsat_int32_ptr(d1->hwnd, d2->hwnd, &ret);
        return ret;
    }
    else if(!d1 && !d2) {
        return 0;
    }
    else {
        return -1;
    }
}

static const tsk_object_def_t tdshow_display_def_s = {
    sizeof(tdshow_display_t),
    tdshow_display_ctor,
    tdshow_display_dtor,
    tdshow_display_cmp,
};
extern const tsk_object_def_t *tdshow_display_def_t = &tdshow_display_def_s;
