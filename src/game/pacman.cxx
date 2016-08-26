/**
 * @file pacman.cxx
 * @desc Pacman module.
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

Pacman::~Pacman()
{
}

Pacman::Pacman()
{
	setTimes(200, 1000);
	getAnimation()->add(ACTION_LEFT, 0, 2, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_RIGHT, 2, 4, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_UP, 4, 6, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_DOWN, 6, 8, TIME_PER_ANI_FRAME);
	getAnimation()->use(ACTION_LEFT);
}

void Pacman::detect()
{
	const float RANGE = 0.03125f;
	float x = getCurrentX();
	float y = getCurrentY();
	float left = x - RANGE;
	float top = y + RANGE;
	float right = x + RANGE;
	float bottom = y - RANGE;

	GameData *gameData = GameData::instance();
	int count = gameData->getDivoCount();
	int i = 0;
	bool detected = false;
	while (i < count) {
		Divo *divo = gameData->getDivo(i);
		float divoX = divo->getCurrentX();
		float divoY = divo->getCurrentY();

		if (!divo->isDead()) {
			if (left < divoX && top > divoY && divoX < right && divoY > bottom) {
				detected = true;
				break;
			}
		}

		i++;
	}

	if (detected) {
		Divo *divo = gameData->getDivo(i);
		divo->kill();
		BOOST_LOG_TRIVIAL(debug) << "eat Divo #" << i;
	}
}

void Pacman::setMap(Map *map)
{
	Movable::setMap(map);

	POINT p;
	POINTFLOAT pf;
	getMap()->getPacmanStartPosition(&p, &pf);
	setXY(p.x, p.y);
	getAnimation()->moveTo(pf.x, pf.y);
}

