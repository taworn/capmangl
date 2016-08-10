pushd %~dp0

cd zlib
nmake /nologo /fwin32\Makefile.msc
mkdir x64
copy /y zlib.lib x64
cd ..

cd libpng
nmake /nologo /fscripts\makefile.vcwin32
mkdir x64
copy /y libpng.lib x64
cd ..

cd libpng
nmake /nologo /fscripts\makefile.vcwin32 clean
copy /y scripts\pnglibconf.h.prebuilt pnglibconf.h
cd ..
cd zlib
nmake /nologo /fwin32\Makefile.msc clean
cd ..

popd
