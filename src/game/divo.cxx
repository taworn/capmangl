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
	animation.add(0, (divoId + 1) * 8 + 0, (divoId + 1) * 8 + 2, TIME_PER_ANI_FRAME);
	animation.add(1, (divoId + 1) * 8 + 2, (divoId + 1) * 8 + 4, TIME_PER_ANI_FRAME);
	animation.add(2, (divoId + 1) * 8 + 4, (divoId + 1) * 8 + 6, TIME_PER_ANI_FRAME);
	animation.add(3, (divoId + 1) * 8 + 6, (divoId + 1) * 8 + 8, TIME_PER_ANI_FRAME);
	animation.use(0);
}

void Divo::setMap(Map *map)
{
	assert(map);
	this->map = map;

	POINTFLOAT pf;
	map->getDivoStartPosition(&point, &pf);
	animation.moveTo(pf.x, pf.y);
}

void Divo::nextMove()
{
	int dirs = map->canPreviewMove(this);
	int count = 0;
	if ((dirs & Map::MOVE_LEFT) == Map::MOVE_LEFT) count++;
	if ((dirs & Map::MOVE_RIGHT) == Map::MOVE_RIGHT) count++;
	if ((dirs & Map::MOVE_UP) == Map::MOVE_UP) count++;
	if ((dirs & Map::MOVE_DOWN) == Map::MOVE_DOWN) count++;

	if (count <= 0)
		return;

	else if (count == 1)
		nextDirection = dirs;

	else if (count == 2) {
		if (!(nextDirection && (dirs & nextDirection) == nextDirection)) {
			int randoms[2] = { 0 };
			int end = 0;
			if ((dirs & Map::MOVE_LEFT) == Map::MOVE_LEFT) randoms[end++] = Map::MOVE_LEFT;
			if ((dirs & Map::MOVE_RIGHT) == Map::MOVE_RIGHT) randoms[end++] = Map::MOVE_RIGHT;
			if ((dirs & Map::MOVE_UP) == Map::MOVE_UP) randoms[end++] = Map::MOVE_UP;
			if ((dirs & Map::MOVE_DOWN) == Map::MOVE_DOWN) randoms[end++] = Map::MOVE_DOWN;
			nextDirection = randoms[rand() % 2];
		}
	}

	else {
		int randoms[8] = { 0 };
		int end = 0;
		if ((dirs & Map::MOVE_LEFT) == Map::MOVE_LEFT) randoms[end++] = Map::MOVE_LEFT;
		if ((dirs & Map::MOVE_RIGHT) == Map::MOVE_RIGHT) randoms[end++] = Map::MOVE_RIGHT;
		if ((dirs & Map::MOVE_UP) == Map::MOVE_UP) randoms[end++] = Map::MOVE_UP;
		if ((dirs & Map::MOVE_DOWN) == Map::MOVE_DOWN) randoms[end++] = Map::MOVE_DOWN;
		if (nextDirection) randoms[end++] = nextDirection;

		nextDirection = randoms[rand() % end];
	}

	move(nextDirection);
}

