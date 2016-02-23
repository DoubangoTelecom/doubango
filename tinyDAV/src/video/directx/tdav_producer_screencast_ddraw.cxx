/* Copyright (C) 2015 Mamadou DIOP.
*  Copyright (C) 2015 Doubango Telecom.
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
#include "tinydav/video/directx/tdav_producer_screencast_ddraw.h"

#if TDAV_UNDER_WINDOWS && !TDAV_UNDER_WINDOWS_RT

#include <windows.h>
#include <ddraw.h>

#include "tsk_thread.h"
#include "tsk_memory.h"
#include "tsk_safeobj.h"
#include "tsk_timer.h"
#include "tsk_time.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#if defined(_MSC_VER)
#	define DDRAW_HAVE_RGB32_TO_I420					1
#	if !TDAV_UNDER_WINDOWS_CE
#		define DDRAW_HAVE_RGB32_TO_I420_INTRIN		1
#		include <intrin.h>
#	endif /* TDAV_UNDER_WINDOWS_CE */
#	if !defined(_M_X64) /*|| _MSC_VER <= 1500*/  // https://msdn.microsoft.com/en-us/library/4ks26t93.aspx: Inline assembly is not supported on the ARM and x64 processors (1500 = VS2008)
#		define DDRAW_HAVE_RGB32_TO_I420_ASM			1
#	endif
#endif /* _MSC_VER */

#if !defined(DDRAW_MEM_ALIGNMENT)
#	define DDRAW_MEM_ALIGNMENT	16 // SSE = 16, AVX = 32. Should be 16.
#endif /* DDRAW_MEM_ALIGNMENT */

#if !defined(DDRAW_IS_ALIGNED)
#	define DDRAW_IS_ALIGNED(p, a) (!((uintptr_t)(p) & ((a) - 1)))
#endif /* DDRAW_IS_ALIGNED */

#if !defined(DDRAW_HIGH_PRIO_MEMCPY)
#	define DDRAW_HIGH_PRIO_MEMCPY	0 // BOOL
#endif /* DDRAW_HIGH_PRIO_MEMCPY */

#if !defined(DDRAW_CPU_MONITOR)
#	define DDRAW_CPU_MONITOR	0 // BOOL
#endif /* DDRAW_CPU_MONITOR */

#if !defined(DDRAW_CPU_THROTTLING)
#	define DDRAW_CPU_THROTTLING		0 // BOOL
#endif /* DDRAW_CPU_THROTTLING */

#if (DDRAW_CPU_MONITOR || DDRAW_CPU_THROTTLING) && !defined(DDRAW_CPU_SCHEDULE_TIMEOUT)
#	define DDRAW_CPU_SCHEDULE_TIMEOUT	800 // millis
#endif /* DDRAW_CPU_MONITOR */

#if defined(DDRAW_CPU_THROTTLING) && !defined(DDRAW_CPU_THROTTLING_FPS_MIN)
#	define DDRAW_CPU_THROTTLING_FPS_MIN	1 // frames per second
#endif /* DDRAW_CPU_THROTTLING_FPS_MIN */

#if defined(DDRAW_CPU_THROTTLING) && !defined(DDRAW_CPU_THROTTLING_THRESHOLD)
#	define DDRAW_CPU_THROTTLING_THRESHOLD	70 // percent
#endif /* DDRAW_CPU_THROTTLING_THRESHOLD */

#if defined(DDRAW_CPU_THROTTLING) && !defined(DDRAW_CPU_THROTTLING_THRESHOLD_MARGIN)
#	define DDRAW_CPU_THROTTLING_THRESHOLD_MARGIN	5 // percent
#endif /* DDRAW_CPU_THROTTLING_THRESHOLD_MARGIN */

#if !defined(DDRAW_MT)
#	define DDRAW_MT	1 // BOOL: Multi-threading
#endif /* DDRAW_MT */

#if defined (DDRAW_MT) && !defined(DDRAW_MT_COUNT)
#	define DDRAW_MT_COUNT 3 // Number of buffers to use
#endif /* DDRAW_MT_COUNT */

#if defined(DDRAW_MT_COUNT)
#	define DDRAW_MT_EVENT_SHUTDOWN_INDEX	DDRAW_MT_COUNT
#endif

#if !defined(DDRAW_MEM_SURFACE_DIRECT_ACCESS)
#	define DDRAW_MEM_SURFACE_DIRECT_ACCESS	0 // direct access to "ddsd.lpSurface" is very slow even if the memory is correctly aligned: to be investigated
#endif /* DDRAW_MEM_SURFACE_DIRECT_ACCESS */

#if !defined(DDRAW_PREVIEW)
#	if TDAV_UNDER_WINDOWS_CE && (BUILD_TYPE_GE || SIN_CITY)
#		define DDRAW_PREVIEW 0 // Do not waste time displaying the preview on "WEC7 + (GE | SINCITY)"
#	else
#		define DDRAW_PREVIEW 1
#	endif
#endif

#define DDRAW_DEBUG_INFO(FMT, ...) TSK_DEBUG_INFO("[DDRAW Producer] " FMT, ##__VA_ARGS__)
#define DDRAW_DEBUG_WARN(FMT, ...) TSK_DEBUG_WARN("[DDRAW Producer] " FMT, ##__VA_ARGS__)
#define DDRAW_DEBUG_ERROR(FMT, ...) TSK_DEBUG_ERROR("[DDRAW Producer] " FMT, ##__VA_ARGS__)
#define DDRAW_DEBUG_FATAL(FMT, ...) TSK_DEBUG_FATAL("[DDRAW Producer] " FMT, ##__VA_ARGS__)

#define DDRAW_SAFE_RELEASE(pp) if ((pp) && *(pp)) (*(pp))->Release(), *(pp) = NULL
#define DDRAW_CHECK_HR(x) { HRESULT __hr__ = (x); if (FAILED(__hr__)) { DDRAW_DEBUG_ERROR("Operation Failed (%08x)", __hr__); goto bail; } }

typedef struct DDrawModule {
    LPDIRECTDRAW lpDD;
    HMODULE hDLL;
} DDrawModule;
typedef struct DDrawModule FAR *LPDDrawModule;
#define DDrawModuleSafeFree(module) DDRAW_SAFE_RELEASE(&module.lpDD); if (module.hDLL) { FreeLibrary(module.hDLL), module.hDLL = NULL; }

typedef struct tdav_producer_screencast_ddraw_s {
    TMEDIA_DECLARE_PRODUCER;

    HWND hwnd_preview;
    HWND hwnd_src;
#if DDRAW_PREVIEW
    BITMAPINFO bi_preview;
#endif /* DDRAW_PREVIEW */

#if DDRAW_CPU_MONITOR || DDRAW_CPU_THROTTLING
    tsk_timer_manager_handle_t *p_timer_mgr;
    struct {
        tsk_timer_id_t id_timer;
        int fps_target;
    } cpu;
#endif /* DDRAW_CPU_MONITOR || DDRAW_CPU_THROTTLING */

#if DDRAW_MT
    struct {
        tsk_thread_handle_t* tid[1];
        void* p_buff_yuv_aligned_array[DDRAW_MT_COUNT];
        BOOL b_flags_array[DDRAW_MT_COUNT];
        HANDLE h_events[DDRAW_MT_COUNT + 1]; // #DDRAW_MT_COUNT events for each buffer plus #1 for the shutdown/stop
    } mt;
#endif /* DDRAW_MT */

    DDrawModule ddrawModule;
    IDirectDrawSurface* p_surf_primary;

    tsk_thread_handle_t* tid[1];

    void* p_buff_rgb_aligned;
    tsk_size_t n_buff_rgb;
    tsk_size_t n_buff_rgb_bitscount;

    void* p_buff_yuv_aligned;
    tsk_size_t n_buff_yuv;

    BOOL b_have_rgb32_conv; // support for RGB32 -> I420 and primary screen format is RGB32

    tsk_bool_t b_started;
    tsk_bool_t b_paused;
    tsk_bool_t b_muted;

    TSK_DECLARE_SAFEOBJ;
}
tdav_producer_screencast_ddraw_t;

static BOOL _tdav_producer_screencast_have_ssse3();
static tmedia_chroma_t _tdav_producer_screencast_get_chroma(const DDPIXELFORMAT* pixelFormat);
static void* TSK_STDCALL _tdav_producer_screencast_grap_thread(void *arg);
#if DDRAW_MT
static void* TSK_STDCALL _tdav_producer_screencast_mt_encode_thread(void *arg);
#endif /* DDRAW_MT */
static int _tdav_producer_screencast_timer_cb(const void* arg, tsk_timer_id_t timer_id);
static int _tdav_producer_screencast_grab(tdav_producer_screencast_ddraw_t* p_self);
static HRESULT _tdav_producer_screencast_create_module(LPDDrawModule lpModule);
static HRESULT _tdav_producer_screencast_alloc_rgb_buff(tdav_producer_screencast_ddraw_t* p_self, DWORD w, DWORD h, DWORD bitsCount);
static HRESULT _tdav_producer_screencast_alloc_yuv_buff(tdav_producer_screencast_ddraw_t* p_self, DWORD w, DWORD h);

#if DDRAW_HAVE_RGB32_TO_I420_INTRIN || DDRAW_HAVE_RGB32_TO_I420_ASM
static __declspec(align(DDRAW_MEM_ALIGNMENT)) const int8_t kYCoeffs[16] = {
    13, 65, 33, 0,
    13, 65, 33, 0,
    13, 65, 33, 0,
    13, 65, 33, 0,
};
static __declspec(align(DDRAW_MEM_ALIGNMENT)) const int8_t kUCoeffs[16] = {
    112, -74, -38, 0,
    112, -74, -38, 0,
    112, -74, -38, 0,
    112, -74, -38, 0,
};
static __declspec(align(DDRAW_MEM_ALIGNMENT)) const int8_t kVCoeffs[16] = {
    -18, -94, 112, 0,
    -18, -94, 112, 0,
    -18, -94, 112, 0,
    -18, -94, 112, 0,
};
static __declspec(align(DDRAW_MEM_ALIGNMENT)) const  int32_t kRGBAShuffleDuplicate[4] = { 0x03020100, 0x0b0a0908, 0x03020100, 0x0b0a0908 }; // RGBA(X) || RGBA(X + 2) || RGBA(X) || RGBA(X + 2) = 2U || 2V
static __declspec(align(DDRAW_MEM_ALIGNMENT)) const uint16_t kY16[8] = {
    16, 16, 16, 16,
    16, 16, 16, 16
};
static __declspec(align(DDRAW_MEM_ALIGNMENT)) const uint16_t kUV128[8] = {
    128, 128, 128, 128,
    128, 128, 128, 128
};
#endif /* DDRAW_HAVE_RGB32_TO_I420_INTRIN || DDRAW_HAVE_RGB32_TO_I420_ASM */

