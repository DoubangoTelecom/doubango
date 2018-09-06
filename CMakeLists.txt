cmake_minimum_required(VERSION 3.4.1)

#project(tinyWRAP c cxx)

include_directories(tinySAK/src)
file(GLOB sak_src tinySAK/src/*.c)

include_directories(tinyNET/src)
file(GLOB net_src tinyNET/src/*.c tinyNET/src/*/*.c)

include_directories(tinyHTTP/include)
file(GLOB http_src tinyHTTP/src/*.c tinyHTTP/src/*/*.c)

include_directories(tinySIP/include)
file(GLOB sip_src tinySIP/src/*.c tinySIP/src/*/*.c)

include_directories(tinyDAV/include)
file(GLOB dav_src tinyDAV/src/*.c tinyDAV/src/*/*.c tinyDAV/src/*/*/*.c)

include_directories(tinySDP/include)
file(GLOB sdp_src tinySDP/src/*.c tinySDP/src/*/*.c)

include_directories(tinyMEDIA/include)
file(GLOB media_src tinyMEDIA/src/*.c tinyMEDIA/src/*/*.c)

include_directories(tinyXCAP/include)
file(GLOB xcap_src tinyXCAP/src/*.c tinyXCAP/src/*/*.c)

include_directories(tinySMS/include)
file(GLOB sms_src tinySMS/src/*.c tinySMS/src/*/*.c)

include_directories(tinyMSRP/include)
file(GLOB msrp_src tinyMSRP/src/*.c tinyMSRP/src/*/*.c)

include_directories(tinyRTP/include)
file(GLOB rtp_src tinyRTP/src/*.c tinyRTP/src/*/*.c)

include_directories(tinyBFCP/include)
file(GLOB bfcp_src tinyBFCP/src/*.c tinyBFCP/src/*/*.c)

include_directories(tinyIPSec/src)
file(GLOB ipsec_src tinyIPSec/src/*.c tinyIPSec/src/*/*.c)

include_directories(tinySIGCOMP/src)
file(GLOB sigcomp_src tinySIGCOMP/src/*.c tinySIGCOMP/src/*/*.c)

include_directories(tinySIGCOMP/src)
file(GLOB sigcomp_src tinySIGCOMP/src/*.c tinySIGCOMP/src/*/*.c)

include_directories(bindings/_common)
include_directories(bindings/java/android)
file(GLOB bindings_common bindings/_common/*.cxx)
file(GLOB bindings_common_java_android bindings/_common/java/android/*.cxx)

add_library(
	tinyWRAP
	SHARED
	${sak_src}
	${net_src}
	${http_src}
	${sip_src}
	${dav_src}
	${sdp_src}
	${media_src}
	${xcap_src}
	${sms_src}
	${msrp_src}
	${rtp_src}
	${bfcp_src}
	${ipsec_src}
	${sigcomp_src}
	${bindings_common}
	${bindings_common_java_android}
)

find_library(log-lib log)
target_link_libraries(tinyWRAP ${log-lib})

