/**
 * @file gamedata.cxx
 * @desc Game data module.
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
#include "common.hxx"

GameData *GameData::singleton = NULL;

GameData::~GameData()
{
	singleton = NULL;
}

GameData::GameData()
{
	assert(singleton == NULL);
	singleton = this;
}

void GameData::clear()
{
	divoList.clear();
}

