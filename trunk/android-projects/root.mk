#########
DOUBANGO_HOME:=$(DOUBANGO_HOME)
#########

export ROOT:=/cygdrive/c
export INSTALL_DIR:= /data/tmp
#export OUTPUT_DIR = $(shell pwd)/output
export OUTPUT_DIR:=$(DOUBANGO_HOME)/android-projects/output
export NDK_PLATFORM_VER:=1.5

export ANDROID_NDK_ROOT:=$(ROOT)/android-ndk
export ANDROID_NDK_HOST:=windows
export ANDROID_SDK_ROOT:=$(ROOT)/android-sdk
export PREBUILD:=$(ANDROID_NDK_ROOT)/build/prebuilt/$(ANDROID_NDK_HOST)/arm-eabi-4.2.1
export BIN:=$(PREBUILD)/bin

#export CPP = $(BIN)/arm-eabi-g++
export CPP:=$(BIN)/arm-eabi-gcc
export CC:=$(BIN)/arm-eabi-gcc
export CFLAGS:=$(OPTIONS) -I$(ANDROID_NDK_ROOT)/build/platforms/android-$(NDK_PLATFORM_VER)/arch-arm/usr/include \
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
export LDFLAGS:=-Wl,--no-gc-sections,--entry=main,-rpath=/system/lib,-rpath-link=$(ANDROID_NDK_ROOT)/build/platforms/android-$(NDK_PLATFORM_VER)/arch-arm/usr/lib,-dynamic-linker=/system/bin/linker -L$(ANDROID_NDK_ROOT)/build/platforms/android-$(NDK_PLATFORM_VER)/arch-arm/usr/lib
export LDFLAGS += -Wl,-T,$(PREBUILD)/arm-eabi/lib/ldscripts/armelf.xsc,-z,nocopyreloc,--gc-sections,--no-undefined
export LDFLAGS += -nostdlib -lc -Wl,--no-whole-archive -L$(OUTPUT_DIR)

gdbserver:
	$(ANDROID_SDK_ROOT)/tools/adb forward tcp:1234: tcp:1234
	$(ANDROID_SDK_ROOT)/tools/adb shell $(INSTALL_DIR)/gdbserver :1234 $(INSTALL_DIR)/test