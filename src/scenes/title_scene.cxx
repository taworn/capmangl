#include <windows.h>
#include <assert.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <boost/log/trivial.hpp>
#include <GL/glew.h>
#include "scene.hxx"
#include "title_scene.hxx"
#include "../game.hxx"

struct point {
	GLfloat x;
	GLfloat y;
	GLfloat s;
	GLfloat t;
};

TitleScene::~TitleScene()
{
	fini();
}

TitleScene::TitleScene() : Scene()
{
	init();
}

void TitleScene::init()
{
	glGenBuffers(1, &vbo);

	FT_Error error = FT_New_Face(Game::instance()->getFreeTypeLibrary(), "C:\\Windows\\Fonts\\arial.ttf", 0, &titleFace);
	if (error) {
		BOOST_LOG_TRIVIAL(debug) << "FreeType cannot load face.";
	}
}

void TitleScene::fini()
{
	FT_Done_Face(titleFace);
	glDeleteBuffers(1, &vbo);
}

bool TitleScene::handleKey(HWND hwnd, WPARAM key)
{
	if (key == VK_RETURN) {
		// ENTER
		OutputDebugStringW(L"ENTER keydown\n");
		Game::instance()->changeScene(SCENE_PLAY);
		return true;
	}
	return false;
}

void TitleScene::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	getTextShader().useProgram();

	RECT rc = getScreenRect();
	float sx = 2.0f / (rc.right - rc.left);
	float sy = 2.0f / (rc.bottom - rc.top);
	GLfloat textColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform4fv(getTextShader().getColor(), 1, textColor);
	float w, h;
	
	FT_Set_Pixel_Sizes(titleFace, 0, 128);
	measureText("Capman", sx, sy, &w, &h);
	render_text("Capman", 0 - (w / 2), h, sx, sy);
	
	FT_Set_Pixel_Sizes(titleFace, 0, 32);
	measureText("Press ENTER Key", sx, sy, &w, &h);
	render_text("Press ENTER Key", 0 - (w / 2), -0.70f, sx, sy);
	
	ULONGLONG fps = getFPS();
	char buffer[64];
	sprintf(buffer, "%ld", fps);
	FT_Set_Pixel_Sizes(titleFace, 0, 24);
	measureText(buffer, sx, sy, &w, &h);
	render_text(buffer, 1 - w * 2, -1 + h, sx, sy);
	
	computeFPS();
	drawFPS();
	SwapBuffers(getDevice());
}

void TitleScene::measureText(const char *text, float sx, float sy, float *w, float *h)
{
	const char *p;
	FT_GlyphSlot g = titleFace->glyph;

	float width = 0;
	float height = 0;
	for (p = text; *p; p++) {
		if (FT_Load_Char(titleFace, *p, FT_LOAD_RENDER))
			continue;

		width += g->bitmap.width;
		if (height < g->bitmap.rows)
			height = g->bitmap.rows;
		//width += (g->advance.x >> 6);
		//height += (g->advance.y >> 6);
	}

	*w = width * sx;
	*h = height * sy;
}

/**
 * Render text using the currently loaded font and currently set font size.
 * Rendering starts at coordinates (x, y), z is always 0.
 * The pixel coordinates that the FreeType2 library uses are scaled by (sx, sy).
 */
void TitleScene::render_text(const char *text, float x, float y, float sx, float sy)
{
	const char *p;
	FT_GlyphSlot g = titleFace->glyph;

	// Create a texture that will be used to hold one "glyph"
	GLuint tex;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(getTextShader().getTex(), 0);

	// We require 1 byte alignment when uploading texture data
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Clamping to edges is important to prevent artifacts when scaling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Linear filtering usually looks best for text
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set up the VBO for our vertex data
	glEnableVertexAttribArray(getTextShader().getCoord());
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(getTextShader().getCoord(), 4, GL_FLOAT, GL_FALSE, 0, 0);

	// Loop through all characters
	for (p = text; *p; p++) {
		// Try to load and render the character
		if (FT_Load_Char(titleFace, *p, FT_LOAD_RENDER))
			continue;

		// Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		// Calculate the vertex and texture coordinates
		float x2 = x + g->bitmap_left * sx;
		float y2 = -y - g->bitmap_top * sy;
		float w = g->bitmap.width * sx;
		float h = g->bitmap.rows * sy;

		point box[4] = {
			{x2, -y2, 0, 0},
			{x2 + w, -y2, 1, 0},
			{x2, -y2 - h, 0, 1},
			{x2 + w, -y2 - h, 1, 1},
		};

		// Draw the character on the screen
		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// Advance the cursor to the start of the next character
		x += (g->advance.x >> 6) * sx;
		y += (g->advance.y >> 6) * sy;
	}

	glDisableVertexAttribArray(getTextShader().getCoord());
	glDeleteTextures(1, &tex);
}

