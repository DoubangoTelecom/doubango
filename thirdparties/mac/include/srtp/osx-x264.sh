echo ""
echo "----------------------------------------------------"
echo "-- Building x264 for MAC OS X"
echo "-- Example: $0 i386"
echo "-- Valid parameters: i386 x86_64 ppc"
echo "-- Location: copy this file in x264 root directory"
echo "-- Git URL: git://git.videolan.org/x264.git"
echo "-- Git date: Wed Aug 10 00:16:46 2011 -070"
echo "--"
echo ""

if [ "$1" == "i386" ]; then
	export ARCH="i386"
	export CFLAGS=$CFLAGS" -arch $ARCH"
	export LDFLAGS=$LDFLAGS" -arch $ARCH"
	export HOST="$ARCH-apple-darwin"
elif [ "$1" == "x86_64" ]; then
	export ARCH="x86_64"
	export CFLAGS=$CFLAGS" -arch $ARCH"
	export LDFLAGS=$LDFLAGS" -arch $ARCH"
	export HOST="$ARCH-apple-darwin"
elif [ "$1" == "ppc" ]; then
	export ARCH="ppc"
	export CFLAGS=$CFLAGS" -arch $ARCH"
	export LDFLAGS=$LDFLAGS" -arch $ARCH"
	export HOST="$ARCH-apple-darwin"
else
	echo "\n=ERROR: '$1' is not a valid architecture\n"
	exit 1
fi

export HOME="$( cd "$( dirname "$0" )" && pwd )"
export PREFIX=$HOME"/"$ARCH

echo "PREFIX="$PREFIX

echo "Start building...\n\n"

$HOME/configure --prefix=$PREFIX  --host=$HOST --enable-static --extra-cflags="$CFLAGS" --extra-ldflags="$LDFLAGS" --disable-cli

make clean && make && make install

#mv ./x264.h $PREFIX/include/x264.h
#mv ./libx264.a $PREFIX/lib/libx264.a