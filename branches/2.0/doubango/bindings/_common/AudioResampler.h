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

/**@file AudioResampler.h
 * @brief Audio resampler
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 */
#ifndef TINYWRAP_AUDIO_RESAMPLER_H
#define TINYWRAP_AUDIO_RESAMPLER_H

#include "tinyWRAP_config.h"
#include "tsk_common.h"

class AudioResampler
{
public:
	AudioResampler(uint32_t nInFreq, uint32_t nOutFreq, uint32_t nFrameDuration, uint32_t nChannels, uint32_t nQuality);
	~AudioResampler();

public:
	inline bool isValid(){ return (m_pWrappedResampler != tsk_null); }
	inline uint32_t getOutputRequiredSizeInShort(){ return (m_nOutFreq * m_nFrameDuration)/1000; }
	inline uint32_t getInputRequiredSizeInShort(){ return (m_nInFreq * m_nFrameDuration)/1000; }
	uint32_t process(const void* pInData, uint32_t nInSizeInBytes, void* pOutData, uint32_t nOutSizeInBytes);

private:
	struct tmedia_resampler_s* m_pWrappedResampler;
	uint32_t m_nOutFreq;
	uint32_t m_nInFreq;
	uint32_t m_nFrameDuration;
	uint32_t m_nChannels;
	uint32_t m_nQuality;
};


#endif /* TINYWRAP_AUDIO_RESAMPLER_H */
