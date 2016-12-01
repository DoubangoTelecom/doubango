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

/**@file tmedia_codec.c
 * @brief Base codec object.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinymedia/tmedia_codec.h"
#include "tinymedia/tmedia_defaults.h"

#include "tinysdp/headers/tsdp_header_M.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <limits.h> /* INT_MAX */

/**@defgroup tmedia_codec_group Codecs
*/

/* pointer to all registered codecs */
const tmedia_codec_plugin_def_t* __tmedia_codec_plugins[TMED_CODEC_MAX_PLUGINS] = {0};


/*== Predicate function to find a codec object by format */
static int __pred_find_codec_by_format(const tsk_list_item_t *item, const void *format)
{
    if(item && item->data) {
        return tsk_strcmp(((tmedia_codec_t *)item->data)->format, format);
    }
    return -1;
}

/*== Predicate function to find a codec object by negociated format */
static int __pred_find_codec_by_neg_format(const tsk_list_item_t *item, const void *format)
{
    if(item && item->data) {
        return tsk_strcmp(((tmedia_codec_t *)item->data)->neg_format, format);
    }
    return -1;
}

/**@ingroup tmedia_codec_group
* Initialize a Codec
* @param self The codec to initialize. Could be any type of codec (e.g. @ref tmedia_codec_audio_t or @ref tmedia_codec_video_t).
* @param type
* @param name the name of the codec. e.g. "G.711u" or "G.711a" etc used in the sdp.
* @param desc full description.
* @param format the format. e.g. "0" for G.711.u or "8" for G.711a or "*" for MSRP.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tmedia_codec_init(tmedia_codec_t* self, tmedia_type_t type, const char* name, const char* desc, const char* format)
{
    if(!self || tsk_strnullORempty(name)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->type = type;
    tsk_strupdate(&self->name, name);
    tsk_strupdate(&self->desc,desc);
    tsk_strupdate(&self->format, format);
    if(!self->bandwidth_max_upload) {
        self->bandwidth_max_upload = (type == tmedia_video ? tmedia_defaults_get_bandwidth_video_upload_max() : INT_MAX);    // INT_MAX or <=0 means undefined
    }
    if(!self->bandwidth_max_download) {
        self->bandwidth_max_download = (type == tmedia_video ? tmedia_defaults_get_bandwidth_video_download_max() : INT_MAX);    // INT_MAX or <=0 means undefined
    }
    if(!self->in.rate) {
        self->in.rate = self->plugin->rate;
    }
    if(!self->out.rate) {
        self->out.rate = self->plugin->rate;
    }

    if(type & tmedia_audio) {
        tmedia_codec_audio_t* audio = TMEDIA_CODEC_AUDIO(self);
        if(!audio->in.ptime) {
            audio->in.ptime = (self->plugin->audio.ptime ? self->plugin->audio.ptime : tmedia_defaults_get_audio_ptime());
        }
        if(!audio->out.ptime) {
            audio->out.ptime = (self->plugin->audio.ptime ? self->plugin->audio.ptime : tmedia_defaults_get_audio_ptime());
        }
        if(!audio->in.channels) {
            audio->in.channels = self->plugin->audio.channels;
        }
        if(!audio->out.channels) {
            audio->out.channels = self->plugin->audio.channels;
        }
        if(!audio->in.timestamp_multiplier) {
            audio->in.timestamp_multiplier = tmedia_codec_audio_get_timestamp_multiplier(self->id, self->in.rate);
        }
        if(!audio->out.timestamp_multiplier) {
            audio->out.timestamp_multiplier = tmedia_codec_audio_get_timestamp_multiplier(self->id, self->out.rate);
        }
    }
    // Video flipping: For backward compatibility we have to initialize the default values
    // according to the CFLAGS: 'FLIP_ENCODED_PICT' and 'FLIP_DECODED_PICT'. At any time you
    // can update thse values (e.g. when the device switch from landscape to portrait) using video_session->set();
    else if(type & tmedia_video) {
        tmedia_codec_video_t* video = TMEDIA_CODEC_VIDEO(self);
#if FLIP_ENCODED_PICT
        video->out.flip = tsk_true;
#endif
#if FLIP_DECODED_PICT
        video->in.flip = tsk_true;
#endif
        if(!video->in.fps) {
            video->in.fps = self->plugin->video.fps ? self->plugin->video.fps : tmedia_defaults_get_video_fps();
        }
        if(!video->out.fps) {
            video->out.fps = self->plugin->video.fps ? self->plugin->video.fps : tmedia_defaults_get_video_fps();
        }
        if(video->in.chroma == tmedia_chroma_none) {
            video->in.chroma = tmedia_chroma_yuv420p;
        }
        if(video->out.chroma == tmedia_chroma_none) {
            video->out.chroma = tmedia_chroma_yuv420p;
        }

        if(0) { // @deprecated
            if(!video->in.width) {
                video->in.width = video->out.width = self->plugin->video.width;
            }
            if(!video->in.height) {
                video->in.height = video->out.height = self->plugin->video.height;
            }
        }
        else {
            int ret;
            unsigned width, height;
            video->pref_size = tmedia_defaults_get_pref_video_size();
            if((ret = tmedia_video_get_size(video->pref_size, &width, &height)) != 0) {
                width = self->plugin->video.width;
                height = self->plugin->video.height;
            }
            if(!video->in.width) {
                video->in.width = video->out.width = width;
            }
            if(!video->in.height) {
                video->in.height = video->out.height = height;
            }
        }

    }

    return 0;
}

int tmedia_codec_set(tmedia_codec_t* self, const struct tmedia_param_s* param)
{
    if(self && self->plugin && self->plugin->set && param) {
        return self->plugin->set(self, param);
    }
    return 0;
}

/**@ingroup tmedia_codec_group
* Prepares a codec by opening it.
* @param self The codec to open.
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref tmedia_codec_close()
*/
int tmedia_codec_open(tmedia_codec_t* self)
{
    if(!self || !self->plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(self->opened) {
        TSK_DEBUG_WARN("Codec already opened");
        return 0;
    }

    if(self->plugin->open) {
        int ret;
        if((ret = self->plugin->open(self))) {
            TSK_DEBUG_ERROR("Failed to open [%s] codec", self->plugin->desc);
            return ret;
        }
        else {
            self->opened = tsk_true;
            return 0;
        }
    }
    else {
        self->opened = tsk_true;
        return 0;
    }
}

/**@ingroup tmedia_codec_group
* UnPrepares a codec by closing it.
* @param self The codec to close.
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref tmedia_codec_open()
*/
int tmedia_codec_close(tmedia_codec_t* self)
{
    if(!self || !self->plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(!self->opened) {
        return 0;
    }

    if(self->plugin->close) {
        int ret;

        if((ret = self->plugin->close(self))) {
            TSK_DEBUG_ERROR("Failed to close [%s] codec", self->plugin->desc);
            return ret;
        }
        else {
            self->opened = tsk_false;
            return 0;
        }
    }
    else {
        self->opened = tsk_false;
        return 0;
    }
}

/**@ingroup tmedia_codec_group
* Generic function to compare two codecs.
* @param codec1 The first codec to compare.
* @param codec2 The second codec to compare.
* @retval Returns an integral value indicating the relationship between the two codecs:
* <0 : @a codec1 less than @a codec2.<br>
* 0  : @a codec1 identical to @a codec2.<br>
* >0 : @a codec1 greater than @a codec2.<br>
*/
int tmedia_codec_cmp(const tsk_object_t* codec1, const tsk_object_t* codec2)
{
    const tmedia_codec_t* _c1 = codec1;
    const tmedia_codec_t* _c2 = codec2;

    if((_c1 && _c2) && (_c1->type == _c2->type)) {
        /* Do not compare names. For example, H264 base profile 1.0 will have the
        * same name than H264 base profile 3.0. */
        return tsk_stricmp(_c1->format, _c2->format);
    }
    else {
        return -1;
    }
}

/**@ingroup tmedia_codec_group
* Registers a codec plugin.
* @param plugin the definition of the plugin.
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref tmedia_codec_create()
*/
int tmedia_codec_plugin_register(const tmedia_codec_plugin_def_t* plugin)
{
    tsk_size_t i;
    if(!plugin || tsk_strnullORempty(plugin->name) || tsk_strnullORempty(plugin->format)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* add or replace the plugin */
    for(i = 0; i<TMED_CODEC_MAX_PLUGINS; i++) {
        if(!__tmedia_codec_plugins[i] || (__tmedia_codec_plugins[i] == plugin)) {
            __tmedia_codec_plugins[i] = plugin;
            TSK_DEBUG_INFO("Register codec: %s, %s", plugin->name, plugin->desc);
            return 0;
        }
        if(__tmedia_codec_plugins[i]->codec_id == plugin->codec_id && plugin->codec_id != tmedia_codec_id_none) { // 'tmedia_codec_id_none' is used for fake codecs
            TSK_DEBUG_INFO("Codec Registration: '%s' ignored because '%s' already registered", plugin->desc, __tmedia_codec_plugins[i]->desc);
            return -3;
        }
    }

    TSK_DEBUG_ERROR("There are already %d plugins.", TMED_CODEC_MAX_PLUGINS);
    return -2;
}

int tmedia_codec_plugin_register_2(const tmedia_codec_plugin_def_t* plugin, int prio)
{
    tsk_size_t index = 0, max;
    tsk_bool_t already_registered = tsk_false;
    const tmedia_codec_plugin_def_t* tmp;
    if (!plugin || tsk_strnullORempty(plugin->name) || tsk_strnullORempty(plugin->format) || (prio + 1) >= TMED_CODEC_MAX_PLUGINS) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // count codecs and found if already registered
    while (__tmedia_codec_plugins[index]) {
        if (__tmedia_codec_plugins[index] == plugin) {
            already_registered = tsk_true;
        }
        ++index;
    }

    if (index >= TMED_CODEC_MAX_PLUGINS) {
        TSK_DEBUG_ERROR("No room");
        return -1;
    }

	// clamp prio (must be done here before unregistering the plugin)
	max = tmedia_codec_plugin_registered_count(__tmedia_codec_plugins, sizeof(__tmedia_codec_plugins)/sizeof(__tmedia_codec_plugins[0]));
	prio = TSK_CLAMP(0, prio, (int)(max > 0 ? (max - 1) : 0));

    // unregister and compact
    if (already_registered) {
        if (tmedia_codec_plugin_unregister(plugin) == 0) {
            --index;
        }
    }

	

    // put current plugin at prio and old (which was at prio) at the end
    tmp = __tmedia_codec_plugins[prio];
    __tmedia_codec_plugins[index] = tmp;// put old codec add prio to the end of the list
    __tmedia_codec_plugins[prio] = plugin;

    return 0;
}

/**@ingroup tmedia_codec_group
 * Checks whether a codec plugin is registered or not.
 * @param plugin the definition of the plugin to check for availability.
 * @retval 1 (tsk_true) if registered and 0 (tsk_false) otherwise.
 * @sa @ref tmedia_codec_plugin_is_registered_2() @ref tmedia_codec_plugin_register() and @ref tmedia_codec_plugin_unregister()
 */
tsk_bool_t tmedia_codec_plugin_is_registered(const tmedia_codec_plugin_def_t* plugin)
{
    if(plugin) {
        tsk_size_t i;
        for(i = 0; i < TMED_CODEC_MAX_PLUGINS && __tmedia_codec_plugins[i]; i++) {
            if(__tmedia_codec_plugins[i] == plugin) {
                return tsk_true;
            }
        }
    }
    return tsk_false;
}

/**@ingroup tmedia_codec_group
 * Checks whether a codec is registered or not.
 * @param codec_id The code id to check.
 * @return 1 @ref tsk_true if registered and tsk_false otherwise.
 * @sa @ref tmedia_codec_plugin_is_registered() @ref tmedia_codec_plugin_register() and @ref tmedia_codec_plugin_unregister()
 */
tsk_bool_t tmedia_codec_plugin_is_registered_2(tmedia_codec_id_t codec_id)
{
    return (tmedia_codec_plugin_registered_get_const(codec_id) != tsk_null);
}

/**@ingroup tmedia_codec_group
 * Gets the list of all registered plugins.
 * @param plugins List of the registered plugins.
 * @param count Number of plugins in the list.
 * @return 0 if succeed and non-zero error code otherwise.
 */
int tmedia_codec_plugin_registered_get_all(const struct tmedia_codec_plugin_def_s*(** plugins)[TMED_CODEC_MAX_PLUGINS], tsk_size_t* count)
{
    if(!plugins || !count) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    *plugins = &__tmedia_codec_plugins;
    *count = sizeof(__tmedia_codec_plugins)/sizeof(__tmedia_codec_plugins[0]);
    return 0;
}

/**@ingroup tmedia_codec_group
*/
const struct tmedia_codec_plugin_def_s* tmedia_codec_plugin_registered_get_const(tmedia_codec_id_t codec_id)
{
    tsk_size_t i;
    for(i = 0; i < TMED_CODEC_MAX_PLUGINS && __tmedia_codec_plugins[i]; i++) {
        if(__tmedia_codec_plugins[i]->codec_id == codec_id) {
            return __tmedia_codec_plugins[i];
        }
    }
    return tsk_null;
}

/**@ingroup tmedia_codec_group
*/
tsk_size_t tmedia_codec_plugin_registered_count(const struct tmedia_codec_plugin_def_s** plugins, tsk_size_t count)
{
	tsk_size_t i;
    for (i = 0; i < count && plugins[i]; ++i) ;
	return i;
}

/**@ingroup tmedia_codec_group
* UnRegisters a codec plugin.
* @param plugin the definition of the plugin.
* @retval 0 if succeed and non-zero error code otherwise.
*/
int tmedia_codec_plugin_unregister(const tmedia_codec_plugin_def_t* plugin)
{
    tsk_size_t i;
    tsk_bool_t found = tsk_false;
    if(!plugin) {
        TSK_DEBUG_ERROR("Invalid Parameter");
        return -1;
    }

    /* find the plugin to unregister */
    for(i = 0; i<TMED_CODEC_MAX_PLUGINS && __tmedia_codec_plugins[i]; i++) {
        if(__tmedia_codec_plugins[i] == plugin) {
            TSK_DEBUG_INFO("UnRegister codec: %s, %s", plugin->name, plugin->desc);
            __tmedia_codec_plugins[i] = tsk_null;
            found = tsk_true;
            break;
        }
    }

    /* compact */
    if(found) {
        for(; i<(TMED_CODEC_MAX_PLUGINS - 1); i++) {
            if(__tmedia_codec_plugins[i+1]) {
                __tmedia_codec_plugins[i] = __tmedia_codec_plugins[i+1];
            }
            else {
                break;
            }
        }
        __tmedia_codec_plugins[i] = tsk_null;
    }
    return (found ? 0 : -2);
}

/**@ingroup tmedia_codec_group
* Unregister all codecs
* @retval 0 if succeed and non-zero error code otherwise.
*/
int tmedia_codec_plugin_unregister_all()
{
    memset((void*)__tmedia_codec_plugins, 0, sizeof(__tmedia_codec_plugins));
    return 0;
}

/**@ingroup tmedia_codec_group
* Creates a new codec using an already registered plugin.
* @param format The format of the codec to create (e.g. "0" for PCMU or "8" for PCMA or "*" for MSRP)
* @sa @ref tmedia_codec_plugin_register()
*/
tmedia_codec_t* tmedia_codec_create(const char* format)
{
    tmedia_codec_t* codec = tsk_null;
    const tmedia_codec_plugin_def_t* plugin;
    tsk_size_t i = 0;

    while((i < TMED_CODEC_MAX_PLUGINS) && (plugin = __tmedia_codec_plugins[i++])) {
        if(plugin->objdef && tsk_striequals(plugin->format, format)) {
            if((codec = tsk_object_new(plugin->objdef))) {
                /* initialize the newly created codec */
                codec->id = plugin->codec_id;
                codec->dyn = plugin->dyn;
                codec->plugin = plugin;
                codec->bl = tmedia_bl_medium;
                switch(plugin->type) {
                case tmedia_audio: {
                    /* Audio codec */
                    tmedia_codec_audio_t* audio = TMEDIA_CODEC_AUDIO(codec);
                    tmedia_codec_audio_init(TMEDIA_CODEC(audio), plugin->name, plugin->desc, plugin->format);
                    break;
                }
                case tmedia_video: {
                    /* Video codec */
                    tmedia_codec_video_t* video = TMEDIA_CODEC_VIDEO(codec);
                    tmedia_codec_video_init(TMEDIA_CODEC(video), plugin->name, plugin->desc, plugin->format);
                    break;
                }
                case tmedia_msrp: {
                    /* Msrp codec */
                    tmedia_codec_msrp_init(codec, plugin->name, plugin->desc);
                    break;
                }
                default: {
                    /* Any other codec */
                    tmedia_codec_init(codec, plugin->type, plugin->name, plugin->desc, plugin->format);
                    break;
                }
                }
                break;
            }
        }
    }

    return codec;
}

/**@ingroup tmedia_codec_group
* Gets the rtpmap attribute associated to this code.
* @param self the codec for which to get the rtpmap attribute. Should be created using @ref tmedia_codec_create().
* @retval rtpmap string (e.g. "AMR-WB/16000/2" or "H261/90000") if succeed and Null otherwise. It's up to the caller to free the
* returned string.
*/
char* tmedia_codec_get_rtpmap(const tmedia_codec_t* self)
{
    char* rtpmap = tsk_null;

    if(!self || !self->plugin) {
        TSK_DEBUG_ERROR("invalid parameter");
        return tsk_null;
    }
    if(self->type & tmedia_video) {
        /* const tmedia_codec_video_t* videoCodec = (const tmedia_codec_video_t*)self; */
        tsk_sprintf(&rtpmap, "%s %s", self->neg_format? self->neg_format : self->format, self->name);
        if(self->plugin->rate) {
            tsk_strcat_2(&rtpmap, "/%d", self->plugin->rate);
        }
    }
    else if(self->type & tmedia_audio) {
        /* const tmedia_codec_audio_t* audioCodec = (const tmedia_codec_audio_t*)self; */

        // special case for G.722 which has fake rate
        if(tsk_strequals(self->plugin->format,TMEDIA_CODEC_FORMAT_G722)) {
            tsk_sprintf(&rtpmap, "%s %s/8000/%d", self->neg_format? self->neg_format : self->format, self->name, self->plugin->audio.channels);
        }
        else {
            tsk_sprintf(&rtpmap, "%s %s", self->neg_format? self->neg_format : self->format, self->name);
            if(self->plugin->rate) {
                tsk_strcat_2(&rtpmap, "/%d", self->plugin->rate);
            }
            if(self->plugin->audio.channels > 0) {
                tsk_strcat_2(&rtpmap, "/%d", self->plugin->audio.channels);
            }
        }
    }
    else if(self->type & tmedia_t140) {
        tsk_sprintf(&rtpmap, "%s %s", self->neg_format? self->neg_format : self->format, self->name);
        if(self->plugin->rate) {
            tsk_strcat_2(&rtpmap, "/%d", self->plugin->rate);
        }
    }
    else {
    }

    return rtpmap;
}

/**@ingroup tmedia_codec_group
* Indicates whether the codec can handle this sdp attribute.
* @param self the codec to match aginst to.
* @param att_name the name of the sdp attribute to match e.g. 'fmtp' or 'imageattr'
* @retval @a tsk_true if the codec can handle this fmtp and @a tsk_false otherwise
*/
tsk_bool_t tmedia_codec_sdp_att_match(const tmedia_codec_t* self, const char* att_name, const char* att_value)
{
    /* checks */
    if(!self || !self->plugin || !self->plugin->sdp_att_match || !att_name) {
        TSK_DEBUG_ERROR("invalid parameter");
        return tsk_false;
    }

    /* if attribute value is null or empty -> always match */
    if(tsk_strnullORempty(att_value)) {
        return tsk_true;
    }
    else {
        return self->plugin->sdp_att_match(self, att_name, att_value);
    }
}

/**@ingroup tmedia_codec_group
* Gets the codec's sdp attribute value
* @att_name the name of the attribute to get e.g. 'fmtp' or 'imageattr'
* @param self the codec for which to get the fmtp attribute. Should be created using @ref tmedia_codec_create().
* @retval sdp attribute attribute string (e.g. "mode-set=0,2,5,7; mode-change-period=2; mode-change-neighbor=1"). It's up to the caller to free the
* returned string.
*/
char* tmedia_codec_sdp_att_get(const tmedia_codec_t* self, const char* att_name)
{
    if(!self || !self->plugin || !att_name) {
        TSK_DEBUG_ERROR("invalid parameter");
        return tsk_null;
    }
    if(self->plugin->sdp_att_get) { /* some codecs, like G711, won't produce fmtp */
        return self->plugin->sdp_att_get(self, att_name);
    }
    return tsk_null;
}


/**@ingroup tmedia_codec_group
* Remove all codecs except the specified ones.
* @param codecs the list of codecs from which to remove codecs.
* @param codecs2keep the codecs which shall not be removed.
* @retval zero if succeed (or nothing to do) and non-zero error code otherwise.
*/
int tmedia_codec_removeAll_exceptThese(tmedia_codecs_L_t* codecs, const tmedia_codecs_L_t * codecs2keep)
{
    tsk_list_item_t* item;
    if(!codecs || !codecs2keep) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
again:
    tsk_list_foreach(item, codecs) {
        if(!tsk_list_find_item_by_pred(codecs2keep, __pred_find_codec_by_format, ((tmedia_codec_t*)item->data)->format)) {
            tsk_list_remove_item(codecs, item);
            goto again;
        }
    }
    return 0;
}

/**@ingroup tmedia_codec_group
* Serialize a list of codecs to sdp (m= line) message.<br>
* Will add: fmt, rtpmap and fmtp.
* @param codecs The list of codecs to convert
* @param m The destination
* @retval Zero if succeed and non-zero error code otherwise
*/
int tmedia_codec_to_sdp(const tmedia_codecs_L_t* codecs, tsdp_header_M_t* m)
{
    const tsk_list_item_t* item;
    const tmedia_codec_t* codec;
    char *fmtp, *rtpmap, *imageattr;
    tsk_bool_t is_audio, is_video, is_text;
    int ret;

    if(!m) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    is_audio = tsk_striequals(m->media, "audio");
    is_video = tsk_striequals(m->media, "video");
    is_text = tsk_striequals(m->media, "text");

    tsk_list_foreach(item, codecs) {
        const char *neg_format;
        codec = item->data;
        /* add fmt */
        neg_format = codec->neg_format?  codec->neg_format : codec->format;
        if((ret = tsdp_header_M_add_fmt(m, neg_format))) {
            TSK_DEBUG_ERROR("Failed to add format");
            return ret;
        }

        if(is_audio || is_video || is_text) {
            char* temp = tsk_null;
            /* add rtpmap attributes */
            if((rtpmap = tmedia_codec_get_rtpmap(codec))) {
                tsdp_header_M_add_headers(m,
                                          TSDP_HEADER_A_VA_ARGS("rtpmap", rtpmap),
                                          tsk_null);
                TSK_FREE(rtpmap);
            }
            /* add 'imageattr' attributes */
            if((imageattr = tmedia_codec_sdp_att_get(codec, "imageattr"))) {
                tsk_sprintf(&temp, "%s %s",  neg_format, imageattr);
                tsdp_header_M_add_headers(m,
                                          TSDP_HEADER_A_VA_ARGS("imageattr", temp),
                                          tsk_null);
                TSK_FREE(temp);
                TSK_FREE(imageattr);
            }
            /* add fmtp attributes */
            if((fmtp = tmedia_codec_sdp_att_get(codec, "fmtp"))) {
                if(is_video && tmedia_defaults_get_screen_x() > 0 && tmedia_defaults_get_screen_y() > 0) {
                    tsk_sprintf(&temp, "%s %s;sx=%d;sy=%d",  neg_format, fmtp, tmedia_defaults_get_screen_x(), tmedia_defaults_get_screen_y());//doubango clients
                }
                else {
                    tsk_sprintf(&temp, "%s %s",  neg_format, fmtp);
                }
                tsdp_header_M_add_headers(m,
                                          TSDP_HEADER_A_VA_ARGS("fmtp", temp),
                                          tsk_null);
                TSK_FREE(temp);
                TSK_FREE(fmtp);
            }
            /* special case for T.140 + red */
            if(is_text && tsk_striequals(codec->format, TMEDIA_CODEC_FORMAT_RED)) {
                const tmedia_codec_t* codec_t140 = tsk_list_find_object_by_pred(codecs, __pred_find_codec_by_format, TMEDIA_CODEC_FORMAT_T140);
                if(codec_t140) {
                    const char* neg_format_t140 = codec_t140->neg_format?  codec_t140->neg_format : codec_t140->format;
                    tsk_sprintf(&temp, "%s %s/%s/%s/%s", neg_format, neg_format_t140, neg_format_t140, neg_format_t140, neg_format_t140);
                    tsdp_header_M_add_headers(m,
                                              TSDP_HEADER_A_VA_ARGS("fmtp", temp),
                                              tsk_null);
                    TSK_FREE(temp);
                }
            }
        }
    }
    return 0;
}

/**@ingroup tmedia_codec_group
* Finds a codec by format. If the codec has a dyn. payload type, then this function will also compare negociate formats.
* @param codecs List of codecs from which to retrieve the matching codec.
* @param format the format of the codec to find.
* @retval Zero if succeed and non-zero error code otherwise.
*/
tmedia_codec_t* tmedia_codec_find_by_format(tmedia_codecs_L_t* codecs, const char* format)
{
    const tmedia_codec_t* codec = tsk_null;

    if(!codecs || !format) {
        TSK_DEBUG_ERROR("Inalid parameter");
        return tsk_null;
    }

    if((codec = tsk_list_find_object_by_pred(codecs, __pred_find_codec_by_format, format)) ||
            (codec = tsk_list_find_object_by_pred(codecs, __pred_find_codec_by_neg_format, format))) {
        return tsk_object_ref((void*)codec);
    }
    else {
        return tsk_null;
    }
}

/**@ingroup tmedia_codec_group
*/
int tmedia_codec_parse_fmtp(const char* fmtp, unsigned* maxbr, unsigned* fps, unsigned *width, unsigned *height)
{
    char *copy, *pch, *saveptr = NULL;
    tsk_bool_t found = tsk_false;

    if(tsk_strnullORempty(fmtp)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    copy = tsk_strdup(fmtp);
    pch = tsk_strtok_r(copy, "; /", &saveptr);

    while(pch) {
        unsigned div = 0;

        if(sscanf(pch, "QCIF=%u", &div) == 1 && div) {
            *fps = 30/div;
            *width = 176;
            *height = 144;
            found = tsk_true;
        }
        else if(sscanf(pch, "CIF=%u", &div) == 1 && div) {
            *fps = 30/div;
            *width = 352;
            *height = 288;
            found = tsk_true;
        }
        else if(sscanf(pch, "SQCIF=%u", &div) == 1 && div) {
            *fps = 30/div;
            *width = 128;
            *height = 96;
            found = tsk_true;
        }
        else if(sscanf(pch, "QVGA=%u", &div) == 1 && div) {
            *fps = 30/div;
            *width = 320;
            *height = 240;
            found = tsk_true;
        }
        // to be continued

        if(found) {
            //found = tsk_false;
            pch = tsk_strtok_r(tsk_null, "; ", &saveptr);
            while(pch) {
                if(sscanf(pch, "MaxBR=%u", maxbr) == 1) {
                    //found = tsk_true;
                    break;
                }
                pch = tsk_strtok_r(tsk_null, "; /", &saveptr);
            }
        }

        if(found) {
            break;
        }

        pch = tsk_strtok_r(tsk_null, "; /", &saveptr);
    }

    TSK_FREE(copy);

    return found ? 0 : -2;
}

/**@ingroup tmedia_codec_group
* DeInitialize a Codec.
* @param self The codec to deinitialize. Could be any type of codec (e.g. @ref tmedia_codec_audio_t or @ref tmedia_codec_video_t).
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tmedia_codec_deinit(tmedia_codec_t* self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(self->opened) {
        tmedia_codec_close(self);
    }

    TSK_FREE(self->name);
    TSK_FREE(self->desc);
    TSK_FREE(self->format);
    TSK_FREE(self->neg_format);

    return 0;
}

int tmedia_codec_video_set_enc_callback(tmedia_codec_video_t *self, tmedia_codec_video_enc_cb_f callback, const void* callback_data)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->out.callback = callback;
    self->out.result.usr_data = callback_data;
    return 0;
}

int tmedia_codec_video_set_dec_callback(tmedia_codec_video_t *self, tmedia_codec_video_dec_cb_f callback, const void* callback_data)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->in.callback = callback;
    self->in.result.usr_data = callback_data;
    return 0;
}

int tmedia_codec_video_clamp_out_size_to_range_max(tmedia_codec_video_t *self)
{
    int ret = 0;
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (tmedia_defaults_get_adapt_video_size_range_enabled()) {
        tmedia_pref_video_size_t min, max;
        if ((ret = tmedia_defaults_get_pref_video_size_range(&min, &max)) == 0) {
            unsigned width, height;
            // clip(max)
            if ((ret = tmedia_video_get_size(max, &width, &height)) == 0) {
                // unsigned new_width = TSK_CLAMP(0, self->out.width, width);
                // unsigned new_height = TSK_CLAMP(0, self->out.height, height);
                TSK_DEBUG_INFO("Pref. video size range defined, video size clipped (%ux%u)->(%ux%u)",
                               width, height,
                               self->out.width, self->out.height);
                self->out.width = width;
                self->out.height = height;
            }
            // no clip(min) as we cannot increase the size to more than what was negotiated without sending reINVITE
        }
    }
    return ret;
}

float tmedia_codec_audio_get_timestamp_multiplier(tmedia_codec_id_t id, uint32_t sample_rate)
{
    switch(id) {
    case tmedia_codec_id_opus: {
        // draft-spittka-payload-rtp-opus-03 - 4.1.  RTP Header Usage
        switch(sample_rate) {
        case 8000:
            return 6.f;
        case 12000:
            return 4.f;
        case 16000:
            return 3.f;
        case 24000:
            return 2.f;
        default:
        case 48000:
            return 1.f;
        }
        break;
    }
    case tmedia_codec_id_g722: {
        /* http://www.ietf.org/rfc/rfc3551.txt
        	 Even though the actual sampling rate for G.722 audio is 16,000 Hz,
        	   the RTP clock rate for the G722 payload format is 8,000 Hz because
        	   that value was erroneously assigned in RFC 1890 and must remain
        	   unchanged for backward compatibility.  The octet rate or sample-pair
        	   rate is 8,000 Hz.
        */
        return .5f;
    }
    default: {
        return 1;
    }
    }
}