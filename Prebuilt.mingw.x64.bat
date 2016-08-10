pushd %~dp0

cd zlib
make -fwin32\Makefile.gcc
mkdir x64
copy /y libz.a x64
cd ..

cd libpng
make -fscripts\makefile.msys
mkdir x64
copy /y libpng.a x64
cd ..

cd libpng
make -fscripts\makefile.msys clean
copy /y scripts\pnglibconf.h.prebuilt pnglibconf.h
cd ..
cd zlib
make -fwin32\Makefile.gcc clean
cd ..

popd
