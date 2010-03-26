/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
#include "tinyMEDIA/tmedia.h"
#include "dummy.h"

#include "tsk.h"

#ifdef _WIN32_WCE
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
	while(1)
	{
		tmedia_t* dummy = tsk_null;
		
		// Register dummy media
		tmedia_plugin_register(dummy_plugin_def_t);
		// ...if you have another one to register
		// ...and another
		// ...again and again
		
		// Create dummy media
		if((dummy = tmedia_factory_create("dummy plugin", "127.0.0.1", tnet_socket_type_udp_ipv4))){

			tmedia_get_local_offer(dummy);
			tmedia_get_negotiated_offer(dummy);
			tmedia_set_remote_offer(dummy, tsk_null);		

			tmedia_start(dummy);
			tmedia_pause(dummy);

			tmedia_perform(dummy, tma_msrp_send_data,
				TSK_PARAM_VA_ARGS("content", "hello world!"),
				TSK_PARAM_VA_ARGS("content-type", "text/plain"),

				tsk_null);

			tmedia_stop(dummy);
			
			TSK_OBJECT_SAFE_FREE(dummy);
		}
	}
	
	return 0;
}

