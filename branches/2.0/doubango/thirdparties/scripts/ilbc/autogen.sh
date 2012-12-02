echo libtoolize
if [ `(uname -s) 2>/dev/null` == 'Darwin' ]
then
	glibtoolize --copy --force
else
	libtoolize --copy --force
fi

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