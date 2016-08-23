/**
 * @file sprite.cxx
 * @desc Sprite module.
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
#include "sprite.hxx"
#include "game.hxx"

Sprite::~Sprite()
{
	glDeleteTextures(1, &textureHandle);
	glDeleteBuffers(1, &indicesHandle);
	glDeleteBuffers(1, &verticesHandle);
}

Sprite::Sprite()
	: verticesHandle(0), indicesHandle(0), textureHandle(0)
	, uData(), vData(), sliceHorz(0), sliceVert(0)
{
	glGenBuffers(1, &verticesHandle);
	glGenBuffers(1, &indicesHandle);
	glGenTextures(1, &textureHandle);

	GLuint indicesData[] = {
		// first triangle
		0, 1, 3,
		// second triangle
		1, 2, 3,
	};
	glBindBuffer(GL_ARRAY_BUFFER, verticesHandle);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesData), indicesData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::init(const PNGImage *image, int sliceHorz, int sliceVert)
{
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	// sets our sprite parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// sets sprite filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// loads, creates sprite and generates mipmaps
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getImage());
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	uData.clear();
	for (int i = 0; i <= sliceHorz; i++)
		uData.push_back((float)i / sliceHorz);
	vData.clear();
	for (int i = 0; i <= sliceVert; i++)
		vData.push_back((float)i / sliceVert);
	this->sliceHorz = sliceHorz;
	this->sliceVert = sliceVert;
}

void Sprite::draw(const glm::mat4 &mvpMatrix, int imageIndex)
{
	TextureShader *shader = Game::instance()->getTextureShader();
	shader->useProgram();
	glEnable(GL_TEXTURE_2D);

	int uIndex = imageIndex % sliceHorz;
	int vIndex = (sliceHorz * sliceVert - imageIndex - 1) / sliceHorz;
	float u0 = uData[uIndex];
	float u1 = uData[uIndex + 1];
	float v0 = vData[vIndex];
	float v1 = vData[vIndex + 1];

	float verticesData[] = {
		// vertex        // coord
		1.0, 1.0, 0.0,   u1, v1,
		1.0, -1.0, 0.0,  u1, v0,
		-1.0, -1.0, 0.0, u0, v0,
		-1.0, 1.0, 0.0,  u0, v1
	};
	glBindBuffer(GL_ARRAY_BUFFER, verticesHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesHandle);

	// uses sprite
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glUniform1i(shader->getSampler(), 0);

	// vertices positions
	glVertexAttribPointer(shader->getPosition(), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(shader->getPosition());

	// sprite coordinates
	glVertexAttribPointer(shader->getCoord(), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(shader->getCoord());

	// drawing
	glUniformMatrix4fv(shader->getMVPMatrix(), 1, false, &mvpMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::drawBatch(const glm::mat4 &mvpMatrix, std::vector<float> horz, std::vector<float> vert, int *imageIndex)
{
	TextureShader *shader = Game::instance()->getTextureShader();
	shader->useProgram();
	glEnable(GL_TEXTURE_2D);

	int width = (int)horz.size() - 1;
	int height = (int)vert.size() - 1;
	float *verticesData = new float[5 * width * height * 6];
	int indices = 0;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			int index = *imageIndex++;
			int uIndex = index % sliceHorz;
			int vIndex = (sliceHorz * sliceVert - index - 1) / sliceHorz;
			float u0 = uData[uIndex];
			float u1 = uData[uIndex + 1];
			float v0 = vData[vIndex];
			float v1 = vData[vIndex + 1];

			// 1
			verticesData[indices++] = horz[i + 1];
			verticesData[indices++] = vert[j + 1];
			verticesData[indices++] = 0.0f;
			verticesData[indices++] = u1;
			verticesData[indices++] = v0;

			// 2
			verticesData[indices++] = horz[i + 1];
			verticesData[indices++] = vert[j];
			verticesData[indices++] = 0.0f;
			verticesData[indices++] = u1;
			verticesData[indices++] = v1;

			// 3
			verticesData[indices++] = horz[i];
			verticesData[indices++] = vert[j + 1];
			verticesData[indices++] = 0.0f;
			verticesData[indices++] = u0;
			verticesData[indices++] = v0;

			// 4
			verticesData[indices++] = horz[i + 1];
			verticesData[indices++] = vert[j];
			verticesData[indices++] = 0.0f;
			verticesData[indices++] = u1;
			verticesData[indices++] = v1;

			// 5
			verticesData[indices++] = horz[i];
			verticesData[indices++] = vert[j];
			verticesData[indices++] = 0.0f;
			verticesData[indices++] = u0;
			verticesData[indices++] = v1;

			// 6
			verticesData[indices++] = horz[i];
			verticesData[indices++] = vert[j + 1];
			verticesData[indices++] = 0.0f;
			verticesData[indices++] = u0;
			verticesData[indices++] = v0;
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, verticesHandle);
	glBufferData(GL_ARRAY_BUFFER, 5 * width * height * 6 * sizeof(float), verticesData, GL_STATIC_DRAW);

	// uses sprite
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glUniform1i(shader->getSampler(), 0);

	// vertices positions
	glVertexAttribPointer(shader->getPosition(), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(shader->getPosition());

	// sprite coordinates
	glVertexAttribPointer(shader->getCoord(), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(shader->getCoord());

	// drawing
	glUniformMatrix4fv(shader->getMVPMatrix(), 1, false, &mvpMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, width * height * 6);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] verticesData;
}

