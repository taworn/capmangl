/**
 * @file texture.cxx
 * @desc Texture module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <png.h>
#include "pngimage.hxx"
#include "texture.hxx"
#include "game.hxx"

Texture::~Texture()
{
	glDeleteTextures(1, &textureHandle);
	glDeleteBuffers(1, &indicesHandle);
	glDeleteBuffers(1, &verticesHandle);
}

Texture::Texture() : shader(), verticesHandle(0), indicesHandle(0), textureHandle(0), image(NULL)
{
	shader = Game::instance()->getTextureShader();

	glGenBuffers(1, &verticesHandle);
	glGenBuffers(1, &indicesHandle);
	glGenTextures(1, &textureHandle);

	GLfloat verticesData[] = {
		// positions         // coords
		1.0f, 1.0f, 0.0f,    1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
	};
	GLuint indicesData[] = {
		// first triangle
		0, 1, 3,
		// second triangle
		1, 2, 3,
	};
	glBindBuffer(GL_ARRAY_BUFFER, verticesHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesData), indicesData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Texture::init(const PNGImage *image)
{
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	// sets our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// sets texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// loads, creates texture and generates mipmaps
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getImage());
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::draw(const glm::mat4x4 &mvpMatrix)
{
	shader->useProgram();
	glEnable(GL_TEXTURE_2D);

	glBindBuffer(GL_ARRAY_BUFFER, verticesHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesHandle);

	// uses texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glUniform1i(shader->getSampler(), 0);

	// vertices positions
	glVertexAttribPointer(shader->getPosition(), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(shader->getPosition());

	// texture coordinates
	glVertexAttribPointer(shader->getCoord(), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(shader->getCoord());

	// drawing
	glUniformMatrix4fv(shader->getMVPMatrix(), 1, false, &mvpMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

