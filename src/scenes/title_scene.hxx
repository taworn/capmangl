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
	TitleScene(const TitleScene&);
	TitleScene& operator=(const TitleScene&);

public:
	virtual bool handleKey(HWND hwnd, WPARAM key);
	virtual void render(ULONGLONG timeCurrent);
};

#endif // SCENES_TITLE_SCENE_HXX

