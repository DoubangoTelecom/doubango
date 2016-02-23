/* Copyright (C) 2011-2013 Doubango Telecom <http://www.doubango.org>
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
#ifndef PLUGIN_DSHOW_VIDEOFRAME_H
#define PLUGIN_DSHOW_VIDEOFRAME_H

// Define supported video formats
typedef enum _VIDEOFORMAT {
    VIDEOFORMAT_NULL = 0,		//    0 x    0 : Null
    VIDEOFORMAT_SQCIF,			//  128 x   96 : SQCIF
    VIDEOFORMAT_QCIF,			//  176 x  144 : QCIF
    VIDEOFORMAT_QVGA,			//  320 x  240 : QVGA
    VIDEOFORMAT_CIF,			//  352 x  288 : CIF
    VIDEOFORMAT_IOS_MEDIUM,		//  480 x  360 : IOS_MEDIUM
    VIDEOFORMAT_VGA,			//  640 x  480 : VGA
    VIDEOFORMAT_4CIF,			//  704 x  576 : 4CIF
    VIDEOFORMAT_SVGA,			//  800 x  600 : SVGA
    VIDEOFORMAT_XGA,			// 1024 x  768 : XGA
    VIDEOFORMAT_SXGA,			// 1280 x 1024 : SXGA
    VIDEOFORMAT_16CIF,			// 1408 x 1152 : 16CIF
} VIDEOFORMAT;


// Macro to convert a video format to its size
#define VIDEOFORMAT_TO_SIZE(format, width, height)						\
	switch(format)														\
	{																	\
	case VIDEOFORMAT_SQCIF:			width = 128;	height = 96;	break;	\
	case VIDEOFORMAT_QCIF:			width = 176;	height = 144;	break;	\
	case VIDEOFORMAT_QVGA:			width = 320;	height = 240;	break;	\
	case VIDEOFORMAT_CIF:			width = 352;	height = 288;	break;	\
	case VIDEOFORMAT_IOS_MEDIUM:	width = 480;	height = 360;	break;	\
	case VIDEOFORMAT_VGA:			width = 640;	height = 480;	break;	\
	case VIDEOFORMAT_4CIF:			width = 704;	height = 576;	break;	\
	case VIDEOFORMAT_SVGA:			width = 800;	height = 600;	break;	\
	case VIDEOFORMAT_XGA:			width = 1024;	height = 768;	break;	\
	case VIDEOFORMAT_SXGA:			width = 1280;	height = 1024;	break;	\
	case VIDEOFORMAT_16CIF:			width = 1408;	height = 1152;	break;	\
	case VIDEOFORMAT_NULL:												\
	default:						width = 0;		height = 0;		break;	\
	}																	\
 

// Macro to get a video format from its size
#define SIZE_TO_VIDEOFORMAT(width, height, format)											\
		if ((width == 128) && (height = 96))			format = VIDEOFORMAT_SQCIF;			\
		else if ((width == 176) && (height = 144))		format = VIDEOFORMAT_QCIF;			\
		else if ((width == 320) && (height = 240))		format = VIDEOFORMAT_QVGA;			\
		else if ((width == 352) && (height = 288))		format = VIDEOFORMAT_CIF;			\
		else if ((width == 480) && (height = 360))		format = VIDEOFORMAT_IOS_MEDIUM;	\
		else if ((width == 640) && (height = 480))		format = VIDEOFORMAT_VGA;			\
		else if ((width == 704) && (height = 576))		format = VIDEOFORMAT_4CIF;			\
		else if ((width == 800) && (height = 600))		format = VIDEOFORMAT_SVGA;			\
		else if ((width == 1024) && (height = 768))		format = VIDEOFORMAT_XGA;			\
		else if ((width == 1280) && (height = 1024))	format = VIDEOFORMAT_SXGA;			\
		else if ((width == 1408) && (height = 1152))	format = VIDEOFORMAT_16CIF;			\
		else											format = VIDEOFORMAT_NULL;			\
 

// Constants for consumer and producer Ids
#define GRABBER_VIDEO_ID 0x1FFFFFFF
#define REMOTE_VIDEO_ID 0x2FFFFFFF


class VideoFrame
{
public:
    VideoFrame() {
        this->data = NULL;
    };
    virtual ~VideoFrame() {
        if(this->data) {
            this->data = NULL;
        }
    };

    int getWidth() {
        return this->width;
    };
    int getHeight() {
        return this->height;
    };
    int getBitsPerPixel() {
        return this->bpp;
    };
    int getTotalBits () {
        return  this->width * this->height * (this->bpp/8);
    };
    void* getData() {
        return this->data;
    };

    void setWidth(int width_) {
        this->width = width_;
    };
    void setHeight(int height_) {
        this->height = height_;
    };
    void setBitsPerPixel( int bpp_) {
        this->bpp = bpp_;
    };
    void setData( void* data_) {
        this->data = data_;
    };

    VIDEOFORMAT getSize();
    void setSize(VIDEOFORMAT format);

private:
    void *data;
    int width;
    int height;
    int bpp;
};

#endif /* VIDEOFRAME_H */
