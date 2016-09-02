/**
 * @file gameover_scene.hxx
 * @desc Game over scene header.
 */
#ifndef SCENES_GAMEOVER_SCENE_HXX
#define SCENES_GAMEOVER_SCENE_HXX

/**
 * Game over scene.
 */
class GameOverScene : public Scene
{
public:
	virtual ~GameOverScene();
	GameOverScene();

protected:
	void init();
	void fini();

private:
	Sprite *spriteUI;
	int menuIndex;

public:
	virtual bool handleKey(HWND hwnd, WPARAM key);
	virtual void render();
};

#endif // SCENES_GAMEOVER_SCENE_HXX

