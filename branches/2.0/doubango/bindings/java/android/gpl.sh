#!/bin/bash
# Build tinyWRAP for Google Android Systems

# GPL parameters
export GPL_PARAMS="LICENSE=gpl NO_FREE=yes FFMPEG=yes H264=yes THEORA=yes OPENCORE_AMR=yes SPEEX_DSP=yes SPEEX_JB=yes SPEEX_RESAMPLER=yes SPEEX_DENOISE=no SPEEX=yes ILBC=yes LIBGSM=yes G729=yes"

for IS_NEON in no yes
do
../bindings/java/android/buildAll.sh $@ NEON=$IS_NEON $GPL_PARAMS
done

