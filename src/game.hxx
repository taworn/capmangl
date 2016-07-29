#ifndef GAME_HXX
#define GAME_HXX

class Game
{
private:
	HDC hdc;
	ULONGLONG timeStart;
	int frameCount;

	GLuint mvpMatrixHandle;
	GLuint positionHandle;
	GLuint colorHandle;
	GLuint programHandle;

	// this is a model we want to draw
	GLuint verticesId;

	// MVP matrices
	glm::mat4x4 translateMatrix;
	glm::mat4x4 rotateMatrix;
	glm::mat4x4 viewMatrix;
	glm::mat4x4 projectionMatrix;
	glm::mat4x4 mvpMatrix;

	float modelX;
	float modelY;
	float modelDx;
	float modelDy;
	float angle;
	float angleToPlus;
	DWORD lastTick;

	void init();
	void fini();
	void fps();

public:
	Game(HDC h) : hdc(h)
	{
		init();
	}

	~Game()
	{
		fini();
	}

	bool handleKey(HWND hwnd, WPARAM key);
	void render();
};

#endif // GAME_HXX

