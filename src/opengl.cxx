#include <windows.h>
#include <GL/glew.h>
#include "opengl.hxx"

bool active = false;
HDC hdc = 0;
HGLRC hrc = 0;

bool OpenGLInit(HWND hwnd)
{
	hdc = GetDC(hwnd);

	int pixelFormat;
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // size of structure
		1,                              // default version
		PFD_DRAW_TO_WINDOW |            // window drawing support
		PFD_SUPPORT_OPENGL |            // opengl support
		PFD_DOUBLEBUFFER,               // double buffering support
		PFD_TYPE_RGBA,                  // RGBA color mode
		32,                             // 32 bit color mode
		0, 0, 0, 0, 0, 0,               // ignore color bits
		0,                              // no alpha buffer
		0,                              // ignore shift bit
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // ignore accumulation bits
		16,                             // 16 bit z-buffer size
		0,                              // no stencil buffer
		0,                              // no aux buffer
		PFD_MAIN_PLANE,                 // main drawing plane
		0,                              // reserved
		0, 0, 0 };                      // layer masks ignored
	pixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pixelFormat, &pfd);

	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	if (glewInit() != GLEW_OK) {
		return false;
	}

	return true;
}

void OpenGLUninit(HWND hwnd)
{
	wglMakeCurrent(hdc, NULL);
	if (hrc != 0) {
		wglDeleteContext(hrc);
		hrc = 0;
	}
	if (hdc != 0) {
		ReleaseDC(hwnd, hdc);
		hdc = 0;
	}
}

