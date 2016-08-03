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
	// vertex shader and fragment shader
	const char vertexShader[] = ""
		"attribute vec4 coord;\n"
		"varying vec2 texpos;\n"
		"void main() {\n"
		"  gl_Position = vec4(coord.xy, 0, 1);\n"
		"  texpos = coord.zw;\n"
		"}\n";
	const char fragmentShader[] = ""
		"varying vec2 texpos;\n"
		"uniform sampler2D tex;\n"
		"uniform vec4 color;\n"
		"void main() {\n"
		"  gl_FragColor = vec4(1, 1, 1, texture2D(tex, texpos).a) * color;\n"
		"}\n";
	int vertexShaderHandle = loadShader(GL_VERTEX_SHADER, vertexShader);
	int fragmentShaderHandle = loadShader(GL_FRAGMENT_SHADER, fragmentShader);
	int programHandle = glCreateProgram();
	if (programHandle != 0) {
		glAttachShader(programHandle, vertexShaderHandle);
		glAttachShader(programHandle, fragmentShaderHandle);
		glLinkProgram(programHandle);
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == 0) {
			glDeleteProgram(programHandle);
			programHandle = 0;
		}
	}
	attribute_coord = glGetAttribLocation(programHandle, "coord");
	uniform_tex = glGetUniformLocation(programHandle, "tex");
	uniform_color = glGetUniformLocation(programHandle, "color");
	glUseProgram(programHandle);

	glGenBuffers(1, &vbo);

	FT_Error error = FT_New_Face(Game::instance()->getFreeTypeLibrary(), "C:\\Windows\\Fonts\\arial.ttf", 0, &titleFace);
	if (error) {
		BOOST_LOG_TRIVIAL(debug) << "FreeType cannot load face.";
	}
}

void TitleScene::fini()
{
	FT_Done_Face(titleFace);
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

	RECT rc = getScreenRect();
	float sx = 2.0f / (rc.right - rc.left);
	float sy = 2.0f / (rc.bottom - rc.top);

	/* Enable blending, necessary for our alpha texture */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLfloat black[4] = { 0, 1, 0, 1 };
	GLfloat red[4] = { 1, 0, 0, 1 };
	GLfloat transparent_green[4] = { 0, 1, 0, 0.5 };

	/* Set font size to 48 pixels, color to black */
	FT_Set_Pixel_Sizes(titleFace, 0, 48);
	glUniform4fv(uniform_color, 1, black);

	//render_text("Capman", -1 + 8 * sx, 1 - 50 * sy, sx, sy);
	//render_text("The Misaligned Fox Jumps Over The Lazy Dog", -1 + 8.5 * sx, 1 - 100.5 * sy, sx, sy);

	//render_text("Capman", -1 + 8 * sx, 1 - 175 * sy, sx * 0.5, sy * 0.5);
	FT_Set_Pixel_Sizes(titleFace, 0, 24);
	//render_text("CapmanGL", -1 + 8 * sx, 1 - 200 * sy, sx, sy);

	//render_text("Capman", -1 + 8 * sx, 1 - 430 * sy, sx, sy);

	glUniform4fv(uniform_color, 1, red);
	//render_text("Cap", -1 + 8 * sx, 1 - 330 * sy, sx, sy);
	//render_text("man", -1 + 28 * sx, 1 - 450 * sy, sx, sy);
	GLfloat textColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	FT_Set_Pixel_Sizes(titleFace, 0, 48);
	glUniform4fv(uniform_color, 1, textColor);
	render_text("Capman", 0, 0, sx, sy);
	render_text("GL", -1 + 18 * sx, 1 - 440 * sy, sx, sy);

	computeFPS();
	drawFPS();
	SwapBuffers(getDevice());
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

	/* Create a texture that will be used to hold one "glyph" */
	GLuint tex;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(uniform_tex, 0);

	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* Set up the VBO for our vertex data */
	glEnableVertexAttribArray(attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* Loop through all characters */
	for (p = text; *p; p++) {
		/* Try to load and render the character */
		if (FT_Load_Char(titleFace, *p, FT_LOAD_RENDER))
			continue;

		/* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		/* Calculate the vertex and texture coordinates */
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

		/* Draw the character on the screen */
		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		/* Advance the cursor to the start of the next character */
		x += (g->advance.x >> 6) * sx;
		y += (g->advance.y >> 6) * sy;
	}

	glDisableVertexAttribArray(attribute_coord);
	glDeleteTextures(1, &tex);
}