// public function used to check that we can use DDRAW plugin before loading it
tsk_bool_t tdav_producer_screencast_ddraw_plugin_is_supported()
{
    static tsk_bool_t __checked = tsk_false; // static guard to avoid checking more than once
    static tsk_bool_t __supported = tsk_false;

    HRESULT hr = DD_OK;
    DDSURFACEDESC ddsd;
    DDPIXELFORMAT DDPixelFormat;
    LPDIRECTDRAWSURFACE lpDDS = NULL;
    DDrawModule ddrawModule = { 0 };

    if (__checked) {
        goto bail;
    }

    __checked = tsk_true;

    DDRAW_CHECK_HR(hr = _tdav_producer_screencast_create_module(&ddrawModule));
    DDRAW_CHECK_HR(hr = ddrawModule.lpDD->SetCooperativeLevel(NULL, DDSCL_NORMAL));

    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    DDRAW_CHECK_HR(hr = ddrawModule.lpDD->CreateSurface(&ddsd, &lpDDS, NULL));

    ZeroMemory(&DDPixelFormat, sizeof(DDPixelFormat));
    DDPixelFormat.dwSize = sizeof(DDPixelFormat);
    DDRAW_CHECK_HR(hr = lpDDS->GetPixelFormat(&DDPixelFormat));
    DDRAW_DEBUG_INFO("dwRGBBitCount:%d, dwRBitMask:%x, dwGBitMask:%x, dwBBitMask:%x, dwRGBAlphaBitMask:%x",
                     DDPixelFormat.dwRGBBitCount, DDPixelFormat.dwRBitMask, DDPixelFormat.dwGBitMask, DDPixelFormat.dwBBitMask, DDPixelFormat.dwRGBAlphaBitMask);
    if (_tdav_producer_screencast_get_chroma(&DDPixelFormat) == tmedia_chroma_none) {
        DDRAW_CHECK_HR(hr = DDERR_INVALIDCAPS);
    }

    __supported = SUCCEEDED(hr);

bail:
    DDRAW_SAFE_RELEASE(&lpDDS);
    DDrawModuleSafeFree(ddrawModule);
    return __supported;
}

static BOOL _tdav_producer_screencast_have_ssse3()
{
    static BOOL __checked = FALSE; // static guard to avoid checking more than once
    static BOOL __supported = FALSE;

    if (__checked) {
        return __supported;
    }
    __checked = TRUE;

#ifndef BIT
#	define BIT(n) (1<<n)
#endif /*BIT*/
#if DDRAW_HAVE_RGB32_TO_I420_ASM
#define cpuid(func, func2, a, b, c, d)\
	__asm mov eax, func\
	__asm mov ecx, func2\
	__asm cpuid\
	__asm mov a, eax\
	__asm mov b, ebx\
	__asm mov c, ecx\
	__asm mov d, edx

#define HAS_MMX     0x01
#define HAS_SSE     0x02
#define HAS_SSE2    0x04
#define HAS_SSE3    0x08
#define HAS_SSSE3   0x10
#define HAS_SSE4_1  0x20
#define HAS_AVX     0x40
#define HAS_AVX2    0x80

    unsigned int reg_eax, reg_ebx, reg_ecx, reg_edx;
    cpuid(0, 0, reg_eax, reg_ebx, reg_ecx, reg_edx);
    if (reg_eax < 1) {
        DDRAW_DEBUG_ERROR("reg_eax < 1");
        return FALSE;
    }
    cpuid(1, 0, reg_eax, reg_ebx, reg_ecx, reg_edx);
    __supported = (reg_ecx & BIT(9)) ? TRUE : FALSE;
#elif DDRAW_HAVE_RGB32_TO_I420_INTRIN
    int cpu_info[4] = { 0 }, num_ids;
    __cpuid(cpu_info, 0);
    num_ids = cpu_info[0];
    __cpuid(cpu_info, 0x80000000);
    if (num_ids > 0) {
        __cpuid(cpu_info, 0x00000001);
        __supported = (cpu_info[2] & BIT(9)) ? TRUE : FALSE;
    }
#endif /* DDRAW_HAVE_RGB32_TO_I420_ASM */

    DDRAW_DEBUG_INFO("SSSE3 supported = %s", __supported ? "YES" : "NO");

    return __supported;
}

#if DDRAW_HAVE_RGB32_TO_I420_INTRIN

#define DDRAW_COPY16_INTRIN(dst, src) \
	_mm_store_si128((__m128i*)dst, _mm_load_si128((__m128i*)src))
#define DDRAW_COPY64_INTRIN(dst, src) \
	_mm_store_si128((__m128i*)dst, _mm_load_si128((__m128i*)src)); \
	_mm_store_si128((__m128i*)&dst[16], _mm_load_si128((__m128i*)&src[16])); \
	_mm_store_si128((__m128i*)&dst[32], _mm_load_si128((__m128i*)&src[32])); \
	_mm_store_si128((__m128i*)&dst[48], _mm_load_si128((__m128i*)&src[48]))
#define DDRAW_COPY128_INTRIN(dst, src) \
	DDRAW_COPY64_INTRIN(dst, src); \
	_mm_store_si128((__m128i*)&dst[64], _mm_load_si128((__m128i*)&src[64])); \
	_mm_store_si128((__m128i*)&dst[80], _mm_load_si128((__m128i*)&src[80])); \
	_mm_store_si128((__m128i*)&dst[96], _mm_load_si128((__m128i*)&src[96])); \
	_mm_store_si128((__m128i*)&dst[112], _mm_load_si128((__m128i*)&src[112]))

