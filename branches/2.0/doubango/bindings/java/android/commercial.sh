#!/bin/bash
# Build tinyWRAP for Google Android Systems

# GPL parameters
export COMMERCIAL_PARAMS=LICENSE=lgpl \
FFMPEG=no  \
LIBYUV=yes  \
VPX=yes  \
H264=no  \
THEORA=yes  \
OPUS=yes \
OPENCORE_AMR=no  \
SPEEX_DSP=yes  \
SPEEX_JB=yes  \
SPEEX_RESAMPLER=yes  \
SPEEX_DENOISE=yes  \
SPEEX=yes  \
ILBC=yes  \
LIBGSM=yes  \
G729=no  \
SRTP=yes  \
TLS=yes

for IS_NEON in no yes
do
../bindings/java/android/buildAll.sh $@ NEON=$IS_NEON $COMMERCIAL_PARAMS WEBRTC=$IS_NEON WEBRTC_DENOISE=$IS_NEON
done

