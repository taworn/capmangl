/**
 * @file title_scene.hxx
 * @desc Title scene header.
 */
#ifndef SCENES_TITLE_SCENE_HXX
#define SCENES_TITLE_SCENE_HXX

 /**
  * Title game scene.
  */
class TitleScene : public Scene
{
public:
	virtual ~TitleScene();
	TitleScene();

protected:
	void init();
	void fini();

private:
	PNGImage image;
	Font *titleFont;

public:
	virtual bool handleKey(HWND hwnd, WPARAM key);
	virtual void render();
};

#endif // SCENES_TITLE_SCENE_HXX

