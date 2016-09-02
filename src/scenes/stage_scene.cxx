/**
 * @file stage_scene.cxx
 * @desc Stage scene module.
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
#include "stage_scene.hxx"

StageScene::~StageScene()
{
	BOOST_LOG_TRIVIAL(debug) << "StageScene::~StageScene() called";
	fini();
}

StageScene::StageScene()
	: Scene()
{
	BOOST_LOG_TRIVIAL(debug) << "StageScene::StageScene() called";
	init();
	timeStart = GetTickCount();
	timeUsed = 0;
	BOOST_LOG_TRIVIAL(debug) << "start stage " << GameData::instance()->getStage() + 1;
}

void StageScene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "StageScene::init() called";
}

void StageScene::fini()
{
	BOOST_LOG_TRIVIAL(debug) << "StageScene::fini() called";
}

void StageScene::handleActivate(HWND hwnd, bool active)
{
	if (active) {
		BOOST_LOG_TRIVIAL(debug) << "window is activate";
		timeStart = GetTickCount();
	}
}

bool StageScene::handleKey(HWND hwnd, WPARAM key)
{
	if (key == VK_RETURN) {
		// ENTER
		BOOST_LOG_TRIVIAL(debug) << "ENTER keydown";
		Game::instance()->changeScene(Game::SCENE_PLAY);
		return true;
	}
	return false;
}

void StageScene::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	RECT rc = getScreenRect();
	float sx = 2.0f / (rc.right - rc.left);
	float sy = 2.0f / (rc.bottom - rc.top);
	float w, h;

	char buffer[64];
	sprintf(buffer, "Stage %d", GameData::instance()->getStage() + 1);
	Font *font = Game::instance()->getBigFont();
	font->setColor(1.0f, 1.0f, 1.0f, 1.0f);
	font->measure(buffer, &w, &h, sx, sy);
	font->draw(buffer, 0 - (w / 2), 0.0f, sx, sy);

	ULONGLONG timeUsed = GetTickCount() - timeStart;
	timeStart = GetTickCount();
	this->timeUsed += timeUsed;
	if (this->timeUsed >= 2000)
		Game::instance()->changeScene(Game::SCENE_PLAY);

	computeFPS();
	SwapBuffers(Game::instance()->getDevice());
}

