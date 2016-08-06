/**
 * @file opengl.hxx
 * @desc OpenGL header.
 */
#ifndef OPENGL_HXX
#define OPENGL_HXX

#ifdef __cplusplus
extern "C" {
#endif

extern bool active;
extern HDC hdc;
extern HGLRC hrc;

/**
 * Initializes OpenGL module.
 */
bool gl_init(HWND hwnd);

/**
 * Uninitializes OpenGL module.
 */
void gl_uninit(HWND hwnd);

#ifdef __cplusplus
}
#endif

#endif // OPENGL_HXX

