/**
 * @file texture.hxx
 * @desc Texture header.
 */
#ifndef TEXTURE_HXX
#define TEXTURE_HXX

#include <glm/glm.hpp>

class TextureShader;

/**
 * A texture class.
 */
class Texture
{
public:
	/**
	 * Destructs a texture.
	 */
	~Texture();

	/**
	 * Constructs a texture.
	 */
	Texture();

	/**
	 * Initializes a texture with PNG image.
	 */
	void init(const PNGImage *image);

	/**
	 * Draws texture.
	 */
	void draw(const glm::mat4x4 &mvpMatrix);

private:
	TextureShader *shader;
	GLuint vao, vbo, ebo;
	GLuint texture;
	PNGImage *image;

	Texture(const Texture&);
	Texture& operator=(const Texture&);
};

#endif // TEXTURE_HXX

