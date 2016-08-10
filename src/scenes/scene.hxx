/**
 * @file scene.hxx
 * @desc Game scene header.
 */
#ifndef SCENES_SCENE_HXX
#define SCENES_SCENE_HXX

#include <glm/glm.hpp>

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

	glm::mat4x4 getViewAndProjectMatrix() const { return viewAndProjectMatrix; }
	RECT getScreenRect() const { return screenRect; }
	int getFPS() const { return fps; }

private:
	glm::mat4x4 viewAndProjectMatrix;
	RECT screenRect;
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

