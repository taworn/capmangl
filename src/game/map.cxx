/**
 * @file map.cxx
 * @desc Map module.
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

Map::~Map()
{
	if (imageData)
		delete[] imageData;
	if (mapData)
		delete[] mapData;
}

Map::Map()
	: width(), height(), startDivo(), startPacman()
	, mapData(), imageData(), itemCount()
	, horzBounds(), horzPoints(), vertBounds(), vertPoints()
{
}

bool Map::load(const char *fileName)
{
	// opens file
	FILE *fp = fopen(fileName, "r");
	if (!fp) {
		BOOST_LOG_TRIVIAL(debug) << "Cannot open file.";
		return false;
	}

	// checks file header, 8 bytes
	char check[8] = { 0x43, 0x41, 0x50, 0x4D, 0x41, 0x50, 0x00, 0x00, };
	char header[8] = { 0 };
	if (fread(header, 1, 8, fp) != 8) {
		BOOST_LOG_TRIVIAL(debug) << "Header is not acceptable.";
		fclose(fp);
		return false;
	}
	for (int i = 0; i < 8; i++) {
		if (header[i] != check[i]) {
			BOOST_LOG_TRIVIAL(debug) << "Header is not acceptable.";
			fclose(fp);
			return false;
		}
	}

	// reads header information
	// 6 int = width, height, divo start width, divo start height, pacman start width, pacman start height
	int32_t buffer[6] = { 0 };
	if (fread(buffer, 1, 6 * sizeof(int32_t), fp) != 6 * sizeof(int32_t)) {
		BOOST_LOG_TRIVIAL(debug) << "Not enough data to read.";
		fclose(fp);
		return false;
	}
	int w = buffer[0];
	int h = buffer[1];
	int size = w * h;

	// reads map data
	char *mapData = new char[size];
	if (fread(mapData, 1, size * sizeof(char), fp) != size * sizeof(char)) {
		BOOST_LOG_TRIVIAL(debug) << "Not enough data to read.";
		fclose(fp);
		return false;
	}

	// reads image data
	int *imageData = new int[size];
	for (int i = 0; i < size; i++) {
		int32_t image;
		if (fread(&image, 1, sizeof(int32_t), fp) != sizeof(int32_t)) {
			BOOST_LOG_TRIVIAL(debug) << "Not enough data to read.";
			fclose(fp);
			return false;
		}
		imageData[i] = (int)image;
	}

	// closes file
	fclose(fp);

	// copying
	this->width = w;
	this->height = h;
	this->startDivo.x = buffer[2];
	this->startDivo.y = buffer[3];
	this->startPacman.x = buffer[4];
	this->startPacman.y = buffer[5];
	if (this->mapData)
		delete[] this->mapData;
	if (this->imageData)
		delete[] this->imageData;
	this->mapData = mapData;
	this->imageData = imageData;

	itemCount = 0;
	for (int i = 0; i < size; i++)
		if ((mapData[i] & 0x10) == 0x10)
			itemCount++;

	horzBounds.clear();
	horzBounds.resize(width + 1);
	for (size_t i = 0; i < horzBounds.size(); i++)
		horzBounds[i] = (float)i / 16 * 2.0f - 1.0f;

	horzPoints.clear();
	horzPoints.resize(width);
	for (size_t i = 0; i < horzPoints.size(); i++)
		horzPoints[i] = (horzBounds[i] + horzBounds[i + 1]) / 2.0f;

	vertBounds.clear();
	vertBounds.resize(height + 1);
	for (size_t i = 0; i < vertBounds.size(); i++)
		vertBounds[i] = (float)(vertBounds.size() - 1 - i) / 16 * 2.0f - 1.0f;

	vertPoints.clear();
	vertPoints.resize(height);
	for (size_t i = 0; i < vertPoints.size(); i++)
		vertPoints[i] = (vertBounds[i] + vertBounds[i + 1]) / 2.0f;

	return true;
}

void Map::getDivoStartPosition(POINT *p, POINTFLOAT *pf)
{
	p->x = startDivo.x;
	p->y = startDivo.y;
	pf->x = horzPoints[p->x];
	pf->y = vertPoints[p->y];
}

void Map::getPacmanStartPosition(POINT *p, POINTFLOAT *pf)
{
	p->x = startPacman.x;
	p->y = startPacman.y;
	pf->x = horzPoints[p->x];
	pf->y = vertPoints[p->y];
}

bool Map::canMove(Movable *movable, int direction, POINT *p, POINTFLOAT *pf)
{
	if (direction == Movable::MOVE_LEFT) {
		int current = movable->getX();
		int next = current - 1;
		if (next >= 0) {
			int block = mapData[movable->getY() * width + next] & 0x01;
			if (!block) {
				p->x = next;
				p->y = movable->getY();
				pf->x = horzPoints[p->x];
				pf->y = vertPoints[p->y];
				return true;
			}
		}
	}
	else if (direction == Movable::MOVE_RIGHT) {
		int current = movable->getX();
		int next = current + 1;
		if (next < width) {
			int block = mapData[movable->getY() * width + next] & 0x01;
			if (!block) {
				p->x = next;
				p->y = movable->getY();
				pf->x = horzPoints[p->x];
				pf->y = vertPoints[p->y];
				return true;
			}
		}
	}
	else if (direction == Movable::MOVE_UP) {
		int current = movable->getY();
		int next = current - 1;
		if (next >= 0) {
			int block = mapData[next * width + movable->getX()] & 0x01;
			if (!block) {
				p->x = movable->getX();
				p->y = next;
				pf->x = horzPoints[p->x];
				pf->y = vertPoints[p->y];
				return true;
			}
		}
	}
	else if (direction == Movable::MOVE_DOWN) {
		int current = movable->getY();
		int next = current + 1;
		if (next < height) {
			int block = mapData[next * width + movable->getX()] & 0x01;
			if (!block) {
				p->x = movable->getX();
				p->y = next;
				pf->x = horzPoints[p->x];
				pf->y = vertPoints[p->y];
				return true;
			}
		}
	}

	return false;
}

int Map::canPreviewMove(Movable *movable)
{
	int x = movable->getX();
	int y = movable->getY();
	int result = 0;

	// left
	if (x > 0 && !(mapData[y * width + x - 1] & 0x01))
		result |= Movable::MOVE_LEFT;
	// right
	if (x < width - 1 && !(mapData[y * width + x + 1] & 0x01))
		result |= Movable::MOVE_RIGHT;
	// up
	if (y > 0 && !(mapData[(y - 1) * width + x] & 0x01))
		result |= Movable::MOVE_UP;
	// down
	if (y < height - 1 && !(mapData[(y + 1) * width + x] & 0x01))
		result |= Movable::MOVE_DOWN;

	return result;
}

bool Map::checkAndGetItem(Movable *movable, int *item)
{
	int index = movable->getY() * width + movable->getX();
	if (mapData[index] & 0x10) {
		*item = imageData[index];
		imageData[index] = 0;
		mapData[index] &= ~0x10;
		itemCount--;

		if (itemCount <= 0) {
			BOOST_LOG_TRIVIAL(debug) << "Game Over because Divoes eat all items";
			Game::instance()->changeScene(Game::SCENE_GAMEOVER);
		}

		return true;
	}
	return false;
}

void Map::draw(Sprite *sprite, const glm::mat4 *viewProjectMatrix, const glm::mat4 *scaleMatrix)
{
	glm::mat4 translateMatrix = glm::mat4(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(0, 0, 0));
	glm::mat4 modelMatrix = translateMatrix * *scaleMatrix;
	glm::mat4 mvpMatrix = *viewProjectMatrix * modelMatrix;
	sprite->drawBatch(mvpMatrix, horzBounds, vertBounds, 0.0f, imageData);
}

