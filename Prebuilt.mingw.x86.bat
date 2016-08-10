pushd %~dp0

cd zlib
make -fwin32\Makefile.gcc
mkdir x86
copy /y libz.a x86
cd ..

cd libpng
make -fscripts\makefile.msys
mkdir x86
copy /y libpng.a x86
cd ..

cd libpng
make -fscripts\makefile.msys clean
copy /y scripts\pnglibconf.h.prebuilt pnglibconf.h
cd ..
cd zlib
make -fwin32\Makefile.gcc clean
cd ..

popd
