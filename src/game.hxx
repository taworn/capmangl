#ifndef GAME_HXX
#define GAME_HXX

class Scene;

class Game
{
public:
	static Game* instance() { return singleton; }

	~Game();
	Game(HDC h);

	HDC getDevice() { return hdc; }
	GLuint getProgramHandle() const { return programHandle; }
	GLuint getPositionHandle() const { return positionHandle; }
	GLuint getColorHandle() const { return colorHandle; }
	GLuint getMVPMatrixHandle() const { return mvpMatrixHandle; }
	Scene* currentScene() { return scene; }

	void changeScene(Scene *newScene);

private:
	HDC hdc;
	GLuint programHandle;
	GLuint positionHandle;
	GLuint colorHandle;
	GLuint mvpMatrixHandle;
	Scene *scene;

	void init();

	static Game *singleton;
};

#endif // GAME_HXX

