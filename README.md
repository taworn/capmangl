**capmangl**
-------

The reversed Pacman for OpenGL.

**Preamble**

This game is NOT a fun game.  It's just a simple game to let you learn how to writing a game.

**Tools and Library Usage**

 - Microsoft Visual Studio 2015 Community
 - Boost v.1.61
 - zlib (included in package)
 - libpng (included in package)
 - freetype (included in package)
 - GLEW (included in package)

**Build Boost**

Use following scripts to compile.  After you run this script, you can go to sleep and checkout after morning.

For 32-bit:

	bjam --layout=tagged --without-python --without-wave --stagedir=./stage/vc toolset=msvc-14.0 threading=multi link=shared runtime-link=shared variant=release stage
	bjam --layout=tagged --without-python --without-wave --stagedir=./stage/vc toolset=msvc-14.0 threading=multi link=static runtime-link=static variant=release stage
	bjam --layout=tagged --without-python --without-wave --stagedir=./stage/vc toolset=msvc-14.0 threading=multi link=static runtime-link=shared variant=release stage
	bjam --layout=tagged --without-python --without-wave --stagedir=./stage/vc toolset=msvc-14.0 threading=multi link=shared runtime-link=shared variant=debug stage
	bjam --layout=tagged --without-python --without-wave --stagedir=./stage/vc toolset=msvc-14.0 threading=multi link=static runtime-link=static variant=debug stage
	bjam --layout=tagged --without-python --without-wave --stagedir=./stage/vc toolset=msvc-14.0 threading=multi link=static runtime-link=shared variant=debug stage

For 64-bit:

	bjam --layout=tagged --without-python --without-wave --stagedir=./stage/vc64 toolset=msvc-14.0 address-model=64 threading=multi link=shared runtime-link=shared variant=release stage
	bjam --layout=tagged --without-python --without-wave --stagedir=./stage/vc64 toolset=msvc-14.0 address-model=64 threading=multi link=static runtime-link=static variant=release stage
	bjam --layout=tagged --without-python --without-wave --stagedir=./stage/vc64 toolset=msvc-14.0 address-model=64 threading=multi link=static runtime-link=shared variant=release stage
	bjam --layout=tagged --without-python --without-wave --stagedir=./stage/vc64 toolset=msvc-14.0 address-model=64 threading=multi link=shared runtime-link=shared variant=debug stage
	bjam --layout=tagged --without-python --without-wave --stagedir=./stage/vc64 toolset=msvc-14.0 address-model=64 threading=multi link=static runtime-link=static variant=debug stage
	bjam --layout=tagged --without-python --without-wave --stagedir=./stage/vc64 toolset=msvc-14.0 address-model=64 threading=multi link=static runtime-link=shared variant=debug stage

**Known Bugs**

 - Some old PC may not run OpenGL 3.3.

**Thank**

 - google.com - to search every time when you want to learn any new things
 - stackoverflow.com - good community
 - www.opengl-tutorial.org - good learning OpenGL
 - en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_01 - text rendering
 - learnopengl.com - another good learning OpenGL
 - www.spriters-resource.com - provide graphics, thank to Techokami and Superjustinbros
 - www.bruneras.com/arcadegamestudio/help_sprite_sheet_formats.php - provide graphics
 - www.atlassian.com/git - learn Git
 - and don't forget github.com - to host this project

----------
Taworn T.

My English is very poor, I'm not English native, sorry. T_T

