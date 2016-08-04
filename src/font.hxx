#ifndef FONT_HXX
#define FONT_HXX

class TextShader;

class Font
{
public:
	struct Point {
		GLfloat x;
		GLfloat y;
		GLfloat s;
		GLfloat t;
	};

	~Font();
	Font();

	bool load(const char *faceName, int size);

	void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void setColor(GLfloat rgba[4]);

	void measureText(const char *text, GLfloat *w, GLfloat *h, GLfloat sx, GLfloat sy);
	void drawText(const char *text, GLfloat x, GLfloat y, GLfloat sx, GLfloat sy);

private:
	TextShader *shader;
	FT_Face face;
	GLuint vbo;

	Font(const Font&);
	Font& operator=(const Font&);
};

#endif // FONT_HXX

