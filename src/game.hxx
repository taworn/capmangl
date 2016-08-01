#ifndef GAME_HXX
#define GAME_HXX

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

private:
	HDC hdc;
	Scene *scene;

	void init();
	void fini();

	static Game *singleton;
};

#endif // GAME_HXX

