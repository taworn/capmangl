/**
 * @file sprite.hxx
 * @desc Sprite header.
 */
#ifndef SPRITE_HXX
#define SPRITE_HXX

#include <vector>
#include <glm/glm.hpp>

class TextureShader;

/**
 * A sprite class.
 */
class Sprite
{
public:
	/**
	 * Destructs a sprite.
	 */
	~Sprite();

	/**
	 * Constructs a sprite.
	 */
	Sprite();

	/**
	 * Initializes a sprite with PNG image.
	 */
	void init(const PNGImage *image, int sliceHorz, int sliceVert);

	/**
	 * Draws sprite.
	 */
	void draw(const glm::mat4 &mvpMatrix, int imageIndex);

	/**
	 * Draws batch sprites.
	 */
	void drawBatch(const glm::mat4 &mvpMatrix, std::vector<float> horz, std::vector<float> vert, float z, int *imageIndex);

	/**
	 * Gets number of images.
	 */
	int getImageCount() const { return sliceHorz * sliceVert; }

private:
	GLuint verticesHandle;
	GLuint indicesHandle;
	GLuint textureHandle;
	std::vector<float> uData;
	std::vector<float> vData;
	int sliceHorz;
	int sliceVert;

	Sprite(const Sprite&);
	Sprite& operator=(const Sprite&);
};

#endif // SPRITE_HXX

