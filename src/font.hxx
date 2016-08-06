/**
 * @file font.hxx
 * @desc Font header.
 */
#ifndef FONT_HXX
#define FONT_HXX

class TextShader;

/**
 * A font and draw text class.
 */
class Font
{
public:
	struct Point {
		GLfloat x;
		GLfloat y;
		GLfloat s;
		GLfloat t;
	};

	/**
	 * Destructs a font.
	 */
	~Font();

	/**
	 * Constructs a font.
	 */
	Font();

	/**
	 * Constructs a font with face and size.
	 */
	Font(const char *faceName, int size);

	/**
	 * Initializes a font with face and size.
	 */
	bool init(const char *faceName, int size);

	/**
	 * Sets color to next draw text.
	 */
	void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

	/**
	 * Sets color to next draw text.
	 */
	void setColor(GLfloat rgba[4]);

	/**
	 * Measures width and height of text.
	 */
	void measureText(const char *text, GLfloat *w, GLfloat *h, GLfloat sx, GLfloat sy);

	/**
	 * Draws text.
	 */
	void drawText(const char *text, GLfloat x, GLfloat y, GLfloat sx, GLfloat sy);

private:
	TextShader *shader;
	GLuint vbo;
	FT_Face face;

	Font(const Font&);
	Font& operator=(const Font&);
};

#endif // FONT_HXX

