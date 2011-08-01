#!/bin/bash
# Build tinyWRAP for Google Android Systems

# GPL parameters
export COMMERCIAL_PARAMS="LICENSE=lgpl FFMPEG=yes H264=no THEORA=yes OPENCORE_AMR=no SPEEX_DSP=yes SPEEX_JB=yes SPEEX_RESAMPLER=yes SPEEX_DENOISE=no SPEEX=yes ILBC=yes LIBGSM=yes G729=no"

for IS_NEON in no yes
do
../bindings/java/android/buildAll.sh $@ NEON=$IS_NEON $COMMERCIAL_PARAMS HAVE_WEBRTC=$IS_NEON HAVE_WEBRTC_DENOISE=$IS_NEON
done

