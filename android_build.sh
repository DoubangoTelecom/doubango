#!/bin/bash

export HOME=`pwd`

### Options (change next values to 'no' to disable some features) ###
export DEBUG=no
export FFMPEG=yes
export LIBYUV=yes
export VPX=yes
export OPENH264=yes
export OPUS=yes
export OPENCORE_AMR=yes
export SPEEX_CODEC=yes
export SPEEX_DSP=yes
export SPEEX=yes
export ILBC=yes
export LIBGSM=yes
export G729=no
export SRTP=yes
export WEBRTC=yes
export SSL=yes

if [ x$NDK = "x" ]
then
	echo ERROR: NDK env variable is not set 
	exit 1;
fi
export ANDROID_NDK_ROOT=$NDK

if [ x$1 = "xcommercial" ]
then
	echo "************************"
	echo "       COMMERCIAL       "
	echo "************************"
	export HOME=$HOME/android-projects/output/commercial
	export ENABLE_NONFREE=no
	export ENABLE_GPL=no
	export FFMPEG=no # Do not use FFmpeg-LGPL because we're using static linking on Android
	export ILBC=no #LGPL
elif [ x$1 = "xlgpl" ]
then
	echo "************************"
	echo "           LGPL         "
	echo "************************"
	export HOME=$HOME/android-projects/output/lgpl
	export ENABLE_NONFREE=yes
	export ENABLE_GPL=no
else
	echo "************************"
	echo "           GPL          "
	echo "************************"
	export HOME=$HOME/android-projects/output/gpl
	export ENABLE_NONFREE=yes
	export ENABLE_GPL=yes
fi

export OPTIONS=
if [ x$FFMPEG = "xno" ]; then
	export OPTIONS="${OPTIONS} --without-ffmpeg"
fi
if [ x$LIBYUV = "xno" ]; then
	export OPTIONS="${OPTIONS} --without-yuv"
fi
if [ x$VPX = "xno" ]; then
	export OPTIONS="${OPTIONS} --without-vpx"
fi
if [ x$OPENH264 = "xno" ]; then
	export OPTIONS="${OPTIONS} --without-openh264"
fi
if [ x$OPUS = "xno" ]; then
	export OPTIONS="${OPTIONS} --without-opus"
fi
if [ x$G729 = "xno" ]; then
	export OPTIONS="${OPTIONS} --without-g729"
else
	export ac_cv_lib_g729b_Init_Decod_ld8a=yes
	export OPENCORE_AMR=no
fi
if [ x$OPENCORE_AMR = "xno" ]; then
	export OPTIONS="${OPTIONS} --without-amr"
fi
if [ x$SPEEX_CODEC = "xno" ]; then
	export OPTIONS="${OPTIONS} --without-speex"
fi
if [ x$SPEEX_DSP = "xno" ]; then
	export OPTIONS="${OPTIONS} --without-speexdsp"
fi
if [ x$ILBC = "xno" ]; then
	export OPTIONS="${OPTIONS} --without-ilbc"
fi
if [ x$LIBGSM = "xno" ]; then
	export OPTIONS="${OPTIONS} --without-gsm"
fi
if [ x$SRTP = "xno" ]; then
	export OPTIONS="${OPTIONS} --without-srtp"
fi
if [ x$WEBRTC = "xno" ]; then
	export OPTIONS="${OPTIONS} --without-webrtc"
fi
if [ x$SSL = "xno" ]; then
	export OPTIONS="${OPTIONS} --without-ssl"
fi

for arch in armv5te armv7-a armv7-a-neon arm64 x86 x64
do
	if [ $arch = "x86" ]; then \
	   export HOST=i686-linux-android; \
	elif [ $arch = "x64" ]; then \
	   export HOST=x86_64-linux-android; \
	elif [ $arch = "arm64" ]; then \
	   export HOST=aarch64-linux-android; \
	else \
	   export HOST=arm-linux-androideabi; \
	fi \
	
	echo -e building for ARCH="$arch, OPTIONS=$OPTIONS.... \n"
	
	./configure --host=$HOST --with-android-cpu=$arch --prefix=$HOME/$arch --with-pic --enable-nonfree=$ENABLE_NONFREE --enable-gpl=$ENABLE_GPL --enable-debug=$DEBUG $OPTIONS
	make clean
	make uninstall
	make all
	if [ $DEBUG = "yes" ]; then \
		make install; \
	else \
		make install-strip; \
	fi \
done

mkdir -p $HOME/imsdroid/libs/armeabi
mkdir -p $HOME/imsdroid/libs/armeabi-v7a
mkdir -p $HOME/imsdroid/libs/arm64-v8a
mkdir -p $HOME/imsdroid/libs/x86
mkdir -p $HOME/imsdroid/libs/x86_64
mkdir -p $HOME/imsdroid/libs/mips

cp --force $HOME/armv5te/lib/libtinyWRAP.so.0.0.0 $HOME/imsdroid/libs/armeabi/libtinyWRAP.so
cp --force $HOME/armv5te/lib/libplugin_audio_opensles.so.0.0.0 $HOME/imsdroid/libs/armeabi/libplugin_audio_opensles.so

cp --force $HOME/armv7-a/lib/libtinyWRAP.so.0.0.0 $HOME/imsdroid/libs/armeabi-v7a/libtinyWRAP.so
cp --force $HOME/armv7-a/lib/libplugin_audio_opensles.so.0.0.0 $HOME/imsdroid/libs/armeabi-v7a/libplugin_audio_opensles.so
cp --force $HOME/armv7-a-neon/lib/libtinyWRAP.so.0.0.0 $HOME/imsdroid/libs/armeabi-v7a/libtinyWRAP_neon.so

cp --force $HOME/arm64/lib/libtinyWRAP.so.0.0.0 $HOME/imsdroid/libs/arm64-v8a/libtinyWRAP.so
cp --force $HOME/arm64/lib/libplugin_audio_opensles.so.0.0.0 $HOME/imsdroid/libs/arm64-v8a/libplugin_audio_opensles.so

cp --force $HOME/x86/lib/libtinyWRAP.so.0.0.0 $HOME/imsdroid/libs/x86/libtinyWRAP.so
cp --force $HOME/x86/lib/libplugin_audio_opensles.so.0.0.0 $HOME/imsdroid/libs/x86/libplugin_audio_opensles.so

cp --force $HOME/x64/lib/libtinyWRAP.so.0.0.0 $HOME/imsdroid/libs/x86_64/libtinyWRAP.so
cp --force $HOME/x64/lib/libplugin_audio_opensles.so.0.0.0 $HOME/imsdroid/libs/x86_64/libplugin_audio_opensles.so


