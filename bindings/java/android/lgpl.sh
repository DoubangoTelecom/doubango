#!/bin/bash
# Build tinyWRAP for Google Android Systems

# GPL parameters
export LGPL_PARAMS=LICENSE=lgpl \
FFMPEG=yes  \
LIBYUV=yes  \
VPX=yes  \
H264=no  \
THEORA=yes  \
OPUS=yes \
OPENCORE_AMR=yes  \
SPEEX_DSP=yes  \
SPEEX_JB=yes  \
SPEEX_RESAMPLER=yes  \
SPEEX_DENOISE=yes  \
SPEEX=yes  \
ILBC=yes  \
LIBGSM=yes  \
G729=yes  \
SRTP=yes  \
TLS=yes

for IS_NEON in no yes
do
../bindings/java/android/buildAll.sh $@ NEON=$IS_NEON $LGPL_PARAMS WEBRTC=$IS_NEON WEBRTC_DENOISE=$IS_NEON
done

