/**
 * @file title_scene.cxx
 * @desc Title scene module.
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
#include "title_scene.hxx"

TitleScene::~TitleScene()
{
	BOOST_LOG_TRIVIAL(debug) << "TitleScene::~TitleScene() called";
	if (aniHero) {
		delete aniHero;
		aniHero = NULL;
	}
	if (aniDivo) {
		delete aniDivo;
		aniDivo = NULL;
	}
	fini();
}

TitleScene::TitleScene()
	: Scene()
	, titleFont()
	, spritePacman()
	, aniDivo()
	, aniHero()
	, modelX(0.0f)
	, menuIndex(0)
{
	BOOST_LOG_TRIVIAL(debug) << "TitleScene::TitleScene() called";
	init();

	const int TIME = 300;
	aniDivo = new Animation();
	aniDivo->add(0, 8, 10, Animation::ON_END_CONTINUE, TIME);
	aniDivo->add(1, 10, 12, Animation::ON_END_CONTINUE, TIME);
	aniDivo->add(2, 12, 14, Animation::ON_END_CONTINUE, TIME);
	aniDivo->add(3, 14, 16, Animation::ON_END_CONTINUE, TIME);
	aniDivo->use(0);

	aniHero = new Animation();
	aniHero->add(0, 0, 2, Animation::ON_END_CONTINUE, TIME);
	aniHero->add(1, 2, 4, Animation::ON_END_CONTINUE, TIME);
	aniHero->add(2, 4, 6, Animation::ON_END_CONTINUE, TIME);
	aniHero->add(3, 6, 8, Animation::ON_END_CONTINUE, TIME);
	aniHero->use(0);
}

void TitleScene::init()
{
	BOOST_LOG_TRIVIAL(debug) << "TitleScene::init() called";

	titleFont = new Font("C:\\WINDOWS\\Fonts\\timesbd.ttf", 128);

	PNGImage image(".\\res\\pacman.png");
	spritePacman = new Sprite();
	spritePacman->init(&image, 8, 8);
	PNGImage imageUI(".\\res\\ui.png");
	spriteUI = new Sprite();
	spriteUI->init(&imageUI, 2, 2);
}

void TitleScene::fini()
{
	BOOST_LOG_TRIVIAL(debug) << "TitleScene::fini() called";
	if (spriteUI) {
		delete spriteUI;
		spriteUI = NULL;
	}
	if (spritePacman) {
		delete spritePacman;
		spritePacman = NULL;
	}
	if (titleFont) {
		delete titleFont;
		titleFont = NULL;
	}
}

bool TitleScene::handleKey(HWND hwnd, WPARAM key)
{
	if (key == VK_RETURN) {
		// ENTER
		BOOST_LOG_TRIVIAL(debug) << "ENTER keydown";
		if (menuIndex == 0) {
			GameData::instance()->reset();
			Game::instance()->changeScene(Game::SCENE_STAGE);
		}
		else if (menuIndex == 1)
			Game::instance()->changeScene(Game::SCENE_STAGE);
		return true;
	}
	else if (key == 0x57 || key == VK_UP) {
		// up
		menuIndex--;
		if (menuIndex < 0)
			menuIndex = 1;
		return true;
	}
	else if (key == 0x53 || key == VK_DOWN) {
		// down
		menuIndex++;
		if (menuIndex > 1)
			menuIndex = 0;
		return true;
	}
	return false;
}

void TitleScene::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	RECT rc = getScreenRect();
	float sx = 2.0f / (rc.right - rc.left);
	float sy = 2.0f / (rc.bottom - rc.top);
	float w, h;

	titleFont->setColor(1.0f, 1.0f, 0.5f, 1.0f);
	titleFont->measure("Capman", &w, &h, sx, sy);
	titleFont->draw("Capman", 0 - (w / 2), h, sx, sy);

	// combines viewing and projecting matrices
	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 2.5f),    // camera
		glm::vec3(0.0f, 0.0f, -25.0f),  // looks
		glm::vec3(0.0f, 1.0f, 0.0f)     // head is up
	);
	glm::mat4 projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 25.0f);
	glm::mat4 viewProjectMatrix = projectionMatrix * viewMatrix;

	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(0.05f, 0.05f, 1.0f));
	glm::mat4 translateMatrix = glm::mat4(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(modelX, -0.2f, 0.0f));
	glm::mat4 modelMatrix = translateMatrix * scaleMatrix;
	glm::mat4 mvpMatrix = viewProjectMatrix * modelMatrix;
	aniHero->draw(mvpMatrix, spritePacman);

	translateMatrix = glm::mat4(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(modelX - 0.25f, -0.2f, 0.0f));
	modelMatrix = translateMatrix * scaleMatrix;
	mvpMatrix = viewProjectMatrix * modelMatrix;
	aniDivo->draw(mvpMatrix, spritePacman);

	modelX -= 0.01f;
	if (modelX < -1.0f)
		modelX = 1.0f;

	char buffer[64];
	Font *font = Game::instance()->getNormalFont();
	font->setColor(1.0f, 1.0f, 1.0f, 1.0f);
	strcpy(buffer, "Start");
	font->measure(buffer, &w, &h, sx, sy);
	font->draw(buffer, 0 - (w / 2), -0.1f, sx, sy);
	strcpy(buffer, "Continue");
	font->measure(buffer, &w, &h, sx, sy);
	font->draw(buffer, 0 - (w / 2), -0.2f, sx, sy);

	scaleMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(0.03f, 0.03f, 1.0f));
	if (menuIndex == 0) {
		glm::mat4 translateMatrix = glm::mat4(1.0f);
		translateMatrix = glm::translate(translateMatrix, glm::vec3(-0.16f, -0.075f, 0.0f));
		glm::mat4 modelMatrix = translateMatrix * scaleMatrix;
		glm::mat4 mvpMatrix = viewProjectMatrix * modelMatrix;
		spriteUI->draw(mvpMatrix, 1);
	}
	else {
		glm::mat4 translateMatrix = glm::mat4(1.0f);
		translateMatrix = glm::translate(translateMatrix, glm::vec3(-0.16f, -0.175f, 0.0f));
		glm::mat4 modelMatrix = translateMatrix * scaleMatrix;
		glm::mat4 mvpMatrix = viewProjectMatrix * modelMatrix;
		spriteUI->draw(mvpMatrix, 1);
	}

	computeFPS();
	SwapBuffers(Game::instance()->getDevice());
}

