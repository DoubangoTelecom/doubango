/*
* Copyright (C) 2010-2013 Doubango Telecom <http://www.doubango.org>.
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

/**@file tdav_codec_opus.c
 * @brief OPUS audio codec.
 * SDP: http://tools.ietf.org/html/draft-spittka-payload-rtp-opus-03
 */
#include "tinydav/codecs/opus/tdav_codec_opus.h"

#if HAVE_LIBOPUS

#include <opus/opus.h>

#include "tinymedia/tmedia_defaults.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tsk_params.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#if !defined(TDAV_OPUS_MAX_FRAME_SIZE_IN_SAMPLES)
#	define TDAV_OPUS_MAX_FRAME_SIZE_IN_SAMPLES (5760) /* 120ms@48kHz */
#endif
#if !defined(TDAV_OPUS_MAX_FRAME_SIZE_IN_BYTES)
#	define TDAV_OPUS_MAX_FRAME_SIZE_IN_BYTES (TDAV_OPUS_MAX_FRAME_SIZE_IN_SAMPLES << 1) /* 120ms@48kHz */
#endif
#if !defined(TDAV_OPUS_FEC_ENABLED)
#	define TDAV_OPUS_FEC_ENABLED	0
#endif
#if !defined(TDAV_OPUS_DTX_ENABLED)
#	define TDAV_OPUS_DTX_ENABLED	0
#endif

typedef struct tdav_codec_opus_s {
    TMEDIA_DECLARE_CODEC_AUDIO;

    struct {
        OpusEncoder *inst;
    } encoder;

    struct {
        OpusDecoder *inst;
        opus_int16 buff[TDAV_OPUS_MAX_FRAME_SIZE_IN_SAMPLES];
        tsk_bool_t fec_enabled;
        tsk_bool_t dtx_enabled;
        uint16_t last_seq;
    } decoder;
}
tdav_codec_opus_t;


static tsk_bool_t _tdav_codec_opus_rate_is_valid(const int32_t rate)
{
    switch(rate) {
    case 8000:
    case 12000:
    case 16000:
    case 24000:
    case 48000:
        return tsk_true;
    default:
        return tsk_false;
    }
}

static int tdav_codec_opus_open(tmedia_codec_t* self)
{
    tdav_codec_opus_t* opus = (tdav_codec_opus_t*)self;
    int opus_err;

    if(!opus) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // Initialize the decoder
    if(!opus->decoder.inst) {
        TSK_DEBUG_INFO("[OPUS] Open decoder: rate=%d, channels=%d", (int)self->in.rate, (int)TMEDIA_CODEC_AUDIO(self)->in.channels);
        if(!(opus->decoder.inst = opus_decoder_create((opus_int32)self->in.rate, (int)TMEDIA_CODEC_AUDIO(self)->in.channels, &opus_err)) || opus_err != OPUS_OK) {
            TSK_DEBUG_ERROR("Failed to create Opus decoder(rate=%d, channels=%d) instance with error code=%d.", (int)self->in.rate, (int)TMEDIA_CODEC_AUDIO(self)->in.channels, opus_err);
            return -2;
        }
    }
    opus->decoder.last_seq  = 0;

    // Initialize the encoder
    if(!opus->encoder.inst) {
        TSK_DEBUG_INFO("[OPUS] Open encoder: rate=%d, channels=%d", (int)self->out.rate, (int)TMEDIA_CODEC_AUDIO(self)->out.channels);
        if(!(opus->encoder.inst = opus_encoder_create((opus_int32)self->out.rate, (int)TMEDIA_CODEC_AUDIO(self)->out.channels, OPUS_APPLICATION_VOIP, &opus_err)) || opus_err != OPUS_OK) {
            TSK_DEBUG_ERROR("Failed to create Opus decoder(rate=%d, channels=%d) instance with error code=%d.", (int)self->out.rate, (int)TMEDIA_CODEC_AUDIO(self)->out.channels, opus_err);
            return -2;
        }
    }
#if TDAV_UNDER_MOBILE /* iOS, Android and WP8 */
    opus_encoder_ctl(opus->encoder.inst, OPUS_SET_COMPLEXITY(3));
#endif
    opus_encoder_ctl(opus->encoder.inst, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));

    return 0;
}

static int tdav_codec_opus_close(tmedia_codec_t* self)
{
    tdav_codec_opus_t* opus = (tdav_codec_opus_t*)self;

    (void)(opus);

    /* resources will be freed by the dctor() */

    return 0;
}

