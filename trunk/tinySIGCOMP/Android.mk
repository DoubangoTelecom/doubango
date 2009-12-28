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

LOCAL_PATH := $(DOUBANGO_HOME)/tinySIGCOMP/src

include $(CLEAR_VARS)

LOCAL_MODULE    := tinySIGCOMP
LOCAL_SRC_FILES := \
	adler32.c \
	compress.c \
	deflate.c \
	tcomp.c \
	tcomp_buffer.c \
	tcomp_compartment.c \
	tcomp_compressordata.c \
	tcomp_compressordisp.c \
	tcomp_compressor_deflate.c \
	tcomp_compressor_dummy.c \
	tcomp_decompressordisp.c \
	tcomp_deflatedata.c \
	tcomp_deflatedata.ghost.c \
	tcomp_deflatedata.zlib.c \
	tcomp_dicts.c \
	tcomp_manager.c \
	tcomp_message.c \
	tcomp_nackinfo.c \
	tcomp_params.c \
	tcomp_reqfeed.c \
	tcomp_result.c \
	tcomp_state.c \
	tcomp_statehandler.c \
	tcomp_udvm.bytecopy.c \
	tcomp_udvm.c \
	tcomp_udvm.instructions.c \
	tcomp_udvm.nack.c \
	tcomp_udvm.operands.c \
	tcomp_udvm.statemanagment.c \
	trees.c \
	zutil.c

LOCAL_CFLAGS := -I$(DOUBANGO_HOME)/tinySAK/src

#LOCAL_SHARED_LIBRARIES := libtinySAK
LOCAL_LDLIBS := -L$(DOUBANGO_HOME)/android/tinySAK/libs/armeabi -ltinySAK

include $(BUILD_SHARED_LIBRARY)
