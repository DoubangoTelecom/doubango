# Copyright (C) 2009 Mamadou Diop.
#
# Contact: Mamadou Diop <diopmamadou@yahoo.fr>
#	
# This file is part of Open Source Doubango Framework.
#
# DOUBANGO is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#	
# DOUBANGO is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#	
# You should have received a copy of the GNU General Public License
# along with DOUBANGO.

LOCAL_PATH := $(DOUBANGO_HOME)/tinySAK/src

include $(CLEAR_VARS)

LOCAL_MODULE    := tinySAK
LOCAL_SRC_FILES := tsk.c\
	tsk_base64.c\
	tsk_binaryutils.c\
	tsk_buffer.c\
	tsk_condwait.c\
	tsk_debug.c\
	tsk_heap.c\
	tsk_hmac.c\
	tsk_list.c\
	tsk_md5.c\
	tsk_memory.c\
	tsk_mutex.c\
	tsk_object.c\
	tsk_params.c\
	tsk_ppfcs16.c\
	tsk_ppfcs32.c\
	tsk_runnable.c\
	tsk_safeobj.c\
	tsk_semaphore.c\
	tsk_sha1.c\
	tsk_string.c\
	tsk_thread.c\
	tsk_time.c\
	tsk_timer.c\
	tsk_url.c\
	tsk_xml.c

include $(BUILD_SHARED_LIBRARY)
