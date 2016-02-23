/*
* Copyright (C) 2010-2011 Mamadou Diop.
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

/**@file tinymedia.h
 * @brief tinyMEDIA API.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYMEDIA_TINYMEDIA_H
#define TINYMEDIA_TINYMEDIA_H

/* == tinyMEDIA == */
#include "tinymedia/tmedia_codec_dummy.h"
#include "tinymedia/tmedia_session_dummy.h"
#include "tinymedia/tmedia_session_ghost.h"
#include "tinymedia/tmedia_jitterbuffer.h"
#include "tinymedia/tmedia_resampler.h"
#include "tinymedia/tmedia_denoise.h"
#include "tinymedia/tmedia_imageattr.h"

#include "tinymedia/tmedia_consumer.h"
#include "tinymedia/tmedia_producer.h"

#include "tinymedia/tmedia_common.h"
#include "tinymedia/tmedia_qos.h"
#include "tinymedia/tmedia_defaults.h"

#include "tinymedia/content/tmedia_content.h"
#include "tinymedia/content/tmedia_content_cpim.h"
#include "tinymedia/content/tmedia_content_multipart.h"
#include "tinymedia/content/tmedia_content_sip_frag.h"

/* == tinyNET == */
#include "tinynet.h"

/* == tinySDP == */
#include "tinysdp.h"

#endif /* TINYMEDIA_TINYMEDIA_H */
