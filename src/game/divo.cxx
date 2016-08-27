/**
 * @file divo.cxx
 * @desc Divo module.
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

Divo::~Divo()
{
}

Divo::Divo()
{
	srand((int)GetTickCount());
}

void Divo::setId(int divoId)
{
	assert(divoId >= 0 && divoId < 4);
	this->divoId = divoId;
	getAnimation()->add(ACTION_LEFT, (divoId + 1) * 8 + 0, (divoId + 1) * 8 + 2, Animation::ON_END_CONTINUE, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_RIGHT, (divoId + 1) * 8 + 2, (divoId + 1) * 8 + 4, Animation::ON_END_CONTINUE, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_UP, (divoId + 1) * 8 + 4, (divoId + 1) * 8 + 6, Animation::ON_END_CONTINUE, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_DOWN, (divoId + 1) * 8 + 6, (divoId + 1) * 8 + 8, Animation::ON_END_CONTINUE, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_REVERSE_LEFT, 40, 44, Animation::ON_END_KEEP_LAST_FRAME, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_REVERSE_RIGHT, 40, 44, Animation::ON_END_KEEP_LAST_FRAME, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_REVERSE_UP, 40, 44, Animation::ON_END_KEEP_LAST_FRAME, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_REVERSE_DOWN, 40, 44, Animation::ON_END_KEEP_LAST_FRAME, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_DEAD_LEFT, 56, 57, Animation::ON_END_KEEP_LAST_FRAME, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_DEAD_RIGHT, 57, 58, Animation::ON_END_KEEP_LAST_FRAME, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_DEAD_UP, 58, 59, Animation::ON_END_KEEP_LAST_FRAME, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_DEAD_DOWN, 59, 60, Animation::ON_END_KEEP_LAST_FRAME, TIME_PER_ANI_FRAME);
	getAnimation()->use(ACTION_LEFT);
}

void Divo::nextAction()
{
	Movable::nextAction();
	if (isDead()) {
		if (GameData::instance()->divoCanRelife()) {
			BOOST_LOG_TRIVIAL(debug) << "Divo #" << divoId << " is relife";
			GameData::instance()->divoLifeDecrease();
			relife();

			POINT p;
			POINTFLOAT pf;
			getMap()->getDivoStartPosition(&p, &pf);
			setXY(p.x, p.y);
			getAnimation()->moveTo(pf.x, pf.y);
			getAnimation()->use(ACTION_LEFT);
		}
		else {
			if (GameData::instance()->checkAllDivoDead()) {
				BOOST_LOG_TRIVIAL(debug) << "all Divoes are dead";
				Game::instance()->changeScene(Game::SCENE_WIN);
			}
		}
	}
}

void Divo::kill()
{
	Movable::kill();

	POINT p;
	POINTFLOAT pf;
	getMap()->getDivoStartPosition(&p, &pf);
	moveDirect(&p, &pf);

	float dX = pf.x - getAnimation()->getCurrentX();
	float dY = pf.y - getAnimation()->getCurrentY();
	if (fabs(dX) >= fabs(dY)) {
		if (dX <= 0.0f)
			getAnimation()->use(ACTION_DEAD_LEFT);
		else
			getAnimation()->use(ACTION_DEAD_RIGHT);
	}
	else {
		if (dY <= 0.0f)
			getAnimation()->use(ACTION_DEAD_DOWN);
		else
			getAnimation()->use(ACTION_DEAD_UP);
	}
}

void Divo::setMap(Map *map)
{
	Movable::setMap(map);

	GameData::instance()->divoLifeDecrease();
	POINT p;
	POINTFLOAT pf;
	getMap()->getDivoStartPosition(&p, &pf);
	setXY(p.x, p.y);
	getAnimation()->moveTo(pf.x, pf.y);
}

int Divo::decision(int moveDirection)
{
	int item = 0;
	if (getMap()->checkAndGetItem(this, &item)) {
		GameData::instance()->getBonus(item);
	}

	// checks directions can move
	int dirs = getMap()->canPreviewMove(this);
	int count = 0;
	if ((dirs & MOVE_LEFT) == MOVE_LEFT) count++;
	if ((dirs & MOVE_RIGHT) == MOVE_RIGHT) count++;
	if ((dirs & MOVE_UP) == MOVE_UP) count++;
	if ((dirs & MOVE_DOWN) == MOVE_DOWN) count++;

	if (count <= 0)
		return moveDirection;
	else if (count == 1) {
		moveDirection = dirs;
		return moveDirection;
	}

	// if movable direction >= 2, deleted opposite direction
	if (count >= 2 && moveDirection) {
		int opposite = 0;
		if (moveDirection == MOVE_LEFT) opposite = MOVE_RIGHT;
		else if (moveDirection == MOVE_RIGHT) opposite = MOVE_LEFT;
		else if (moveDirection == MOVE_UP) opposite = MOVE_DOWN;
		else if (moveDirection == MOVE_DOWN) opposite = MOVE_UP;
		dirs &= ~opposite;
	}

	if (count <= 2) {
		if (!(moveDirection && (dirs & moveDirection) == moveDirection)) {
			int randoms[2] = { 0 };
			int end = 0;
			if ((dirs & MOVE_LEFT) == MOVE_LEFT) randoms[end++] = MOVE_LEFT;
			if ((dirs & MOVE_RIGHT) == MOVE_RIGHT) randoms[end++] = MOVE_RIGHT;
			if ((dirs & MOVE_UP) == MOVE_UP) randoms[end++] = MOVE_UP;
			if ((dirs & MOVE_DOWN) == MOVE_DOWN) randoms[end++] = MOVE_DOWN;
			moveDirection = randoms[rand() % 2];
		}
	}
	else {
		int randoms[8] = { 0 };
		int end = 0;
		if (moveDirection) {
			randoms[end++] = moveDirection;
			randoms[end++] = moveDirection;
		}
		if ((dirs & MOVE_LEFT) == MOVE_LEFT) randoms[end++] = MOVE_LEFT;
		if ((dirs & MOVE_RIGHT) == MOVE_RIGHT) randoms[end++] = MOVE_RIGHT;
		if ((dirs & MOVE_UP) == MOVE_UP) randoms[end++] = MOVE_UP;
		if ((dirs & MOVE_DOWN) == MOVE_DOWN) randoms[end++] = MOVE_DOWN;
		moveDirection = randoms[rand() % end];
	}
	return moveDirection;
}

