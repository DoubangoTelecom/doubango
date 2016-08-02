This section explains how to build the project using **CentOS 64** but could be easily adapted for any _Linux_, _Windows_ or _OS X_.<br />




## Preparing the system ##
```
sudo yum update
sudo yum install make libtool autoconf subversion git wget libogg-devel gcc gcc-c++ pkgconfig nasm
```

The first step is to checkout Doubango 2.0 source code:
```
git clone https://github.com/DoubangoTelecom/doubango.git
```

### Building libsrtp ###
libsrtp is optional.
```
git clone https://github.com/cisco/libsrtp/
cd libsrtp
git checkout v1.5.0
CFLAGS="-fPIC" ./configure --enable-pic && make && make install
```

### Building OpenSSL ###
**OpenSSL** is required if you want to use the _RTCWeb Breaker_ module or Secure WebSocket transport (WSS). **OpenSSL version _1.0.1_ is required if you want support for _DTLS-SRTP_**.<br />
This section is only required if you don’t have _OpenSSL_ installed on your system or using version prior to _1.0.1_ and want to enable _DTLS-SRTP_.<br />
A quick way to have _OpenSSL_ may be installing **openssl-devel** package but this version will most likely be outdated (prior to _1.0.1_). Anyway, you can check the version like this:
```
openssl version 
```
To build OpenSSL:
```
wget http://www.openssl.org/source/openssl-1.0.1c.tar.gz
tar -xvzf openssl-1.0.1c.tar.gz
cd openssl-1.0.1c
./config shared --prefix=/usr/local --openssldir=/usr/local/openssl && make && make install
```

### Building libspeex and libspeexdsp ###
**libspeex** (audio codec) an **libspeexdsp** (audio processing and jitter buffer) are optional. It’s highly recommended to enable libspeexdsp.

You can install the devel packages:
```
sudo yum install speex-devel
```
Or build the source by yourself:
```
wget http://downloads.xiph.org/releases/speex/speex-1.2beta3.tar.gz
tar -xvzf speex-1.2beta3.tar.gz
cd speex-1.2beta3
./configure --disable-oggtest --without-libogg && make && make install
```

### Building YASM ###
**YASM** is only required if you want to enable **VPX** (VP8 video codec) or **x264** (H.264 codec).
```
wget http://www.tortall.net/projects/yasm/releases/yasm-1.2.0.tar.gz
tar -xvzf yasm-1.2.0.tar.gz
cd yasm-1.2.0
./configure && make && make install
```

### Building libvpx ###
_Date: December 1, 2012_ <br />
**libvpx** adds support for **VP8** and is optional but highly recommended if you want support for video when using Google Chrome or Mozilla Firefox.

You can install the devel packages:
```
sudo yum install libvpx-devel
```
Or build the source by yourself:
```
git clone http://git.chromium.org/webm/libvpx.git
cd libvpx
./configure --enable-realtime-only --enable-error-concealment --disable-examples --enable-vp8 --enable-pic --enable-shared --as=yasm
make && make install
```

### Building libyuv ###
**libyuv** is optional. Adds support for video scaling and chroma conversion.
```
mkdir libyuv && cd libyuv
svn co http://src.chromium.org/svn/trunk/tools/depot_tools . 
./gclient config http://libyuv.googlecode.com/svn/trunk
./gclient sync && cd trunk
make -j6 V=1 -r libyuv BUILDTYPE=Release
#cp out/Release/obj.target/libyuv.a /usr/local/lib
#mkdir /usr/local/include/libyuv && cp -r include/* /usr/local/include/libyuv
```

