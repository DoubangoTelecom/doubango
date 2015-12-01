export NDK=/tmp/android-ndk-r9c
export PREFIX_BASE=/tmp/android
export HOST=arm-linux-androideabi
export CPU=ARM

if [ $1 = "x86" ]
then
	echo "************************"
	echo "           X86          "
	echo "************************"
	export HOST=i686-linux-android
	export CPU=x86
	export SYSROOT=$NDK/platforms/android-9/arch-x86
	export ANDROID_TOOLCHAIN=$(pwd)/my-android-toolchain-android-x86
	$NDK/build/tools/make-standalone-toolchain.sh --platform=android-9 --arch=x86 --install-dir=$ANDROID_TOOLCHAIN
	export CFLAGS='-fPIC' # For FFmpeg: '-fno-PIC'
	export LDFLAGS=''
elif [ $1 = "armv7a" ]
then
	echo "************************"
	echo "           ARMv7-a      "
	echo "************************"
	export HOST=arm-linux-androideabi
	export CPU=armv7-a
	export SYSROOT=$NDK/platforms/android-3/arch-arm
	export ANDROID_TOOLCHAIN=$(pwd)/my-android-toolchain-android-armv7-a
    	$NDK/build/tools/make-standalone-toolchain.sh --platform=android-3 --install-dir=$ANDROID_TOOLCHAIN
    	export CFLAGS='-march=armv7-a -mfloat-abi=softfp -fPIC'
    	export LDFLAGS='-Wl,--fix-cortex-a8'
elif [ $1 = "neon" ]
then
	echo "************************"
	echo "           NEON         "
	echo "************************"
	export HOST=arm-linux-androideabi
	export CPU=armv7-a-neon
	export SYSROOT=$NDK/platforms/android-3/arch-arm
    	export ANDROID_TOOLCHAIN=$(pwd)/my-android-toolchain-android-armv7-a-neon
    	$NDK/build/tools/make-standalone-toolchain.sh --platform=android-3 --install-dir=$ANDROID_TOOLCHAIN
    	export CFLAGS='-march=armv7-a -mfloat-abi=softfp -mfpu=neon -fPIC'
    	export LDFLAGS='-Wl,--fix-cortex-a8'
else
	echo "************************"
	echo "           ARMv5TE      "
	echo "************************"
	export HOST=arm-linux-androideabi
	export CPU=armv5te
	export SYSROOT=$NDK/platforms/android-3/arch-arm
	export ANDROID_TOOLCHAIN=$(pwd)/my-android-toolchain-armv5te
    	$NDK/build/tools/make-standalone-toolchain.sh --platform=android-3 --install-dir=$ANDROID_TOOLCHAIN
	export CFLAGS='-fPIC'
	export LDFLAGS=''
fi


### COMMON ###
export PATH=$ANDROID_TOOLCHAIN/bin:$PATH
export CC=$ANDROID_TOOLCHAIN/bin/$HOST-gcc
export AR=$ANDROID_TOOLCHAIN/bin/$HOST-ar
export CXX=$ANDROID_TOOLCHAIN/bin/$HOST-g++
#export AS=$ANDROID_TOOLCHAIN/bin/$HOST-as
export AS=$ANDROID_TOOLCHAIN/bin/$HOST-gcc
export RANLIB=$ANDROID_TOOLCHAIN/bin/$HOST-ranlib

./configure --prefix=$PREFIX_BASE/$CPU --host=$HOST --enable-pic --disable-oggtest --without-libogg --enable-static && make clean && make && make install
