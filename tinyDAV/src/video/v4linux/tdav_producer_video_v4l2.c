/* Copyright (C) 2014 Mamadou DIOP.
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
#include "tinydav/video/v4linux/tdav_producer_video_v4l2.h"

#if HAVE_LINUX_VIDEODEV2_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<sys/ioctl.h>

#include<linux/videodev2.h>

#include "tinymedia/tmedia_defaults.h"

#include "tsk_memory.h"
#include "tsk_safeobj.h"
#include "tsk_timer.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#if !defined(V4L2_FAKE_UYVY)
#	define V4L2_FAKE_UYVY 0
#endif /* V4L2_FAKE_UYVY */

#define V4L2_CLEAR(x) memset(&(x), 0, sizeof(x))
#define V4L2_DEBUG_INFO(FMT, ...) TSK_DEBUG_INFO("[V4L2 Producer] " FMT, ##__VA_ARGS__)
#define V4L2_DEBUG_WARN(FMT, ...) TSK_DEBUG_WARN("[V4L2 Producer] " FMT, ##__VA_ARGS__)
#define V4L2_DEBUG_ERROR(FMT, ...) TSK_DEBUG_ERROR("[V4L2 Producer] " FMT, ##__VA_ARGS__)
#define V4L2_DEBUG_FATAL(FMT, ...) TSK_DEBUG_FATAL("[V4L2 Producer] " FMT, ##__VA_ARGS__)

typedef enum v4l2_io_method_e {
    V4L2_IO_METHOD_NONE = 0,
    V4L2_IO_METHOD_READ,
    V4L2_IO_METHOD_MMAP,
    V4L2_IO_METHOD_USERPTR,
}
v4l2_io_method_t;

typedef struct v4l2_buffer_s {
    void   *p_start;
    size_t  n_length;
}
v4l2_buffer_t;

// By preference order
static const v4l2_io_method_t io_method_prefs[] = {
    V4L2_IO_METHOD_MMAP,
    V4L2_IO_METHOD_USERPTR,
    V4L2_IO_METHOD_READ,
};
static const unsigned int pix_format_prefs[] = {
    V4L2_PIX_FMT_YUV420,
    V4L2_PIX_FMT_NV12,
    V4L2_PIX_FMT_NV21,
    V4L2_PIX_FMT_YUYV,
    V4L2_PIX_FMT_UYVY, // SINCITY
    V4L2_PIX_FMT_RGB24,
    V4L2_PIX_FMT_RGB32,
    V4L2_PIX_FMT_MJPEG
};

typedef struct tdav_producer_video_v4l2_s {
    TMEDIA_DECLARE_PRODUCER;

    tsk_bool_t b_muted;
    tsk_bool_t b_started;
    tsk_bool_t b_prepared;
    tsk_bool_t b_paused;

    int fd;
    v4l2_io_method_t io;
    struct v4l2_format fmt;
    struct v4l2_capability cap;
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    unsigned int n_buffers;
    v4l2_buffer_t* p_buffers;

    tsk_timer_manager_handle_t *p_timer_mgr;
    tsk_timer_id_t id_timer_grab;
    uint64_t u_timout_grab;

    TSK_DECLARE_SAFEOBJ;
}
tdav_producer_video_v4l2_t;

static int _v4l2_prepare(tdav_producer_video_v4l2_t* p_self);
static int _v4l2_start(tdav_producer_video_v4l2_t* p_self);
static int _v4l2_pause(tdav_producer_video_v4l2_t* p_self);
static int _v4l2_stop(tdav_producer_video_v4l2_t* p_self);
static int _v4l2_unprepare(tdav_producer_video_v4l2_t* p_self);
static int _v4l2_xioctl(int fh, int request, void *arg);
static int _v4l2_get_best_format(tdav_producer_video_v4l2_t* p_self, const char* device_name, struct v4l2_format* fmt_ret);
static int _v4l2_init_read(tdav_producer_video_v4l2_t* p_self, unsigned int buffer_size);
static int _v4l2_init_mmap(tdav_producer_video_v4l2_t* p_self, const char* device_name);
static int _v4l2_init_userp(tdav_producer_video_v4l2_t* p_self, unsigned int buffer_size, const char* device_name);
static int _v4l2_send_frame(tdav_producer_video_v4l2_t* p_self);

static int _tdav_producer_video_v4l2_timer_cb(const void* arg, tsk_timer_id_t timer_id);
static int _tdav_producer_video_v4l2_grab(tdav_producer_video_v4l2_t* p_self);

