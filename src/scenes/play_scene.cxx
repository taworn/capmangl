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
	fini();
}

PlayScene::PlayScene()
	: Scene()
	, modelX(0.0f), modelY(0.0f), modelDx(0.0f), modelDy(0.0f)
{
	BOOST_LOG_TRIVIAL(debug) << "PlayScene::PlayScene() called";
	init();
}

void PlayScene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "PlayScene::init() called";
	PNGImage image(".\\res\\pacman.png");
	sprite = new Sprite();
	sprite->init(&image, 8, 8);

	const int TIME = 300;
	aniHero = new Animation(sprite);
	aniHero->add(0, 0, 2, TIME);
	aniHero->add(1, 2, 4, TIME);
	aniHero->add(2, 4, 6, TIME);
	aniHero->add(3, 6, 8, TIME);
	aniHero->use(0);

	for (int i = 0; i < 4; i++) {
		int j = (i + 1) * 8;
		aniDivoes[i] = new Animation(sprite);
		aniDivoes[i]->add(0, j + 0, j + 2, TIME);
		aniDivoes[i]->add(1, j + 2, j + 4, TIME);
		aniDivoes[i]->add(2, j + 4, j + 6, TIME);
		aniDivoes[i]->add(3, j + 6, j + 8, TIME);
		aniDivoes[i]->use(0);
	}
}

void PlayScene::fini()
{
	BOOST_LOG_TRIVIAL(debug) << "PlayScene::fini() called";
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
		aniHero->use(2);
		modelDx = 0.0f;
		modelDy = 0.005f;
		return true;
	}
	else if (key == 0x53 || key == VK_DOWN) {
		// down
		OutputDebugStringW(L"S -or- DOWN keydown\n");
		aniHero->use(3);
		modelDx = 0.0f;
		modelDy = -0.005f;
		return true;
	}
	else if (key == 0x41 || key == VK_LEFT) {
		// left
		OutputDebugStringW(L"A -or- LEFT keydown\n");
		aniHero->use(0);
		modelDx = -0.005f;
		modelDy = 0.0f;
		return true;
	}
	else if (key == 0x44 || key == VK_RIGHT) {
		// right
		OutputDebugStringW(L"D -or- RIGHT keydown\n");
		aniHero->use(1);
		modelDx = 0.005f;
		modelDy = 0.0f;
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
	translateMatrix = glm::translate(translateMatrix, glm::vec3(modelX, modelY, 0.0f));
	if (modelDx > 0.0f && modelX < 0.95f)
		modelX += modelDx;
	else if (modelDx < 0.0f && modelX > -0.95f)
		modelX += modelDx;
	if (modelDy > 0.0f && modelY < 0.95f)
		modelY += modelDy;
	else if (modelDy < 0.0f && modelY > -0.95f)
		modelY += modelDy;
	glm::mat4 modelMatrix = translateMatrix * scaleMatrix;
	glm::mat4 mvpMatrix = getViewAndProjectMatrix() * modelMatrix;
	aniHero->draw(mvpMatrix);

	translateMatrix = glm::mat4(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(-0.5f, 0.5f, 0.0f));
	modelMatrix = translateMatrix * scaleMatrix;
	mvpMatrix = getViewAndProjectMatrix() * modelMatrix;
	aniDivoes[0]->draw(mvpMatrix);

	translateMatrix = glm::mat4(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(0.5f, 0.5f, 0.0f));
	modelMatrix = translateMatrix * scaleMatrix;
	mvpMatrix = getViewAndProjectMatrix() * modelMatrix;
	aniDivoes[1]->draw(mvpMatrix);

	translateMatrix = glm::mat4(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(-0.5f, -0.5f, 0.0f));
	modelMatrix = translateMatrix * scaleMatrix;
	mvpMatrix = getViewAndProjectMatrix() * modelMatrix;
	aniDivoes[2]->draw(mvpMatrix);

	translateMatrix = glm::mat4(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(0.5f, -0.5f, 0.0f));
	modelMatrix = translateMatrix * scaleMatrix;
	mvpMatrix = getViewAndProjectMatrix() * modelMatrix;
	aniDivoes[3]->draw(mvpMatrix);

	computeFPS();
	SwapBuffers(Game::instance()->getDevice());
}

