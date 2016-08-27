/**
 * @file play_scene.cxx
 * @desc Playing scene module.
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
#include "../game/common.hxx"
#include "scene.hxx"
#include "play_scene.hxx"

PlayScene::~PlayScene()
{
	BOOST_LOG_TRIVIAL(debug) << "PlayScene::~PlayScene() called";
	fini();
}

PlayScene::PlayScene()
	: Scene()
	, spriteMap(), spritePacman()
	, map()
	, movDivoes(), movHero()
{
	BOOST_LOG_TRIVIAL(debug) << "PlayScene::PlayScene() called";
	init();

	GameData::instance()->clear();
	map.load();
	for (int i = 0; i < 4; i++) {
		movDivoes[i].setId(i);
		movDivoes[i].setMap(&map);
		GameData::instance()->addDivo(&movDivoes[i]);
	}
	movHero.setMap(&map);
	timeStart = GetTickCount();
}

void PlayScene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "PlayScene::init() called";

	PNGImage imageMap(".\\res\\map.png");
	spriteMap = new Sprite();
	spriteMap->init(&imageMap, 2, 2);

	PNGImage imagePacman(".\\res\\pacman.png");
	spritePacman = new Sprite();
	spritePacman->init(&imagePacman, 8, 8);
}

void PlayScene::fini()
{
	BOOST_LOG_TRIVIAL(debug) << "PlayScene::fini() called";
	if (spritePacman) {
		delete spritePacman;
		spritePacman = NULL;
	}
	if (spriteMap) {
		delete spriteMap;
		spriteMap = NULL;
	}
}

bool PlayScene::handleKey(HWND hwnd, WPARAM key)
{
	if (key == VK_SPACE) {
		// space
		BOOST_LOG_TRIVIAL(debug) << "SPACE keydown";
		return true;
	}
	else if (key == 0x41 || key == VK_LEFT) {
		// left
		BOOST_LOG_TRIVIAL(debug) << "A -or- LEFT keydown";
		movHero.move(Movable::MOVE_LEFT);
		return true;
	}
	else if (key == 0x44 || key == VK_RIGHT) {
		// right
		BOOST_LOG_TRIVIAL(debug) << "D -or- RIGHT keydown";
		movHero.move(Movable::MOVE_RIGHT);
		return true;
	}
	else if (key == 0x57 || key == VK_UP) {
		// up
		BOOST_LOG_TRIVIAL(debug) << "W -or- UP keydown";
		movHero.move(Movable::MOVE_UP);
		return true;
	}
	else if (key == 0x53 || key == VK_DOWN) {
		// down
		BOOST_LOG_TRIVIAL(debug) << "S -or- DOWN keydown";
		movHero.move(Movable::MOVE_DOWN);
		return true;
	}
	return false;
}

void PlayScene::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// sets timing
	ULONGLONG timeUsed = GetTickCount() - timeStart;
	timeStart = GetTickCount();
	//BOOST_LOG_TRIVIAL(trace) << "used " << timeUsed << " ms";
	GameData::instance()->update(timeUsed);
	movDivoes[0].play(timeUsed);
	movDivoes[1].play(timeUsed);
	movDivoes[2].play(timeUsed);
	movDivoes[3].play(timeUsed);
	movHero.play(timeUsed);
	movHero.detect();

	// combines viewing and projecting matrices
	glm::mat4 viewMatrix = glm::lookAt(
		//glm::vec3(movHero.getCurrentX(), movHero.getCurrentY(), 2.5f),    // camera
		//glm::vec3(movHero.getCurrentX(), movHero.getCurrentY(), -25.0f),  // looks
		glm::vec3(0.0f, 0.0f, 2.5f),    // camera
		glm::vec3(0.0f, 0.0f, -25.0f),  // looks
		glm::vec3(0.0f, 1.0f, 0.0f)     // head is up
	);
	glm::mat4 projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 25.0f);
	glm::mat4 viewProjectMatrix = projectionMatrix * viewMatrix;

	// drawing map
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
	map.draw(spriteMap, &viewProjectMatrix, &scaleMatrix);

	// drawing movables
	scaleMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(0.0625f, 0.0625f, 1.0f));
	movDivoes[0].draw(spritePacman, &viewProjectMatrix, &scaleMatrix);
	movDivoes[1].draw(spritePacman, &viewProjectMatrix, &scaleMatrix);
	movDivoes[2].draw(spritePacman, &viewProjectMatrix, &scaleMatrix);
	movDivoes[3].draw(spritePacman, &viewProjectMatrix, &scaleMatrix);
	movHero.draw(spritePacman, &viewProjectMatrix, &scaleMatrix);

	// checks idling
	for (int i = 0; i < 4; i++) {
		if (movDivoes[i].isIdle())
			movDivoes[i].nextAction();
	}

	computeFPS();
	SwapBuffers(Game::instance()->getDevice());
}

