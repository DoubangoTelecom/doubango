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
#ifndef _DOUBANGO_AUDIOOPENSLES_CONSUMER_H
#define _DOUBANGO_AUDIOOPENSLES_CONSUMER_H

#include "audio_opensles_config.h"

AUDIO_OPENSLES_BEGIN_DECLS

extern const struct tmedia_consumer_plugin_def_s *audio_consumer_opensles_plugin_def_t;

int audio_consumer_opensles_get_data_10ms(const struct audio_consumer_opensles_s* self, void* audioSamples, int nSamples, int nBytesPerSample, int nChannels, int samplesPerSec, uint32_t &nSamplesOut);
bool audio_consumer_opensles_is_speakerOn(const struct audio_consumer_opensles_s* self);

AUDIO_OPENSLES_END_DECLS

#endif /* _DOUBANGO_AUDIOOPENSLES_CONSUMER_H */
