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
	menuIndex = 0;
}

void GameOverScene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "GameOverScene::init() called";
	PNGImage imageUI(".\\res\\ui.png");
	spriteUI = new Sprite();
	spriteUI->init(&imageUI, 2, 2);
}

void GameOverScene::fini()
{
	BOOST_LOG_TRIVIAL(debug) << "GameOverScene::fini() called";
	if (spriteUI) {
		delete spriteUI;
		spriteUI = NULL;
	}
}

bool GameOverScene::handleKey(HWND hwnd, WPARAM key)
{
	if (key == VK_RETURN) {
		// ENTER
		BOOST_LOG_TRIVIAL(debug) << "ENTER keydown";
		if (menuIndex == 0)
			Game::instance()->changeScene(Game::SCENE_STAGE);
		else if (menuIndex == 1)
			Game::instance()->changeScene(Game::SCENE_TITLE);
		return true;
	}
	else if (key == 0x57 || key == VK_UP) {
		// up
		menuIndex--;
		if (menuIndex < 0)
			menuIndex = 1;
		return true;
	}
	else if (key == 0x53 || key == VK_DOWN) {
		// down
		menuIndex++;
		if (menuIndex > 1)
			menuIndex = 0;
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

	char buffer[64] = "Game Over";
	Font *font = Game::instance()->getBigFont();
	font->setColor(1.0f, 0.0f, 0.0f, 1.0f);
	font->measure(buffer, &w, &h, sx, sy);
	font->draw(buffer, 0 - (w / 2), 0.25f, sx, sy);

	font = Game::instance()->getNormalFont();
	font->setColor(1.0f, 1.0f, 1.0f, 1.0f);
	strcpy(buffer, "Continue");
	font->measure(buffer, &w, &h, sx, sy);
	font->draw(buffer, 0 - (w / 2), -0.1f, sx, sy);
	strcpy(buffer, "Restart");
	font->measure(buffer, &w, &h, sx, sy);
	font->draw(buffer, 0 - (w / 2), -0.2f, sx, sy);

	// combines viewing and projecting matrices
	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 2.5f),    // camera
		glm::vec3(0.0f, 0.0f, -25.0f),  // looks
		glm::vec3(0.0f, 1.0f, 0.0f)     // head is up
	);
	glm::mat4 projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 25.0f);
	glm::mat4 viewProjectMatrix = projectionMatrix * viewMatrix;

	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(0.03f, 0.03f, 1.0f));
	if (menuIndex == 0) {
		glm::mat4 translateMatrix = glm::mat4(1.0f);
		translateMatrix = glm::translate(translateMatrix, glm::vec3(-0.16f, -0.075f, 0.0f));
		glm::mat4 modelMatrix = translateMatrix * scaleMatrix;
		glm::mat4 mvpMatrix = viewProjectMatrix * modelMatrix;
		spriteUI->draw(mvpMatrix, 1);
	}
	else {
		glm::mat4 translateMatrix = glm::mat4(1.0f);
		translateMatrix = glm::translate(translateMatrix, glm::vec3(-0.16f, -0.175f, 0.0f));
		glm::mat4 modelMatrix = translateMatrix * scaleMatrix;
		glm::mat4 mvpMatrix = viewProjectMatrix * modelMatrix;
		spriteUI->draw(mvpMatrix, 1);
	}

	computeFPS();
	SwapBuffers(Game::instance()->getDevice());
}

