######
##	Root configuration file shared by all android projects.
######

ifeq ($(ANDROID_NDK_ROOT),)
	export ANDROID_NDK_ROOT=/cygdrive/c/android-ndk-r7c
endif
ifeq ($(ANDROID_SDK_ROOT),)
	export ANDROID_SDK_ROOT=/cygdrive/c/android-sdk
endif
ifeq ($(ANDROID_PLATFORM),)
	export ANDROID_PLATFORM=android-3
endif
ifeq ($(ANDROID_HOST),)
	export ANDROID_HOST=windows
endif
ifeq ($(NEON), yes)
	export MARCH=armv7-a
	export ANDROID_EABI=armeabi-v7a
else
	export MARCH=armv5te
	export ANDROID_EABI=armeabi
endif

export ANDROID_GCC_VERSIONS=4.6 4.4.3 4.4.0 4.2.1 # list of preferred GCC versions seperated by SPACE (most preferred first)
export ANDROID_GCC_PREFIXES=arm-eabi arm-linux-androideabi # list of preferred GCC prefixes seperated by SPACE (most preferred first)

# Output directory
export OUTPUT_DIR=$(shell pwd)/output
$(shell mkdir -p $(OUTPUT_DIR))

# Some toolchains cannot find libs in cygwin paths
export OUTPUT_DIR_CYGWIN=$(shell \
	if [[ $(OUTPUT_DIR) = /cygdrive/c/* ]]; then \
	echo $(OUTPUT_DIR) | sed -e "s/\/cygdrive\/c/C:/g"; \
	fi \
)

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

export ANDROID_NDK_TOOL_BASE=$(shell \
for gcc_prefix in $(ANDROID_GCC_PREFIXES) ; do \
	for gcc_version in $(ANDROID_GCC_VERSIONS) ; do \
		if [ -r $(ANDROID_NDK_ROOT)/build/prebuilt/$(ANDROID_HOST)/$$gcc_prefix-$$gcc_version ] ; then  \
			echo "$(ANDROID_NDK_ROOT)/build/prebuilt/$(ANDROID_HOST)/$$gcc_prefix-$$gcc_version"; \
			break; \
		elif [ -r $(ANDROID_NDK_ROOT)/toolchains/$$gcc_prefix-$$gcc_version/prebuilt/$(ANDROID_HOST) ] ; then  \
			echo "$(ANDROID_NDK_ROOT)/toolchains/$$gcc_prefix-$$gcc_version/prebuilt/$(ANDROID_HOST)"; \
			break; \
		fi \
		done \
	done \
)
export ANDROID_GCC_PREFIX=$(shell \
for gcc_prefix in $(ANDROID_GCC_PREFIXES) ; do \
	for gcc_version in $(ANDROID_GCC_VERSIONS) ; do \
		if [ -r $(ANDROID_NDK_ROOT)/build/prebuilt/$(ANDROID_HOST)/$$gcc_prefix-$$gcc_version ] ; then  \
			echo "$$gcc_prefix"; \
			break; \
		elif [ -r $(ANDROID_NDK_ROOT)/toolchains/$$gcc_prefix-$$gcc_version/prebuilt/$(ANDROID_HOST) ] ; then  \
			echo "$$gcc_prefix"; \
			break; \
		fi \
		done \
	done \
)

ifeq ($(ANDROID_NDK_TOOL_BASE),)
	export ANDROID_NDK_TOOL_BASE="Err_Unable_to_recognize_the_NDK_structure_for_ANDROID_NDK_TOOL_BASE"
endif

export ANDROID_GNU_STL_ROOT=$(shell \
if [ -r $(ANDROID_NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/libs/$(ANDROID_EABI) ] ; then  \
	echo "$(ANDROID_NDK_ROOT)/sources/cxx-stl/gnu-libstdc++"; \
else \
	for gcc_version in $(ANDROID_GCC_VERSIONS) ; do \
		if [ -r $(ANDROID_NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/$$gcc_version/libs/$(ANDROID_EABI) ] ; then  \
			echo "$(ANDROID_NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/$$gcc_version"; \
			break; \
		fi \
	done \
fi \
)

ifeq ($(ANDROID_GNU_STL_ROOT),)
	export ANDROID_GNU_STL_ROOT="Err_Unable_to_find_gnu_stl_root_dir"
endif

##################################################################################

export PATH:=$(PATH):$(ANDROID_NDK_TOOL_BASE)/bin
export AR=$(ANDROID_GCC_PREFIX)-ar
export AS=$(ANDROID_GCC_PREFIX)-as
export CC=$(ANDROID_GCC_PREFIX)-gcc
export CPP=$(ANDROID_GCC_PREFIX)-g++
export STRIP=$(ANDROID_GCC_PREFIX)-strip
export TARGET_ARCH=arm

ifeq ($(ANDROID_GCC_PREFIX), arm-linux-androideabi)
	export ARMELF_XSC=$(ANDROID_NDK_TOOL_BASE)/$(ANDROID_GCC_PREFIX)/lib/ldscripts/armelf_linux_eabi.xsc
else
	export ARMELF_XSC=$(ANDROID_NDK_TOOL_BASE)/$(ANDROID_GCC_PREFIX)/lib/ldscripts/armelf.xsc
endif

ifeq ($(LICENSE),)
	export LICENSE=gpl
endif


export CFLAGS_COMMON=$(CFLAGS) $(DEBUG_FLAGS) -I$(ANDROID_NDK_BASE)/$(ANDROID_PLATFORM)/arch-arm/usr/include -L$(ANDROID_GNU_STL_ROOT)/include \
-msoft-float \
-fpic \
-fno-exceptions \
-mthumb-interwork \
-funwind-tables \
-fstack-protector \
-D__arm__\
-D__ARM_ARCH__ \
-D__ARM_ARCH_5__ \
-D__ARM_ARCH_5T__ \
-D__ARM_ARCH_5E__ \
-D__ARM_ARCH_5TE__ \
-MMD \
-MP \
-fno-short-enums \
-DWEBRTC_ANDROID \
-DWEBRTC_ARCH_ARM \
-D__ANDROID__ \
-DANDROID=1 \

# Size reduction: use separate sections ("-ffunction-sections -fdata-sections") then link executable/shared lib using "-Wl,-gc-sections,-u,main"
export CFLAGS_COMMON+=-ffunction-sections -fdata-sections

ifeq ($(NEON), yes)
export MARCH=armv7-a
export ARCH_ARM_HAVE_NEON=true
export ARCH_ARM_HAVE_ARMV7A=true
export CFLAGS_COMMON+=-march=$(MARCH) -mno-thumb-interwork -mtune=cortex-a8 -mfpu=neon -flax-vector-conversions -DHAVE_NEON=1 -DWEBRTC_DETECT_ARM_NEON -DARCH_ARM_HAVE_NEON -DARCH_ARM_HAVE_ARMV7A \
-D__ARM_ARCH_7__ -D__ARM_ARCH_7A__ -DWEBRTC_ARM_INLINE_CALLS -DWEBRTC_ANDROID_ARMV7A_NEON -DWEBRTC_ARCH_ARM_NEON -Wno-psabi -mfloat-abi=softfp
else
export MARCH=armv5te
export CFLAGS_COMMON+=-march=$(MARCH) -mtune=xscale
endif

export CFLAGS_LIB= $(CFLAGS_COMMON) \
-fomit-frame-pointer \
-fno-strict-aliasing \
-finline-limit=64

ifeq ($(DEBUG), yes)
export CFLAGS_LIB+=-O0 -g -DDEBUG_LEVEL=DEBUG_LEVEL_INFO
else
export CFLAGS_LIB+=-Os -s
DEBUG:=no
endif

export LDFLAGS_COMMON=$(LDFLAGS) -Wl,--gc-sections,-rpath=/system/lib,-rpath-link=$(ANDROID_NDK_BASE)/$(ANDROID_PLATFORM)/arch-arm/usr/lib,-rpath-link=$(OUTPUT_DIR),-dynamic-linker=/system/bin/linker,-T,$(ARMELF_XSC) -L$(ANDROID_NDK_BASE)/$(ANDROID_PLATFORM)/arch-arm/usr/lib -L$(ANDROID_GNU_STL_ROOT)/libs/$(ANDROID_EABI)
export LDFLAGS_COMMON+=-nostdlib -lc -L$(OUTPUT_DIR) -L$(OUTPUT_DIR_CYGWIN)

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
