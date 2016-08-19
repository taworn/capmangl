/**
 * @file title_scene.cxx
 * @desc Title scene module.
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
#include "title_scene.hxx"

TitleScene::~TitleScene()
{
	BOOST_LOG_TRIVIAL(debug) << "TitleScene::~TitleScene() called";
	if (aniDivo) {
		delete aniDivo;
		aniDivo = NULL;
	}
	if (aniHero) {
		delete aniHero;
		aniHero = NULL;
	}
	fini();
}

TitleScene::TitleScene() : Scene(), modelX(0.0f)
{
	BOOST_LOG_TRIVIAL(debug) << "TitleScene::TitleScene() called";
	init();

	const int TIME = 300;
	aniHero = new Animation();
	aniHero->add(0, 0, 2, TIME);
	aniHero->add(1, 2, 4, TIME);
	aniHero->add(2, 4, 6, TIME);
	aniHero->add(3, 6, 8, TIME);
	aniHero->use(0);

	aniDivo = new Animation();
	aniDivo->add(0, 8, 10, TIME);
	aniDivo->add(1, 10, 12, TIME);
	aniDivo->add(2, 12, 14, TIME);
	aniDivo->add(3, 14, 16, TIME);
	aniDivo->use(0);
}

void TitleScene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "TitleScene::init() called";
	titleFont = new Font("C:\\WINDOWS\\Fonts\\timesbd.ttf", 128);

	PNGImage image(".\\res\\pacman.png");
	sprite = new Sprite();
	sprite->init(&image, 8, 8);
}

void TitleScene::fini()
{
	BOOST_LOG_TRIVIAL(debug) << "TitleScene::fini() called";
	if (sprite) {
		delete sprite;
		sprite = NULL;
	}
	if (titleFont) {
		delete titleFont;
		titleFont = NULL;
	}
}

bool TitleScene::handleKey(HWND hwnd, WPARAM key)
{
	if (key == VK_RETURN) {
		// ENTER
		OutputDebugStringW(L"ENTER keydown\n");
		Game::instance()->changeScene(SCENE_PLAY);
		return true;
	}
	return false;
}

void TitleScene::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	RECT rc = getScreenRect();
	float sx = 2.0f / (rc.right - rc.left);
	float sy = 2.0f / (rc.bottom - rc.top);
	float w, h;

	titleFont->setColor(1.0f, 1.0f, 0.5f, 1.0f);
	titleFont->measure("Capman", &w, &h, sx, sy);
	titleFont->draw("Capman", 0 - (w / 2), h, sx, sy);

	Font *normalFont = Game::instance()->getNormalFont();
	normalFont->setColor(1.0f, 1.0f, 1.0f, 1.0f);
	normalFont->measure("Press ENTER to Start", &w, &h, sx, sy);
	normalFont->draw("Press ENTER to Start", 0 - (w / 2), -0.70f, sx, sy);

	Game::instance()->getTextureShader()->useProgram();
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(0.05f, 0.05f, 1.0f));
	glm::mat4 translateMatrix = glm::mat4(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(modelX, -0.2f, 0.0f));
	glm::mat4 modelMatrix = translateMatrix * scaleMatrix;
	glm::mat4 mvpMatrix = getViewAndProjectMatrix() * modelMatrix;
	aniHero->draw(mvpMatrix, sprite);

	translateMatrix = glm::mat4(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(modelX - 0.25f, -0.2f, 0.0f));
	modelMatrix = translateMatrix * scaleMatrix;
	mvpMatrix = getViewAndProjectMatrix() * modelMatrix;
	aniDivo->draw(mvpMatrix, sprite);

	modelX -= 0.01f;
	if (modelX < -1.0f)
		modelX = 1.0f;

	computeFPS();
	SwapBuffers(Game::instance()->getDevice());
}

