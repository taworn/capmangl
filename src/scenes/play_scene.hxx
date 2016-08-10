/**
 * @file play_scene.hxx
 * @desc Playing scene header.
 */
#ifndef SCENES_PLAY_SCENE_HXX
#define SCENES_PLAY_SCENE_HXX

/**
 * Playing game scene.
 */
class PlayScene : public Scene
{
public:
	virtual ~PlayScene();
	PlayScene();

protected:
	void init();
	void fini();

private:
	PNGImage image;
	Texture *texture;

	float modelX;
	float modelY;
	float modelDx;
	float modelDy;

public:
	virtual bool handleKey(HWND hwnd, WPARAM key);
	virtual void render();
};

#endif // SCENES_PLAY_SCENE_HXX

