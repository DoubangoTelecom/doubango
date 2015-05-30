DOUBANGO_HOME="../mydoubs/doubango/branches/2.0/doubango/thirdparties/iphone/lib"
ARM64="${DOUBANGO_HOME}/arm64"
ARMV6="${DOUBANGO_HOME}/armv6"
ARMV7="${DOUBANGO_HOME}/armv7"
ARMV7S="${DOUBANGO_HOME}/armv7s"
I386="${DOUBANGO_HOME}/i386"
X86_64="${DOUBANGO_HOME}/x86_64"
OUTPUT_HOME="./doubango-universal/iOS"

for lib in libavcodec-lgpl.a libavcodec.a libavdevice-lgpl.a libavdevice.a libavfilter-lgpl.a libavfilter.a libavformat-lgpl.a libavformat.a libavutil-lgpl.a libavutil.a libcommon.a libcrypto.a libdecoder.a libencoder.a libg729b.a libgsm.a libopenh264.a libopus.a libpostproc.a libprocessing.a libspeex.a libspeexdsp.a libsrtp.a libssl.a libswresample-lgpl.a libswresample.a libswscale-lgpl.a libswscale.a libvpx.a libx264.a libyuv.a
do

libs="${ARM64}/$lib ${ARMV7}/$lib ${ARMV7S}/$lib ${I386}/$lib ${X86_64}/$lib"
#armv6 is deprecated
if [ ! -e ${ARMV6}/$lib ]; then
    echo "\n\n*****${ARMV6}/$lib not found :(******\n\n"
else
    libs="$libs ${ARMV6}/$lib"
fi

echo "lipo ${libs} -create -output ${OUTPUT_HOME}/$lib"
lipo ${libs} -create -output ${OUTPUT_HOME}/$lib

done

echo "done :)"
