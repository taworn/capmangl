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
	: score(0)
	, reverseMode(false), reverseTime(0)
	, divoLife(0), divoList()
{
	assert(singleton == NULL);
	singleton = this;
}

void GameData::clear()
{
	reverseMode = false;
	divoLife = 5;
	divoList.clear();
}

bool GameData::checkAllDivoDead()
{
	int i = 0;
	while (i < (int)divoList.size()) {
		if (!divoList[i]->isDead())
			return false;
		else
			i++;
	}
	return true;
}

void GameData::getBonus(int item)
{
	if (item == 0x01) {
		score += 10;
	}
	else if (item == 0x02) {
		score += 100;
		reverseMode = true;
		reverseTime = 1000 * 3;
	}
}

void GameData::update(ULONGLONG timeUsed)
{
	if (reverseMode) {
		reverseTime -= timeUsed;
		if (reverseTime <= 0)
			reverseMode = false;
	}
}

