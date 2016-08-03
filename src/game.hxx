#ifndef GAME_HXX
#define GAME_HXX

#include "shaders/shader.hxx"
#include "shaders/normal_shader.hxx"
#include "shaders/text_shader.hxx"

#define SCENE_DEFAULT 0
#define SCENE_TITLE 1
#define SCENE_PLAY 2

class Scene;

class Game
{
public:
	static Game* instance() { return singleton; }

	~Game();
	Game(HDC h);

	void changeScene(int sceneId);
	bool handleKey(HWND hwnd, WPARAM key);
	void render();

	HDC getDevice() const { return hdc; }
	NormalShader& getNormalShader() { return *normalShader; }
	TextShader& getTextShader() { return *textShader; }
	FT_Library getFreeTypeLibrary() const { return freeTypeLibrary; }

private:
	HDC hdc;
	NormalShader *normalShader;
	TextShader *textShader;
	FT_Library freeTypeLibrary;
	Scene *scene;

	void init();
	void fini();

	static Game *singleton;
};

#endif // GAME_HXX

