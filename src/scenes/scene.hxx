#ifndef SCENES_SCENE_HXX
#define SCENES_SCENE_HXX

class Shader;
class NormalShader;
class TextShader;
class Font;

class Scene
{
public:
	virtual ~Scene();
	Scene();

protected:
	void init();
	void fini();

	void computeFPS();
	void drawFPS();

	HDC getDevice() { return hdc; }
	NormalShader& getNormalShader() { return normalShader; }
	TextShader& getTextShader() { return textShader; }
	RECT getScreenRect() const { return screenRect; }
	ULONGLONG getFPS() const { return fps; }

private:
	HDC hdc;
	NormalShader& normalShader;
	TextShader& textShader;
	RECT screenRect;
	Font smallFont;
	int frameCount;
	ULONGLONG fps;
	ULONGLONG timeStart;

	Scene(const Scene&);
	Scene& operator=(const Scene&);

public:
	virtual bool handleKey(HWND hwnd, WPARAM key);
	virtual void render();
};

#endif // SCENES_SCENE_HXX

