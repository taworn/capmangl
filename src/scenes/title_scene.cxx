#include <windows.h>
#include <assert.h>
#include <ft2build.h>
#include FT_FREETYPE_H
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
	FT_Error error = FT_New_Face(Game::instance()->getFreeTypeLibrary(), "C:\\Windows\\Fonts\\arial.ttf", 0, &titleFace);
	if (error) {
		BOOST_LOG_TRIVIAL(debug) << "FreeType cannot load face.";
	}
}

void TitleScene::fini()
{
	FT_Done_Face(titleFace);
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

