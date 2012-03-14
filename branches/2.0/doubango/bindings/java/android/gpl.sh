#!/bin/bash
# Build tinyWRAP for Google Android Systems

# GPL parameters
export GPL_PARAMS="LICENSE=gpl NO_FREE=yes FFMPEG=yes H264=yes THEORA=yes OPENCORE_AMR=yes SPEEX_DSP=yes SPEEX_JB=yes SPEEX_RESAMPLER=yes SPEEX_DENOISE=1 SPEEX=yes ILBC=yes LIBGSM=yes G729=yes SRTP=yes"

for IS_NEON in no yes
do
../bindings/java/android/buildAll.sh $@ NEON=$IS_NEON $GPL_PARAMS VPX=$IS_NEON HAVE_WEBRTC=$IS_NEON HAVE_WEBRTC_DENOISE=$IS_NEON
done

