#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <GL/glew.h>
#include "opengl.hxx"
#include "scenes/scene.hxx"
#include "scenes/title_scene.hxx"
#include "scenes/play_scene.hxx"
#include "game.hxx"

Game *Game::singleton = NULL;

Game::~Game()
{
	delete scene;
	fini();
	singleton = NULL;
}

Game::Game(HDC h) : hdc(h)
{
	assert(singleton == NULL);
	singleton = this;
	init();
	scene = new TitleScene();
}

void Game::changeScene(int sceneId)
{
	delete scene;
	switch (sceneId) {
	case SCENE_TITLE:
		scene = new TitleScene();
		break;
	case SCENE_PLAY:
		scene = new PlayScene();
		break;
	case SCENE_DEFAULT:
	default:
		scene = new Scene();
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

