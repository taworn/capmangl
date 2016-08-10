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
	fini();
}

PlayScene::PlayScene()
	: Scene()
	, modelX(0.0f), modelY(0.0f), modelDx(0.0f), modelDy(0.0f)
{
	init();
}

void PlayScene::init()
{
	image.init("res\\a.png");
	texture = new Texture();
	texture->init(&image);
}

void PlayScene::fini()
{
	delete texture;
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
		modelDx = 0.0f;
		modelDy = 0.05f;
		return true;
	}
	else if (key == 0x53 || key == VK_DOWN) {
		// down
		OutputDebugStringW(L"S -or- DOWN keydown\n");
		modelDx = 0.0f;
		modelDy = -0.05f;
		return true;
	}
	else if (key == 0x41 || key == VK_LEFT) {
		// left
		OutputDebugStringW(L"A -or- LEFT keydown\n");
		modelDx = -0.05f;
		modelDy = 0.0f;
		return true;
	}
	else if (key == 0x44 || key == VK_RIGHT) {
		// right
		OutputDebugStringW(L"D -or- RIGHT keydown\n");
		modelDx = 0.05f;
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

	glm::mat4x4 translateMatrix = glm::mat4(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(modelX, modelY, 0.0f));
	if (modelDx > 0.0f && modelX < 8.0f)
		modelX += modelDx;
	else if (modelDx < 0.0f && modelX > -8.0f)
		modelX += modelDx;
	if (modelDy > 0.0f && modelY < 8.0f)
		modelY += modelDy;
	else if (modelDy < 0.0f && modelY > -8.0f)
		modelY += modelDy;
	glm::mat4x4 scaleMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(0.2f, 0.2f, 1.0f));
	glm::mat4x4 mvpMatrix = getViewAndProjectMatrix() * scaleMatrix * translateMatrix;
	texture->draw(mvpMatrix);

	scaleMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(0.1f, 0.1f, 1.0f));
	for (int i = -18; i < 19; i += 2) {
		translateMatrix = glm::mat4(1.0f);
		translateMatrix = glm::translate(translateMatrix, glm::vec3((float)i, 18.0f, 0.0f));
		mvpMatrix = getViewAndProjectMatrix() * scaleMatrix * translateMatrix;
		texture->draw(mvpMatrix);
	}
	for (int i = -18; i < 19; i += 2) {
		translateMatrix = glm::mat4(1.0f);
		translateMatrix = glm::translate(translateMatrix, glm::vec3((float)i, -18.0f, 0.0f));
		mvpMatrix = getViewAndProjectMatrix() * scaleMatrix * translateMatrix;
		texture->draw(mvpMatrix);
	}
	for (int i = -18; i < 19; i += 2) {
		translateMatrix = glm::mat4(1.0f);
		translateMatrix = glm::translate(translateMatrix, glm::vec3(18.0f, (float)i, 0.0f));
		mvpMatrix = getViewAndProjectMatrix() * scaleMatrix * translateMatrix;
		texture->draw(mvpMatrix);
	}
	for (int i = -18; i < 19; i += 2) {
		translateMatrix = glm::mat4(1.0f);
		translateMatrix = glm::translate(translateMatrix, glm::vec3(-18.0f, (float)i, 0.0f));
		mvpMatrix = getViewAndProjectMatrix() * scaleMatrix * translateMatrix;
		texture->draw(mvpMatrix);
	}

	computeFPS();
	SwapBuffers(Game::instance()->getDevice());
}

