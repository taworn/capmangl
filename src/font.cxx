/**
 * @file font.cxx
 * @desc Font module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include "font.hxx"
#include "game.hxx"

Font::~Font()
{
	if (face)
		FT_Done_Face(face);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &handle);
}

Font::Font() : handle(0), face(0), rgba()
{
	glGenBuffers(1, &handle);
}

Font::Font(const char *faceName, int size) : handle(0), face(0), rgba()
{
	glGenBuffers(1, &handle);
	if (!init(faceName, size))
		throw std::runtime_error("Font is not created.");
}

bool Font::init(const char *faceName, int size)
{
	FT_Face face;
	FT_Error error = FT_New_Face(Game::instance()->getFreeTypeLibrary(), faceName, 0, &face);
	if (error)
		return false;
	FT_Set_Pixel_Sizes(face, 0, size);

	if (this->face)
		FT_Done_Face(this->face);
	this->face = face;

	return true;
}

void Font::measure(const char *text, GLfloat *w, GLfloat *h, GLfloat sx, GLfloat sy)
{
	FT_GlyphSlot g = face->glyph;
	const char *p;
	unsigned int width = 0;
	unsigned int height = 0;

	for (p = text; *p; p++) {
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER) == 0) {
			width += g->advance.x >> 6;
			if (height < g->bitmap.rows)
				height = g->bitmap.rows;
		}
	}

	*w = (GLfloat)width * sx;
	*h = (GLfloat)height * sy;
}

void Font::draw(const char *text, GLfloat x, GLfloat y, GLfloat sx, GLfloat sy)
{
	FT_GlyphSlot g = face->glyph;
	const char *p;
	GLuint tex;

	TextShader *shader = Game::instance()->getTextShader();
	shader->useProgram();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUniform4fv(shader->getColor(), 1, rgba);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(shader->getTex(), 0);

	// We require 1 byte alignment when uploading texture data
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Clamping to edges is important to prevent artifacts when scaling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Linear filtering usually looks best for text
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set up the VBO for our vertex data
	glEnableVertexAttribArray(shader->getCoord());
	glBindBuffer(GL_ARRAY_BUFFER, handle);
	glVertexAttribPointer(shader->getCoord(), 4, GL_FLOAT, GL_FALSE, 0, 0);

	// Loop through all characters
	for (p = text; *p; p++) {
		// Try to load and render the character
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER) == 0) {
			// Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

			// Calculate the vertex and texture coordinates
			GLfloat x2 = x + g->bitmap_left * sx;
			GLfloat y2 = -y - g->bitmap_top * sy;
			GLfloat w = g->bitmap.width * sx;
			GLfloat h = g->bitmap.rows * sy;

			GLfloat box[16] = {
				// x, y, s, t
				x2, -y2, 0, 0,
				x2 + w, -y2, 1, 0,
				x2, -y2 - h, 0, 1,
				x2 + w, -y2 - h, 1, 1,
			};

			// Draw the character on the screen
			glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			// Advance the cursor to the start of the next character
			x += (g->advance.x >> 6) * sx;
			y += (g->advance.y >> 6) * sy;
		}
	}

	glDisableVertexAttribArray(shader->getCoord());
	glDeleteTextures(1, &tex);
}

