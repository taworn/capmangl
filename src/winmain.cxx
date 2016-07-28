#include <windows.h>
#include <boost/log/trivial.hpp>
#include <GL/glew.h>
#include "opengl.hxx"
#include "render.hxx"

bool Initialize(HWND hwnd)
{
	if (!OpenGLInit(hwnd))
		return false;
	if (!RenderInit())
		return false;
	return true;
}

void Uninitialize(HWND hwnd)
{
	RenderUninit();
	OpenGLUninit(hwnd);
}

LRESULT CALLBACK
WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_ACTIVATE:
		if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE)
			active = true;
		else
			active = false;
		return 0;
	case WM_DESTROY:
		Uninitialize(hwnd);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	BOOST_LOG_TRIVIAL(trace) << "CapmanGL";

	const wchar_t windowClassName[] = L"capmangl";
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = windowClassName;
	wc.hIconSm = 0;
	RegisterClassEx(&wc);

	HWND hwnd = CreateWindowEx(0,
		windowClassName, L"Capman",
		WS_POPUP,
		0, 0, 1024, 768,
		HWND_DESKTOP, 0, hInstance, NULL);
	if (hwnd == 0) {
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);

	if (!Initialize(hwnd)) {
		Uninitialize(hwnd);
		return 0;
	}

	MSG msg;
	while (1) {
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
			break;
		if (active)
			Render();
	}

	return (int)msg.wParam;
}

