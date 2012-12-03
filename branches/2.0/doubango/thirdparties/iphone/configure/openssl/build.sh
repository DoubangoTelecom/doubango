#!/bin/bash
TARGET=openssl-1.0.1-beta2
SDK_VERSION=4.3

OPATH=$PATH

############################################################

build_openssl() {

LIBNAME=$1
DISTDIR=`pwd`/dist-$LIBNAME
PLATFORM=$2

echo "Building binary for iPhone $LIBNAME $PLATFORM to $DISTDIR"

echo Removing ${TARGET}
/bin/rm -rf ${TARGET}
echo Extracting ${TARGET}
tar zxf ${TARGET}.tar.gz

case $LIBNAME in
device)  ARCH="armv6";ASSEMBLY="no-asm";;
*)       ARCH="i386";ASSEMBLY="";;
esac

cd ${TARGET}

echo Patching crypto/ui/ui_openssl.c
echo From: `fgrep 'intr_signal;' crypto/ui/ui_openssl.c`
perl -pi.bak \
    -e 's/static volatile sig_atomic_t intr_signal;/static volatile int intr_signal;/;' \
    crypto/ui/ui_openssl.c
echo To: `fgrep 'intr_signal;' crypto/ui/ui_openssl.c`

# Compile a version for the device...

PATH=/Developer/Platforms/${PLATFORM}.platform/Developer/usr/bin:$OPATH
export PATH

SDKPATH="/Developer/Platforms/${PLATFORM}.platform/Developer/SDKs/${PLATFORM}${SDK_VERSION}.sdk"

mkdir ${DISTDIR}

./config --openssldir=${DISTDIR} ${ASSEMBLY}

perl -pi.bak \
    -e "s;CC= cc;CC = /Developer/Platforms/${PLATFORM}.platform/Developer/usr/bin/gcc; ;" \
    -e "s;CFLAG= ;CFLAG=-arch ${ARCH} -isysroot ${SDKPATH} ; ;" \
    -e "s;-arch i386;-arch ${ARCH}; ;" \
	Makefile

case $LIBNAME in
simulator)
	perl -pi.bak \
	    -e 'if (/LIBDEPS=/) { s/\)}";/\)} -L.";/; }' \
            Makefile.shared
	(cd apps; ln -s ${SDKPATH}/usr/lib/crt1.10.5.o crt1.10.6.o);
	(cd test; ln -s ${SDKPATH}/usr/lib/crt1.10.5.o crt1.10.6.o);
	;;
esac

make
make install

cd ..

echo Clean-up ${TARGET}
/bin/rm -rf ${TARGET}
}

build_openssl "device" "iPhoneOS"
build_openssl "simulator" "iPhoneSimulator"

### Then, combine them into one..

echo "Creating combined binary into directory 'dist'"
/bin/rm -rf dist
mkdir dist
(cd dist-device; tar cf - . ) | (cd dist; tar xf -)

for i in crypto ssl
do
    lipo -create dist-device/lib/lib$i.a dist-simulator/lib/lib$i.a \
	-o dist/lib/lib$i.a
done

/bin/rm -rf dist-simulator dist-device

echo "Now package is ready in 'dist' directory'"