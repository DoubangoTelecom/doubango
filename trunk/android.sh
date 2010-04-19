export ANDROID_NDK_ROOT=/cygdrive/c/android-ndk
export ANDROID_PLATFORM=$ANDROID_NDK_ROOT/build/platforms/android-4
export OUTPUT_DIR=$(pwd)/../output

export CC=arm-eabi-gcc
export CPPFLAGS="-I$ANDROID_PLATFORM/arch-arm/usr/include -march=armv5te -mtune=xscale -msoft-float -fPIC -mthumb-interwork -ffunction-sections -funwind-tables -fstack-protector -fno-short-enums -D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__ -DANDROID -MMD -MP"

export LDFLAGS="-Wl,-rpath=/system/lib,-rpath-link=$ANDROID_PLATFORM/arch-arm/usr/lib,-dynamic-linker=/system/bin/linker -L$ANDROID_PLATFORM/arch-arm/usr/lib"
export LDFLAGS="$LDFLAGS -Wl,--no-undefined"
export LDFLAGS="$LDFLAGS -nostdlib -lc -L$OUTPUT_DIR"

-host=arm-eabi --enable-shared=yes --enable-static=no