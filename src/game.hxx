/**
 * @file game.hxx
 * @desc Game engine header.
 */
#ifndef GAME_HXX
#define GAME_HXX

#include "shaders/shader.hxx"
#include "shaders/normal_shader.hxx"
#include "shaders/text_shader.hxx"
#include "font.hxx"

#define SCENE_DEFAULT 0
#define SCENE_TITLE 1
#define SCENE_PLAY 2

class Scene;

/**
 * A simple game engine class.
 */
class Game
{
public:
	static Game* instance() { return singleton; }

	/**
	 * Destructs the game engine.
	 */
	~Game();

	/**
	 * Constructs the game engine.
	 */
	Game(HDC h);

	/**
	 * Changes the new scene.
	 * @param sceneId A scene identifier, look at SCENE_*.
	 */
	void changeScene(int sceneId);

	/**
	 * Called when user press keyboard.
	 */
	bool handleKey(HWND hwnd, WPARAM key);

	/**
	 * Called every render frame.
	 */
	void render();

	HDC getDevice() const { return hdc; }
	NormalShader* getNormalShader() { return normalShader; }
	TextShader* getTextShader() { return textShader; }
	Font* getSmallFont() { return smallFont; }
	Font* getNormalFont() { return normalFont; }
	Font* getBigFont() { return bigFont; }
	FT_Library getFreeTypeLibrary() const { return freeTypeLibrary; }

private:
	HDC hdc;
	NormalShader *normalShader;
	TextShader *textShader;
	FT_Library freeTypeLibrary;
	Font *smallFont;
	Font *normalFont;
	Font *bigFont;
	Scene *scene;

	void init();
	void fini();

	static Game *singleton;
};

#endif // GAME_HXX

