/**
 * @file play_scene.cxx
 * @desc Playing scene module.
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
#include "play_scene.hxx"

PlayScene::~PlayScene()
{
	BOOST_LOG_TRIVIAL(debug) << "PlayScene::~PlayScene() called";
	for (int i = 0; i < 4; i++) {
		if (aniDivoes[i]) {
			delete aniDivoes[i];
			aniDivoes[i] = NULL;
		}
	}
	if (aniHero) {
		delete aniHero;
		aniHero = NULL;
	}
	fini();
}

PlayScene::PlayScene() : Scene()
{
	BOOST_LOG_TRIVIAL(debug) << "PlayScene::PlayScene() called";
	init();

	const int TIME = 300;
	aniHero = new Animation();
	aniHero->add(0, 0, 2, TIME);
	aniHero->add(1, 2, 4, TIME);
	aniHero->add(2, 4, 6, TIME);
	aniHero->add(3, 6, 8, TIME);
	aniHero->use(0);

	for (int i = 0; i < 4; i++) {
		aniDivoes[i] = new Animation();
		aniDivoes[i]->add(0, (i + 1) * 8 + 0, (i + 1) * 8 + 2, TIME);
		aniDivoes[i]->add(1, (i + 1) * 8 + 2, (i + 1) * 8 + 4, TIME);
		aniDivoes[i]->add(2, (i + 1) * 8 + 4, (i + 1) * 8 + 6, TIME);
		aniDivoes[i]->add(3, (i + 1) * 8 + 6, (i + 1) * 8 + 8, TIME);
		aniDivoes[i]->use(0);
	}
}

void PlayScene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "PlayScene::init() called";

	PNGImage image(".\\res\\pacman.png");
	sprite = new Sprite();
	sprite->init(&image, 8, 8);
}

void PlayScene::fini()
{
	BOOST_LOG_TRIVIAL(debug) << "PlayScene::fini() called";
	if (sprite) {
		delete sprite;
		sprite = NULL;
	}
}

bool PlayScene::handleKey(HWND hwnd, WPARAM key)
{
	if (key == VK_SPACE) {
		// space
		OutputDebugStringW(L"SPACE keydown\n");
		return true;
	}
	else if (key == VK_RETURN) {
		// ENTER
		OutputDebugStringW(L"ENTER keydown\n");
		Game::instance()->changeScene(SCENE_TITLE);
		return true;
	}
	else if (key == 0x57 || key == VK_UP) {
		// up
		OutputDebugStringW(L"W -or- UP keydown\n");
		aniHero->setVelocity(0.0f, 0.005f);
		aniHero->use(2);
		return true;
	}
	else if (key == 0x53 || key == VK_DOWN) {
		// down
		OutputDebugStringW(L"S -or- DOWN keydown\n");
		aniHero->setVelocity(0.0f, -0.005f);
		aniHero->use(3);
		return true;
	}
	else if (key == 0x41 || key == VK_LEFT) {
		// left
		OutputDebugStringW(L"A -or- LEFT keydown\n");
		aniHero->setVelocity(-0.005f, 0.0f);
		aniHero->use(0);
		return true;
	}
	else if (key == 0x44 || key == VK_RIGHT) {
		// right
		OutputDebugStringW(L"D -or- RIGHT keydown\n");
		aniHero->setVelocity(0.005f, 0.0f);
		aniHero->use(1);
		return true;
	}
	return false;
}

void PlayScene::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	Game::instance()->getTextureShader()->useProgram();

	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(0.04f, 0.04f, 1.0f));
	glm::mat4 translateMatrix = glm::mat4(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(aniHero->getCurrentX(), aniHero->getCurrentY(), 0.0f));
	glm::mat4 modelMatrix = translateMatrix * scaleMatrix;
	glm::mat4 mvpMatrix = getViewAndProjectMatrix() * modelMatrix;
	bool enableX = false, enableY = false;
	if (aniHero->getVelocityX() > 0.0f && aniHero->getCurrentX() < 0.95f)
		enableX = true;
	else if (aniHero->getVelocityX() < 0.0f && aniHero->getCurrentX() > -0.95f)
		enableX = true;
	if (aniHero->getVelocityY() > 0.0f && aniHero->getCurrentY() < 0.95f)
		enableY = true;
	else if (aniHero->getVelocityY() < 0.0f && aniHero->getCurrentY() > -0.95f)
		enableY = true;
	aniHero->playFrame(enableX, enableY);
	aniHero->draw(mvpMatrix, sprite);

	translateMatrix = glm::mat4(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(-0.5f, 0.5f, 0.0f));
	modelMatrix = translateMatrix * scaleMatrix;
	mvpMatrix = getViewAndProjectMatrix() * modelMatrix;
	aniDivoes[0]->draw(mvpMatrix, sprite);

	translateMatrix = glm::mat4(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(0.5f, 0.5f, 0.0f));
	modelMatrix = translateMatrix * scaleMatrix;
	mvpMatrix = getViewAndProjectMatrix() * modelMatrix;
	aniDivoes[1]->draw(mvpMatrix, sprite);

	translateMatrix = glm::mat4(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(-0.5f, -0.5f, 0.0f));
	modelMatrix = translateMatrix * scaleMatrix;
	mvpMatrix = getViewAndProjectMatrix() * modelMatrix;
	aniDivoes[2]->draw(mvpMatrix, sprite);

	translateMatrix = glm::mat4(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(0.5f, -0.5f, 0.0f));
	modelMatrix = translateMatrix * scaleMatrix;
	mvpMatrix = getViewAndProjectMatrix() * modelMatrix;
	aniDivoes[3]->draw(mvpMatrix, sprite);

	computeFPS();
	SwapBuffers(Game::instance()->getDevice());
}