static void _tdav_producer_screencast_rgb32_to_yuv420_intrin_ssse3(uint8_t *yuvPtr, const uint8_t *rgbPtr, int width, int height)
{
    // rgbPtr contains (samplesCount * 16) bytes
    // yPtr contains samplesCount bytes
    const int samplesCount = (width * height); // "width" and "height" are in samples
    const uint8_t *rgbPtr_;
    uint8_t* yPtr_ = yuvPtr, *uPtr_ = (yPtr_ + samplesCount), *vPtr_ = uPtr_ + (samplesCount >> 2);
    __m128i mmRgb0, mmRgb1, mmRgb2, mmRgb3, mmY0, mmY1, mmY;
    __m128i mmRgbU0, mmRgbU1, mmRgbV0, mmRgbV1;

    // Convert 16 RGBA samples to 16 Y samples
    rgbPtr_ = rgbPtr;
    /* const */__m128i yCoeffs = _mm_load_si128((__m128i*)kYCoeffs);
    /* const */__m128i y16 = _mm_load_si128((__m128i*)kY16);
    for(int i = 0; i < samplesCount; i += 16) {
        // load 16 RGBA samples
        _mm_store_si128(&mmRgb0, _mm_load_si128((__m128i*)rgbPtr_)); // 4 RGBA samples
        _mm_store_si128(&mmRgb1, _mm_load_si128((__m128i*)&rgbPtr_[16])); // 4 RGBA samples
        _mm_store_si128(&mmRgb2, _mm_load_si128((__m128i*)&rgbPtr_[32])); // 4 RGBA samples
        _mm_store_si128(&mmRgb3, _mm_load_si128((__m128i*)&rgbPtr_[48])); // 4 RGBA samples

        _mm_store_si128(&mmRgb0, _mm_maddubs_epi16(mmRgb0/*unsigned*/, yCoeffs/*signed*/)); // mmRgb0 = ((yCoeffs[j] * mmRgb0[j]) +  (yCoeffs[j + 1] * mmRgb0[j + 1]))
        _mm_store_si128(&mmRgb1, _mm_maddubs_epi16(mmRgb1/*unsigned*/, yCoeffs/*signed*/));
        _mm_store_si128(&mmRgb2, _mm_maddubs_epi16(mmRgb2/*unsigned*/, yCoeffs/*signed*/));
        _mm_store_si128(&mmRgb3, _mm_maddubs_epi16(mmRgb3/*unsigned*/, yCoeffs/*signed*/));

        _mm_store_si128(&mmY0, _mm_hadd_epi16(mmRgb0, mmRgb1)); // horizontal add
        _mm_store_si128(&mmY1, _mm_hadd_epi16(mmRgb2, mmRgb3));

        _mm_store_si128(&mmY0, _mm_srai_epi16(mmY0, 7)); // >> 7
        _mm_store_si128(&mmY1, _mm_srai_epi16(mmY1, 7));

        _mm_store_si128(&mmY0, _mm_add_epi16(mmY0, y16)); // + 16
        _mm_store_si128(&mmY1, _mm_add_epi16(mmY1, y16));

        _mm_store_si128(&mmY, _mm_packus_epi16(mmY0, mmY1)); // Saturate(I16 -> U8)

        _mm_store_si128((__m128i*)yPtr_, mmY);

        rgbPtr_ += 64; // 16samples * 4bytes
        yPtr_ += 16; // 16samples * 1byte
    }

    // U+V planes
    /* const */__m128i uCoeffs = _mm_load_si128((__m128i*)kUCoeffs);
    /* const */__m128i vCoeffs = _mm_load_si128((__m128i*)kVCoeffs);
    /* const */__m128i rgbaShuffleDuplicate = _mm_load_si128((__m128i*)kRGBAShuffleDuplicate);
    /* const */__m128i uv128 = _mm_load_si128((__m128i*)kUV128);
    rgbPtr_ = rgbPtr;
    for(int i = 0; i < samplesCount; ) {
        // load 16 RGBA samples
        _mm_store_si128(&mmRgb0, _mm_load_si128((__m128i*)rgbPtr_)); // 4 RGBA samples
        _mm_store_si128(&mmRgb1, _mm_load_si128((__m128i*)&rgbPtr_[16])); // 4 RGBA samples
        _mm_store_si128(&mmRgb2, _mm_load_si128((__m128i*)&rgbPtr_[32])); // 4 RGBA samples
        _mm_store_si128(&mmRgb3, _mm_load_si128((__m128i*)&rgbPtr_[48])); // 4 RGBA samples

        _mm_store_si128(&mmRgb0, _mm_shuffle_epi8(mmRgb0, rgbaShuffleDuplicate));
        _mm_store_si128(&mmRgb1, _mm_shuffle_epi8(mmRgb1, rgbaShuffleDuplicate));
        _mm_store_si128(&mmRgb2, _mm_shuffle_epi8(mmRgb2, rgbaShuffleDuplicate));
        _mm_store_si128(&mmRgb3, _mm_shuffle_epi8(mmRgb3, rgbaShuffleDuplicate));

        _mm_store_si128(&mmRgbU0, _mm_unpacklo_epi64(mmRgb0, mmRgb1));
        _mm_store_si128(&mmRgbV0, _mm_unpackhi_epi64(mmRgb0, mmRgb1)); // same as mmRgbU0: Use _mm_store_si128??
        _mm_store_si128(&mmRgbU1, _mm_unpacklo_epi64(mmRgb2, mmRgb3));
        _mm_store_si128(&mmRgbV1, _mm_unpackhi_epi64(mmRgb2, mmRgb3)); // same as mmRgbU0: Use _mm_store_si128??

        _mm_store_si128(&mmRgbU0, _mm_maddubs_epi16(mmRgbU0/*unsigned*/, uCoeffs/*signed*/));
        _mm_store_si128(&mmRgbV0, _mm_maddubs_epi16(mmRgbV0/*unsigned*/, vCoeffs/*signed*/));
        _mm_store_si128(&mmRgbU1, _mm_maddubs_epi16(mmRgbU1/*unsigned*/, uCoeffs/*signed*/));
        _mm_store_si128(&mmRgbV1, _mm_maddubs_epi16(mmRgbV1/*unsigned*/, vCoeffs/*signed*/));

        _mm_store_si128(&mmY0, _mm_hadd_epi16(mmRgbU0, mmRgbU1)); // horizontal add
        _mm_store_si128(&mmY1, _mm_hadd_epi16(mmRgbV0, mmRgbV1));

        _mm_store_si128(&mmY0, _mm_srai_epi16(mmY0, 8)); // >> 8
        _mm_store_si128(&mmY1, _mm_srai_epi16(mmY1, 8));

        _mm_store_si128(&mmY0, _mm_add_epi16(mmY0, uv128)); // + 128
        _mm_store_si128(&mmY1, _mm_add_epi16(mmY1, uv128));

        // Y contains 8 samples for U then 8 samples for V
        _mm_store_si128(&mmY, _mm_packus_epi16(mmY0, mmY1)); // Saturate(I16 -> U8)
        _mm_storel_pi((__m64*)uPtr_, _mm_load_ps((float*)&mmY));
        _mm_storeh_pi((__m64*)vPtr_, _mm_load_ps((float*)&mmY));

        uPtr_ += 8; // 8samples * 1byte
        vPtr_ += 8; // 8samples * 1byte

        // move to next 16 samples
        i += 16;
        rgbPtr_ += 64; // 16samples * 4bytes

        if (/*i % width == 0*/ !(i & (width - 1))) {
            // skip next line
            i += width;
            rgbPtr_ += (width * 4);
        }
    }
}
#endif /* DDRAW_HAVE_RGB32_TO_I420_INTRIN */

#if DDRAW_HAVE_RGB32_TO_I420_ASM

// __asm keyword must be duplicated in macro: https://msdn.microsoft.com/en-us/library/aa293825(v=vs.60).aspx
#define DDRAW_COPY16_ASM(dst, src) \
	__asm { \
	__asm mov eax, dword ptr [src] \
	__asm mov ecx, dword ptr [dst] \
	\
	__asm movdqa xmm0, xmmword ptr [eax] \
	__asm movdqa xmmword ptr [ecx], xmm0 \
	}
#define DDRAW_COPY64_ASM(dst, src) \
	__asm { \
	__asm mov eax, dword ptr [src] \
	__asm mov ecx, dword ptr [dst] \
	\
	__asm movdqa xmm0, xmmword ptr [eax] \
	__asm add eax, dword ptr 16 \
	__asm movdqa xmm1, xmmword ptr [eax] \
	__asm add eax, dword ptr 16 \
	__asm movdqa xmm2, xmmword ptr [eax] \
	__asm add eax, dword ptr 16 \
	__asm movdqa xmm3, xmmword ptr [eax] \
	 \
	__asm movdqa xmmword ptr [ecx], xmm0 \
	__asm add ecx, dword ptr 16 \
	__asm movdqa xmmword ptr [ecx], xmm1 \
	__asm add ecx, dword ptr 16 \
	__asm movdqa xmmword ptr [ecx], xmm2 \
	__asm add ecx, dword ptr 16 \
	__asm movdqa xmmword ptr [ecx], xmm3 \
	}
#define DDRAW_COPY128_ASM(dst, src) \
	__asm { \
	__asm mov eax, dword ptr [src] \
	__asm mov ecx, dword ptr [dst] \
	\
	__asm movdqa xmm0, xmmword ptr [eax] \
	__asm add eax, dword ptr 16 \
	__asm movdqa xmm1, xmmword ptr [eax] \
	__asm add eax, dword ptr 16 \
	__asm movdqa xmm2, xmmword ptr [eax] \
	__asm add eax, dword ptr 16 \
	__asm movdqa xmm3, xmmword ptr [eax] \
	__asm add eax, dword ptr 16 \
	__asm movdqa xmm4, xmmword ptr [eax] \
	__asm add eax, dword ptr 16 \
	__asm movdqa xmm5, xmmword ptr [eax] \
	__asm add eax, dword ptr 16 \
	__asm movdqa xmm6, xmmword ptr [eax] \
	__asm add eax, dword ptr 16 \
	__asm movdqa xmm7, xmmword ptr [eax] \
	 \
	__asm movdqa xmmword ptr [ecx], xmm0 \
	__asm add ecx, dword ptr 16 \
	__asm movdqa xmmword ptr [ecx], xmm1 \
	__asm add ecx, dword ptr 16 \
	__asm movdqa xmmword ptr [ecx], xmm2 \
	__asm add ecx, dword ptr 16 \
	__asm movdqa xmmword ptr [ecx], xmm3 \
	__asm add ecx, dword ptr 16 \
	__asm movdqa xmmword ptr [ecx], xmm4 \
	__asm add ecx, dword ptr 16 \
	__asm movdqa xmmword ptr [ecx], xmm5 \
	__asm add ecx, dword ptr 16 \
	__asm movdqa xmmword ptr [ecx], xmm6 \
	__asm add ecx, dword ptr 16 \
	__asm movdqa xmmword ptr [ecx], xmm7 \
	}

