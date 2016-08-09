/**
 * @file scene.cxx
 * @desc Game scene module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include "../game.hxx"
#include "scene.hxx"

Scene::~Scene()
{
	fini();
}

Scene::Scene()
	: hdc(0), normalShader(), textShader(), textureShader()
	, screenRect(), smallFont(), normalFont(), bigFont()
	, fps(0), frameCount(0), timeStart(0)
{
	Game *game = Game::instance();
	hdc = game->getDevice();
	normalShader = game->getNormalShader();
	textShader = game->getTextShader();
	textureShader = game->getTextureShader();

	int params[4];
	glGetIntegerv(GL_VIEWPORT, params);
	screenRect.left = params[0];
	screenRect.top = params[1];
	screenRect.right = screenRect.left + params[2];
	screenRect.bottom = screenRect.top + params[3];

	smallFont = game->getSmallFont();
	normalFont = game->getNormalFont();
	bigFont = game->getBigFont();

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
		fps = (int)(frameCount * 1000 / timeUsage);
		timeStart = timeCurrent;
		frameCount = 0;

		wchar_t buffer[64];
		wsprintf(buffer, L"FPS: %d\n", fps);
		OutputDebugStringW(buffer);
		BOOST_LOG_TRIVIAL(trace) << "FPS: " << fps;
	}

	char buffer[64];
	sprintf(buffer, "%d", fps);
	RECT rc = getScreenRect();
	float sx = 2.0f / (rc.right - rc.left);
	float sy = 2.0f / (rc.bottom - rc.top);
	float w, h;
	getTextShader()->useProgram();
	getSmallFont()->setColor(1.0f, 1.0f, 1.0f, 0.5f);
	getSmallFont()->measure(buffer, &w, &h, sx, sy);
	getSmallFont()->draw(buffer, 1 - w * 2, -1 + h, sx, sy);
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
	SwapBuffers(getDevice());
}

