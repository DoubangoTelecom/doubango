echo libtoolize
libtoolize --copy --force

echo "aclocal"
aclocal

echo "autoheader"
autoheader

echo "create NEWS, README, AUTHORS and ChangeLog"
touch NEWS README AUTHORS ChangeLog

echo "automake"
automake -a

echo "autoconf"
autoconf