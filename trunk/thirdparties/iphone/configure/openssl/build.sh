#!/bin/sh

######################################
# Copyright (C) 2010 Larent Etiemble
######################################

VERSION="4.0"
BASEDIR=`pwd`

function patch() {
    echo "Patching OpenSSL..."
    FILE="./crypto/ui/ui_openssl.c"
    cp "$FILE" "$FILE.orig"
    sed -e "s/static volatile sig_atomic_t intr_signal;/static volatile int intr_signal;/"  "$FILE.orig" > "$FILE"
}

function build() {
    TARGET="$1"
    ARCHITECTURE="$2"
    PREFIX="/Developer/Platforms/$TARGET.platform/Developer"

    mkdir "$BASEDIR/$TARGET-$ARCHITECTURE"
    ./Configure BSD-generic32 --openssldir="$BASEDIR/$TARGET-$ARCHITECTURE"

    LINE_CC="CC= $PREFIX/usr/bin/gcc -arch $ARCHITECTURE"
    LINE_CFLAG=`egrep "^CFLAG=" Makefile`
    LINE_CFLAG=`echo $LINE_CFLAG | awk -F"=" '{ print $2 }'`
    LINE_CFLAG="CFLAG= -isysroot $PREFIX/SDKs/$TARGET$VERSION.sdk $LINE_CFLAG"

    cp Makefile Makefile.tmp
    sed -e "s|^CC=.*$|$LINE_CC|" -e "s|^CFLAG=.*$|$LINE_CFLAG|" Makefile.tmp > Makefile

    make clean
    find . -name "*.o" -exec rm {} \;

    make
    make install
}

function merge() {
    DISTDIR="iPhoneOS"
    
    rm -Rf "$DISTDIR"
    FILES=`ls -d iPhone*`
    mkdir -p "$DISTDIR/include"
    mkdir -p "$DISTDIR/lib"
    
    for f in $FILES; do
        cp -R "$f/include/" "$DISTDIR/include"
    done

    OPTIONS=""
    for f in $FILES; do
        OPTIONS="$OPTIONS $f/lib/libssl.a"
    done
    lipo -create $OPTIONS -output $DISTDIR/lib/libssl.a
    
    OPTIONS=""
    for f in $FILES; do
        OPTIONS="$OPTIONS $f/lib/libcrypto.a"
    done
    lipo -create $OPTIONS -output $DISTDIR/lib/libcrypto.a
}

patch

build   "iPhoneSimulator"   "i386"
build   "iPhoneOS"          "armv6"
build   "iPhoneOS"          "armv7"

merge
