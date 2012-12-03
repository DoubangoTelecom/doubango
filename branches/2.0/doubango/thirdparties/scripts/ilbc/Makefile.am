lib_LTLIBRARIES         = libiLBC.la
libiLBC_la_CPPFLAGS = -fPIC

libiLBC_la_SOURCES  = \
 anaFilter.c\
 constants.c\
 createCB.c\
 doCPLC.c\
 enhancer.c\
 filter.c\
 FrameClassify.c\
 gainquant.c\
 getCBvec.c\
 helpfun.c\
 hpInput.c\
 hpOutput.c\
 iCBConstruct.c\
 iCBSearch.c\
 iLBC_decode.c\
 iLBC_encode.c\
 LPCdecode.c\
 LPCencode.c\
 lsf.c\
 packing.c\
 StateConstructW.c\
 StateSearchW.c\
 syntFilter.c
	
libiLBC_la_LDFLAGS = $LDFLAGS -no-undefined
libiLBC_la_HEADERS = *.h
libiLBC_ladir = $(includedir)/ilbc