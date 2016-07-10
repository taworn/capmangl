#include <windows.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "opengl.hxx"
#include "render.hxx"

bool RenderInit()
{
	return true;
}

void RenderUninit()
{
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	SwapBuffers(hdc);
}

