/**
 * @file scene.cxx
 * @desc Game scene module.
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

Scene::~Scene()
{
	fini();
}

Scene::Scene()
	: viewAndProjectMatrix(1.0f), screenRect()
	, fps(0), frameCount(0), timeStart(0)
{
	// combines viewing and projecting matrices
	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 2.5f),    // camera
		glm::vec3(0.0f, 0.0f, -25.0f),  // looks
		glm::vec3(0.0f, 1.0f, 0.0f)     // head is up
	);
	//glm::mat4 projectionMatrix = glm::perspective(45.0f, 1.3333f, 1.0f, 25.0f);
	glm::mat4 projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 25.0f);
	viewAndProjectMatrix = projectionMatrix * viewMatrix;

	int params[4];
	glGetIntegerv(GL_VIEWPORT, params);
	screenRect.left = params[0];
	screenRect.top = params[1];
	screenRect.right = screenRect.left + params[2];
	screenRect.bottom = screenRect.top + params[3];

	timeStart = GetTickCount();

	init();
}

void Scene::init()
{
}

void Scene::fini()
{
}

void Scene::computeFPS()
{
	frameCount++;
	ULONGLONG timeCurrent = GetTickCount();
	ULONGLONG timeUsage = timeCurrent - timeStart;
	if (timeUsage > 1000) {
		fps = (int)(frameCount * 1000 / timeUsage);
		timeStart = timeCurrent;
		frameCount = 0;

		wchar_t buffer[64];
		wsprintf(buffer, L"FPS: %d\n", fps);
		OutputDebugStringW(buffer);
		BOOST_LOG_TRIVIAL(trace) << "FPS: " << fps;
	}

	char buffer[64];
	sprintf(buffer, "%d", fps);
	RECT rc = getScreenRect();
	float sx = 2.0f / (rc.right - rc.left);
	float sy = 2.0f / (rc.bottom - rc.top);
	float w, h;

	Game *game = Game::instance();
	game->getTextShader()->useProgram();
	game->getSmallFont()->setColor(1.0f, 1.0f, 1.0f, 0.5f);
	game->getSmallFont()->measure(buffer, &w, &h, sx, sy);
	game->getSmallFont()->draw(buffer, 1 - w * 2, -1 + h, sx, sy);
}

bool Scene::handleKey(HWND hwnd, WPARAM key)
{
	return false;
}

void Scene::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	computeFPS();
	SwapBuffers(Game::instance()->getDevice());
}

