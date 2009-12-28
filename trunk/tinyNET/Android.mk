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

LOCAL_PATH := $(DOUBANGO_HOME)/tinyNET/src

include $(CLEAR_VARS)

LOCAL_MODULE    := tinyNET
LOCAL_SRC_FILES := \
	tnet.c\
	tnet_poll.c\
	tnet_socket.c\
	tnet_transport.c\
	tnet_transport_poll.c\
	tnet_transport_win32.c\
	tnet_utils.c	

LOCAL_CFLAGS := -I$(DOUBANGO_HOME)/tinySAK/src

#LOCAL_SHARED_LIBRARIES := libtinySAK
LOCAL_LDLIBS := -L$(DOUBANGO_HOME)/android/tinySAK/libs/armeabi -ltinySAK

include $(BUILD_SHARED_LIBRARY)