__declspec(naked) __declspec(align(DDRAW_MEM_ALIGNMENT))
static void _tdav_producer_screencast_rgb32_to_yuv420_asm_ssse3(uint8_t *yuvPtr, const uint8_t *rgbPtr, int width, int height)
{
    __asm {
        push esi
        push edi
        push ebx
        /*** Y Samples ***/
        mov edx, [esp + 12 + 4]   // yuvPtr
        mov eax, [esp + 12 + 8]   // rgbPtr
        mov ecx, [esp + 12 + 12] // width
        imul ecx, [esp + 12 + 16] // (width * height) = samplesCount

        movdqa xmm7, kYCoeffs // yCoeffs
        movdqa xmm6, kY16 // y16
        /* loopY start */
        loopY:
        // load 16 RGBA samples
        movdqa xmm0, [eax] // mmRgb0
        movdqa xmm1, [eax + 16] // mmRgb1
        movdqa xmm2, [eax + 32] // mmRgb2
        movdqa xmm3, [eax + 48] // mmRgb3
        lea eax, [eax + 64] // rgbPtr_ += 64
        // (yCoeffs[0] * mmRgbX[0]) + (yCoeffs[1] * mmRgbX[1])
        pmaddubsw xmm0, xmm7
        pmaddubsw xmm1, xmm7
        pmaddubsw xmm2, xmm7
        pmaddubsw xmm3, xmm7
        // horizontal add
        phaddw xmm0, xmm1
        phaddw xmm2, xmm3
        // >> 7
        psraw xmm0, 7
        psraw xmm2, 7
        // + 16
        paddw xmm0, xmm6
        paddw xmm2, xmm6
        // Saturate(I16 -> U8) - Packs
        packuswb xmm0, xmm2
        // Copy to yuvPtr
        movdqa [edx], xmm0
        lea edx, [edx + 16] // yPtr_ += 16
        sub ecx, 16 // samplesCount -= 16
        jnz loopY // goto loop if (samplesCount != 0)

        //==================================//
        //=========== UV Samples ===========//
        //==================================//
        mov esi, [esp + 12 + 4]   // yuvPtr
        mov eax, [esp + 12 + 8]   // rgbPtr
        mov ecx, [esp + 12 + 12] // width
        imul ecx, [esp + 12 + 16] // (width * height) = samplesCount
        mov edx, ecx
        shr edx, 2 // edx = samplesCount / 4
        add esi,  ecx // [[esi = uPtr_]]
        mov edi, esi // edi = uPtr_
        add edi, edx // [[edi = uPtr_ + edx = uPtr_ + (samplesCount / 4) = vPtr_]]
        xor edx, edx // edx = 0 = i
        mov ebx, [esp + 12 + 12] // ebx = width
        sub ebx, 1 // ebx = (width - 1)

        movdqa xmm7, kUCoeffs // uCoeffs
        movdqa xmm6, kVCoeffs // vCoeffs
        movdqa xmm5, kRGBAShuffleDuplicate // rgbaShuffleDuplicate
        movdqa xmm4, kUV128 // uv128

        /* loopUV start */
        loopUV:
        // load 16 RGBA samples
        movdqa xmm0, [eax] // mmRgb0
        movdqa xmm1, [eax + 16] // mmRgb1
        movdqa xmm2, [eax + 32] // mmRgb2
        movdqa xmm3, [eax + 48] // mmRgb3
        lea eax, [eax + 64] // rgbPtr_ += 64

        pshufb xmm0, xmm5
        pshufb xmm1, xmm5
        pshufb xmm2, xmm5
        pshufb xmm3, xmm5

        punpcklqdq xmm0, xmm1 // mmRgbU0
        punpcklqdq xmm2, xmm3 // mmRgbU1
        movdqa xmm1, xmm0 // mmRgbV0
        movdqa xmm3, xmm2 // mmRgbV1

        pmaddubsw xmm0, xmm7 // mmRgbU0
        pmaddubsw xmm1, xmm6 // mmRgbV0
        pmaddubsw xmm2, xmm7 // mmRgbU1
        pmaddubsw xmm3, xmm6 // mmRgbV1

        phaddw xmm0, xmm2 // mmY0
        phaddw xmm1, xmm3 // mmY1

        psraw xmm0, 8
        psraw xmm1, 8

        paddw xmm0, xmm4
        paddw xmm1, xmm4

        packuswb xmm0, xmm1
        movlps [esi], xmm0
        movhps [edi], xmm0

        lea esi, [esi + 8]
        lea edi, [edi + 8]

        add edx, 16 // i += 16;
        push edx // save edx
        and edx, ebx // edx = (ebx & ebx) = (ebx & (width - 1)) = (ebx % width)
        cmp edx, 0 // (ebx % width) == 0 ?
        pop edx // restore edx
        jne loopUV_NextLine

        // loopUV_EndOfLine: ((ebx % width) == 0)
        add ebx, 1// change ebx value from width-1 to width
        add edx, ebx // i += width
        lea eax, [eax + 4 * ebx]// rgbPtr_ += (width * 4);
        sub ebx, 1// change back ebx value to width - 1
        loopUV_NextLine:
        cmp edx, ecx
        jl loopUV

        pop ebx
        pop edi
        pop esi
        ret
    }
}
#endif /* DDRAW_HAVE_RGB32_TO_I420_ASM */

