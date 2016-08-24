/**
 * @file game.hxx
 * @desc Game engine header.
 */
#ifndef GAME_HXX
#define GAME_HXX

#include "shaders/shader.hxx"
#include "shaders/normal_shader.hxx"
#include "shaders/text_shader.hxx"
#include "shaders/texture_shader.hxx"

#include "font.hxx"
#include "pngimage.hxx"
#include "texture.hxx"
#include "sprite.hxx"
#include "animation.hxx"

class Scene;

/**
 * A simple game engine class.
 */
class Game
{
public:
	static Game* instance() { return singleton; }

	static const int SCENE_DEFAULT = 0;
	static const int SCENE_TITLE = 1;
	static const int SCENE_STAGE = 2;
	static const int SCENE_PLAY = 3;
	static const int SCENE_GAMEOVER = 4;
	static const int SCENE_WIN = 5;

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
	 * Called when window is activated/deactivated.
	 */
	void handleActivate(HWND hwnd, bool active);

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
	TextureShader* getTextureShader() { return textureShader; }
	Font* getSmallFont() { return smallFont; }
	Font* getNormalFont() { return normalFont; }
	Font* getBigFont() { return bigFont; }
	FT_Library getFreeTypeLibrary() const { return freeTypeLibrary; }

private:
	HDC hdc;
	NormalShader *normalShader;
	TextShader *textShader;
	TextureShader *textureShader;
	FT_Library freeTypeLibrary;
	Font *smallFont;
	Font *normalFont;
	Font *bigFont;
	Scene *scene;
	int nextSceneId;

	/**
	 * Performs real scene switching.
	 */
	void switchScene();

	static Game *singleton;
};

#endif // GAME_HXX

