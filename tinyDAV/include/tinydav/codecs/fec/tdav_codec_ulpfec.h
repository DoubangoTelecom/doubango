/*
* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tdav_codec_ulpfec.h
 * @brief Forward Error Correction (FEC) implementation as per RFC 5109
 */
#ifndef TINYDAV_CODEC_ULPFEC_H
#define TINYDAV_CODEC_ULPFEC_H

#include "tinydav_config.h"

#include "tinymedia/tmedia_codec.h"

TDAV_BEGIN_DECLS

struct tdav_codec_ulpfec_s;
struct trtp_rtp_packet_s;

int tdav_codec_ulpfec_enc_reset(struct tdav_codec_ulpfec_s* self);
int tdav_codec_ulpfec_enc_protect(struct tdav_codec_ulpfec_s* self, const struct trtp_rtp_packet_s* rtp_packet);
tsk_size_t tdav_codec_ulpfec_enc_serialize(const struct tdav_codec_ulpfec_s* self, void** out_data, tsk_size_t* out_max_size);

TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_ulpfec_plugin_def_t;

TDAV_END_DECLS

#endif /* TINYDAV_CODEC_ULPFEC_H */