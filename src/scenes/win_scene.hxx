/**
 * @file win_scene.hxx
 * @desc Win scene header.
 */
#ifndef SCENES_WIN_SCENE_HXX
#define SCENES_WIN_SCENE_HXX

/**
 * You win scene.
 */
class WinScene : public Scene
{
public:
	virtual ~WinScene();
	WinScene();

protected:
	void init();
	void fini();

public:
	virtual bool handleKey(HWND hwnd, WPARAM key);
	virtual void render();
};

#endif // SCENES_WIN_SCENE_HXX

