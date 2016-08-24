/**
 * @file gameover_scene.cxx
 * @desc Game over scene module.
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
#include "gameover_scene.hxx"
#include "title_scene.hxx"

GameOverScene::~GameOverScene()
{
	BOOST_LOG_TRIVIAL(debug) << "GameOverScene::~GameOverScene() called";
	fini();
}

GameOverScene::GameOverScene()
	: Scene()
{
	BOOST_LOG_TRIVIAL(debug) << "GameOverScene::GameOverScene() called";
	init();
}

void GameOverScene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "GameOverScene::init() called";
}

void GameOverScene::fini()
{
	BOOST_LOG_TRIVIAL(debug) << "GameOverScene::fini() called";
}

bool GameOverScene::handleKey(HWND hwnd, WPARAM key)
{
	if (key == VK_RETURN) {
		// ENTER
		BOOST_LOG_TRIVIAL(debug) << "ENTER keydown";
		Game::instance()->changeScene(Game::SCENE_TITLE);
		return true;
	}
	return false;
}

void GameOverScene::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	RECT rc = getScreenRect();
	float sx = 2.0f / (rc.right - rc.left);
	float sy = 2.0f / (rc.bottom - rc.top);
	float w, h;

	char text[] = "Game Over";
	Font *font = Game::instance()->getBigFont();
	font->setColor(1.0f, 0.0f, 0.0f, 1.0f);
	font->measure(text, &w, &h, sx, sy);
	font->draw(text, 0 - (w / 2), 0.0f, sx, sy);

	computeFPS();
	SwapBuffers(Game::instance()->getDevice());
}

