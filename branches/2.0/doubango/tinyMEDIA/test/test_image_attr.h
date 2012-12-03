/*
* Copyright (C) 20012 Doubango Telecom <http://www.doubango.org>
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo[dot]fr>
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
#ifndef _TEST_IMG_ATTR_H_
#define _TEST_IMG_ATTR_H_

static const char* __test_imageattrs[] = 
{
	"send [x=800,y=640,sar=1.1,q=0.6] [x=480,y=320] recv [x=330,y=250]",
	"recv [x=800,y=640,sar=1.1] send [x=330,y=250]",
	"recv [x=800,y=640,sar=1.1] send [x=[320:16:640],y=[240:16:480],par=[1.2-1.3]]",
	"send [x=[480:16:800],y=[320:16:640],par=[1.2-1.3],q=0.6] [x=[176:8:208],y=[144:8:176],par=[1.2-1.3]] recv *",
	"send [x=176,y=144] [x=224,y=176] [x=272,y=224] [x=320,y=240] recv [x=176,y=144] [x=224,y=176] [x=272,y=224,q=0.6] [x=320,y=240]",
	"send [x=320,y=240]",
	"recv [x=320,y=240]",
	"send [x=[400:16:800],y=[320:16:640],sar=[1.0-1.3],par=[1.2-1.3]] recv [x=800,y=600,sar=1.1]",
	"recv [x=464,y=384,sar=1.15] send [x=800,y=600,sar=1.1]"
};

void test_qos_imageattr()
{
	tsk_size_t i;
	tmedia_imageattr_xt imageattr;
	for(i = 0; i < sizeof(__test_imageattrs)/sizeof(__test_imageattrs[0]); ++i){
		if(tmedia_imageattr_parse(&imageattr, __test_imageattrs[i], tsk_strlen(__test_imageattrs[i])) == 0){
			TSK_DEBUG_INFO("image-attr parsed (OK): %s", __test_imageattrs[i]);
		}
		else{
			TSK_DEBUG_ERROR("image-attr parsed (NOK): %s", __test_imageattrs[i]);
		}
	}
}



#endif /* _TEST_IMG_ATTR_H_ */
