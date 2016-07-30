#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <GL/glew.h>
#include "../game.hxx"
#include "scene.hxx"
#include "title_scene.hxx"
#include "play_scene.hxx"

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
		Game::instance()->changeScene(new PlayScene());
		return true;
	}
	return false;
}

void TitleScene::render(ULONGLONG timeCurrent)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	fps(timeCurrent);
	SwapBuffers(Game::instance()->getDevice());
}

