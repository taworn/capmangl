#ifndef SHADERS_TEXTURE_SHADER_HXX
#define SHADERS_TEXTURE_SHADER_HXX

class TextureShader : public Shader
{
public:
	TextureShader() : position(-1), texcoord(-1)
	{
		const char vertexSourceCode[] = ""
			"attribute vec3 aVertexPosition;\n"
			"attribute vec2 aTextureCoord;\n"
			"uniform mat4 uMVPMatrix;\n"
			"varying vec2 vTextureCoord;\n"
			"void main(void) {\n"
			"  gl_Position = uMVPMatrix * vec4(aVertexPosition, 1.0);\n"
			"  //gl_Position = vec4(aVertexPosition, 1.0);\n"
			"  vTextureCoord = aTextureCoord;\n"
			"}\n";
		const char fragmentSourceCode[] = ""
			"precision mediump float;\n"
			"varying vec2 vTextureCoord;\n"
			"uniform sampler2D uSampler;\n"
			"void main(void) {\n"
			"  vec4 textureColor = texture2D(uSampler, vTextureCoord);\n"
			"  gl_FragColor = vec4(textureColor.rgb, textureColor.a);\n"
			"  //if (gl_FragColor < 0.5) discard;\n"
			"}\n";
		if (init(vertexSourceCode, fragmentSourceCode)) {
			position = getAttrib("aVertexPosition");
			texcoord = getAttrib("aTextureCoord");
			mvpMatrix = getUniform("uMVPMatrix");
		}
	}

	GLint getPosition() const { return position; }
	GLint getTexcoord() const { return texcoord; }
	GLint getMVPMatrix() const { return mvpMatrix; }

private:
	GLint position;
	GLint texcoord;
	GLint mvpMatrix;
};

#endif // SHADERS_TEXTURE_SHADER_HXX

