echo libtoolize
libtoolize --copy --force

echo "aclocal"
aclocal

echo "autoheader"
autoheader

echo "create NEWS, README, AUTHORS and ChangeLog"
touch NEWS README AUTHORS ChangeLog

echo "autoreconf"
autoreconf -i

echo "automake"
automake -a