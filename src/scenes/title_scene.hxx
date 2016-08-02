#ifndef SCENES_TITLE_SCENE_HXX
#define SCENES_TITLE_SCENE_HXX

class TitleScene : public Scene
{
public:
	virtual ~TitleScene();
	TitleScene();

protected:
	void init();
	void fini();

private:
	FT_Face titleFace;

public:
	virtual bool handleKey(HWND hwnd, WPARAM key);
	virtual void render();
};

#endif // SCENES_TITLE_SCENE_HXX

