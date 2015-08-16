/* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
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
#ifndef DOUBANGO_AUDIO_WEBRTC_CONSUMER_H
#define DOUBANGO_AUDIO_WEBRTC_CONSUMER_H

#include "audio_webrtc_config.h"

DOUBANGO_AUDIO_WEBRTC_BEGIN_DECLS

extern const struct tmedia_consumer_plugin_def_s *audio_consumer_webrtc_plugin_def_t; 

int audio_consumer_webrtc_get_data_10ms(const struct audio_consumer_webrtc_s* self, void* audioSamples, int nSamples, int nBytesPerSample, int nChannels, int samplesPerSec, uint32_t &nSamplesOut);

DOUBANGO_AUDIO_WEBRTC_END_DECLS

#endif /* DOUBANGO_AUDIO_WEBRTC_CONSUMER_H */


