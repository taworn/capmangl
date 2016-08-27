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
#include <glm/glm.hpp>
#include "opengl.hxx"
#include "game.hxx"
#include "scenes/scene.hxx"
#include "scenes/title_scene.hxx"
//#include "scenes/stage_scene.hxx"
#include "scenes/play_scene.hxx"
#include "scenes/gameover_scene.hxx"
#include "scenes/win_scene.hxx"

Game *Game::singleton = NULL;

Game::~Game()
{
	glUseProgram(0);
	delete scene;
	delete GameData::instance();

	delete bigFont;
	delete normalFont;
	delete smallFont;

	FT_Done_FreeType(freeTypeLibrary);

	delete textShader;
	delete normalShader;

	singleton = NULL;
}

Game::Game(HDC h)
	: hdc(h), normalShader(), textShader(), textureShader()
	, freeTypeLibrary(0), smallFont(), normalFont(), bigFont()
	, scene(), nextSceneId(SCENE_TITLE)
{
	assert(singleton == NULL);
	singleton = this;

	normalShader = new NormalShader();
	textShader = new TextShader();
	textureShader = new TextureShader();

	FT_Error error = FT_Init_FreeType(&freeTypeLibrary);
	if (error) {
		BOOST_LOG_TRIVIAL(debug) << "FreeType initialization failed!";
	}

	smallFont = new Font("C:\\WINDOWS\\Fonts\\arial.ttf", 24);
	normalFont = new Font("C:\\WINDOWS\\Fonts\\arial.ttf", 32);
	bigFont = new Font("C:\\WINDOWS\\Fonts\\timesbd.ttf", 64);

	new GameData();
}

void Game::changeScene(int sceneId)
{
	BOOST_LOG_TRIVIAL(debug) << "changeScene() called, sceneId = " << sceneId;
	nextSceneId = sceneId;
}

void Game::handleActivate(HWND hwnd, bool active)
{
	if (scene)
		scene->handleActivate(hwnd, active);
}

bool Game::handleKey(HWND hwnd, WPARAM key)
{
	if (scene)
		return scene->handleKey(hwnd, key);
	else
		return false;
}

void Game::render()
{
	if (nextSceneId < 0) {
		if (scene)
			scene->render();
	}
	else
		switchScene();
}

void Game::switchScene()
{
	BOOST_LOG_TRIVIAL(debug) << "perform switchScene() called, sceneId = " << nextSceneId;
	if (scene)
		delete scene;

	switch (nextSceneId) {
	default:
	case SCENE_DEFAULT:
		scene = new Scene();
		break;
	case SCENE_TITLE:
		scene = new TitleScene();
		break;
	case SCENE_STAGE:
		//scene = new StageScene();
		break;
	case SCENE_PLAY:
		scene = new PlayScene();
		break;
	case SCENE_GAMEOVER:
		scene = new GameOverScene();
		break;
	case SCENE_WIN:
		scene = new WinScene();
		break;
	}
	nextSceneId = -1;
}

