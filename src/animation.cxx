/**
 * @file animation.cxx
 * @desc Animation module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <png.h>
#include "pngimage.hxx"
#include "sprite.hxx"
#include "animation.hxx"
#include "game.hxx"

Animation::~Animation()
{
}

Animation::Animation()
	: plays(), currentPlaying(-1), currentImage(0)
	, currentX(0), currentY(0), velocityX(0), velocityY(0)
{
	timeStart = GetTickCount();
}

void Animation::add(int number, int start, int end, int time)
{
	assert(number >= 0 && number < PLAYING_MAX);
	plays[number].start = start;
	plays[number].end = end;
	plays[number].time = time;
}

void Animation::use(int number)
{
	assert(number >= 0 && number < PLAYING_MAX);
	if (number != currentPlaying) {
		currentPlaying = number;
		currentImage = plays[number].start;
		timeStart = GetTickCount();
	}
}

void Animation::draw(const glm::mat4 &mvpMatrix, Sprite *sprite)
{
	sprite->draw(mvpMatrix, currentImage);

	ULONGLONG usage = GetTickCount() - timeStart;
	if (usage > (ULONGLONG)plays[currentPlaying].time) {
		currentImage++;
		if (currentImage >= plays[currentPlaying].end)
			currentImage = plays[currentPlaying].start;
		timeStart = GetTickCount();
	}
}

