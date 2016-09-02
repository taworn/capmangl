/**
 * @file stage_scene.hxx
 * @desc Stage scene header.
 */
#ifndef SCENES_STAGE_SCENE_HXX
#define SCENES_STAGE_SCENE_HXX

/**
 * Stage scene.
 */
class StageScene : public Scene
{
public:
	virtual ~StageScene();
	StageScene();

protected:
	void init();
	void fini();

private:
	ULONGLONG timeStart;
	ULONGLONG timeUsed;

public:
	virtual void handleActivate(HWND hwnd, bool active);
	virtual bool handleKey(HWND hwnd, WPARAM key);
	virtual void render();
};

#endif // SCENES_STAGE_SCENE_HXX

