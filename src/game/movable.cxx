/**
 * @file movable.cxx
 * @desc Movable module.
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

Movable::~Movable()
{
}

Movable::Movable()
	: point()
	, walking()
	, distance(0.0f), target(0.0f)
	, currentDirection(0), nextDirection(0)
	, timePerDistance(350), timeUsed(0)
	, animation()
	, map()
{
}

void Movable::move(int direction)
{
	if (!walking) {
		POINTFLOAT pf;
		if (direction == Map::MOVE_LEFT) {
			animation.use(0);
			if (map->canMove(this, direction, &point, &pf)) {
				distance = animation.getCurrentX() - pf.x;
				target = pf.x;
				currentDirection = direction;
				nextDirection = direction;
				timeUsed = 0;
				walking = true;
			}
		}
		else if (direction == Map::MOVE_RIGHT) {
			animation.use(1);
			if (map->canMove(this, direction, &point, &pf)) {
				distance = pf.x - animation.getCurrentX();
				target = pf.x;
				currentDirection = direction;
				nextDirection = direction;
				timeUsed = 0;
				walking = true;
			}
		}
		else if (direction == Map::MOVE_UP) {
			animation.use(2);
			if (map->canMove(this, direction, &point, &pf)) {
				distance = pf.y - animation.getCurrentY();
				target = pf.y;
				currentDirection = direction;
				nextDirection = direction;
				timeUsed = 0;
				walking = true;
			}
		}
		else if (direction == Map::MOVE_DOWN) {
			animation.use(3);
			if (map->canMove(this, direction, &point, &pf)) {
				distance = animation.getCurrentY() - pf.y;
				target = pf.y;
				currentDirection = direction;
				nextDirection = direction;
				timeUsed = 0;
				walking = true;
			}
		}
	}
	else {
		nextDirection = direction;
	}
}

void Movable::nextMove()
{
	move(nextDirection);
}

void Movable::play(ULONGLONG timeUsed)
{
	if (walking) {
		if (currentDirection == Map::MOVE_LEFT) {
			if (this->timeUsed + timeUsed < timePerDistance) {
				float d = timeUsed * distance / timePerDistance;
				animation.moveBy(-d, 0);
				this->timeUsed += timeUsed;
			}
			else {
				animation.moveTo(target, animation.getCurrentY());
				walking = false;
				nextMove();
			}
		}
		else if (currentDirection == Map::MOVE_RIGHT) {
			if (this->timeUsed + timeUsed < timePerDistance) {
				float d = timeUsed * distance / timePerDistance;
				animation.moveBy(d, 0);
				this->timeUsed += timeUsed;
			}
			else {
				animation.moveTo(target, animation.getCurrentY());
				walking = false;
				nextMove();
			}
		}
		else if (currentDirection == Map::MOVE_UP) {
			if (this->timeUsed + timeUsed < timePerDistance) {
				float d = timeUsed * distance / timePerDistance;
				animation.moveBy(0, d);
				this->timeUsed += timeUsed;
			}
			else {
				animation.moveTo(animation.getCurrentX(), target);
				walking = false;
				nextMove();
			}
		}
		else if (currentDirection == Map::MOVE_DOWN) {
			if (this->timeUsed + timeUsed < timePerDistance) {
				float d = timeUsed * distance / timePerDistance;
				animation.moveBy(0, -d);
				this->timeUsed += timeUsed;
			}
			else {
				animation.moveTo(animation.getCurrentX(), target);
				walking = false;
				nextMove();
			}
		}
	}
}

void Movable::draw(Sprite *sprite, const glm::mat4 *viewProjectMatrix, const glm::mat4 *scaleMatrix)
{
	glm::mat4 translateMatrix = glm::mat4(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(getCurrentX(), getCurrentY(), 0));
	glm::mat4 modelMatrix = translateMatrix * *scaleMatrix;
	glm::mat4 mvpMatrix = *viewProjectMatrix * modelMatrix;
	animation.draw(mvpMatrix, sprite);
}

