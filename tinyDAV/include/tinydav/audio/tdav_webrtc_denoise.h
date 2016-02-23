/*
* Copyright (C) 2011 Doubango Telecom <http://www.doubango.org>
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

/**@file tdav_webrtc_denoise.h
 * @brief Google WebRTC Denoiser (Noise suppression, AGC, AEC) Plugin
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#ifndef TINYDAV_WEBRTC_DENOISE_H
#define TINYDAV_WEBRTC_DENOISE_H

#include "tinydav_config.h"

#if HAVE_WEBRTC && (!defined(HAVE_WEBRTC_DENOISE) || HAVE_WEBRTC_DENOISE)

#include "tinymedia/tmedia_denoise.h"

#include "tsk_safeobj.h"

/* Speex denoiser works better than WebRTC's denoiser. This is obvious on Android. */
#if !defined(PREFER_SPEEX_DENOISER)
#	define PREFER_SPEEX_DENOISER	1
#endif

#if TDAV_UNDER_MOBILE
#	include <webrtc/echo_control_mobile.h>
#	define	TDAV_WebRtcAec_Create(aecInst)	WebRtcAecm_Create(aecInst)
#	define	TDAV_WebRtcAec_Free(aecInst)	WebRtcAecm_Free(aecInst)
#	define	TDAV_WebRtcAec_Init(aecInst, sampFreq, scSampFreq)		WebRtcAecm_Init(aecInst, sampFreq)
#	define	TDAV_WebRtcAec_BufferFarend(aecInst, farend, nrOfSamples)	WebRtcAecm_BufferFarend(aecInst, farend, nrOfSamples)
#	define	TDAV_WebRtcAec_Process(aecInst, nearend, nearendH, out, outH, nrOfSamples, msInSndCardBuf, skew)	WebRtcAecm_Process(aecInst, nearend, nearend, out, nrOfSamples, msInSndCardBuf)
#else
#	include <webrtc/echo_cancellation.h>
#	define	TDAV_WebRtcAec_Create(aecInst)	WebRtcAec_Create(aecInst)
#	define	TDAV_WebRtcAec_Free(aecInst)	WebRtcAec_Free(aecInst)
#	define	TDAV_WebRtcAec_Init(aecInst, sampFreq, scSampFreq)	WebRtcAec_Init(aecInst, sampFreq, scSampFreq)
#	define	TDAV_WebRtcAec_BufferFarend(aecInst, farend, nrOfSamples)	WebRtcAec_BufferFarend(aecInst, farend, nrOfSamples)
#	define	TDAV_WebRtcAec_Process(aecInst, nearend, nearendH, out, outH, nrOfSamples, msInSndCardBuf, skew)	WebRtcAec_Process(aecInst, nearend, nearendH, out, outH, nrOfSamples, msInSndCardBuf, skew)
#endif

#if HAVE_SPEEX_DSP && PREFER_SPEEX_DENOISER
#	include <speex/speex_preprocess.h>
#else
#	if TDAV_UNDER_MOBILE // Use fixed implementation for Noise Suppression
#		include <webrtc/noise_suppression_x.h>
#		define	TDAV_WebRtcNs_Process(NS_inst, spframe, spframe_H, outframe, outframe_H) WebRtcNsx_Process(NS_inst, spframe, spframe_H, outframe, outframe_H)
#		define	TDAV_WebRtcNs_Init(NS_inst, fs) WebRtcNsx_Init(NS_inst, fs)
#		define	TDAV_WebRtcNs_Free(NS_inst) WebRtcNsx_Free(NS_inst)
#		define	TDAV_WebRtcNs_Create(NS_inst) WebRtcNsx_Create(NS_inst)
#		define  TDAV_NsHandle NsxHandle
#	else
#		include <webrtc/noise_suppression.h>
#		define	TDAV_WebRtcNs_Process(NS_inst, spframe, spframe_H, outframe, outframe_H) WebRtcNs_Process(NS_inst, spframe, spframe_H, outframe, outframe_H)
#		define	TDAV_WebRtcNs_Init(NS_inst, fs) WebRtcNs_Init(NS_inst, fs)
#		define	TDAV_WebRtcNs_Free(NS_inst) WebRtcNs_Free(NS_inst)
#		define	TDAV_WebRtcNs_Create(NS_inst) WebRtcNs_Create(NS_inst)
#		define  TDAV_NsHandle NsHandle
#	endif
#endif

TDAV_BEGIN_DECLS

extern const tmedia_denoise_plugin_def_t *tdav_webrtc_denoise_plugin_def_t;

TDAV_END_DECLS

#endif /* #if HAVE_WEBRTC */

#endif /* TINYDAV_WEBRTC_DENOISE_H */
