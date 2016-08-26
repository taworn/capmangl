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
	getAnimation()->add(ACTION_LEFT, (divoId + 1) * 8 + 0, (divoId + 1) * 8 + 2, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_RIGHT, (divoId + 1) * 8 + 2, (divoId + 1) * 8 + 4, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_UP, (divoId + 1) * 8 + 4, (divoId + 1) * 8 + 6, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_DOWN, (divoId + 1) * 8 + 6, (divoId + 1) * 8 + 8, TIME_PER_ANI_FRAME);
	getAnimation()->add(ACTION_DEAD, 56, 60, TIME_PER_ANI_FRAME);
	getAnimation()->use(ACTION_LEFT);
}

void Divo::kill()
{
	Movable::kill();
	getAnimation()->use(ACTION_DEAD);

	POINT p;
	POINTFLOAT pf;
	getMap()->getDivoStartPosition(&p, &pf);
	moveDirect(&p, &pf);
}

void Divo::setMap(Map *map)
{
	Movable::setMap(map);

	POINT p;
	POINTFLOAT pf;
	getMap()->getDivoStartPosition(&p, &pf);
	setXY(p.x, p.y);
	getAnimation()->moveTo(pf.x, pf.y);
}

int Divo::decision(int moveDirection)
{
	if (getMap()->hasItem(this)) {

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

