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
	void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	{
		this->rgba[0] = r;
		this->rgba[1] = g;
		this->rgba[2] = b;
		this->rgba[3] = a;
	}

	/**
	 * Sets color to next draw text.
	 */
	void setColor(GLfloat rgba[4])
	{
		this->rgba[0] = rgba[0];
		this->rgba[1] = rgba[1];
		this->rgba[2] = rgba[2];
		this->rgba[3] = rgba[3];
	}

	/**
	 * Measures width and height of text.
	 */
	void measure(const char *text, GLfloat *w, GLfloat *h, GLfloat sx, GLfloat sy);

	/**
	 * Draws text.
	 */
	void draw(const char *text, GLfloat x, GLfloat y, GLfloat sx, GLfloat sy);

private:
	GLuint handle;
	FT_Face face;
	GLfloat rgba[4];

	Font(const Font&);
	Font& operator=(const Font&);
};

#endif // FONT_HXX

