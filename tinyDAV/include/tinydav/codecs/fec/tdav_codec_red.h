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

/**@file tdav_codec_red.h
 * @brief RTP Payload for Redundant Audio Data as per RFC 2198
 */
#ifndef TINYDAV_CODEC_RED_H
#define TINYDAV_CODEC_RED_H

#include "tinydav_config.h"

#include "tinymedia/tmedia_codec.h"

TDAV_BEGIN_DECLS

#define TDAV_CODEC_RED_MIN_PKT_SIZE	4

struct trtp_rtp_packet_s;
struct tdav_codec_red_s;

/** callback for redundant data */
typedef int (*tdav_codec_red_rtppacket_cb_f)(const void* callback_data, const struct trtp_rtp_packet_s* packet);

int tdav_codec_red_set_callback(struct tdav_codec_red_s *self, tdav_codec_red_rtppacket_cb_f callback, const void* callback_data);

TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_red_plugin_def_t;

TDAV_END_DECLS

#endif /* TINYDAV_CODEC_RED_H */
