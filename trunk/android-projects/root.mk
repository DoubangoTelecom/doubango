######
##	Root configuration file shared by all android projects.
######

export ANDROID_NDK_ROOT=/cygdrive/c/android-ndk
export ANDROID_SDK_ROOT=/cygdrive/c/android-sdk
export ANDROID_PLATFORM=$(ANDROID_NDK_ROOT)/build/platforms/android-4

# Output directory
export OUTPUT_DIR=$(shell pwd)/output
$(shell mkdir -p $(OUTPUT_DIR))

# Path where to copy executables -on the device or emulator-
export EXEC_DIR=/data/tmp
# Path where to copy libraries (*.so) -on the device or emulator-
export LIB_DIR=/system/lib

export AR=arm-eabi-ar
export CC=arm-eabi-gcc
export CPP=arm-eabi-g++

export CFLAGS_COMMON=$(CFLAGS) $(DEBUG_FLAGS) -I$(ANDROID_PLATFORM)/arch-arm/usr/include \
-march=armv5te \
-mtune=xscale \
-msoft-float \
-D__ARM_ARCH_5__ \
-D__ARM_ARCH_5T__ \
-D__ARM_ARCH_5E__ \
-D__ARM_ARCH_5TE__ \
-MMD \
-MP \
-DANDROID

export CFLAGS_LIB= $(CFLAGS_COMMON) \
-fpic \
-mthumb-interwork \
-ffunction-sections \
-funwind-tables \
-fstack-protector \
-fno-short-enums \
-mthumb \
-Os \
-fomit-frame-pointer \
-fno-strict-aliasing \
-finline-limit=64


export LDFLAGS_COMMON=$(LDFLAGS) -Wl,-rpath=/system/lib,-rpath-link=$(ANDROID_PLATFORM)/arch-arm/usr/lib,-rpath-link=$(OUTPUT_DIR),-dynamic-linker=/system/bin/linker -L$(ANDROID_PLATFORM)/arch-arm/usr/lib
export LDFLAGS_COMMON+=-nostdlib -lc -L$(OUTPUT_DIR)

ifeq ($(BT), static)
	export EXT=a
	export LDFLAGS_LIB=
	
	export CFLAGS_LIB+=-static
else
	export EXT=so
	
	export LDFLAGS_LIB=$(LDFLAGS_COMMON)
	export LDFLAGS_LIB+=-Wl,--no-undefined
	export LDFLAGS_LIB+=-Wl,--no-whole-archive
	export LDFLAGS_LIB+=-Wl,-soname,lib$(PROJECT).$(EXT),-Bsymbolic,-shared,--whole-archive
endif
