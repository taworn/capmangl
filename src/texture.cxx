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
	glDeleteTextures(1, &texture);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

Texture::Texture() : shader(), vao(0), vbo(0), ebo(0), texture(0), image(NULL)
{
	shader = Game::instance()->getTextureShader();
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenTextures(1, &texture);

	glBindVertexArray(vao);

	GLfloat vertices[] = {
		// Positions         // Texture Coords
		1.0f,  1.0f, 0.0f,   1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,   1.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,  0.0f, 1.0f,
		-1.0f,  1.0f, 0.0f,  0.0f, 0.0f,
	};
	GLuint indices[] = {
		// First Triangle
		0, 1, 3,
		// Second Triangle
		1, 2, 3,
	};
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Texture::init(const PNGImage *image)
{
	glBindTexture(GL_TEXTURE_2D, texture);

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

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(shader->getSampler(), 0);

	// position attribute
	glVertexAttribPointer(shader->getPosition(), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(shader->getPosition());
	// texcoord attribute
	glVertexAttribPointer(shader->getCoord(), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(shader->getCoord());

	// drawing
	glUniformMatrix4fv(shader->getMVPMatrix(), 1, false, &mvpMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

