rem Genarate compatible win32 libs for Visual Studio C++ libraries

set ROOT_DIR=.
set VC_DIR="C:\Program Files\Microsoft Visual Studio 9.0\VC"
set MAKEDEF_DIR=%ROOT_DIR%\thirdparties\bin\win32\makedef
set WIN32_LIBS_DIR=%ROOT_DIR%\thirdparties\lib\win32
set WIN32_BIN_DIR=%ROOT_DIR%\thirdparties\bin\win32


%MAKEDEF_DIR%\MakeDef.exe /DEF:%WIN32_LIBS_DIR%\libxml\libxml2.dll.def /IMAGE:%WIN32_BIN_DIR%\libxml\libxml2-2.dll
lib.exe /DEF:%WIN32_LIBS_DIR%\libxml\libxml2.dll.def /OUT:%WIN32_LIBS_DIR%\libxml\vs_libxml2.dll.a /MACHINE:X86
DEL %WIN32_LIBS_DIR%\libxml\libxml2.dll.def
DEL %WIN32_LIBS_DIR%\libxml\vs_libxml2.dll.exp


%MAKEDEF_DIR%\MakeDef.exe /DEF:%WIN32_LIBS_DIR%\curl\libcurl.dll.def /IMAGE:%WIN32_BIN_DIR%\curl\libcurl-4.dll
lib.exe /DEF:%WIN32_LIBS_DIR%\curl\libcurl.dll.def /OUT:%WIN32_LIBS_DIR%\curl\vs_libcurl.dll.a /MACHINE:X86
DEL %WIN32_LIBS_DIR%\curl\libcurl.dll.def
DEL %WIN32_LIBS_DIR%\curl\vs_libcurl.dll.exp

pause