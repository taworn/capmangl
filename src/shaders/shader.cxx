/**
 * @file shader.cxx
 * @desc Shader program module.
 */
#include <windows.h>
#include <GL/glew.h>
#include "shader.hxx"

Shader::~Shader()
{
	if (program)
		glDeleteProgram(program);
}

Shader::Shader() : program(0)
{
}

bool Shader::init(const char *vertexSourceCode, const char *fragmentSourceCode)
{
	program = glCreateProgram();
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSourceCode);
	GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSourceCode);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glDetachShader(program, fragmentShader);
	glDetachShader(program, vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	if (!glIsProgram(program))
		return false;

	return true;
}

GLuint Shader::loadShader(GLenum shaderType, const char *sourceCode)
{
	GLint result = GL_FALSE;
	GLuint id = glCreateShader(shaderType);
	glShaderSource(id, 1, &sourceCode, NULL);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result) {
		glDeleteShader(id);
		return 0;
	}
	return id;
}

GLint Shader::getAttrib(const char *name)
{
	return glGetAttribLocation(program, name);
}

GLint Shader::getUniform(const char *name)
{
	return glGetUniformLocation(program, name);
}

