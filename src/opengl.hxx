#ifndef OPENGL_HXX
#define OPENGL_HXX

#ifdef __cplusplus
extern "C" {
#endif

extern bool active;
extern HDC hdc;
extern HGLRC hrc;

bool OpenGLInit(HWND hwnd);
void OpenGLUninit(HWND hwnd);

#ifdef __cplusplus
}
#endif

#endif // OPENGL_HXX

