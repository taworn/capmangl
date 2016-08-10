/**
 * @file shader.hxx
 * @desc Shader program header.
 */
#ifndef SHADERS_SHADER_HXX
#define SHADERS_SHADER_HXX

/**
 * A shader program.
 */
class Shader
{
public:
	virtual ~Shader();
	Shader();

	GLuint getProgram() const { return program; }

	/**
	 * Uses this program.
	 */
	void useProgram() const { glUseProgram(program); }

	/**
	 * Unuses this program.
	 */
	void unuseProgram() const { glUseProgram(0); }

protected:
	/**
	 * Initializes shader programs.
	 * @param vertexSourceCode   A vertex shader source code.
	 * @param fragmentSourceCode A fragment shader source code.
	 * @return Returns true if successful, otherwise, it is false.
	 */
	bool init(const char *vertexSourceCode, const char *fragmentSourceCode);

	/**
	 * Loads shader program.
	 * @param shaderType A shader program type.
	 * @param sourceCode A shader source code.
	 */
	GLuint loadShader(GLenum shaderType, const char *sourceCode);

private:
	GLuint program;

	Shader(const Shader&);
	Shader& operator=(const Shader&);
};

#endif // SHADERS_SHADER_HXX

