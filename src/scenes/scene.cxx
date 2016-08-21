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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../game.hxx"
#include "scene.hxx"

Scene::~Scene()
{
	BOOST_LOG_TRIVIAL(debug) << "Scene::~Scene() called";
	fini();
}

Scene::Scene()
	: screenRect()
	, fps(0), frameCount(0), timeStart(0)
{
	BOOST_LOG_TRIVIAL(debug) << "Scene::Scene() called";

	int params[4];
	glGetIntegerv(GL_VIEWPORT, params);
	screenRect.left = params[0];
	screenRect.top = params[1];
	screenRect.right = screenRect.left + params[2];
	screenRect.bottom = screenRect.top + params[3];

	timeStart = GetTickCount();

	init();
}

void Scene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "Scene::init() called";
}

void Scene::fini()
{
	BOOST_LOG_TRIVIAL(debug) << "Scene::fini() called";
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
		BOOST_LOG_TRIVIAL(trace) << "FPS: " << fps;
	}

	char buffer[64];
	sprintf(buffer, "%d", fps);
	RECT rc = getScreenRect();
	float sx = 2.0f / (rc.right - rc.left);
	float sy = 2.0f / (rc.bottom - rc.top);
	float w, h;
	Font *font = Game::instance()->getSmallFont();
	font->setColor(1.0f, 1.0f, 1.0f, 0.5f);
	font->measure(buffer, &w, &h, sx, sy);
	font->draw(buffer, 1 - w, -1, sx, sy);
}

void Scene::handleActivate(HWND hwnd, bool active)
{
	if (active) {
		BOOST_LOG_TRIVIAL(debug) << "window is activate";
		frameCount = 0;
		timeStart = GetTickCount();
	}
	else {
		BOOST_LOG_TRIVIAL(debug) << "window is deactivated";
	}
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
	SwapBuffers(Game::instance()->getDevice());
}

