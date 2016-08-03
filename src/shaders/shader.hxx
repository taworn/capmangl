#ifndef SHADERS_SHADER_HXX
#define SHADERS_SHADER_HXX

class Shader
{
public:
	virtual ~Shader();
	Shader();

	GLuint getProgram() const { return program; }
	void useProgram() const { glUseProgram(program); }
	void unuseProgram() const { glUseProgram(0); }

protected:
	bool init(const char *vertexSourceCode, const char *fragmentSourceCode);

	GLuint loadShader(GLenum shaderType, const char *sourceCode);

	GLint getAttrib(const char *name);
	GLint getUniform(const char *name);

private:
	GLuint program;

	Shader(const Shader&);
	Shader& operator=(const Shader&);
};

#endif // SHADERS_SHADER_HXX

