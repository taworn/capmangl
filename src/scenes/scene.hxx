#ifndef SCENES_SCENE_HXX
#define SCENES_SCENE_HXX

class Scene
{
public:
	virtual ~Scene();
	Scene();

protected:
	void fps(ULONGLONG timeCurrent);

private:
	ULONGLONG timeStart;
	int frameCount;

	Scene(const Scene&);
	Scene& operator=(const Scene&);

public:
	virtual bool handleKey(HWND hwnd, WPARAM key) = 0;
	virtual void render(ULONGLONG timeCurrent) = 0;
};

#endif // SCENES_SCENE_HXX

