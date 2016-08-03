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
	GLuint vbo;
	FT_Face titleFace;

	void measureText(const char *text, float sx, float sy, float *w, float *h);
	void render_text(const char *text, float x, float y, float sx, float sy);

public:
	virtual bool handleKey(HWND hwnd, WPARAM key);
	virtual void render();
};

#endif // SCENES_TITLE_SCENE_HXX

