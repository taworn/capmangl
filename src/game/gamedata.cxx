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
#include <shlobj.h>

GameData *GameData::singleton = NULL;

GameData::~GameData()
{
	singleton = NULL;
}

GameData::GameData()
	: score(0), stage(0)
	, reverseMode(false), reverseTime(0)
	, divoLife(0), divoList()
{
	assert(singleton == NULL);
	singleton = this;

	wchar_t file[MAX_PATH] = { 0 };
	SHGetFolderPath(0, CSIDL_LOCAL_APPDATA, 0, 0, file);
	wcscat(file, L"\\capmangl.ini");

	UINT score = GetPrivateProfileInt(L"capman", L"score", 0, file);
	UINT stage = GetPrivateProfileInt(L"capman", L"stage", 0, file);
	this->score = (int)score;
	this->stage = (int)stage;
}

void GameData::reset()
{
	score = 0;
	stage = 0;
	clear();
}

void GameData::clear()
{
	reverseMode = false;
	divoLife = 5 * (stage + 1);
	divoList.clear();
}

void GameData::save()
{
	wchar_t file[MAX_PATH] = { 0 };
	SHGetFolderPath(0, CSIDL_LOCAL_APPDATA, 0, 0, file);
	wcscat(file, L"\\capmangl.ini");

	wchar_t buffer[64];
	wsprintf(buffer, L"%d", score);
	WritePrivateProfileString(L"capman", L"score", buffer, file);
	wsprintf(buffer, L"%d", stage);
	WritePrivateProfileString(L"capman", L"stage", buffer, file);
}

bool GameData::nextStage()
{
	if (stage < 2) {
		stage++;
		return true;
	}
	else
		return false;
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
		reverseTime = maxReverseTime;
	}
}

void GameData::update(ULONGLONG timeUsed)
{
	if (reverseMode) {
		reverseTime -= timeUsed;
		if ((LONGLONG)reverseTime <= 0)
			reverseMode = false;
	}
}