static tsk_size_t tdav_codec_opus_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
    tdav_codec_opus_t* opus = (tdav_codec_opus_t*)self;
    opus_int32 ret;

    if(!self || !in_data || !in_size || !out_data) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    if(!opus->encoder.inst) {
        TSK_DEBUG_ERROR("Encoder not ready");
        return 0;
    }

    // we're sure that the output (encoded) size cannot be higher than the input (raw)
    if(*out_max_size < in_size) {
        if(!(*out_data = tsk_realloc(*out_data, in_size))) {
            TSK_DEBUG_ERROR("Failed to allocate buffer with size = %u", in_size);
            *out_max_size  = 0;
            return 0;
        }
        *out_max_size = in_size;
    }

    ret = opus_encode(opus->encoder.inst,
                      (const opus_int16 *)in_data, (int)(in_size >> 1),
                      (unsigned char *)*out_data, (opus_int32)*out_max_size);

    if(ret < 0) {
        TSK_DEBUG_ERROR("opus_encode() failed with error code = %d", ret);
        return 0;
    }

    return (tsk_size_t)ret;
}

static tsk_size_t tdav_codec_opus_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
    tdav_codec_opus_t* opus = (tdav_codec_opus_t*)self;
    int frame_size;
    const trtp_rtp_header_t* rtp_hdr = proto_hdr;

    if(!self || !in_data || !in_size || !out_data) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    if(!opus->decoder.inst) {
        TSK_DEBUG_ERROR("Decoder not ready");
        return 0;
    }

    /* Packet loss? */
    if(opus->decoder.last_seq != (rtp_hdr->seq_num - 1) && opus->decoder.last_seq) {
        if(opus->decoder.last_seq == rtp_hdr->seq_num) {
            // Could happen on some stupid emulators
            //TSK_DEBUG_INFO("Packet duplicated, seq_num=%d", rtp_hdr->seq_num);
            return 0;
        }
        TSK_DEBUG_INFO("[Opus] Packet loss, seq_num=%d", rtp_hdr->seq_num);
        opus_decode(opus->decoder.inst, tsk_null/*packet loss*/, (opus_int32)0, opus->decoder.buff, TDAV_OPUS_MAX_FRAME_SIZE_IN_SAMPLES, opus->decoder.fec_enabled);
    }
    opus->decoder.last_seq = rtp_hdr->seq_num;

    frame_size = opus_decode(opus->decoder.inst, (const unsigned char *)in_data, (opus_int32)in_size, opus->decoder.buff, TDAV_OPUS_MAX_FRAME_SIZE_IN_SAMPLES, opus->decoder.fec_enabled ? 1 : 0);
    if(frame_size > 0) {
        tsk_size_t frame_size_inbytes = (frame_size << 1);
        if(*out_max_size < frame_size_inbytes) {
            if(!(*out_data = tsk_realloc(*out_data, frame_size_inbytes))) {
                TSK_DEBUG_ERROR("Failed to allocate new buffer");
                *out_max_size = 0;
                return 0;
            }
            *out_max_size = frame_size_inbytes;
        }
        memcpy(*out_data, opus->decoder.buff, frame_size_inbytes);
        return frame_size_inbytes;
    }
    else {
        return 0;
    }
}

static tsk_bool_t tdav_codec_opus_sdp_att_match(const tmedia_codec_t* codec, const char* att_name, const char* att_value)
{
    tdav_codec_opus_t* opus = (tdav_codec_opus_t*)codec;

    if(!opus) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_false;
    }

    TSK_DEBUG_INFO("[OPUS] Trying to match [%s:%s]", att_name, att_value);

    if(tsk_striequals(att_name, "fmtp")) {
        int val_int;
        tsk_params_L_t* params;
        /* e.g. FIXME */
        if((params = tsk_params_fromstring(att_value, ";", tsk_true))) {
            tsk_bool_t ret = tsk_false;
            /* === maxplaybackrate ===*/
            if((val_int = tsk_params_get_param_value_as_int(params, "maxplaybackrate")) != -1) {
                if(!_tdav_codec_opus_rate_is_valid(val_int)) {
                    TSK_DEBUG_ERROR("[OPUS] %d not valid as maxplaybackrate value", val_int);
                    goto done;
                }
                TMEDIA_CODEC(opus)->out.rate = TSK_MIN((int32_t)TMEDIA_CODEC(opus)->out.rate, val_int);
                TMEDIA_CODEC_AUDIO(opus)->out.timestamp_multiplier = tmedia_codec_audio_get_timestamp_multiplier(codec->id, codec->out.rate);
            }
            /* === sprop-maxcapturerate ===*/
            if((val_int = tsk_params_get_param_value_as_int(params, "sprop-maxcapturerate")) != -1) {
                if(!_tdav_codec_opus_rate_is_valid(val_int)) {
                    TSK_DEBUG_ERROR("[OPUS] %d not valid as sprop-maxcapturerate value", val_int);
                    goto done;
                }
                TMEDIA_CODEC(opus)->in.rate = TSK_MIN((int32_t)TMEDIA_CODEC(opus)->in.rate, val_int);
                TMEDIA_CODEC_AUDIO(opus)->in.timestamp_multiplier = tmedia_codec_audio_get_timestamp_multiplier(codec->id, codec->in.rate);
            }
            ret = tsk_true;
done:
            TSK_OBJECT_SAFE_FREE(params);
            return ret;
        }
    }

    return tsk_true;
}

