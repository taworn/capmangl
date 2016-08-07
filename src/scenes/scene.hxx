/**
 * @file scene.hxx
 * @desc Game scene header.
 */
#ifndef SCENES_SCENE_HXX
#define SCENES_SCENE_HXX

class Shader;
class NormalShader;
class TextShader;
class Font;

/**
 * A single game scene.
 */
class Scene
{
public:
	/**
	 * Destructs a game scene.
	 */
	virtual ~Scene();

	/**
	 * Constructs a game scene.
	 */
	Scene();

protected:
	/**
	 * Initializes a game scene.
	 */
	void init();

	/**
	 * Uninitializes a game scene.
	 */
	void fini();

	/**
	 * Computes current frames per second.
	 */
	void computeFPS();

	HDC getDevice() { return hdc; }
	NormalShader* getNormalShader() { return normalShader; }
	TextShader* getTextShader() { return textShader; }
	TextureShader* getTextureShader() { return textureShader; }
	RECT getScreenRect() const { return screenRect; }
	Font* getSmallFont() { return smallFont; }
	Font* getNormalFont() { return normalFont; }
	Font* getBigFont() { return bigFont; }
	int getFPS() const { return fps; }

private:
	HDC hdc;
	NormalShader *normalShader;
	TextShader *textShader;
	TextureShader *textureShader;
	RECT screenRect;
	Font *smallFont;
	Font *normalFont;
	Font *bigFont;
	int fps;
	int frameCount;
	ULONGLONG timeStart;

	Scene(const Scene&);
	Scene& operator=(const Scene&);

public:
	/**
	 * Called when user press keyboard.
	 */
	virtual bool handleKey(HWND hwnd, WPARAM key);

	/**
	 * Called every render frame.
	 */
	virtual void render();
};

#endif // SCENES_SCENE_HXX