/* ============ Media Producer Interface ================= */
static int _tdav_producer_video_v4l2_set(tmedia_producer_t *p_self, const tmedia_param_t* pc_param)
{
    int ret = 0;
    tdav_producer_video_v4l2_t* p_v4l2 = (tdav_producer_video_v4l2_t*)p_self;

    if (!p_v4l2 || !pc_param) {
        V4L2_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (pc_param->value_type == tmedia_pvt_pchar) {
        if (tsk_striequals(pc_param->key, "local-hwnd") || tsk_striequals(pc_param->key, "preview-hwnd")) {
            V4L2_DEBUG_ERROR("Not implemented yet");
        }
        else if (tsk_striequals(pc_param->key, "src-hwnd")) {
            V4L2_DEBUG_ERROR("Not implemented yet");
        }
    }
    else if (pc_param->value_type == tmedia_pvt_int32) {
        if (tsk_striequals(pc_param->key, "mute")) {
            p_v4l2->b_muted = (TSK_TO_INT32((uint8_t*)pc_param->value) != 0);
        }
    }

    return ret;
}

static int _tdav_producer_video_v4l2_prepare(tmedia_producer_t* p_self, const tmedia_codec_t* pc_codec)
{
    tdav_producer_video_v4l2_t* p_v4l2 = (tdav_producer_video_v4l2_t*)p_self;
    int ret = 0;

    if (!p_v4l2 || !pc_codec) {
        V4L2_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(p_v4l2);

    if (!p_v4l2->p_timer_mgr && !(p_v4l2->p_timer_mgr = tsk_timer_manager_create())) {
        V4L2_DEBUG_ERROR("Failed to create timer manager");
        ret = -2;
        goto bail;
    }

    TMEDIA_PRODUCER(p_v4l2)->video.fps = TMEDIA_CODEC_VIDEO(pc_codec)->out.fps;
    TMEDIA_PRODUCER(p_v4l2)->video.width = TMEDIA_CODEC_VIDEO(pc_codec)->out.width;
    TMEDIA_PRODUCER(p_v4l2)->video.height = TMEDIA_CODEC_VIDEO(pc_codec)->out.height;

    p_v4l2->u_timout_grab = (1000/TMEDIA_PRODUCER(p_v4l2)->video.fps);

    // prepare()
    if ((ret = _v4l2_prepare(p_v4l2))) {
        goto bail;
    }

    // update() - up to the "converter" to perform chroma conversion and scaling
    TMEDIA_PRODUCER(p_v4l2)->video.width = p_v4l2->fmt.fmt.pix.width;
    TMEDIA_PRODUCER(p_v4l2)->video.height = p_v4l2->fmt.fmt.pix.height;
#if V4L2_FAKE_UYVY
    TMEDIA_PRODUCER(p_v4l2)->video.chroma = tmedia_chroma_uyvy422;
#else
    switch (p_v4l2->fmt.fmt.pix.pixelformat) {
    case V4L2_PIX_FMT_YUV420:
        TMEDIA_PRODUCER(p_v4l2)->video.chroma = tmedia_chroma_yuv420p;
        break;
    case V4L2_PIX_FMT_NV12:
        TMEDIA_PRODUCER(p_v4l2)->video.chroma = tmedia_chroma_nv12;
        break;
    case V4L2_PIX_FMT_NV21:
        TMEDIA_PRODUCER(p_v4l2)->video.chroma = tmedia_chroma_nv21;
        break;
    case V4L2_PIX_FMT_YUYV:
        TMEDIA_PRODUCER(p_v4l2)->video.chroma = tmedia_chroma_yuyv422;
        break;
    case V4L2_PIX_FMT_UYVY:
        TMEDIA_PRODUCER(p_v4l2)->video.chroma = tmedia_chroma_uyvy422; // SINCITY
        break;
    case V4L2_PIX_FMT_RGB24:
        TMEDIA_PRODUCER(p_v4l2)->video.chroma = tmedia_chroma_rgb24;
        break;
    case V4L2_PIX_FMT_RGB32:
        TMEDIA_PRODUCER(p_v4l2)->video.chroma = tmedia_chroma_rgb32;
        break;
    case V4L2_PIX_FMT_MJPEG:
        TMEDIA_PRODUCER(p_v4l2)->video.chroma = tmedia_chroma_mjpeg;
        break;
    default:
        V4L2_DEBUG_ERROR("Failed to match negotiated format: %d", p_v4l2->fmt.fmt.pix.pixelformat);
        ret = -1;
        goto bail;
    }
#endif /* V4L2_FAKE_UYVY */

    V4L2_DEBUG_INFO("Negotiated caps: fps=%d, width=%d, height=%d, chroma=%d",
                    TMEDIA_PRODUCER(p_v4l2)->video.fps,
                    TMEDIA_PRODUCER(p_v4l2)->video.width,
                    TMEDIA_PRODUCER(p_v4l2)->video.height,
                    TMEDIA_PRODUCER(p_v4l2)->video.chroma);
    p_v4l2->b_prepared = (ret == 0) ? tsk_true : tsk_false;

bail:
    tsk_safeobj_unlock(p_v4l2);
    return ret;
}

static int _tdav_producer_video_v4l2_start(tmedia_producer_t* p_self)
{
    tdav_producer_video_v4l2_t* p_v4l2 = (tdav_producer_video_v4l2_t*)p_self;
    int ret = 0;

    if (!p_v4l2) {
        V4L2_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(p_v4l2);

    if (!p_v4l2->b_prepared) {
        V4L2_DEBUG_INFO("Not prepared");
        ret = -1;
        goto bail;
    }

    p_v4l2->b_paused = tsk_false;

    if (p_v4l2->b_started) {
        V4L2_DEBUG_INFO("Already started");
        goto bail;
    }

    if ((ret = tsk_timer_manager_start(p_v4l2->p_timer_mgr))) {
        goto bail;
    }

    // start()
    if ((ret = _v4l2_start(p_v4l2))) {
        goto bail;
    }

    p_v4l2->b_started = tsk_true;

    // Schedule frame grabbing
    p_v4l2->id_timer_grab = tsk_timer_manager_schedule(p_v4l2->p_timer_mgr, p_v4l2->u_timout_grab, _tdav_producer_video_v4l2_timer_cb, p_v4l2);
    if (!TSK_TIMER_ID_IS_VALID(p_v4l2->id_timer_grab)) {
        V4L2_DEBUG_ERROR("Failed to schedule timer with timeout=%llu", p_v4l2->u_timout_grab);
        ret = -2;
        goto bail;
    }

bail:
    if (ret) {
        _v4l2_stop(p_v4l2);
        p_v4l2->b_started = tsk_false;
        if (p_v4l2->p_timer_mgr) {
            tsk_timer_manager_stop(p_v4l2->p_timer_mgr);
        }
    }
    else {
        V4L2_DEBUG_INFO("Started :)");
    }
    tsk_safeobj_unlock(p_v4l2);

    return ret;
}

static int _tdav_producer_video_v4l2_pause(tmedia_producer_t* p_self)
{
    tdav_producer_video_v4l2_t* p_v4l2 = (tdav_producer_video_v4l2_t*)p_self;
    int ret;

    if (!p_v4l2) {
        V4L2_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(p_v4l2);

    if ((ret = _v4l2_pause(p_v4l2))) {
        goto bail;
    }

    p_v4l2->b_paused = tsk_true;
    goto bail;

bail:
    tsk_safeobj_unlock(p_v4l2);

    return ret;
}

static int _tdav_producer_video_v4l2_stop(tmedia_producer_t* p_self)
{
    tdav_producer_video_v4l2_t* p_v4l2 = (tdav_producer_video_v4l2_t*)p_self;

    if (!p_v4l2) {
        V4L2_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(p_v4l2);

    if (!p_v4l2->b_started) {
        V4L2_DEBUG_INFO("Already stopped");
        goto bail;
    }

    if (p_v4l2->p_timer_mgr) {
        tsk_timer_manager_stop(p_v4l2->p_timer_mgr);
    }

    // next start will be called after prepare()
    _v4l2_unprepare(p_v4l2); // stop() then unprepare()

    p_v4l2->b_started = tsk_false;
    p_v4l2->b_paused = tsk_false;
    p_v4l2->b_prepared = tsk_false;

bail:
    tsk_safeobj_unlock(p_v4l2);
    V4L2_DEBUG_INFO("Stopped");

    return 0;
}

static int _v4l2_prepare(tdav_producer_video_v4l2_t* p_self)
{
    const char* device_names[] = {
        tmedia_producer_get_friendly_name(TMEDIA_PRODUCER(p_self)->plugin->type),
        "/dev/video0",
    }; // FIXME: VIDIOC_C_ENUM_INPUT and choose best one
    const char* device_name;
    int i, err = -1;
    struct stat st;
    unsigned int min;

    V4L2_DEBUG_INFO("--- PREPARE ---");

    if (p_self->fd > 0) {
        V4L2_DEBUG_WARN("Producer already prepared");
        return 0;
    }
    for (i = 0; i < sizeof(device_names)/sizeof(device_names[0]); ++i) {
        if ((device_name = device_names[i])) {
            V4L2_DEBUG_INFO("Preparing '%s'...", device_name);
            if (stat(device_name, &st) == -1) {
                V4L2_DEBUG_WARN("stat('%s'): %d, %s", device_name, errno, strerror(errno));
                continue;
            }
            if (!S_ISCHR(st.st_mode)) {
                V4L2_DEBUG_WARN("'%s' not a valid device", device_name);
                continue;
            }
            if ((p_self->fd = open(device_name, O_RDWR /* required */ | O_NONBLOCK, 0)) == -1) {
                V4L2_DEBUG_WARN("Failed to open '%s': %d, %s\n", device_name, errno, strerror(errno));
                continue;
            }
            V4L2_DEBUG_INFO("'%s' successfully opened", device_name);
        }
    }
    if (p_self->fd == -1) {
        V4L2_DEBUG_ERROR("No valid device found");
        goto bail;
    }

    if (-1 == _v4l2_xioctl(p_self->fd, VIDIOC_QUERYCAP, &p_self->cap)) {
        if (EINVAL == errno) {
            V4L2_DEBUG_ERROR("%s is no V4L2 device", device_name);
            goto bail;
        }
        else {
            V4L2_DEBUG_ERROR("xioctl(%s, VIDIOC_QUERYCAP) failed: %s error %d", device_name, strerror(errno), errno);
            goto bail;
        }
    }

    if (!(p_self->cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        V4L2_DEBUG_ERROR("%s is no video capture device", device_name);
        goto bail;
    }

    // Get best io method
    p_self->io = V4L2_IO_METHOD_NONE;
    for (i = 0; i < sizeof(io_method_prefs)/sizeof(io_method_prefs[0]) && p_self->io == V4L2_IO_METHOD_NONE; ++i) {
        V4L2_DEBUG_INFO("Trying with io method=%d", io_method_prefs[i]);
        switch (io_method_prefs[i]) {
        case V4L2_IO_METHOD_READ:
            if (!(p_self->cap.capabilities & V4L2_CAP_READWRITE)) {
                V4L2_DEBUG_WARN("%s does not support read i/o", device_name);
                continue;
            }
            p_self->io = io_method_prefs[i];
            break;

        case V4L2_IO_METHOD_MMAP:
        case V4L2_IO_METHOD_USERPTR:
            if (!(p_self->cap.capabilities & V4L2_CAP_STREAMING)) {
                V4L2_DEBUG_WARN("%s does not support streaming i/o", device_name);
                continue;
            }
            p_self->io = io_method_prefs[i];
            break;
        }
    }
    if (p_self->io == V4L2_IO_METHOD_NONE) {
        V4L2_DEBUG_ERROR("Failed to peek an i/o method for '%s' device", device_name);
        goto bail;
    }
    V4L2_DEBUG_INFO("i/o method for '%s' device is %d", device_name, p_self->io);

    /* Select video input, video standard and tune here. */

    V4L2_CLEAR(p_self->cropcap);

    p_self->cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (0 == _v4l2_xioctl(p_self->fd, VIDIOC_CROPCAP, &p_self->cropcap)) {
        p_self->crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        p_self->crop.c = p_self->cropcap.defrect; /* reset to default */

        if (-1 == _v4l2_xioctl(p_self->fd, VIDIOC_S_CROP, &p_self->crop)) {
            switch (errno) {
            case EINVAL:
            default:
                V4L2_DEBUG_INFO("'%s' device doesn't support cropping", device_name);
                break;
            }
        }
        else {
            V4L2_DEBUG_INFO("'%s' device supports cropping with type = %d", device_name, p_self->crop.type);
        }
    }
    else {
        V4L2_DEBUG_INFO("'%s' device doesn't support cropping", device_name);
    }

    /* Best format */
    V4L2_CLEAR(p_self->fmt);
    // get()
    if (_v4l2_get_best_format(p_self, device_name, &p_self->fmt) != 0) {
        V4L2_DEBUG_ERROR("Failed to peek best format for '%s' device", device_name);
        goto bail;
    }
    // set()
    if (_v4l2_xioctl(p_self->fd, VIDIOC_S_FMT, &p_self->fmt) == -1) {
        goto bail;
    }
    V4L2_DEBUG_INFO("device '%s' best format: width:%d, height:%d, field:%d, pixelformat:%d",
                    device_name, p_self->fmt.fmt.pix.width, p_self->fmt.fmt.pix.height, p_self->fmt.fmt.pix.field, p_self->fmt.fmt.pix.pixelformat);

    /* Buggy driver paranoia. */
#if 1
    min = p_self->fmt.fmt.pix.width * 2;
    if (p_self->fmt.fmt.pix.bytesperline < min) {
        p_self->fmt.fmt.pix.bytesperline = min;
    }
    min = p_self->fmt.fmt.pix.bytesperline * p_self->fmt.fmt.pix.height;
    if (p_self->fmt.fmt.pix.sizeimage < min) {
        p_self->fmt.fmt.pix.sizeimage = min;
    }
#endif

    switch (p_self->io) {
    case V4L2_IO_METHOD_READ:
        if (_v4l2_init_read(p_self, p_self->fmt.fmt.pix.sizeimage) != 0) {
            goto bail;
        }
        break;

    case V4L2_IO_METHOD_MMAP:
        if (_v4l2_init_mmap(p_self, device_name) != 0) {
            goto bail;
        }
        break;

    case V4L2_IO_METHOD_USERPTR:
        if (_v4l2_init_userp(p_self, p_self->fmt.fmt.pix.sizeimage, device_name) != 0) {
            goto bail;
        }
        break;
    }
    V4L2_DEBUG_INFO("'%s' device initialized using i/o method=%d", device_name, p_self->io);

    // all is OK
    err = 0;

bail:
    if (err) {
        _v4l2_unprepare(p_self);
    }
    else {
        V4L2_DEBUG_INFO("Prepared :)");
    }
    return err;
}


static int _v4l2_start(tdav_producer_video_v4l2_t* p_self)
{
    unsigned int i;
    enum v4l2_buf_type type;

    V4L2_DEBUG_INFO("--- START ---");

    if (p_self->b_started) {
        V4L2_DEBUG_WARN("Already started");
        return 0;
    }

    switch (p_self->io) {
    case V4L2_IO_METHOD_READ:
        /* Nothing to do. */
        break;

    case V4L2_IO_METHOD_MMAP:
        for (i = 0; i < p_self->n_buffers; ++i) {
            struct v4l2_buffer buf;

            V4L2_CLEAR(buf);
            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_MMAP;
            buf.index = i;

            if (-1 == _v4l2_xioctl(p_self->fd, VIDIOC_QBUF, &buf)) {
                V4L2_DEBUG_ERROR("xioctl(VIDIOC_QBUF) failed: %s error %d", strerror(errno), errno);
                return -1;
            }
        }
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (-1 == _v4l2_xioctl(p_self->fd, VIDIOC_STREAMON, &type)) {
            V4L2_DEBUG_ERROR("xioctl(VIDIOC_STREAMON) failed: %s error %d", strerror(errno), errno);
            return -1;
        }
        break;

    case V4L2_IO_METHOD_USERPTR:
        for (i = 0; i < p_self->n_buffers; ++i) {
            struct v4l2_buffer buf;

            V4L2_CLEAR(buf);
            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_USERPTR;
            buf.index = i;
            buf.m.userptr = (unsigned long)p_self->p_buffers[i].p_start;
            buf.length = p_self->p_buffers[i].n_length;

            if (-1 == _v4l2_xioctl(p_self->fd, VIDIOC_QBUF, &buf)) {
                V4L2_DEBUG_ERROR("xioctl(VIDIOC_QBUF) failed: %s error %d", strerror(errno), errno);
                return -1;
            }
        }
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (-1 == _v4l2_xioctl(p_self->fd, VIDIOC_STREAMON, &type)) {
            V4L2_DEBUG_ERROR("xioctl(VIDIOC_STREAMON) failed: %s error %d", strerror(errno), errno);
            return -1;
        }
        break;
    }
    return 0;
}

static int _v4l2_pause(tdav_producer_video_v4l2_t* p_self)
{
    V4L2_DEBUG_INFO("--- PAUSE ---");

    return 0;
}

static int _v4l2_stop(tdav_producer_video_v4l2_t* p_self)
{
    enum v4l2_buf_type type;

    V4L2_DEBUG_INFO("--- STOP ---");

    switch (p_self->io) {
    case V4L2_IO_METHOD_READ:
        /* Nothing to do. */
        break;

    case V4L2_IO_METHOD_MMAP:
    case V4L2_IO_METHOD_USERPTR:
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (p_self->fd != -1 && -1 == _v4l2_xioctl(p_self->fd, VIDIOC_STREAMOFF, &type)) {
            if (p_self->b_started) { // display error only if the device is marked as "started"
                V4L2_DEBUG_ERROR("xioctl(VIDIOC_STREAMOFF) failed: %s error %d", strerror(errno), errno);
                return -1;
            }
        }
        break;
    }

    return 0;
}

static int _v4l2_unprepare(tdav_producer_video_v4l2_t* p_self)
{
    unsigned int i;
    V4L2_DEBUG_INFO("--- UNPREPARE ---");

    _v4l2_stop(p_self);

    switch (p_self->io) {
    case V4L2_IO_METHOD_READ:
        if (p_self->p_buffers && p_self->p_buffers[0].p_start) {
            free(p_self->p_buffers[0].p_start);
            p_self->p_buffers[0].p_start = NULL;
        }
        break;
    case V4L2_IO_METHOD_MMAP:
        for (i = 0; i < p_self->n_buffers; ++i) {
            if (p_self->p_buffers && p_self->p_buffers[i].p_start) {
                if (-1 == munmap(p_self->p_buffers[i].p_start, p_self->p_buffers[i].n_length)) {
                    V4L2_DEBUG_ERROR("munmap(%d) failed", i);
                }
            }
        }
        break;

    case V4L2_IO_METHOD_USERPTR:
        for (i = 0; i < p_self->n_buffers; ++i) {
            if (p_self->p_buffers && p_self->p_buffers[i].p_start) {
                free(p_self->p_buffers[i].p_start);
                p_self->p_buffers[i].p_start = NULL;
            }
        }
        break;
    }

    if (p_self->p_buffers) {
        free(p_self->p_buffers);
        p_self->p_buffers = NULL;
    }
    p_self->n_buffers = 0;

    if (p_self->fd > 0) {
        close(p_self->fd);
    }
    p_self->fd = -1;

    return 0;
}

static int _v4l2_xioctl(int fh, int request, void *arg)
{
    int r;
    do {
        r = ioctl(fh, request, arg);
    }
    while (-1 == r && EINTR == errno);
    return r;
}

static int _v4l2_get_best_format(tdav_producer_video_v4l2_t* p_self, const char* device_name, struct v4l2_format* fmt_ret)
{
    struct v4l2_format fmt, fmt_default, fmt_best;
    struct v4l2_fmtdesc fmtdesc;
    int i, j, field, size;
    int ok = 0;

    if (!fmt_ret) {
        V4L2_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // get default format
    V4L2_CLEAR(fmt_default);
    fmt_default.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (_v4l2_xioctl(p_self->fd, VIDIOC_G_FMT, &fmt_default) == -1) {
        V4L2_DEBUG_ERROR("xioctl(%s, VIDIOC_G_FMT) failed: %s error %d", device_name, strerror(errno), errno);
        return -1;
    }
    V4L2_DEBUG_INFO("device '%s' default format: width:%d, height:%d, field:%d, pixelformat:%d",
                    device_name, fmt_default.fmt.pix.width, fmt_default.fmt.pix.height, fmt_default.fmt.pix.field, fmt_default.fmt.pix.pixelformat);

    /* Best format (using preference) */
    V4L2_CLEAR(fmt);
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    for (i = 0; i < sizeof(pix_format_prefs)/sizeof(pix_format_prefs[0]); ++i) {
        for (size = 0; size < 2; ++size) {
            for (field = 0; field < 2; ++field) {
                fmt.fmt.pix.width = (size == 0) ? TMEDIA_PRODUCER(p_self)->video.width : fmt_default.fmt.pix.width;
                fmt.fmt.pix.height = (size == 0) ? TMEDIA_PRODUCER(p_self)->video.height : fmt_default.fmt.pix.height;
                fmt.fmt.pix.pixelformat = pix_format_prefs[i];
                fmt.fmt.pix.field = (field == 0) ? V4L2_FIELD_NONE : V4L2_FIELD_INTERLACED;
                if ((ok = (_v4l2_xioctl(p_self->fd, VIDIOC_TRY_FMT, &fmt) != -1))) {
                    goto bail;
                }
            }
        }
    }

    /* Best format (using caps) */
    for (i = 0; ; ++i) {
        V4L2_CLEAR(fmtdesc);
        fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmtdesc.index = i;

        if (_v4l2_xioctl(p_self->fd, VIDIOC_ENUM_FMT, &fmtdesc) == -1) {
            break;
        }
        V4L2_DEBUG_INFO("CAPS: device name=%s, fmtdesc index=%d, type=%d, description=%s, pixelformat=%d",
                        device_name, fmtdesc.index, fmtdesc.type, fmtdesc.description, fmtdesc.pixelformat);
        for (j = 0; j < sizeof(pix_format_prefs)/sizeof(pix_format_prefs[0]); ++j) {
            if (fmtdesc.pixelformat == pix_format_prefs[j]) {
                for (size = 0; size < 2; ++size) {
                    for (field = 0; field < 2; ++field) {
                        fmt.fmt.pix.width = (size == 0) ? TMEDIA_PRODUCER(p_self)->video.width : fmt_default.fmt.pix.width;
                        fmt.fmt.pix.height = (size == 0) ? TMEDIA_PRODUCER(p_self)->video.height : fmt_default.fmt.pix.height;
                        fmt.fmt.pix.pixelformat = pix_format_prefs[i];
                        fmt.fmt.pix.field = (field == 0) ? V4L2_FIELD_NONE : V4L2_FIELD_INTERLACED;
                        if ((ok = (_v4l2_xioctl(p_self->fd, VIDIOC_TRY_FMT, &fmt) != -1))) {
                            goto bail;
                        }
                    }
                }
            }
        }
    }

bail:
    if (ok) {
        memcpy(fmt_ret, &fmt, sizeof(fmt));
    }
    return ok ? 0 : -1;
}

static int _v4l2_init_read(tdav_producer_video_v4l2_t* p_self, unsigned int buffer_size)
{
    if (p_self->p_buffers) {
        V4L2_DEBUG_ERROR("Buffers already initialized");
        return -1;
    }
    if (!buffer_size) {
        V4L2_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (!(p_self->p_buffers = calloc(1, sizeof(*p_self->p_buffers)))) {
        V4L2_DEBUG_ERROR("Out of memory");
        return -1;
    }

    p_self->p_buffers[0].n_length = buffer_size;
    p_self->p_buffers[0].p_start = tsk_malloc(buffer_size);

    if (!p_self->p_buffers[0].p_start) {
        V4L2_DEBUG_ERROR("Out of memory");
        return -1;
    }

    return 0;
}

static int _v4l2_init_mmap(tdav_producer_video_v4l2_t* p_self, const char* device_name)
{
    struct v4l2_requestbuffers req;

    V4L2_CLEAR(req);

    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (-1 == _v4l2_xioctl(p_self->fd, VIDIOC_REQBUFS, &req)) {
        if (EINVAL == errno) {
            V4L2_DEBUG_ERROR("%s does not support memory mapping", device_name);
            return -1;
        }
        else {
            V4L2_DEBUG_ERROR("xioctl(%s, VIDIOC_REQBUFS) failed: %s error %d", device_name, strerror(errno), errno);
            return -1;
        }
    }

    if (req.count < 2) {
        V4L2_DEBUG_ERROR("Insufficient buffer memory on %s", device_name);
        return -1;
    }

    if (!(p_self->p_buffers = tsk_calloc(req.count, sizeof(*p_self->p_buffers)))) {
        V4L2_DEBUG_ERROR("Out of memory");
        return -1;
    }

    for (p_self->n_buffers = 0; p_self->n_buffers < req.count; ++p_self->n_buffers) {
        struct v4l2_buffer buf;

        V4L2_CLEAR(buf);

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = p_self->n_buffers;

        if (-1 == _v4l2_xioctl(p_self->fd, VIDIOC_QUERYBUF, &buf)) {
            V4L2_DEBUG_ERROR("xioctl(%s, VIDIOC_REQBUFS) failed: %s error %d", device_name, strerror(errno), errno);
            return -1;
        }

        p_self->p_buffers[p_self->n_buffers].n_length = buf.length;
        p_self->p_buffers[p_self->n_buffers].p_start = mmap(NULL /* start anywhere */,
                buf.length,
                PROT_READ | PROT_WRITE /* required */,
                MAP_SHARED /* recommended */,
                p_self->fd, buf.m.offset);

        if (MAP_FAILED == p_self->p_buffers[p_self->n_buffers].p_start) {
            V4L2_DEBUG_ERROR("mmap(%s) failed: %s error %d", device_name, strerror(errno), errno);
            return -1;
        }
    }

    return 0;
}

static int _v4l2_init_userp(tdav_producer_video_v4l2_t* p_self, unsigned int buffer_size, const char* device_name)
{
    struct v4l2_requestbuffers req;

    V4L2_CLEAR(req);

    req.count  = 4;
    req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_USERPTR;

    if (-1 == _v4l2_xioctl(p_self->fd, VIDIOC_REQBUFS, &req)) {
        if (EINVAL == errno) {
            V4L2_DEBUG_ERROR("%s does not support user pointer i/o", device_name);
            return -1;
        }
        else {
            V4L2_DEBUG_ERROR("xioctl(%s, VIDIOC_REQBUFS) failed: %s error %d", device_name, strerror(errno), errno);
            return -1;
        }
    }

    if (!(p_self->p_buffers = tsk_calloc(4, sizeof(*p_self->p_buffers)))) {
        V4L2_DEBUG_ERROR("Out of memory");
        return -1;
    }

    for (p_self->n_buffers = 0; p_self->n_buffers < 4; ++p_self->n_buffers) {
        p_self->p_buffers[p_self->n_buffers].n_length = buffer_size;
        p_self->p_buffers[p_self->n_buffers].p_start = tsk_malloc(buffer_size);

        if (!p_self->p_buffers[p_self->n_buffers].p_start) {
            V4L2_DEBUG_ERROR("Out of memory");
            return -1;
        }
    }

    return 0;
}

static int _v4l2_send_frame(tdav_producer_video_v4l2_t* p_self)
{
    struct v4l2_buffer buf;
    unsigned int i;

#define V4L2_SEND_BUFF(_buff, _size) \
	TMEDIA_PRODUCER(p_self)->enc_cb.callback(TMEDIA_PRODUCER(p_self)->enc_cb.callback_data, (_buff), (_size));

#if V4L2_FAKE_UYVY
    {
        tsk_size_t size = (TMEDIA_PRODUCER(p_self)->video.width * TMEDIA_PRODUCER(p_self)->video.height) << 1;
        uint8_t* buff = (uint8_t*)tsk_malloc(size);
        if (buff) {
            tsk_size_t i;
            for (i = 0; i < size; ++i) {
                buff[i] = rand() & 254;
            }
            V4L2_SEND_BUFF(buff, size);
            tsk_free((void**)&buff);
        }
        return 0;
    }
#endif

    switch (p_self->io) {
    case V4L2_IO_METHOD_READ:
        if (-1 == read(p_self->fd, p_self->p_buffers[0].p_start, p_self->p_buffers[0].n_length)) {
            switch (errno) {
            case EAGAIN:
                return 0;

            case EIO:
                /* Could ignore EIO, see spec. */

                /* fall through */

            default:
                V4L2_DEBUG_ERROR("read() failed: %s error %d", strerror(errno), errno);
                break;
            }
        }

        V4L2_SEND_BUFF(p_self->p_buffers[0].p_start, p_self->p_buffers[0].n_length);
        return 0;

    case V4L2_IO_METHOD_MMAP:
        V4L2_CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;

        if (-1 == _v4l2_xioctl(p_self->fd, VIDIOC_DQBUF, &buf)) {
            switch (errno) {
            case EAGAIN:
                V4L2_DEBUG_INFO("EAGAIN");
                return 0;

            case EIO:
                /* Could ignore EIO, see spec. */

                /* fall through */

            default:
                V4L2_DEBUG_ERROR("xioctl(VIDIOC_DQBUF) failed: %s error %d", strerror(errno), errno);
                break;
            }
        }

        assert(buf.index < p_self->n_buffers);

        V4L2_SEND_BUFF(p_self->p_buffers[buf.index].p_start, buf.bytesused);

        if (-1 == _v4l2_xioctl(p_self->fd, VIDIOC_QBUF, &buf)) {
            V4L2_DEBUG_ERROR("xioctl(VIDIOC_DQBUF) failed: %s error %d", strerror(errno), errno);
            break;
        }
        return 0;

    case V4L2_IO_METHOD_USERPTR:
        V4L2_CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_USERPTR;

        if (-1 == _v4l2_xioctl(p_self->fd, VIDIOC_DQBUF, &buf)) {
            switch (errno) {
            case EAGAIN:
                V4L2_DEBUG_INFO("EAGAIN");
                return 0;

            case EIO:
                /* Could ignore EIO, see spec. */

                /* fall through */

            default:
                V4L2_DEBUG_ERROR("xioctl(VIDIOC_DQBUF) failed: %s error %d", strerror(errno), errno);
                break;
            }
        }

        for (i = 0; i < p_self->n_buffers; ++i) {
            if (buf.m.userptr == (unsigned long)p_self->p_buffers[i].p_start && buf.length == p_self->p_buffers[i].n_length) {
                break;
            }
        }

        V4L2_SEND_BUFF((void *)buf.m.userptr, buf.bytesused);

        if (-1 == _v4l2_xioctl(p_self->fd, VIDIOC_QBUF, &buf)) {
            V4L2_DEBUG_ERROR("xioctl(VIDIOC_DQBUF) failed: %s error %d", strerror(errno), errno);
            break;
        }
        return 0;
    }

    return -1;
}

static int _tdav_producer_video_v4l2_timer_cb(const void* arg, tsk_timer_id_t timer_id)
{
    tdav_producer_video_v4l2_t* p_v4l2 = (tdav_producer_video_v4l2_t*)arg;
    int ret = 0;

    tsk_safeobj_lock(p_v4l2);

    if (p_v4l2->id_timer_grab == timer_id) {
        if (ret = _tdav_producer_video_v4l2_grab(p_v4l2)) {
            // goto bail;
        }
        if (p_v4l2->b_started) {
            p_v4l2->id_timer_grab = tsk_timer_manager_schedule(p_v4l2->p_timer_mgr, p_v4l2->u_timout_grab, _tdav_producer_video_v4l2_timer_cb, p_v4l2);
            if (!TSK_TIMER_ID_IS_VALID(p_v4l2->id_timer_grab)) {
                V4L2_DEBUG_ERROR("Failed to schedule timer with timeout=%llu", p_v4l2->u_timout_grab);
                ret = -2;
                goto bail;
            }
        }
    }

bail:
    tsk_safeobj_unlock(p_v4l2);
    return ret;
}

static int _tdav_producer_video_v4l2_grab(tdav_producer_video_v4l2_t* p_self)
{
    int ret = 0, r;
    fd_set fds;
    struct timeval tv;

    if (!p_self) {
        V4L2_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(p_self);

    if (!p_self->b_started) {
        V4L2_DEBUG_ERROR("producer not started yet");
        ret = -2;
        goto bail;
    }

    if (!TMEDIA_PRODUCER(p_self)->enc_cb.callback) {
        goto bail;
    }

    FD_ZERO(&fds);
    FD_SET(p_self->fd, &fds);

    /* Timeout. */
    tv.tv_sec = 0;
    tv.tv_usec = (p_self->id_timer_grab * 1000);
    while (tv.tv_usec >= 1000000) {
        tv.tv_usec -= 1000000;
        tv.tv_sec++;
    }

    r = select(p_self->fd + 1, &fds, NULL, NULL, &tv);

    if (-1 == r) {
        if (EINTR == errno) {
            V4L2_DEBUG_INFO("select() returned EINTR");
        }
        else {
            V4L2_DEBUG_ERROR("select() failed: %s error %d", strerror(errno), errno);
        }
        goto bail;
    }

    if (0 == r) {
        V4L2_DEBUG_INFO("select() timeout: %s error %d", strerror(errno), errno);
        goto bail;
    }
    // Grab a frame
    if ((ret = _v4l2_send_frame(p_self))) {
        goto bail;
    }
bail:
    tsk_safeobj_unlock(p_self);

    return ret;
}

//
//	V4L2 video producer object definition
//
/* constructor */
static tsk_object_t* _tdav_producer_video_v4l2_ctor(tsk_object_t *self, va_list * app)
{
    tdav_producer_video_v4l2_t *p_v4l2 = (tdav_producer_video_v4l2_t *)self;
    if (p_v4l2) {
        /* init base */
        tmedia_producer_init(TMEDIA_PRODUCER(p_v4l2));
        TMEDIA_PRODUCER(p_v4l2)->video.chroma = tmedia_chroma_yuv420p;
        /* init self with default values*/
        p_v4l2->fd = -1;
        TMEDIA_PRODUCER(p_v4l2)->video.fps = 15;
        TMEDIA_PRODUCER(p_v4l2)->video.width = 352;
        TMEDIA_PRODUCER(p_v4l2)->video.height = 288;

        tsk_safeobj_init(p_v4l2);
    }
    return self;
}
/* destructor */
static tsk_object_t* _tdav_producer_video_v4l2_dtor(tsk_object_t * self)
{
    tdav_producer_video_v4l2_t *p_v4l2 = (tdav_producer_video_v4l2_t *)self;
    if (p_v4l2) {
        /* stop */
        if (p_v4l2->b_started) {
            _tdav_producer_video_v4l2_stop((tmedia_producer_t*)p_v4l2);
        }

        /* deinit base */
        tmedia_producer_deinit(TMEDIA_PRODUCER(p_v4l2));
        /* deinit self */
        _v4l2_unprepare(p_v4l2);
        TSK_OBJECT_SAFE_FREE(p_v4l2->p_timer_mgr);
        tsk_safeobj_deinit(p_v4l2);

        V4L2_DEBUG_INFO("*** destroyed ***");
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_producer_video_v4l2_def_s = {
    sizeof(tdav_producer_video_v4l2_t),
    _tdav_producer_video_v4l2_ctor,
    _tdav_producer_video_v4l2_dtor,
    tsk_null,
};
/* plugin definition*/
// Video
static const tmedia_producer_plugin_def_t tdav_producer_video_v4l2_plugin_def_s = {
    &tdav_producer_video_v4l2_def_s,
    tmedia_video,
    "V4L2 video producer",

    _tdav_producer_video_v4l2_set,
    _tdav_producer_video_v4l2_prepare,
    _tdav_producer_video_v4l2_start,
    _tdav_producer_video_v4l2_pause,
    _tdav_producer_video_v4l2_stop
};
const tmedia_producer_plugin_def_t *tdav_producer_video_v4l2_plugin_def_t = &tdav_producer_video_v4l2_plugin_def_s;
// Screencast
static const tmedia_producer_plugin_def_t tdav_producer_screencast_v4l2_plugin_def_s = {
    &tdav_producer_video_v4l2_def_s,
    tmedia_bfcp_video,
    "V4L2 screencast producer",

    _tdav_producer_video_v4l2_set,
    _tdav_producer_video_v4l2_prepare,
    _tdav_producer_video_v4l2_start,
    _tdav_producer_video_v4l2_pause,
    _tdav_producer_video_v4l2_stop
};
const tmedia_producer_plugin_def_t *tdav_producer_screencast_v4l2_plugin_def_t = &tdav_producer_screencast_v4l2_plugin_def_s;
#endif /* HAVE_LINUX_VIDEODEV2_H */

