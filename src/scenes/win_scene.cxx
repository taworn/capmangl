/**
 * @file win_scene.cxx
 * @desc Win scene module.
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
#include "../game/common.hxx"
#include "scene.hxx"
#include "win_scene.hxx"

WinScene::~WinScene()
{
	BOOST_LOG_TRIVIAL(debug) << "WinScene::~WinScene() called";
	fini();
}

WinScene::WinScene()
	: Scene()
{
	BOOST_LOG_TRIVIAL(debug) << "WinScene::WinScene() called";
	init();
}

void WinScene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "WinScene::init() called";
}

void WinScene::fini()
{
	BOOST_LOG_TRIVIAL(debug) << "WinScene::fini() called";
}

bool WinScene::handleKey(HWND hwnd, WPARAM key)
{
	return false;
}

void WinScene::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	RECT rc = getScreenRect();
	float sx = 2.0f / (rc.right - rc.left);
	float sy = 2.0f / (rc.bottom - rc.top);
	float w, h;

	char text[] = "You Win";
	Font *font = Game::instance()->getBigFont();
	font->setColor(0.25f, 1.0f, 0.25f, 1.0f);
	font->measure(text, &w, &h, sx, sy);
	font->draw(text, 0 - (w / 2), 0.0f, sx, sy);

	computeFPS();
	SwapBuffers(Game::instance()->getDevice());
}

