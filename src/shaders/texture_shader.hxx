#ifndef SHADERS_TEXTURE_SHADER_HXX
#define SHADERS_TEXTURE_SHADER_HXX

class TextureShader : public Shader
{
public:
	TextureShader() : position(-1), coord(-1), mvpMatrix(-1), sampler(-1)
	{
		const char vertexSourceCode[] = ""
			"attribute vec3 aVertexPosition;\n"
			"attribute vec2 aTextureCoord;\n"
			"uniform mat4 uMVPMatrix;\n"
			"varying vec2 vTextureCoord;\n"
			"void main(void) {\n"
			"  gl_Position = uMVPMatrix * vec4(aVertexPosition, 1.0);\n"
			"  vTextureCoord = aTextureCoord;\n"
			"}\n";
		const char fragmentSourceCode[] = ""
			"precision mediump float;\n"
			"varying vec2 vTextureCoord;\n"
			"uniform sampler2D uSampler;\n"
			"void main(void) {\n"
			"  vec4 textureColor = texture2D(uSampler, vTextureCoord);\n"
			"  gl_FragColor = vec4(textureColor.rgb, textureColor.a);\n"
			"}\n";
		if (init(vertexSourceCode, fragmentSourceCode)) {
			position = glGetAttribLocation(getProgram(), "aVertexPosition");
			coord = glGetAttribLocation(getProgram(), "aTextureCoord");
			mvpMatrix = glGetUniformLocation(getProgram(), "uMVPMatrix");
			sampler = glGetUniformLocation(getProgram(), "uSampler");
		}
	}

	GLint getPosition() const { return position; }
	GLint getCoord() const { return coord; }
	GLint getMVPMatrix() const { return mvpMatrix; }
	GLint getSampler() const { return sampler; }

private:
	GLint position;
	GLint coord;
	GLint mvpMatrix;
	GLint sampler;
};

#endif // SHADERS_TEXTURE_SHADER_HXX

