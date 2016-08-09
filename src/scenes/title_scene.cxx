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
#include "../game.hxx"
#include "scene.hxx"
#include "title_scene.hxx"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

	getTextShader()->useProgram();

	RECT rc = getScreenRect();
	float sx = 2.0f / (rc.right - rc.left);
	float sy = 2.0f / (rc.bottom - rc.top);
	float w, h;

	titleFont->setColor(1.0f, 1.0f, 0.5f, 1.0f);
	titleFont->measure("Capman", &w, &h, sx, sy);
	titleFont->draw("Capman", 0 - (w / 2), h, sx, sy);

	getNormalFont()->setColor(1.0f, 1.0f, 1.0f, 1.0f);
	getNormalFont()->measure("Press ENTER to Start", &w, &h, sx, sy);
	getNormalFont()->draw("Press ENTER to Start", 0 - (w / 2), -0.70f, sx, sy);

	getTextureShader()->useProgram();
	glm::mat4x4 translateMatrix = glm::mat4(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(0.5f, 0.5f, 0.0f));
	glm::mat4x4 scaleMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(0.25f, 0.75f, 1.0f));
	glm::mat4x4 viewMatrix = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 1.5f),    // camera
		glm::vec3(0.0f, 0.0f, -15.0f),  // looks
		glm::vec3(0.0f, 1.0f, 0.0f)     // head is up
	);
	glm::mat4x4 projectionMatrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 25.0f);
	glm::mat4x4 mvpMatrix = projectionMatrix * viewMatrix * scaleMatrix * translateMatrix;
	GLint mvp = getTextureShader()->getMVPMatrix();
	glUniformMatrix4fv(mvp, 1, false, &mvpMatrix[0][0]);

	texture->draw();

	computeFPS();
	SwapBuffers(getDevice());
}

