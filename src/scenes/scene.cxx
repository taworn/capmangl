#include <windows.h>
#include <assert.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <boost/log/trivial.hpp>
#include <GL/glew.h>
#include "../game.hxx"
#include "scene.hxx"

Scene::~Scene()
{
	fini();
}

Scene::Scene()
	: hdc(Game::instance()->getDevice())
	, normalShader(Game::instance()->getNormalShader()), textShader(Game::instance()->getTextShader())
	, screenRect(), smallFont(), frameCount(0), fps(0), timeStart(0)
{
	int params[4];
	glGetIntegerv(GL_VIEWPORT, params);
	screenRect.left = params[0];
	screenRect.top = params[1];
	screenRect.right = screenRect.left + params[2];
	screenRect.bottom = screenRect.top + params[3];

	smallFont.load("C:\\WINDOWS\\Fonts\\arial.ttf", 24);
	smallFont.setColor(1.0f, 1.0f, 1.0f, 1.0f);

	timeStart = GetTickCount();

	init();
}

void Scene::init()
{
}

void Scene::fini()
{
}

void Scene::computeFPS()
{
	frameCount++;
	ULONGLONG timeCurrent = GetTickCount();
	ULONGLONG timeUsage = timeCurrent - timeStart;
	if (timeUsage > 1000) {
		fps = frameCount * 1000 / timeUsage;
		timeStart = timeCurrent;
		frameCount = 0;
		BOOST_LOG_TRIVIAL(trace) << "FPS: " << fps;
		wchar_t buffer[64];
		wsprintf(buffer, L"FPS: %ld\n", fps);
		OutputDebugStringW(buffer);
	}
}

void Scene::drawFPS()
{
	ULONGLONG fps = getFPS();
	char buffer[64];
	sprintf(buffer, "%ld", (long)fps);

	RECT rc = getScreenRect();
	float sx = 2.0f / (rc.right - rc.left);
	float sy = 2.0f / (rc.bottom - rc.top);

	float w, h;
	textShader.useProgram();
	smallFont.measureText(buffer, &w, &h, sx, sy);
	smallFont.drawText(buffer, 1 - w * 2, -1 + h, sx, sy);
}

bool Scene::handleKey(HWND hwnd, WPARAM key)
{
	return false;
}

void Scene::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	computeFPS();
	drawFPS();
	SwapBuffers(getDevice());
}

