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
	fini();
}

TitleScene::TitleScene() : Scene(), image()
{
	init();
}

void TitleScene::init()
{
	image.init("res\\a.png");
	texture = new Texture();
	texture->init(&image);
	titleFont = new Font("C:\\WINDOWS\\Fonts\\timesbd.ttf", 128);
}

void TitleScene::fini()
{
	delete titleFont;
	delete texture;
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

	Game::instance()->getTextShader()->useProgram();

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
	glm::mat4x4 translateMatrix = glm::mat4(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(0.5f, 0.5f, 0.0f));
	glm::mat4x4 scaleMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(0.25f, 0.75f, 1.0f));
	glm::mat4x4 mvpMatrix = getViewAndProjectMatrix() * scaleMatrix * translateMatrix;
	texture->draw(mvpMatrix);

	computeFPS();
	SwapBuffers(Game::instance()->getDevice());
}