static char* tdav_codec_opus_sdp_att_get(const tmedia_codec_t* codec, const char* att_name)
{
    tdav_codec_opus_t* opus = (tdav_codec_opus_t*)codec;

    if(!opus) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    if(tsk_striequals(att_name, "fmtp")) {
        char* fmtp = tsk_null;
        tsk_sprintf(&fmtp, "maxplaybackrate=%d; sprop-maxcapturerate=%d; stereo=%d; sprop-stereo=%d; useinbandfec=%d; usedtx=%d",
                    TMEDIA_CODEC(opus)->in.rate,
                    TMEDIA_CODEC(opus)->out.rate,
                    (TMEDIA_CODEC_AUDIO(opus)->in.channels == 2) ? 1 : 0,
                    (TMEDIA_CODEC_AUDIO(opus)->out.channels == 2) ? 1 : 0,
                    opus->decoder.fec_enabled ? 1 : 0,
                    opus->decoder.dtx_enabled ? 1 : 0
                   );
        return fmtp;
    }

    return tsk_null;
}

//
//	OPUS Plugin definition
//

/* constructor */
static tsk_object_t* tdav_codec_opus_ctor(tsk_object_t * self, va_list * app)
{
    tdav_codec_opus_t *opus = self;
    if(opus) {
        /* init base: called by tmedia_codec_create() */
        /* init self */
        TMEDIA_CODEC(opus)->in.rate = tmedia_defaults_get_opus_maxplaybackrate();
        TMEDIA_CODEC(opus)->out.rate = tmedia_defaults_get_opus_maxcapturerate();
        TMEDIA_CODEC_AUDIO(opus)->in.channels = 1;
        TMEDIA_CODEC_AUDIO(opus)->out.channels = 1;
#if TDAV_OPUS_FEC_ENABLED
        opus->decoder.fec_enabled = tsk_true;
#endif
#if TDAV_OPUS_DTX_ENABLED
        opus->decoder.dtx_enabled = tsk_true;
#endif
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_codec_opus_dtor(tsk_object_t * self)
{
    tdav_codec_opus_t *opus = self;
    if(opus) {
        /* deinit base */
        tmedia_codec_audio_deinit(opus);
        /* deinit self */
        if(opus->decoder.inst) {
            opus_decoder_destroy(opus->decoder.inst), opus->decoder.inst = tsk_null;
        }
        if(opus->encoder.inst) {
            opus_encoder_destroy(opus->encoder.inst), opus->encoder.inst = tsk_null;
        }
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_opus_def_s = {
    sizeof(tdav_codec_opus_t),
    tdav_codec_opus_ctor,
    tdav_codec_opus_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_opus_plugin_def_s = {
    &tdav_codec_opus_def_s,

    tmedia_audio,
    tmedia_codec_id_opus,
    "opus",
    "opus Codec",
    TMEDIA_CODEC_FORMAT_OPUS,
    tsk_true,
    48000, // this is the default sample rate

    { /* audio */
        2, // channels
        0 // ptime @deprecated
    },

    /* video */
    {0},

    tsk_null, // set()
    tdav_codec_opus_open,
    tdav_codec_opus_close,
    tdav_codec_opus_encode,
    tdav_codec_opus_decode,
    tdav_codec_opus_sdp_att_match,
    tdav_codec_opus_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_opus_plugin_def_t = &tdav_codec_opus_plugin_def_s;


#endif /* HAVE_LIBOPUS */
