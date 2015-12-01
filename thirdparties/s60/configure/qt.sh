cd C:\git
git clone git://gitorious.org/+qt-s60-developers/qt/qt-s60.git
cd qt-s60
set PATH=C:\git\qt-s60\bin;%PATH%
configure -platform win32-msvc2008 -xplatform symbian-abld -openssl-linked -qt-sql-sqlite -system-sqlite
make debug-winscw
or
make debug-armv5
or
release-armv5