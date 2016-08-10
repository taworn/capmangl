#ifndef SHADERS_TEXT_SHADER_HXX
#define SHADERS_TEXT_SHADER_HXX

class TextShader : public Shader
{
public:
	TextShader() : coord(-1), color(-1), tex(-1)
	{
		const char vertexSourceCode[] = ""
			"attribute vec4 coord;\n"
			"varying vec2 texpos;\n"
			"void main() {\n"
			"  gl_Position = vec4(coord.xy, 0, 1);\n"
			"  texpos = coord.zw;\n"
			"}\n";
		const char fragmentSourceCode[] = ""
			"varying vec2 texpos;\n"
			"uniform sampler2D tex;\n"
			"uniform vec4 color;\n"
			"void main() {\n"
			"  gl_FragColor = vec4(1, 1, 1, texture2D(tex, texpos).a) * color;\n"
			"}\n";
		if (init(vertexSourceCode, fragmentSourceCode)) {
			coord = glGetAttribLocation(getProgram(), "coord");
			color = glGetUniformLocation(getProgram(), "color");
			tex = glGetUniformLocation(getProgram(), "tex");
		}
	}

	GLint getCoord() const { return coord; }
	GLint getColor() const { return color; }
	GLint getTex() const { return tex; }

private:
	GLint coord;
	GLint color;
	GLint tex;
};

#endif // SHADERS_TEXT_SHADER_HXX

