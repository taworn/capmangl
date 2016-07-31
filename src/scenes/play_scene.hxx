#ifndef SCENES_PLAY_SCENE_HXX
#define SCENES_PLAY_SCENE_HXX

class PlayScene : public Scene
{
public:
	virtual ~PlayScene();
	PlayScene();

protected:
	void init();
	void fini();

private:
	float modelX;
	float modelY;
	float modelDx;
	float modelDy;
	float angle;
	float angleToPlus;
	GLuint verticesId;
	DWORD lastTick;

	PlayScene(const PlayScene&);
	PlayScene& operator=(const PlayScene&);

public:
	virtual bool handleKey(HWND hwnd, WPARAM key);
	virtual void render(ULONGLONG timeCurrent);
};

#endif // SCENES_PLAY_SCENE_HXX