### Building libopus ###
**libopus** is optional but highly recommended as it’s an MTI codec for WebRTC. Adds support for [Opus audio codec](http://www.opus-codec.org/).
```
wget http://downloads.xiph.org/releases/opus/opus-1.0.2.tar.gz
tar -xvzf opus-1.0.2.tar.gz
cd opus-1.0.2
./configure --with-pic --enable-float-approx && make && make install
```

### Building opencore-amr ###
**opencore-amr** is optional. Adds support for **AMR** audio codec.
```
git clone git://opencore-amr.git.sourceforge.net/gitroot/opencore-amr/opencore-amr
cd opencore-amr && autoreconf --install && ./configure && make && make install
```

### Building libgsm ###
**libgsm** is optional. Adds support for **GSM** audio codec.

You can install the devel packages (**recommended**):
```
sudo yum install gsm-devel
```
Or build the source by yourself:
```
wget http://www.quut.com/gsm/gsm-1.0.13.tar.gz
tar -xvzf gsm-1.0.13.tar.gz
cd gsm-1.0-pl13 && make && make install
#cp -rf ./inc/* /usr/local/include
#cp -rf ./lib/* /usr/local/lib
```

### Building g729 ###
**G729** is optional. Adds support for **G.729** audio codec.
```
svn co http://g729.googlecode.com/svn/trunk/ g729b
cd g729b
./autogen.sh && ./configure --enable-static --disable-shared && make && make install
```

### Building iLBC ###
**iLBC** is optional. Adds support for **iLBC** audio codec.
```
svn co http://doubango.googlecode.com/svn/branches/2.0/doubango/thirdparties/scripts/ilbc
cd ilbc
wget http://www.ietf.org/rfc/rfc3951.txt
awk -f extract.awk rfc3951.txt
./autogen.sh && ./configure
make && make install
```

### Building x264 ###
_Date: December 2, 2012_ <br />
**x264** is optional and adds support for **H.264** video codec (requires FFmpeg).
```
wget ftp://ftp.videolan.org/pub/x264/snapshots/last_x264.tar.bz2
tar -xvjf last_x264.tar.bz2
# the output directory may be difference depending on the version and date
cd x264-snapshot-20121201-2245
./configure --enable-shared --enable-pic && make && make install
```

### Building FFmpeg ###
_Date: December 2, 2012_ <br />
**FFmpeg** is optional and adds support for **H.263**, **H.264** (requires **x264**) and **MP4V-ES** video codecs.
```
git clone git://source.ffmpeg.org/ffmpeg.git ffmpeg
cd ffmpeg

# grap a release branch
git checkout n1.2

# configure source code
./configure \
--extra-cflags="-fPIC" \
--extra-ldflags="-lpthread" \
\
--enable-pic --enable-memalign-hack --enable-pthreads \
--enable-shared --disable-static \
--disable-network --enable-pthreads \
--disable-ffmpeg --disable-ffplay --disable-ffserver --disable-ffprobe \
\
--enable-gpl --enable-nonfree \
\
--disable-debug

# to force enabling h264, append to the configure command: --enable-libx264 --enable-encoder=libx264 --enable-decoder=h264
# to force enabling h263 and h263+, append to the configure command: --enable-encoder=h263 --enable-encoder=h263p --enable-decoder=h263

# build and install
make && make install
```

### Building `OpenH264` ###
**OpenH264** is optional. Adds support for **H.264** constrained baseline video codec.
```
git clone https://github.com/cisco/openh264.git
cd openh264
git checkout v1.5.0
make ENABLE64BIT=Yes # Use ENABLE64BIT=No for 32bit platforms
make install
```

### Building Doubango ###
  * Minimal build
```
cd doubango && ./autogen.sh && ./configure --with-ssl --with-srtp --with-speexdsp
make && make install
```
  * Recommended build
```
cd doubango && ./autogen.sh && ./configure --with-ssl --with-srtp --with-speexdsp --with-ffmpeg --with-opus
make && make install
```
  * Full build
```
cd doubango && ./autogen.sh && ./configure --with-ssl --with-srtp --with-vpx --with-yuv --with-amr --with-speex --with-speexdsp --enable-speexresampler --enable-speexdenoiser --with-opus --with-gsm --with-ilbc --with-g729 --with-ffmpeg
make && make install
```

<br />
Et voilà
