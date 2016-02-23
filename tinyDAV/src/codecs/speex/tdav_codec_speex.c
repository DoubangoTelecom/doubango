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

/**@file tdav_codec_speex.c
 * @brief Speex codecs
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinydav/codecs/speex/tdav_codec_speex.h"

#if HAVE_LIB_SPEEX

#include "tsk_memory.h"
#include "tsk_debug.h"

#define SPEEX_BUFFER_MAX_SIZE		1024
#define SPEEX_DEFAULT_QUALITY		6

/* ============ Common ================= */
int tdav_codec_speex_init(tdav_codec_speex_t* self, tdav_codec_speex_type_t type);
int tdav_codec_speex_deinit(tdav_codec_speex_t* self);

/* ============ Speex Plugin interface ================= */

int tdav_codec_speex_open(tmedia_codec_t* self)
{
    static int quality = SPEEX_DEFAULT_QUALITY;
    tdav_codec_speex_t* speex = (tdav_codec_speex_t*)self;

    switch(speex->type) {
    case tdav_codec_speex_type_nb:
        speex->encoder.state = speex_encoder_init(&speex_nb_mode);
        speex->decoder.state = speex_decoder_init(&speex_nb_mode);
        break;
    case tdav_codec_speex_type_wb:
        speex->encoder.state = speex_encoder_init(&speex_wb_mode);
        speex->decoder.state = speex_decoder_init(&speex_wb_mode);
        break;
    case tdav_codec_speex_type_uwb:
        speex->encoder.state = speex_encoder_init(&speex_uwb_mode);
        speex->decoder.state = speex_decoder_init(&speex_uwb_mode);
        break;
    default:
        TSK_DEBUG_ERROR("Not implemented");
        return -2;
    }

    speex_decoder_ctl(speex->decoder.state, SPEEX_GET_FRAME_SIZE, &speex->decoder.size);
    speex->decoder.size *= sizeof(spx_int16_t);
    if(!(speex->decoder.buffer = tsk_calloc(speex->decoder.size, 1))) {
        speex->decoder.size = speex->decoder.size = 0;
        TSK_DEBUG_ERROR("Failed to allocate new buffer");
        return -3;
    }

    speex_encoder_ctl(speex->encoder.state, SPEEX_SET_QUALITY, &quality);
    speex_encoder_ctl(speex->encoder.state, SPEEX_GET_FRAME_SIZE, &speex->encoder.size);

    speex_bits_init(&speex->encoder.bits);
    speex_bits_init(&speex->decoder.bits);
    speex_bits_reset(&speex->encoder.bits);
    speex_bits_reset(&speex->decoder.bits);

    return 0;
}

int tdav_codec_speex_close(tmedia_codec_t* self)
{
    tdav_codec_speex_t* speex = (tdav_codec_speex_t*)self;

    (void)(speex);

    return 0;
}

tsk_size_t tdav_codec_speex_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
    tdav_codec_speex_t* speex = (tdav_codec_speex_t*)self;
    tsk_size_t outsize = 0;

    if(!self || !in_data || !in_size || !out_data) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    speex_bits_reset(&speex->encoder.bits);
    speex_encode_int(speex->encoder.state, (spx_int16_t*)in_data, &speex->encoder.bits);

    if(*out_max_size <speex->encoder.size) {
        if((*out_data = tsk_realloc(*out_data, speex->encoder.size))) {
            *out_max_size = speex->encoder.size;
        }
        else {
            *out_max_size = 0;
            return 0;
        }
    }

    outsize = speex_bits_write(&speex->encoder.bits, *out_data, (speex->encoder.size >> 1));

    return outsize;
}

tsk_size_t tdav_codec_speex_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
    int ret;
    tsk_size_t out_size = 0;
    tdav_codec_speex_t* speex = (tdav_codec_speex_t*)self;

    if(!self || !in_data || !in_size || !out_data) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    // initializes the bit-stream
    speex_bits_read_from(&speex->decoder.bits, (char*)in_data, in_size);

    do {
        // performs decode()
        if((ret = speex_decode_int(speex->decoder.state, &speex->decoder.bits, speex->decoder.buffer))) {
            TSK_DEBUG_ERROR("Failed to decode the buffer. retcode=%d", ret);
            break;
        }

        if(*out_max_size <(out_size + speex->decoder.size)) {
            if((*out_data = tsk_realloc(*out_data, (out_size + speex->decoder.size)))) {
                *out_max_size = (out_size + speex->decoder.size);
            }
            else {
                *out_max_size = 0;
                return 0;
            }
        }

        // copy output buffer
        memcpy(&((uint8_t*)*out_data)[out_size], speex->decoder.buffer, speex->decoder.size);
        out_size += speex->decoder.size;
    }
    while(speex_bits_remaining(&speex->decoder.bits) >= 5);


    return out_size;
}

