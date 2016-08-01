#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <GL/glew.h>
#include "scene.hxx"
#include "title_scene.hxx"
#include "../game.hxx"

TitleScene::~TitleScene()
{
	fini();
}

TitleScene::TitleScene() : Scene()
{
	init();
}

void TitleScene::init()
{
}

void TitleScene::fini()
{
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

	computeFPS();
	drawFPS();
	SwapBuffers(getDevice());
}

