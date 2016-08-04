#include <windows.h>
#include <assert.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <boost/log/trivial.hpp>
#include <GL/glew.h>
#include "../game.hxx"
#include "scene.hxx"
#include "title_scene.hxx"

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
	titleFont = new Font();
	titleFont->load("C:\\WINDOWS\\Fonts\\times.ttf", 128);
}

void TitleScene::fini()
{
	delete titleFont;
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

	getTextShader().useProgram();

	RECT rc = getScreenRect();
	float sx = 2.0f / (rc.right - rc.left);
	float sy = 2.0f / (rc.bottom - rc.top);
	GLfloat textColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	titleFont->setColor(textColor);
	float w, h;

	//FT_Set_Pixel_Sizes(titleFace, 0, 128);
	titleFont->measureText("Capman", &w, &h, sx, sy);
	titleFont->drawText("Capman", 0 - (w / 2), h, sx, sy);

	//FT_Set_Pixel_Sizes(titleFace, 0, 32);
	//measureText("Press ENTER Key", sx, sy, &w, &h);
	//render_text("Press ENTER Key", 0 - (w / 2), -0.70f, sx, sy);

	computeFPS();
	drawFPS();
	SwapBuffers(getDevice());
}

