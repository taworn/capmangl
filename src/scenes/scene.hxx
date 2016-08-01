#ifndef SCENES_SCENE_HXX
#define SCENES_SCENE_HXX

class Scene
{
public:
	virtual ~Scene();
	Scene();

protected:
	void init();
	void fini();

	GLuint loadShader(GLenum shaderType, const char *sourceCode);
	void computeFPS();
	void drawFPS();

	HDC getDevice() { return hdc; }
	GLuint getProgramHandle() const { return programHandle; }
	GLuint getPositionHandle() const { return positionHandle; }
	GLuint getColorHandle() const { return colorHandle; }
	GLuint getMVPMatrixHandle() const { return mvpMatrixHandle; }
	RECT getScreenRect() const { return screenRect; }
	ULONGLONG getFPS() const { return fps; }

private:
	HDC hdc;
	GLuint vertexShaderHandle;
	GLuint fragmentShaderHandle;
	GLuint programHandle;
	GLuint positionHandle;
	GLuint colorHandle;
	GLuint mvpMatrixHandle;
	RECT screenRect;
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

