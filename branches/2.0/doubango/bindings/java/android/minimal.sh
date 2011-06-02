#!/bin/bash
# Build tinyWRAP for Google Android Systems

# GPL parameters
export MINIMAL_PARAMS="FFMPEG=no H264=no THEORA=no OPENCORE_AMR=no SPEEX_DSP=yes SPEEX_JB=yes SPEEX_RESAMPLER=yes SPEEX_DENOISE=no SPEEX=no ILBC=no LIBGSM=no G729=no"

for IS_NEON in no yes
do
../bindings/java/android/buildAll.sh $@ NEON=$IS_NEON $MINIMAL_PARAMS
done
