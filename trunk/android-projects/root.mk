######
##	Root configuration file shared by all android projects.
######

export ANDROID_NDK_ROOT=/cygdrive/c/android-ndk
export ANDROID_SDK_ROOT=/cygdrive/c/android-sdk
export ANDROID_PLATFORM=$(ANDROID_NDK_ROOT)/build/platforms/android-1.5

# Output directory
export OUTPUT_DIR=$(shell pwd)/output
$(shell mkdir -p $(OUTPUT_DIR))

# Path where to copy executables -on the device or emulator-
export EXEC_DIR=/data/tmp
# Path where to copy libraries (*.so) -on the device or emulator-
export LIB_DIR=/system/lib

export CC=arm-eabi-gcc
export CFLAGS+=$(DEBUG_FLAGS) -I$(ANDROID_PLATFORM)/arch-arm/usr/include \
-march=armv5te \
-mtune=xscale \
-msoft-float \
-fpic \
-mthumb-interwork \
-ffunction-sections \
-funwind-tables \
-fstack-protector \
-fno-short-enums \
-D__ARM_ARCH_5__ \
-D__ARM_ARCH_5T__ \
-D__ARM_ARCH_5E__ \
-D__ARM_ARCH_5TE__ \
-DANDROID \
-MMD \
-MP
export LDFLAGS=-Wl,--entry=main,-rpath=/system/lib,-rpath-link=$(ANDROID_PLATFORM)/arch-arm/usr/lib,-rpath-link=$(OUTPUT_DIR),-dynamic-linker=/system/bin/linker -L$(ANDROID_PLATFORM)/arch-arm/usr/lib
export LDFLAGS += -Wl,--no-undefined
export LDFLAGS += -nostdlib -lc -Wl,--no-whole-archive -L$(OUTPUT_DIR)