/* ============ Media Producer Interface ================= */
static int _tdav_producer_screencast_ddraw_set(tmedia_producer_t *p_self, const tmedia_param_t* pc_param)
{
    int ret = 0;
    tdav_producer_screencast_ddraw_t* p_ddraw = (tdav_producer_screencast_ddraw_t*)p_self;

    if (!p_ddraw || !pc_param) {
        DDRAW_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (pc_param->value_type == tmedia_pvt_int64) {
        if (tsk_striequals(pc_param->key, "local-hwnd") || tsk_striequals(pc_param->key, "preview-hwnd")) {
            p_ddraw->hwnd_preview = (HWND)*((int64_t*)pc_param->value);
        }
        else if (tsk_striequals(pc_param->key, "src-hwnd")) {
            p_ddraw->hwnd_src = (HWND)*((int64_t*)pc_param->value);
        }
    }
    else if (pc_param->value_type == tmedia_pvt_int32) {
        if (tsk_striequals(pc_param->key, "mute")) {
            p_ddraw->b_muted = (TSK_TO_INT32((uint8_t*)pc_param->value) != 0);
        }
    }

    return ret;
}


static int _tdav_producer_screencast_ddraw_prepare(tmedia_producer_t* p_self, const tmedia_codec_t* pc_codec)
{
    tdav_producer_screencast_ddraw_t* p_ddraw = (tdav_producer_screencast_ddraw_t*)p_self;
    int ret = 0;
    HRESULT hr = DD_OK;
#if 0
    DDPIXELFORMAT DDPixelFormat;
#endif
    DDSURFACEDESC ddsd;

    if (!p_ddraw || !pc_codec) {
        DDRAW_DEBUG_ERROR("Invalid parameter");
        DDRAW_CHECK_HR(hr = E_INVALIDARG);
    }

    tsk_safeobj_lock(p_ddraw);

    // check support for DirectDraw again
    if (!tdav_producer_screencast_ddraw_plugin_is_supported()) {
        DDRAW_CHECK_HR(hr = E_FAIL);
    }

    TMEDIA_PRODUCER(p_ddraw)->video.fps = TMEDIA_CODEC_VIDEO(pc_codec)->out.fps;
    TMEDIA_PRODUCER(p_ddraw)->video.width = TMEDIA_CODEC_VIDEO(pc_codec)->out.width;
    TMEDIA_PRODUCER(p_ddraw)->video.height = TMEDIA_CODEC_VIDEO(pc_codec)->out.height;

    // Hack the codec to avoid flipping
    TMEDIA_CODEC_VIDEO(pc_codec)->out.flip = tsk_false;

    DDRAW_DEBUG_INFO("Prepare with fps:%d, width:%d; height:%d", TMEDIA_PRODUCER(p_ddraw)->video.fps, TMEDIA_PRODUCER(p_ddraw)->video.width, TMEDIA_PRODUCER(p_ddraw)->video.height);

    if (!p_ddraw->ddrawModule.lpDD || !p_ddraw->ddrawModule.hDLL) {
        DDRAW_CHECK_HR(hr = _tdav_producer_screencast_create_module(&p_ddraw->ddrawModule));
    }
    DDRAW_CHECK_HR(hr = p_ddraw->ddrawModule.lpDD->SetCooperativeLevel(NULL, DDSCL_NORMAL));

    if (!p_ddraw->p_surf_primary) {
        ZeroMemory(&ddsd, sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);
        ddsd.dwFlags = DDSD_CAPS;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

        DDRAW_CHECK_HR(hr = p_ddraw->ddrawModule.lpDD->CreateSurface(&ddsd, &p_ddraw->p_surf_primary, NULL));
    }
#if 0
    ZeroMemory(&DDPixelFormat, sizeof(DDPixelFormat));
    DDPixelFormat.dwSize = sizeof(DDPixelFormat);
    DDRAW_CHECK_HR(hr = DDRAW_VTBL(p_ddraw->p_surf_primary)->GetPixelFormat(p_ddraw->p_surf_primary, &DDPixelFormat));
    DDRAW_DEBUG_INFO("dwRGBBitCount:%d, dwRBitMask:%x, dwGBitMask:%x, dwBBitMask:%x, dwRGBAlphaBitMask:%x",
                     DDPixelFormat.dwRGBBitCount, DDPixelFormat.dwRBitMask, DDPixelFormat.dwGBitMask, DDPixelFormat.dwBBitMask, DDPixelFormat.dwRGBAlphaBitMask);
    if ((TMEDIA_PRODUCER(p_ddraw)->video.chroma = _tdav_producer_screencast_get_chroma(&DDPixelFormat)) == tmedia_chroma_none) {
        DDRAW_CHECK_HR(hr = DDERR_INVALIDCAPS);
    }
#else
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_PITCH | DDSD_PIXELFORMAT;
    DDRAW_CHECK_HR(hr = p_ddraw->p_surf_primary->GetSurfaceDesc(&ddsd));
    DDRAW_DEBUG_INFO("Prepare with neg. width:%d, height:%d, pitch=%ld", ddsd.dwWidth, ddsd.dwHeight, ddsd.lPitch);
    TMEDIA_PRODUCER(p_ddraw)->video.width = ddsd.dwWidth;
    TMEDIA_PRODUCER(p_ddraw)->video.height = ddsd.dwHeight;
    p_ddraw->n_buff_rgb_bitscount = ddsd.ddpfPixelFormat.dwRGBBitCount;
    DDRAW_DEBUG_INFO("Prepare with dwRGBBitCount:%d, dwRBitMask:%x, dwGBitMask:%x, dwBBitMask:%x, dwRGBAlphaBitMask:%x",
                     ddsd.ddpfPixelFormat.dwRGBBitCount, ddsd.ddpfPixelFormat.dwRBitMask, ddsd.ddpfPixelFormat.dwGBitMask, ddsd.ddpfPixelFormat.dwBBitMask, ddsd.ddpfPixelFormat.dwRGBAlphaBitMask);
    if ((TMEDIA_PRODUCER(p_ddraw)->video.chroma = _tdav_producer_screencast_get_chroma(&ddsd.ddpfPixelFormat)) == tmedia_chroma_none) {
        DDRAW_CHECK_HR(hr = DDERR_INVALIDCAPS);
    }
#endif
    // allocate RGB buffer
    DDRAW_CHECK_HR(hr = _tdav_producer_screencast_alloc_rgb_buff(p_ddraw, ddsd.dwWidth, ddsd.dwHeight, ddsd.ddpfPixelFormat.dwRGBBitCount));

    // Check if we can use built-in chroma conversion
#if DDRAW_HAVE_RGB32_TO_I420_INTRIN || DDRAW_HAVE_RGB32_TO_I420_ASM
    p_ddraw->b_have_rgb32_conv =
        _tdav_producer_screencast_have_ssse3() // SSSE3 supported
        && DDRAW_IS_ALIGNED(TMEDIA_PRODUCER(p_ddraw)->video.width, DDRAW_MEM_ALIGNMENT) // width multiple of 16
        /* && DDRAW_IS_ALIGNED(TMEDIA_PRODUCER(p_ddraw)->video.height, DDRAW_MEM_ALIGNMENT) // height multiple of 16 */
        && TMEDIA_PRODUCER(p_ddraw)->video.chroma == tmedia_chroma_rgb32; // Primary screen RGB32
    if (p_ddraw->b_have_rgb32_conv) {
        TMEDIA_PRODUCER(p_ddraw)->video.chroma = tmedia_chroma_yuv420p;
    }
#endif
    DDRAW_DEBUG_INFO("RGB32 -> I420 conversion supported: %s", p_ddraw->b_have_rgb32_conv ? "YES" : "NO");

    // allocate YUV buffer
    if (p_ddraw->b_have_rgb32_conv) {
        DDRAW_CHECK_HR(hr = _tdav_producer_screencast_alloc_yuv_buff(p_ddraw, (DWORD)TMEDIA_PRODUCER(p_ddraw)->video.width, (DWORD)TMEDIA_PRODUCER(p_ddraw)->video.height));
    }

    // BitmapInfo for preview
#if DDRAW_PREVIEW
    ZeroMemory(&p_ddraw->bi_preview, sizeof(p_ddraw->bi_preview));
    p_ddraw->bi_preview.bmiHeader.biSize = (DWORD)sizeof(BITMAPINFOHEADER);
    p_ddraw->bi_preview.bmiHeader.biCompression = BI_RGB;
    p_ddraw->bi_preview.bmiHeader.biPlanes = 1;
    p_ddraw->bi_preview.bmiHeader.biWidth = ddsd.dwWidth;
    p_ddraw->bi_preview.bmiHeader.biHeight = ddsd.dwHeight;
    p_ddraw->bi_preview.bmiHeader.biBitCount = (WORD)ddsd.ddpfPixelFormat.dwRGBBitCount;
    p_ddraw->bi_preview.bmiHeader.biSizeImage = (p_ddraw->bi_preview.bmiHeader.biWidth * p_ddraw->bi_preview.bmiHeader.biHeight * (p_ddraw->bi_preview.bmiHeader.biBitCount >> 3));
#endif /* DDRAW_PREVIEW */

#if DDRAW_CPU_MONITOR || DDRAW_CPU_THROTTLING
    if (!p_ddraw->p_timer_mgr) {
        p_ddraw->p_timer_mgr = tsk_timer_manager_create();
    }
#endif /* DDRAW_CPU_MONITOR ||DDRAW_CPU_THROTTLING */

#if DDRAW_CPU_THROTTLING
    p_ddraw->cpu.fps_target = (TMEDIA_PRODUCER(p_ddraw)->video.fps + DDRAW_CPU_THROTTLING_FPS_MIN) >> 1; // start with minimum fps and increase the value based on the fps
#endif /* DDRAW_CPU_THROTTLING */

    bail:
    tsk_safeobj_unlock(p_ddraw);
    return SUCCEEDED(hr) ? 0 : -1;
}

static int _tdav_producer_screencast_ddraw_start(tmedia_producer_t* p_self)
{
    tdav_producer_screencast_ddraw_t* p_ddraw = (tdav_producer_screencast_ddraw_t*)p_self;
    int ret = 0;

    if (!p_ddraw) {
        DDRAW_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    ret = tsk_safeobj_lock(p_ddraw);

    p_ddraw->b_paused = tsk_false;

    if (p_ddraw->b_started) {
        DDRAW_DEBUG_INFO("Already started");
        goto bail;
    }

    p_ddraw->b_started = tsk_true;

    // Create notify events (must be done here before starting the grabber thread)
#if DDRAW_MT
    for (int i = 0; i < sizeof(p_ddraw->mt.h_events) / sizeof(p_ddraw->mt.h_events[0]); ++i) {
        if (!p_ddraw->mt.h_events[i] && !(p_ddraw->mt.h_events[i] = CreateEvent(NULL, FALSE, FALSE, NULL))) {
            DDRAW_DEBUG_ERROR("Failed to create event at %d", i);
            ret = -1;
            goto bail;
        }
    }
#endif /* DDRAW_MT */

    ret = tsk_thread_create(&p_ddraw->tid[0], _tdav_producer_screencast_grap_thread, p_ddraw);
    if (ret != 0) {
        DDRAW_DEBUG_ERROR("Failed to create thread");
        goto bail;
    }
    //BOOL okSetTA = CeSetThreadAffinity((HANDLE)p_ddraw->tid[0], 0x01);
#if DDRAW_MT
    ret = tsk_thread_create(&p_ddraw->mt.tid[0], _tdav_producer_screencast_mt_encode_thread, p_ddraw);
    if (ret != 0) {
        DDRAW_DEBUG_ERROR("Failed to create thread");
        goto bail;
    }
    //okSetTA = CeSetThreadAffinity((HANDLE)p_ddraw->mt.tid[0], 0x02);
#endif /* DDRAW_MT */
#if DDRAW_HIGH_PRIO_MEMCPY
    if (p_ddraw->tid[0]) {
        tsk_thread_set_priority(p_ddraw->tid[0], TSK_THREAD_PRIORITY_TIME_CRITICAL);
    }
#if DDRAW_MT
    if (p_ddraw->mt.tid[0]) {
        tsk_thread_set_priority(p_ddraw->mt.tid[0], TSK_THREAD_PRIORITY_TIME_CRITICAL);
    }
#endif /* DDRAW_MT */
#endif /* DDRAW_HIGH_PRIO_MEMCPY */
#if DDRAW_CPU_MONITOR || DDRAW_CPU_THROTTLING
    ret = tsk_timer_manager_start(p_ddraw->p_timer_mgr);
    if (ret == 0) {
        p_ddraw->cpu.id_timer = tsk_timer_manager_schedule(p_ddraw->p_timer_mgr, DDRAW_CPU_SCHEDULE_TIMEOUT, _tdav_producer_screencast_timer_cb, p_ddraw);
    }
    else {
        ret = 0; // not fatal error
        DDRAW_DEBUG_WARN("Failed to start CPU timer");
    }
#endif /* DDRAW_CPU_MONITOR || DDRAW_CPU_THROTTLING */

    bail:
    if (ret) {
        p_ddraw->b_started = tsk_false;
        if (p_ddraw->tid[0]) {
            tsk_thread_join(&(p_ddraw->tid[0]));
        }
#if DDRAW_MT
        if (p_ddraw->mt.tid[0]) {
            tsk_thread_join(&(p_ddraw->mt.tid[0]));
        }
#endif /* DDRAW_MT */
    }
    ret = tsk_safeobj_unlock(p_ddraw);

    return ret;
}

static int _tdav_producer_screencast_ddraw_pause(tmedia_producer_t* p_self)
{
    tdav_producer_screencast_ddraw_t* p_ddraw = (tdav_producer_screencast_ddraw_t*)p_self;

    if (!p_ddraw) {
        DDRAW_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(p_ddraw);

    p_ddraw->b_paused = tsk_true;
    goto bail;

    bail:
    tsk_safeobj_unlock(p_ddraw);

    return 0;
}

static int _tdav_producer_screencast_ddraw_stop(tmedia_producer_t* p_self)
{
    tdav_producer_screencast_ddraw_t* p_ddraw = (tdav_producer_screencast_ddraw_t*)p_self;

    if (!p_ddraw) {
        DDRAW_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(p_ddraw);

    if (!p_ddraw->b_started) {
        DDRAW_DEBUG_INFO("Already stopped");
        goto bail;
    }

    p_ddraw->b_started = tsk_false;
    p_ddraw->b_paused = tsk_false;

#if DDRAW_CPU_MONITOR || DDRAW_CPU_THROTTLING
    if (p_ddraw->p_timer_mgr) {
        tsk_timer_manager_stop(p_ddraw->p_timer_mgr);
    }
#endif /* DDRAW_CPU_MONITOR ||DDRAW_CPU_THROTTLING */

    // stop grabber thread
    if (p_ddraw->tid[0]) {
        tsk_thread_join(&(p_ddraw->tid[0]));
    }

#if DDRAW_MT
    if (p_ddraw->mt.h_events[DDRAW_MT_EVENT_SHUTDOWN_INDEX]) {
        SetEvent(p_ddraw->mt.h_events[DDRAW_MT_EVENT_SHUTDOWN_INDEX]);
    }
    if (p_ddraw->mt.tid[0]) {
        tsk_thread_join(&(p_ddraw->mt.tid[0]));
    }
    for (int i = 0; i < sizeof(p_ddraw->mt.h_events) / sizeof(p_ddraw->mt.h_events[0]); ++i) {
        if (p_ddraw->mt.h_events[i]) {
            CloseHandle(p_ddraw->mt.h_events[i]);
            p_ddraw->mt.h_events[i] = NULL;
        }
    }
#endif

    bail:
    tsk_safeobj_unlock(p_ddraw);

    return 0;
}

static int _tdav_producer_screencast_grab(tdav_producer_screencast_ddraw_t* p_self)
{
    int ret = 0;
    HRESULT hr = S_OK;
    DDSURFACEDESC ddsd;
    DWORD nSizeWithoutPadding, nRowLengthInBytes, lockFlags;
    tmedia_producer_t* p_base = TMEDIA_PRODUCER(p_self);
    LPVOID lpBuffToSend, lpBuffYUV;
    BOOL bDirectMemSurfAccess = DDRAW_MEM_SURFACE_DIRECT_ACCESS;
#if DDRAW_MT
    INT iMtFreeBuffIndex = -1;
#endif
    //--uint64_t timeStart, timeEnd;

    //--timeStart = tsk_time_now();

    if (!p_self) {
        DDRAW_CHECK_HR(hr = E_INVALIDARG);
    }

    if (!p_self->b_started) {
#if defined(E_ILLEGAL_METHOD_CALL)
        DDRAW_CHECK_HR(hr = E_ILLEGAL_METHOD_CALL);
#else
        DDRAW_CHECK_HR(hr = E_FAIL);
#endif
    }

#if DDRAW_MT
    {
        INT iIndex = 0;
        for (; (iIndex < DDRAW_MT_COUNT) && (p_self->mt.b_flags_array[iIndex] == TRUE); ++iIndex);
        if (iIndex == DDRAW_MT_COUNT) {
            goto bail;
        }
    }
#endif /* DDRAW_MT */

    if (p_self->p_surf_primary->IsLost() == DDERR_SURFACELOST) {
        DDRAW_CHECK_HR(hr = p_self->p_surf_primary->Restore());
    }

    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_PITCH | DDSD_PIXELFORMAT;
    lockFlags = DDLOCK_READONLY |
#if TDAV_UNDER_WINDOWS_CE
                // This flag has a slightly different name under Windows CE vs. Desktop, but it's the same behavior.
                DDLOCK_WAITNOTBUSY;
#else
                DDLOCK_WAIT;
#endif
    DDRAW_CHECK_HR(hr = p_self->p_surf_primary->Lock(NULL, &ddsd, lockFlags, NULL));
    // make sure surface size and number of bits per pixel haven't changed
    if (TMEDIA_PRODUCER(p_self)->video.width != ddsd.dwWidth || TMEDIA_PRODUCER(p_self)->video.height != ddsd.dwHeight || p_self->n_buff_rgb_bitscount != ddsd.ddpfPixelFormat.dwRGBBitCount) {
        tsk_size_t n_buff_rgb_new;
        tmedia_chroma_t chroma_new;
        DDRAW_DEBUG_WARN("surface has changed: width %d<>%d or height %d<>%d or rgb_bits_count %d<>%d",
                         p_base->video.width, ddsd.dwWidth,
                         p_base->video.height, ddsd.dwHeight,
                         p_self->n_buff_rgb_bitscount, ddsd.ddpfPixelFormat.dwRGBBitCount);
        if ((chroma_new = _tdav_producer_screencast_get_chroma(&ddsd.ddpfPixelFormat)) == tmedia_chroma_none) {
            DDRAW_CHECK_HR(hr = DDERR_INVALIDCAPS);
        }
        // allocate RGB buffer
        n_buff_rgb_new = (ddsd.dwWidth * ddsd.dwHeight * (ddsd.ddpfPixelFormat.dwRGBBitCount >> 3));
        if (p_self->n_buff_rgb < n_buff_rgb_new) {
            hr = _tdav_producer_screencast_alloc_rgb_buff(p_self, ddsd.dwWidth, ddsd.dwHeight, ddsd.ddpfPixelFormat.dwRGBBitCount);
            if (FAILED(hr)) {
                p_self->p_surf_primary->Unlock(NULL); // unlock before going to bail
                DDRAW_CHECK_HR(hr);
            }
        }
        p_base->video.width = ddsd.dwWidth;
        p_base->video.height = ddsd.dwHeight;
        p_base->video.chroma = chroma_new;
        p_self->n_buff_rgb_bitscount = ddsd.ddpfPixelFormat.dwRGBBitCount;
        // Check if we can use built-in chroma conversion
#if DDRAW_HAVE_RGB32_TO_I420_INTRIN || DDRAW_HAVE_RGB32_TO_I420_ASM
        p_self->b_have_rgb32_conv =
            _tdav_producer_screencast_have_ssse3() // SSSE3 supported
            && DDRAW_IS_ALIGNED(p_base->video.width, DDRAW_MEM_ALIGNMENT) // width multiple of 16
            /* && DDRAW_IS_ALIGNED(p_base->video.height, DDRAW_MEM_ALIGNMENT) // height multiple of 16 */
            && p_base->video.chroma == tmedia_chroma_rgb32; // Primary screen RGB32
        if (p_self->b_have_rgb32_conv) {
            p_base->video.chroma = tmedia_chroma_yuv420p;
        }
#endif
        DDRAW_DEBUG_INFO("RGB32 -> I420 conversion supported: %s", p_self->b_have_rgb32_conv ? "YES" : "NO");
        // allocate YUV buffer
        if (p_self->b_have_rgb32_conv) {
            hr = _tdav_producer_screencast_alloc_yuv_buff(p_self, (DWORD)p_base->video.width, (DWORD)p_base->video.height);
            if (FAILED(hr)) {
                p_self->p_surf_primary->Unlock(NULL); // unlock before going to bail
                DDRAW_CHECK_HR(hr);
            }
        }
        // preview
#if DDRAW_PREVIEW
        p_self->bi_preview.bmiHeader.biWidth = ddsd.dwWidth;
        p_self->bi_preview.bmiHeader.biHeight = ddsd.dwHeight;
        p_self->bi_preview.bmiHeader.biBitCount = (WORD)ddsd.ddpfPixelFormat.dwRGBBitCount;
        p_self->bi_preview.bmiHeader.biSizeImage = (p_self->bi_preview.bmiHeader.biWidth * p_self->bi_preview.bmiHeader.biHeight * (p_self->bi_preview.bmiHeader.biBitCount >> 3));
#endif /* DDRAW_PREVIEW */
    }
    nRowLengthInBytes = ddsd.dwWidth * (ddsd.ddpfPixelFormat.dwRGBBitCount >> 3);
    nSizeWithoutPadding = ddsd.dwHeight * nRowLengthInBytes;

    // init lpBuffToSend
    if (DDRAW_MEM_SURFACE_DIRECT_ACCESS && ddsd.lPitch == nRowLengthInBytes && (!p_self->b_have_rgb32_conv || DDRAW_IS_ALIGNED(ddsd.lpSurface, DDRAW_MEM_ALIGNMENT))) {
        // no padding
        lpBuffToSend = ddsd.lpSurface;
        bDirectMemSurfAccess = TRUE;
    }
    else {
        // with padding or copy requested
        UINT8 *pSurfBuff = (UINT8 *)ddsd.lpSurface, *pNegBuff = (UINT8 *)p_self->p_buff_rgb_aligned;
        DWORD y;
        bDirectMemSurfAccess = FALSE;
        //--timeStart = tsk_time_now();
        if (ddsd.lPitch == nRowLengthInBytes) {
            // copy without padding padding
            const UINT8* src = pSurfBuff;
            UINT8* dst = (UINT8*)p_self->p_buff_rgb_aligned;
            if (DDRAW_IS_ALIGNED(src, 16) && (nSizeWithoutPadding & 15) == 0) {
#if DDRAW_HAVE_RGB32_TO_I420_INTRIN || DDRAW_HAVE_RGB32_TO_I420_ASM
                if ((nSizeWithoutPadding & 127) == 0) {
                    for (DWORD i = 0; i < nSizeWithoutPadding; i += 128, src += 128, dst += 128) {
#if defined(DDRAW_COPY128_ASM)
                        DDRAW_COPY128_ASM(dst, src);
#else
                        DDRAW_COPY128_INTRIN(dst, src);
#endif /* DDRAW_COPY128_ASM */
                    }
                }
                else if((nSizeWithoutPadding & 63) == 0) {
                    for (DWORD i = 0; i < nSizeWithoutPadding; i += 64, src += 64, dst += 64) {
#if defined(DDRAW_COPY64_ASM)
                        DDRAW_COPY64_ASM(dst, src);
#else
                        DDRAW_COPY64_INTRIN(dst, src);
#endif /* DDRAW_COPY64_ASM */
                    }
                }
                else { // (nSizeWithoutPadding & 15) == 0
                    for (DWORD i = 0; i < nSizeWithoutPadding; i += 16, src += 16, dst += 16) {
#if defined(DDRAW_COPY16_ASM)
                        DDRAW_COPY16_ASM(dst, src);
#else
                        DDRAW_COPY16_INTRIN(dst, src);
#endif /* DDRAW_COPY16_ASM */
                    }
                }
#else // neither ASM nor INTRINSIC support
                CopyMemory(dst, src, nSizeWithoutPadding);
#endif /* DDRAW_HAVE_RGB32_TO_I420_INTRIN || DDRAW_HAVE_RGB32_TO_I420_ASM */
            }
            else { // not 16bytes aligned
                CopyMemory(dst, src, nSizeWithoutPadding);
            }
        }
        else {
            // copy with padding padding
            for (y = 0; y < ddsd.dwHeight; ++y) {
                CopyMemory(pNegBuff, pSurfBuff, nRowLengthInBytes);
                pSurfBuff += ddsd.lPitch;
                pNegBuff += nRowLengthInBytes;
            }
        }
        lpBuffToSend = p_self->p_buff_rgb_aligned;
        //--timeEnd = tsk_time_now();
        //--DDRAW_DEBUG_INFO("Mem copy: start=%llu, end=%llu, duration=%llu", timeStart, timeEnd, (timeEnd - timeStart));
    }
    if (!bDirectMemSurfAccess) {
        // surface buffer no longer needed, unlock
        DDRAW_CHECK_HR(hr = p_self->p_surf_primary->Unlock(NULL));
    }
    // display preview
#if DDRAW_PREVIEW
    if (p_self->hwnd_preview) {
        HWND hWnd; // copy for thread-safeness
        HDC hDC = GetDC((hWnd = p_self->hwnd_preview));
        if (hDC) {
            RECT rcPreview;
            if (GetWindowRect(hWnd, &rcPreview)) {
                LONG nPreviewWidth = (rcPreview.right - rcPreview.left);
                LONG nPreviewHeight = (rcPreview.bottom - rcPreview.top);
                StretchDIBits(
                    hDC,
                    0, 0, nPreviewWidth, nPreviewHeight,
                    0, 0, p_self->bi_preview.bmiHeader.biWidth, p_self->bi_preview.bmiHeader.biHeight,
                    lpBuffToSend,
                    &p_self->bi_preview,
                    DIB_RGB_COLORS,
                    SRCCOPY);
            }
            ReleaseDC(hWnd, hDC);
        }
    }
#endif /* DDRAW_PREVIEW */

    // check we have a free buffer
#if DDRAW_MT
    {
        for (INT iIndex = 0; iIndex < DDRAW_MT_COUNT; ++iIndex) {
            if (p_self->mt.b_flags_array[iIndex] != TRUE) {
                iMtFreeBuffIndex = iIndex;
                lpBuffYUV = p_self->mt.p_buff_yuv_aligned_array[iIndex];
                break;
            }
        }
        if (iMtFreeBuffIndex < 0) {
            lpBuffToSend = NULL; // do not waste time converting or encoding
            lpBuffYUV = NULL;
        }
    }
#else
    lpBuffYUV = p_self->p_buff_yuv_aligned;
#endif /* DDRAW_MT */

    //--timeStart = tsk_time_now();
    if (lpBuffToSend && (lpBuffYUV || !p_self->b_have_rgb32_conv)) {
        if (p_self->b_have_rgb32_conv) {
            // Convert from RGB32 to I420
#if DDRAW_HAVE_RGB32_TO_I420_ASM
            _tdav_producer_screencast_rgb32_to_yuv420_asm_ssse3((uint8_t*)lpBuffYUV, (const uint8_t*)lpBuffToSend, (int)p_base->video.width, (int)p_base->video.height);
#elif DDRAW_HAVE_RGB32_TO_I420_INTRIN
            _tdav_producer_screencast_rgb32_to_yuv420_intrin_ssse3((uint8_t*)lpBuffYUV, (const uint8_t*)lpBuffToSend, (int)p_base->video.width, (int)p_base->video.height);
#else
            DDRAW_CHECK_HR(hr = E_NOTIMPL); // never called
#endif
#if DDRAW_MT
            p_self->mt.b_flags_array[iMtFreeBuffIndex] = TRUE;
            if (!SetEvent(p_self->mt.h_events[iMtFreeBuffIndex])) {
                DDRAW_CHECK_HR(hr = E_FAIL);
            }
#else
            p_base->enc_cb.callback(p_base->enc_cb.callback_data, lpBuffYUV, p_self->n_buff_yuv);
#endif
        }
        else {
            // Send RGB32 buffer to the encode callback and let conversion be done by libyuv
            // do not multi-thread as we cannot perform chroma conversion and encoding in parallel
            p_base->enc_cb.callback(p_base->enc_cb.callback_data, lpBuffToSend, nSizeWithoutPadding);
        }
    }
    //--timeEnd = tsk_time_now();
    //--DDRAW_DEBUG_INFO("Encode callback: start=%llu, end=%llu, duration=%llu", timeStart, timeEnd, (timeEnd - timeStart));

    if (bDirectMemSurfAccess) {
        // surface buffer was used in preview and encode callback, unlock now
        DDRAW_CHECK_HR(hr = p_self->p_surf_primary->Unlock(NULL));
    }

    bail:
    if (hr == DDERR_SURFACELOST) {
        /*hr = */p_self->p_surf_primary->Restore();
        hr = S_OK;
    }

    //--timeEnd = tsk_time_now();
    //--DDRAW_DEBUG_INFO("Grab and encode duration=%llu", (timeEnd - timeStart));

    return SUCCEEDED(hr) ? 0 : -1;
}

static tmedia_chroma_t _tdav_producer_screencast_get_chroma(const DDPIXELFORMAT* pixelFormat)
{
    HRESULT hr = DD_OK;
    if (pixelFormat->dwFlags != DDPF_RGB) {
        DDRAW_DEBUG_ERROR("dwFlags(%d) != DDPF_RGB", pixelFormat->dwFlags);
        DDRAW_CHECK_HR(hr = DDERR_INVALIDCAPS);
    }
    switch (pixelFormat->dwRGBBitCount) {
    case 32: // RGB32
        case 24: // RGB24
                // pixels must be aligned for fast copy
                if (pixelFormat->dwRBitMask != 0xff0000 || pixelFormat->dwGBitMask != 0xff00 || pixelFormat->dwBBitMask != 0xff || pixelFormat->dwRGBAlphaBitMask != 0) {
                    DDRAW_DEBUG_ERROR("Pixels not aligned");
                }
        return pixelFormat->dwRGBBitCount == 24 ? tmedia_chroma_bgr24 : tmedia_chroma_rgb32;
    case 16: // RGB565
            // pixels must be aligned for fast copy
            if (pixelFormat->dwRBitMask != 0xF800 || pixelFormat->dwGBitMask != 0x7E0 || pixelFormat->dwBBitMask != 0x1F) {
                DDRAW_DEBUG_ERROR("Pixels not aligned");
            }
        return tmedia_chroma_rgb565le;
    default:
            DDRAW_DEBUG_ERROR("dwRGBBitCount(%d) != 24 and 32", pixelFormat->dwRGBBitCount);
        DDRAW_CHECK_HR(hr = DDERR_INVALIDCAPS);
        break;
    }

    bail:
    return tmedia_chroma_none;
}

static HRESULT _tdav_producer_screencast_create_module(LPDDrawModule lpModule)
{
    typedef HRESULT (WINAPI *pDirectDrawCreateFunc)(_In_  GUID FAR         *lpGUID,
            _Out_ LPDIRECTDRAW FAR *lplpDD,
            _In_  IUnknown FAR     *pUnkOuter);
    HRESULT hr = S_OK;
    pDirectDrawCreateFunc DirectDrawCreate_ = NULL;

    if (!lpModule) {
        DDRAW_CHECK_HR(hr = E_INVALIDARG);
    }

    if (!lpModule->hDLL && !(lpModule->hDLL = LoadLibrary(TEXT("ddraw.dll")))) {
        DDRAW_DEBUG_ERROR("Failed to load ddraw.dll: %d", GetLastError());
        DDRAW_CHECK_HR(hr = E_FAIL);
    }
    if (!lpModule->lpDD) {
        // Hum, "GetProcAddressA" is missing but ""GetProcAddressW" exists on CE
#if TDAV_UNDER_WINDOWS_CE
#	define DirectDrawCreateName TEXT("DirectDrawCreate")
#else
#	define DirectDrawCreateName "DirectDrawCreate"
#endif
        if (!(DirectDrawCreate_ = (pDirectDrawCreateFunc)GetProcAddress(lpModule->hDLL, DirectDrawCreateName))) {
            DDRAW_DEBUG_ERROR("Failed to find DirectDrawCreate in ddraw.dll: %d", GetLastError());
            DDRAW_CHECK_HR(hr = E_FAIL);
        }
        DDRAW_CHECK_HR(hr = DirectDrawCreate_(NULL, &lpModule->lpDD, NULL));
    }

    bail:
    return hr;
}

static HRESULT _tdav_producer_screencast_alloc_rgb_buff(tdav_producer_screencast_ddraw_t* p_ddraw, DWORD w, DWORD h, DWORD bitsCount)
{
    HRESULT hr = S_OK;
    DWORD n_buff_rgb_new = (w * h * (bitsCount >> 3));

    if (p_ddraw->n_buff_rgb < n_buff_rgb_new) {
        p_ddraw->p_buff_rgb_aligned = tsk_realloc_aligned(p_ddraw->p_buff_rgb_aligned, n_buff_rgb_new, DDRAW_MEM_ALIGNMENT);
        if (!p_ddraw->p_buff_rgb_aligned) {
            p_ddraw->n_buff_rgb = 0;
            DDRAW_CHECK_HR(hr = DDERR_OUTOFMEMORY);
        }
        p_ddraw->n_buff_rgb = n_buff_rgb_new;
    }

    bail:
    return hr;
}

static HRESULT _tdav_producer_screencast_alloc_yuv_buff(tdav_producer_screencast_ddraw_t* p_ddraw, DWORD w, DWORD h)
{
    HRESULT hr = S_OK;
    void** pp_buff_yuv_aligned;
    int n_buff_yuv_aligned_count;

#if DDRAW_MT
    pp_buff_yuv_aligned = p_ddraw->mt.p_buff_yuv_aligned_array;
    n_buff_yuv_aligned_count = sizeof(p_ddraw->mt.p_buff_yuv_aligned_array)/sizeof(p_ddraw->mt.p_buff_yuv_aligned_array[0]);
#else
    pp_buff_yuv_aligned = &p_ddraw->p_buff_yuv_aligned;
    n_buff_yuv_aligned_count = 1;
#endif /* DDRAW_MT */

    p_ddraw->n_buff_yuv = (w * h * 3) >> 1;
    for (int i = 0; i < n_buff_yuv_aligned_count; ++i) {
        pp_buff_yuv_aligned[i] = tsk_realloc_aligned(pp_buff_yuv_aligned[i], p_ddraw->n_buff_yuv, DDRAW_MEM_ALIGNMENT);
        if (!pp_buff_yuv_aligned[i]) {
            p_ddraw->n_buff_yuv = 0;
            DDRAW_CHECK_HR(hr = DDERR_OUTOFMEMORY);
        }
    }

    bail:
    return hr;
}

static void* TSK_STDCALL _tdav_producer_screencast_grap_thread(void *arg)
{
    tdav_producer_screencast_ddraw_t* p_ddraw = (tdav_producer_screencast_ddraw_t*)arg;
    tmedia_producer_t* p_base = TMEDIA_PRODUCER(arg);
    int ret = 0;

    // FPS manager
    uint64_t TimeNow, TimeLastFrame = 0;
    uint64_t TimeFrameDuration = (1000 / p_base->video.fps);

    DDRAW_DEBUG_INFO("Grab thread -- START");

    while (ret == 0 && p_ddraw->b_started) {
#if DDRAW_CPU_THROTTLING
        TimeFrameDuration = (1000 / p_ddraw->cpu.fps_target);
#endif /* DDRAW_CPU_THROTTLING */
        TimeNow = tsk_time_now();
        if ((TimeNow - TimeLastFrame) > TimeFrameDuration) {
            if (!p_ddraw->b_muted && !p_ddraw->b_paused) {
                if (ret = _tdav_producer_screencast_grab(p_ddraw)) {
                    goto next;
                }
            }
            TimeLastFrame = TimeNow;
        }
        else {
            tsk_thread_sleep(1);
#if 0
            DDRAW_DEBUG_INFO("Skip frame");
#endif
        }
        next:
        ;
    }
    DDRAW_DEBUG_INFO("Grab thread -- STOP");
    return tsk_null;
}

#if DDRAW_MT
static void* TSK_STDCALL _tdav_producer_screencast_mt_encode_thread(void *arg)
{
    tdav_producer_screencast_ddraw_t* p_ddraw = (tdav_producer_screencast_ddraw_t*)arg;
    tmedia_producer_t* p_base = TMEDIA_PRODUCER(arg);
    DWORD dwEvent, dwIndex;
    int ret = 0;
    DWORD events_count = sizeof(p_ddraw->mt.h_events) / sizeof(p_ddraw->mt.h_events[0]);

    DDRAW_DEBUG_INFO("Encode MT thread -- START");

    while (ret == 0 && p_ddraw->b_started) {
        dwEvent = WaitForMultipleObjects(events_count, p_ddraw->mt.h_events, FALSE, INFINITE);
        if (!p_ddraw->b_started) {
            break;
        }
        if (dwEvent < WAIT_OBJECT_0 || dwEvent >(WAIT_OBJECT_0 + events_count)) {
            DDRAW_DEBUG_ERROR("Invalid dwEvent(%d)", dwEvent);
            break;
        }
        dwIndex = (dwEvent - WAIT_OBJECT_0);
        if (p_ddraw->mt.b_flags_array[dwIndex] != TRUE) {
            // must never happen
            DDRAW_DEBUG_ERROR("Invalid b_flags_array(%d)", dwIndex);
            break;
        }

        p_base->enc_cb.callback(p_base->enc_cb.callback_data, p_ddraw->mt.p_buff_yuv_aligned_array[dwIndex], p_ddraw->n_buff_yuv);
        p_ddraw->mt.b_flags_array[dwIndex] = FALSE;
    }
    DDRAW_DEBUG_INFO("Encode MT -- STOP");
    return tsk_null;
}
#endif /* DDRAW_MT */

#if DDRAW_CPU_MONITOR || DDRAW_CPU_THROTTLING
static unsigned long long FileTimeToInt64(const FILETIME & ft)
{
    return (((unsigned long long)(ft.dwHighDateTime))<<32) | ((unsigned long long)ft.dwLowDateTime);
}
static BOOL GetCpuPercents(unsigned long long* PercentIdle, unsigned long long* PercentUsage)
{
    static unsigned long long _prevTicks = 0;
    static unsigned long long _prevIdleTime = 0;
    unsigned long long ticks, idleTime;
    BOOL bSaveValues = FALSE, bSet = FALSE;
#if TDAV_UNDER_WINDOWS_CE
    bSaveValues = TRUE;
    ticks = GetTickCount();
    idleTime = GetIdleTime();
#else
    {
        FILETIME _idleTime, _kernelTime, _userTime;
        if (GetSystemTimes(&_idleTime, &_kernelTime, &_userTime)) {
            idleTime = FileTimeToInt64(_idleTime);
            ticks = FileTimeToInt64(_kernelTime) + FileTimeToInt64(_userTime);
            bSaveValues = TRUE;
        }
    }
#endif
    if (_prevTicks > 0) {
        *PercentIdle = ((100 * (idleTime - _prevIdleTime)) / (ticks - _prevTicks));
        *PercentUsage = 100 - *PercentIdle;
        bSet = TRUE;
    }
    if (bSaveValues) {
        _prevTicks = ticks;
        _prevIdleTime = idleTime;
    }

    return bSet;
}

static int _tdav_producer_screencast_timer_cb(const void* arg, tsk_timer_id_t timer_id)
{
    tdav_producer_screencast_ddraw_t* p_ddraw = (tdav_producer_screencast_ddraw_t*)arg;
    int ret = 0;

    if (!p_ddraw->b_started) {
        return 0;
    }

    if (p_ddraw->cpu.id_timer == timer_id) {
        unsigned long long PercentIdle, PercentUsage;
        if (GetCpuPercents(&PercentIdle, &PercentUsage) == TRUE) {
            TSK_DEBUG_INFO("\n\n****\n\nCPU Usage = %lld\n\n***", PercentUsage);
#if DDRAW_CPU_THROTTLING
            {
                if ((PercentUsage + DDRAW_CPU_THROTTLING_THRESHOLD_MARGIN) > DDRAW_CPU_THROTTLING_THRESHOLD) {
                    unsigned long long NewTargetPercentUsage = TSK_CLAMP(DDRAW_CPU_THROTTLING_THRESHOLD_MARGIN, DDRAW_CPU_THROTTLING_THRESHOLD - DDRAW_CPU_THROTTLING_THRESHOLD_MARGIN, INT_MAX);
                    int NewTargetFps = (int)((NewTargetPercentUsage * p_ddraw->cpu.fps_target) / PercentUsage);
                    NewTargetFps = TSK_CLAMP(DDRAW_CPU_THROTTLING_FPS_MIN, NewTargetFps, TMEDIA_PRODUCER(p_ddraw)->video.fps);
                    TSK_DEBUG_INFO("\n\n****\n\nCPU throttling = (%lld+%d)>%d, NewTargetPercentUsage=%lld, NewTargetFps=%d\n\n***",
                                   PercentUsage, DDRAW_CPU_THROTTLING_THRESHOLD_MARGIN, DDRAW_CPU_THROTTLING_THRESHOLD, NewTargetPercentUsage, NewTargetFps);
                    p_ddraw->cpu.fps_target = NewTargetFps;
                }
                else if (PercentUsage < DDRAW_CPU_THROTTLING_THRESHOLD) {
                    if ((p_ddraw->cpu.fps_target + DDRAW_CPU_THROTTLING_THRESHOLD_MARGIN) < TMEDIA_PRODUCER(p_ddraw)->video.fps) { // not honoring the negotiated fps yet?
                        p_ddraw->cpu.fps_target += 1; // TODO: this is ok only if the timer timeout is set to 1s or less
                    }
                }
            }
#endif /* DDRAW_CPU_THROTTLING */
        }

        if (p_ddraw->b_started) {
            p_ddraw->cpu.id_timer = tsk_timer_manager_schedule(p_ddraw->p_timer_mgr, DDRAW_CPU_SCHEDULE_TIMEOUT, _tdav_producer_screencast_timer_cb, p_ddraw);
        }
    }
    return 0;
}

#endif /* DDRAW_CPU_MONITOR || DDRAW_CPU_THROTTLING */

//
//	ddraw screencast producer object definition
//
/* constructor */
static tsk_object_t* _tdav_producer_screencast_ddraw_ctor(tsk_object_t *self, va_list * app)
{
    tdav_producer_screencast_ddraw_t *p_ddraw = (tdav_producer_screencast_ddraw_t *)self;
    if (p_ddraw) {
        /* init base */
        tmedia_producer_init(TMEDIA_PRODUCER(p_ddraw));
        TMEDIA_PRODUCER(p_ddraw)->video.chroma = tmedia_chroma_bgr24; // RGB24 on x86 (little endians) stored as BGR24
        /* init self with default values*/
        TMEDIA_PRODUCER(p_ddraw)->video.fps = 15;
        TMEDIA_PRODUCER(p_ddraw)->video.width = 352;
        TMEDIA_PRODUCER(p_ddraw)->video.height = 288;

        tsk_safeobj_init(p_ddraw);
    }
    return self;
}
/* destructor */
static tsk_object_t* _tdav_producer_screencast_ddraw_dtor(tsk_object_t * self)
{
    tdav_producer_screencast_ddraw_t *p_ddraw = (tdav_producer_screencast_ddraw_t *)self;
    if (p_ddraw) {
        /* stop */
        if (p_ddraw->b_started) {
            _tdav_producer_screencast_ddraw_stop((tmedia_producer_t*)p_ddraw);
        }

        /* deinit base */
        tmedia_producer_deinit(TMEDIA_PRODUCER(p_ddraw));
        /* deinit self */
#if DDRAW_CPU_MONITOR || DDRAW_CPU_THROTTLING
        if (p_ddraw->p_timer_mgr) {
            tsk_timer_manager_destroy(&p_ddraw->p_timer_mgr);
        }
#endif /* DDRAW_CPU_MONITOR || DDRAW_CPU_THROTTLING */
#if DDRAW_MT
        for (int i = 0; i < sizeof(p_ddraw->mt.p_buff_yuv_aligned_array) / sizeof(p_ddraw->mt.p_buff_yuv_aligned_array[0]); ++i) {
            TSK_FREE_ALIGNED(p_ddraw->mt.p_buff_yuv_aligned_array[i]);
        }
        for (int i = 0; i < sizeof(p_ddraw->mt.h_events) / sizeof(p_ddraw->mt.h_events[0]); ++i) {
            if (p_ddraw->mt.h_events[i]) {
                CloseHandle(p_ddraw->mt.h_events[i]);
                p_ddraw->mt.h_events[i] = NULL;
            }
        }
#endif /* DDRAW_MT */
        TSK_FREE_ALIGNED(p_ddraw->p_buff_rgb_aligned);
        TSK_FREE_ALIGNED(p_ddraw->p_buff_yuv_aligned);
        DDRAW_SAFE_RELEASE(&p_ddraw->p_surf_primary);
        DDrawModuleSafeFree(p_ddraw->ddrawModule);
        tsk_safeobj_deinit(p_ddraw);

        DDRAW_DEBUG_INFO("*** destroyed ***");
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_producer_screencast_ddraw_def_s = {
    sizeof(tdav_producer_screencast_ddraw_t),
    _tdav_producer_screencast_ddraw_ctor,
    _tdav_producer_screencast_ddraw_dtor,
    tsk_null,
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t tdav_producer_screencast_ddraw_plugin_def_s = {
    &tdav_producer_screencast_ddraw_def_s,
    tmedia_bfcp_video,
    "Microsoft DirectDraw screencast producer",

    _tdav_producer_screencast_ddraw_set,
    _tdav_producer_screencast_ddraw_prepare,
    _tdav_producer_screencast_ddraw_start,
    _tdav_producer_screencast_ddraw_pause,
    _tdav_producer_screencast_ddraw_stop
};
const tmedia_producer_plugin_def_t *tdav_producer_screencast_ddraw_plugin_def_t = &tdav_producer_screencast_ddraw_plugin_def_s;

#endif /* TDAV_UNDER_WINDOWS && !TDAV_UNDER_WINDOWS_RT */
