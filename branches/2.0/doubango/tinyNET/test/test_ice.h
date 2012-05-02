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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*/

#ifndef TNET_TEST_ICE_H
#define TNET_TEST_ICE_H

#define ICE_CANDIDATES "1 1 udp 1 192.168.196.1 57806 typ host name video_rtcp network_name {0C0137CC-DB78-46B6-9B6C-7E097FFA79FE} username StFEVThMK2DHThkv password qkhKUDr4WqKRwZTo generation 0\r\n" \
			"1 2 udp 1 192.168.211.1 57808 typ srflx name video_rtcp network_name {F53974D9-C92C-4644-AF7A-EA09D29BD5A5} username 9ONjPsYvSFh0JvAc password rDHSkokdvp9dyXqQ generation 0\r\n" \
			"1 1 udp 1 192.168.196.1 57809 typ prflx name video_rtp network_name {0C0137CC-DB78-46B6-9B6C-7E097FFA79FE} username S1vDZTVVky3r0pT+ password XgLb+H9uofxuWg7G generation 0\r\n" \
			"1 2 udp 1 192.168.211.1 57811 typ relay name video_rtp network_name {F53974D9-C92C-4644-AF7A-EA09D29BD5A5} username x64BhO4BXjBFkpz2 password ZwHhRhu0KU9R6iWd generation 0\r\n" \
			"1 1 udp 1 192.168.211.1 57811 typ token name video_rtp network_name {F53974D9-C92C-4644-AF7A-EA09D29BD5A5} username x64BhO4BXjBFkpz2 password ZwHhRhu0KU9R6iWd generation 0\r\n"

static int tnet_ice_callback(const tnet_ice_event_t *e)
{
	TSK_DEBUG_INFO("ICE callback: %s", e->phrase);

	switch(e->type)
	{
		case tnet_ice_event_type_gathering_completed:
			{
				struct tnet_ice_ctx_s *ctx;
				if((ctx = (struct tnet_ice_ctx_s *)e->userdata)){
					tnet_ice_ctx_set_remote_candidates(ctx, ICE_CANDIDATES, "ice-ufrag", "ice-pwd", tsk_true, tsk_false);
				}
				break;
			}
	}
	return 0;
}

void test_ice()
{
	struct tnet_ice_ctx_s *ctx;
	int ret;
	static const tsk_bool_t use_ipv6 = tsk_false;
	static const tsk_bool_t use_rtcp = tsk_true;

	
	long a = tnet_htonl(0x6b0c76a7);
	long b = tnet_htonl(0x034aa76b);
	long c = tnet_htonl(0x510da598);

	ctx = tnet_ice_ctx_create(use_ipv6, use_rtcp, tsk_false, tnet_ice_callback, tsk_null);
	tnet_ice_ctx_set_userdata(ctx, ctx);
	ret = tnet_ice_ctx_set_stun(ctx, "numb.viagenie.ca", 3478, "Doubango", "bossiel@yahoo.fr", "stun-password");
	ret = tnet_ice_ctx_start(ctx);
	// ret = tnet_ice_ctx_set_remote_candidates(ctx, ICE_CANDIDATES);

	getchar();
	
	ret = tnet_ice_ctx_stop(ctx);

	TSK_OBJECT_SAFE_FREE(ctx);
}


#endif /* TNET_TEST_ICE_H */
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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*/

#ifndef TNET_TEST_ICE_H
#define TNET_TEST_ICE_H

#define ICE_CANDIDATES "1 1 udp 1 192.168.196.1 57806 typ host name video_rtcp network_name {0C0137CC-DB78-46B6-9B6C-7E097FFA79FE} username StFEVThMK2DHThkv password qkhKUDr4WqKRwZTo generation 0\r\n" \
			"1 2 udp 1 192.168.211.1 57808 typ srflx name video_rtcp network_name {F53974D9-C92C-4644-AF7A-EA09D29BD5A5} username 9ONjPsYvSFh0JvAc password rDHSkokdvp9dyXqQ generation 0\r\n" \
			"1 1 udp 1 192.168.196.1 57809 typ prflx name video_rtp network_name {0C0137CC-DB78-46B6-9B6C-7E097FFA79FE} username S1vDZTVVky3r0pT+ password XgLb+H9uofxuWg7G generation 0\r\n" \
			"1 2 udp 1 192.168.211.1 57811 typ relay name video_rtp network_name {F53974D9-C92C-4644-AF7A-EA09D29BD5A5} username x64BhO4BXjBFkpz2 password ZwHhRhu0KU9R6iWd generation 0\r\n" \
			"1 1 udp 1 192.168.211.1 57811 typ token name video_rtp network_name {F53974D9-C92C-4644-AF7A-EA09D29BD5A5} username x64BhO4BXjBFkpz2 password ZwHhRhu0KU9R6iWd generation 0\r\n"

static int tnet_ice_callback(const tnet_ice_event_t *e)
{
	TSK_DEBUG_INFO("ICE callback: %s", e->phrase);

	switch(e->type)
	{
		case tnet_ice_event_type_gathering_completed:
			{
				struct tnet_ice_ctx_s *ctx;
				if((ctx = (struct tnet_ice_ctx_s *)e->userdata)){
					tnet_ice_ctx_set_remote_candidates(ctx, ICE_CANDIDATES);
				}
				break;
			}
	}
	return 0;
}

void test_ice()
{
	struct tnet_ice_ctx_s *ctx;
	int ret;
	static const tsk_bool_t use_ipv6 = tsk_false;
	static const tsk_bool_t use_rtcp = tsk_true;

	ctx = tnet_ice_ctx_create(use_ipv6, use_rtcp, tnet_ice_callback, tsk_null);
	tnet_ice_ctx_set_userdata(ctx, ctx);
	ret = tnet_ice_ctx_stun_configure(ctx, "numb.viagenie.ca", 3478, "Doubango", "bossiel@yahoo.fr", "stun-password");
	ret = tnet_ice_ctx_start(ctx);
	// ret = tnet_ice_ctx_set_remote_candidates(ctx, ICE_CANDIDATES);

	getchar();
	
	ret = tnet_ice_ctx_stop(ctx);

	TSK_OBJECT_SAFE_FREE(ctx);
}


#endif /* TNET_TEST_ICE_H */
