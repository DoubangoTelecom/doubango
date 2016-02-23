/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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
#ifndef _TEST_CODECS_H_
#define _TEST_CODECS_H_

void test_codecs()
{
    tmedia_codec_t* pcmu, *pcma;

    /* creates codecs  */
    pcmu = tmedia_codec_create(TMEDIA_CODEC_FORMAT_G711u);
    pcma = tmedia_codec_create(TMEDIA_CODEC_FORMAT_G711a);

    TSK_OBJECT_SAFE_FREE(pcmu);
    TSK_OBJECT_SAFE_FREE(pcma);
}

#endif /* _TEST_CODECS_H_ */
