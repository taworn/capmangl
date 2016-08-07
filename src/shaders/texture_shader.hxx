#ifndef SHADERS_TEXTURE_SHADER_HXX
#define SHADERS_TEXTURE_SHADER_HXX

class TextureShader : public Shader
{
public:
	TextureShader() : position(-1), color(-1), texcoord(-1)
	{
		const char vertexSourceCode[] = ""
			"layout (location = 0) in vec3 position;\n"
			"layout (location = 1) in vec3 color;\n"
			"layout (location = 2) in vec2 texCoord;\n"
			"uniform mat4 mvp;"
			"out vec3 ourColor;\n"
			"out vec2 TexCoord;\n"
			"void main() {\n"
			"  gl_Position = mvp * vec4(position, 1.0f);\n"
			"  ourColor = color;\n"
			"  TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);\n"
			"}\n";
		const char fragmentSourceCode[] = ""
			"in vec3 ourColor;\n"
			"in vec2 TexCoord;\n"
			"out vec4 color;\n"
			"uniform sampler2D ourTexture1;\n"
			"uniform sampler2D ourTexture2;\n"
			"void main() {\n"
			"  color = texture(ourTexture2, TexCoord);\n"
			"}\n";
		if (init(vertexSourceCode, fragmentSourceCode)) {
			position = getAttrib("position");
			color = getAttrib("color");
			texcoord = getAttrib("texcoord");
			mvpMatrix = getUniform("mvp");
		}
	}

	GLint getPosition() const { return position; }
	GLint getColor() const { return color; }
	GLint getTexcoord() const { return texcoord; }
	GLint getMVPMatrix() const { return mvpMatrix; }

private:
	GLint position;
	GLint color;
	GLint texcoord;
	GLint mvpMatrix;
};

#endif // SHADERS_TEXTURE_SHADER_HXX

