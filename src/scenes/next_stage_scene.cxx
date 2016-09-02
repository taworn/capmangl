/**
 * @file next_next_stage_scene.cxx
 * @desc Next stage scene module.
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
#include "../game/common.hxx"
#include "scene.hxx"
#include "next_stage_scene.hxx"

NextStageScene::~NextStageScene()
{
	BOOST_LOG_TRIVIAL(debug) << "NextStageScene::~NextStageScene() called";
	fini();
}

NextStageScene::NextStageScene()
	: Scene()
{
	BOOST_LOG_TRIVIAL(debug) << "NextStageScene::NextStageScene() called";
	init();
}

void NextStageScene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "NextStageScene::init() called";
}

void NextStageScene::fini()
{
	BOOST_LOG_TRIVIAL(debug) << "NextStageScene::fini() called";
}

void NextStageScene::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	if (GameData::instance()->nextStage())
		Game::instance()->changeScene(Game::SCENE_STAGE);
	else
		Game::instance()->changeScene(Game::SCENE_WIN);

	computeFPS();
	SwapBuffers(Game::instance()->getDevice());
}