char* tdav_codec_speex_sdp_att_get(const tmedia_codec_t* codec, const char* att_name)
{
    return tsk_null;
}

tsk_bool_t tdav_codec_speex_sdp_att_match(const tmedia_codec_t* codec, const char* att_name, const char* att_value)
{
    return tsk_true;
}


//
//	Speex Codec Object definition
//
#define SPEEX_OBJECT_DEFINITION(mode,name,description,format,rate) \
	static tsk_object_t* tdav_codec_speex_##mode##_ctor(tsk_object_t * self, va_list * app) \
	{ \
		tdav_codec_speex_t *speex = self; \
		if(speex){ \
			tdav_codec_speex_init(speex, tdav_codec_speex_type_##mode); \
		} \
		return self; \
	} \
	static tsk_object_t* tdav_codec_speex_##mode##_dtor(tsk_object_t * self) \
	{  \
		tdav_codec_speex_t *speex = self; \
		if(speex){ \
			/* deinit base */ \
			tmedia_codec_audio_deinit(speex); \
			/* deinit self */ \
			tdav_codec_speex_deinit(speex); \
		} \
	 \
		return self; \
	} \
	static const tsk_object_def_t tdav_codec_speex_##mode##_def_s = \
	{ \
		sizeof(tdav_codec_speex_t), \
		tdav_codec_speex_##mode##_ctor,  \
		tdav_codec_speex_##mode##_dtor, \
		tmedia_codec_cmp,  \
	}; \
	static const tmedia_codec_plugin_def_t tdav_codec_speex_##mode##_plugin_def_s =  \
	{ \
	&tdav_codec_speex_##mode##_def_s, \
	 \
		tmedia_audio, \
		tmedia_codec_id_speex_##mode, \
		name, \
		description, \
		format, \
		tsk_true, \
		rate, /* rate*/ \
		 \
		{ /* audio */ \
			1, /* channels*/ \
			0 /* ptime @deprecated*/ \
		}, \
	 \
		/* video */ \
		{0}, \
	 \
		tsk_null, /* set()*/ \
		tdav_codec_speex_open, \
		tdav_codec_speex_close, \
		tdav_codec_speex_encode, \
		tdav_codec_speex_decode, \
		tdav_codec_speex_sdp_att_match, \
		tdav_codec_speex_sdp_att_get \
	}; \
	const tmedia_codec_plugin_def_t *tdav_codec_speex_##mode##_plugin_def_t = &tdav_codec_speex_##mode##_plugin_def_s;


SPEEX_OBJECT_DEFINITION(nb,"SPEEX","Speex-NB Codec",TMEDIA_CODEC_FORMAT_SPEEX_NB,8000);
SPEEX_OBJECT_DEFINITION(wb,"SPEEX","Speex-WB Codec",TMEDIA_CODEC_FORMAT_SPEEX_WB,16000);
SPEEX_OBJECT_DEFINITION(uwb,"SPEEX","Speex-UWB Codec",TMEDIA_CODEC_FORMAT_SPEEX_UWB,32000);

//
// Common functions
//
int tdav_codec_speex_init(tdav_codec_speex_t* self, tdav_codec_speex_type_t type)
{
    if(self) {
        self->type = type;
        return 0;
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
}

int tdav_codec_speex_deinit(tdav_codec_speex_t* self)
{
    if(self) {
        if(self->decoder.state) {
            speex_decoder_destroy(self->decoder.state);
            self->decoder.state = tsk_null;
        }
        speex_bits_destroy(&self->decoder.bits);
        if(self->decoder.buffer) {
            TSK_FREE(self->decoder.buffer);
            self->decoder.size = 0;
        }

        if(self->encoder.state) {
            speex_encoder_destroy(self->encoder.state);
            self->encoder.state = tsk_null;
        }
        speex_bits_destroy(&self->encoder.bits);
        self->encoder.size = 0;

        return 0;
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
}

#endif /* HAVE_LIB_SPEEX */
