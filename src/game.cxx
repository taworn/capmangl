/**
 * @file game.cxx
 * @desc Game engine module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include "opengl.hxx"
#include "game.hxx"
#include "scenes/scene.hxx"
#include "scenes/title_scene.hxx"
#include "scenes/play_scene.hxx"

Game *Game::singleton = NULL;

Game::~Game()
{
	glUseProgram(0);
	delete scene;

	delete bigFont;
	delete normalFont;
	delete smallFont;

	FT_Done_FreeType(freeTypeLibrary);

	delete textShader;
	delete normalShader;

	singleton = NULL;
}

Game::Game(HDC h)
	: hdc(h), normalShader(), textShader()
	, freeTypeLibrary(0), smallFont(), normalFont(), bigFont()
	, scene()
{
	assert(singleton == NULL);
	singleton = this;

	normalShader = new NormalShader();
	textShader = new TextShader();

	FT_Error error = FT_Init_FreeType(&freeTypeLibrary);
	if (error) {
		BOOST_LOG_TRIVIAL(debug) << "FreeType initialization failed!";
	}

	smallFont = new Font("C:\\WINDOWS\\Fonts\\arial.ttf", 24);
	normalFont = new Font("C:\\WINDOWS\\Fonts\\arial.ttf", 32);
	bigFont = new Font("C:\\WINDOWS\\Fonts\\timesbd.ttf", 64);

	scene = new TitleScene();
}

void Game::changeScene(int sceneId)
{
	delete scene;
	switch (sceneId) {
	default:
	case SCENE_DEFAULT:
		scene = new Scene();
		break;
	case SCENE_TITLE:
		scene = new TitleScene();
		break;
	case SCENE_PLAY:
		scene = new PlayScene();
		break;
	}
}

bool Game::handleKey(HWND hwnd, WPARAM key)
{
	return scene->handleKey(hwnd, key);
}

void Game::render()
{
	scene->render();
}

void Game::init()
{
}

void Game::fini()
{
}

