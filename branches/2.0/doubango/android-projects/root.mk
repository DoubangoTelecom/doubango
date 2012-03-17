######
##	Root configuration file shared by all android projects.
######

export ANDROID_NDK_ROOT=/cygdrive/c/android-ndk-r4b
export ANDROID_SDK_ROOT=/cygdrive/c/android-sdk
export ANDROID_PLATFORM=android-3
export ANDROID_HOST=windows
export ANDROID_GCC_VER=4.4.0


# Output directory
export OUTPUT_DIR=$(shell pwd)/output
$(shell mkdir -p $(OUTPUT_DIR))

# Path where to copy executables -on the device or emulator-
export EXEC_DIR=/data/tmp
# Path where to copy libraries (*.so) -on the device or emulator-
export LIB_DIR=/system/lib

# Test the NDK directory structure
export ANDROID_NDK_BASE=$(shell if [ -r $(ANDROID_NDK_ROOT)/build/platforms ] ; then \
		echo $(ANDROID_NDK_ROOT)/build/platforms ; \
	elif [ -r $(ANDROID_NDK_ROOT)/platforms ] ; then \
		echo $(ANDROID_NDK_ROOT)/platforms ; \
	else \
		echo "Err_Unable_to_recognize_the_NDK_structure_for_ANDROID_NDK_BASE" ; \
	fi)

export ANDROID_NDK_TOOL_BASE=$(shell if [ -r $(ANDROID_NDK_ROOT)/build/prebuilt/$(ANDROID_HOST)/arm-eabi-$(ANDROID_GCC_VER) ] ; then \
		echo $(ANDROID_NDK_ROOT)/build/prebuilt/$(ANDROID_HOST)/arm-eabi-$(ANDROID_GCC_VER) ; \
	elif [ -r $(ANDROID_NDK_ROOT)/toolchains/arm-eabi-$(ANDROID_GCC_VER)/prebuilt/$(ANDROID_HOST) ] ; then \
		echo $(ANDROID_NDK_ROOT)/toolchains/arm-eabi-$(ANDROID_GCC_VER)/prebuilt/$(ANDROID_HOST) ; \
	else \
		echo "Err_Unable_to_recognize_the_NDK_structure_for_ANDROID_NDK_TOOL_BASE" ; \
	fi)

##################################################################################

export AR=arm-eabi-ar
export CC=arm-eabi-gcc-$(ANDROID_GCC_VER)
export CPP=arm-eabi-g++


ifeq ($(LICENSE),)
	export LICENSE=gpl
endif


export CFLAGS_COMMON=$(CFLAGS) $(DEBUG_FLAGS) -I$(ANDROID_NDK_BASE)/$(ANDROID_PLATFORM)/arch-arm/usr/include \
-msoft-float \
-fpic \
-mthumb-interwork \
-ffunction-sections \
-funwind-tables \
-fstack-protector \
-D__ARM_ARCH_5__ \
-D__ARM_ARCH_5T__ \
-D__ARM_ARCH_5E__ \
-D__ARM_ARCH_5TE__ \
-MMD \
-MP \
-fno-short-enums \
-DANDROID 

ifeq ($(NEON), yes)
export MARCH=armv7-a
export CFLAGS_COMMON+=-march=$(MARCH) -mtune=cortex-a8 -mfpu=neon -DHAVE_NEON=1 \
-D__ARM_ARCH_7__ -D__ARM_ARCH_7A__ -Wno-psabi -mfloat-abi=softfp
else
export MARCH=armv5te
export CFLAGS_COMMON+=-march=$(MARCH) -mtune=xscale
endif

export CFLAGS_LIB= $(CFLAGS_COMMON) \
-Os \
-fomit-frame-pointer \
-fno-strict-aliasing \
-finline-limit=64 \
-DANDROID
#-mthumb

export LDFLAGS_COMMON=$(LDFLAGS) -Wl,-rpath=/system/lib,-rpath-link=$(ANDROID_NDK_BASE)/$(ANDROID_PLATFORM)/arch-arm/usr/lib,-rpath-link=$(OUTPUT_DIR),-dynamic-linker=/system/bin/linker,-T,$(ANDROID_NDK_TOOL_BASE)/arm-eabi/lib/ldscripts/armelf.xsc -L$(ANDROID_NDK_BASE)/$(ANDROID_PLATFORM)/arch-arm/usr/lib
export LDFLAGS_COMMON+=-nostdlib -lc -ldl -L$(OUTPUT_DIR)

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
