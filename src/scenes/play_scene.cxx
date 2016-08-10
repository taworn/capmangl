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
	, angle(0.0f), angleToPlus(0.1f)
	, verticesId(0)
{
	init();
}

void PlayScene::init()
{
	// generates buffer
	const GLfloat verticesData[] = {
		// X, Y, Z, R, G, B, A
		-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	};
	glGenBuffers(1, &verticesId);
	glBindBuffer(GL_ARRAY_BUFFER, verticesId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
}

void PlayScene::fini()
{
	glDeleteBuffers(1, &verticesId);
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
		modelDy = -0.1f;
		return true;
	}
	else if (key == 0x53 || key == VK_DOWN) {
		// down
		OutputDebugStringW(L"S -or- DOWN keydown\n");
		modelDy = 0.1f;
		return true;
	}
	else if (key == 0x41 || key == VK_LEFT) {
		// left
		OutputDebugStringW(L"A -or- LEFT keydown\n");
		modelDx = -0.1f;
		return true;
	}
	else if (key == 0x44 || key == VK_RIGHT) {
		// right
		OutputDebugStringW(L"D -or- RIGHT keydown\n");
		modelDx = 0.1f;
		return true;
	}
	return false;
}

void PlayScene::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	NormalShader *normalShader = Game::instance()->getNormalShader();
	normalShader->useProgram();

	// translating
	glm::mat4x4 translateMatrix = glm::mat4(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(modelX + modelDx, modelY + modelDy, 0));
	modelX += modelDx;
	modelY += modelDy;
	modelDx = 0.0f;
	modelDy = 0.0f;

	// rotating
	glm::mat4x4 rotateMatrix = glm::mat4(1.0f);
	rotateMatrix = glm::rotate(rotateMatrix, angle, glm::vec3(0, 1, 0));
	angle += angleToPlus;
	if (angle > 89.0f || angle < -89.0f)
		angleToPlus = -angleToPlus;

	// combining
	glm::mat4x4 mvpMatrix = getViewAndProjectMatrix() * rotateMatrix * translateMatrix;

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, verticesId);

	// passes in the position information
	glVertexAttribPointer(normalShader->getPosition(), 3, GL_FLOAT, false, 7 * 4, (void*)(0 * 4));
	glEnableVertexAttribArray(normalShader->getPosition());

	// passes in the color information
	glVertexAttribPointer(normalShader->getColor(), 4, GL_FLOAT, false, 7 * 4, (void*)(3 * 4));
	glEnableVertexAttribArray(normalShader->getColor());

	glUniformMatrix4fv(normalShader->getMVPMatrix(), 1, false, &mvpMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);

	computeFPS();
	SwapBuffers(Game::instance()->getDevice());
}

