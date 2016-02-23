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
#ifndef DOUBANGO_AUDIO_WEBRTC_PRODUCER_H
#define DOUBANGO_AUDIO_WEBRTC_PRODUCER_H

#include "audio_webrtc_config.h"

DOUBANGO_AUDIO_WEBRTC_BEGIN_DECLS

extern const struct tmedia_producer_plugin_def_s *audio_producer_webrtc_plugin_def_t;

// handle recorded data
int audio_producer_webrtc_handle_data_10ms(const struct audio_producer_webrtc_s* self, const void* audioSamples, int nSamples, int nBytesPerSample, int samplesPerSec, int nChannels);

DOUBANGO_AUDIO_WEBRTC_END_DECLS

#endif /* DOUBANGO_AUDIO_WEBRTC_PRODUCER_H */
