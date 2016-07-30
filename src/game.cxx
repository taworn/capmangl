#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "game.hxx"
#include "scenes/scene.hxx"
#include "scenes/title_scene.hxx"

Game *Game::singleton = NULL;

Game::~Game()
{
	delete scene;
	singleton = NULL;
}

Game::Game(HDC h) : hdc(h)
{
	assert(singleton == NULL);
	singleton = this;
	init();
	scene = new TitleScene();
}

void Game::changeScene(Scene *newScene)
{
	delete scene;
	scene = newScene;
}

static GLuint loadShader(GLenum shaderType, const char *sourceCode)
{
	GLint result = GL_FALSE;
	GLuint id = glCreateShader(shaderType);
	glShaderSource(id, 1, &sourceCode, NULL);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	return result ? id : 0;
}

void Game::init()
{
	// vertex shader and fragment shader
	const char vertexShader[] = ""
		"uniform mat4 u_MVPMatrix;  \n"  // A constant representing the combined model/view/projection matrix.
		"attribute vec4 a_Position; \n"  // Per-vertex position information we will pass in.
		"attribute vec4 a_Color;    \n"  // Per-vertex color information we will pass in.
		"varying vec4 v_Color;      \n"  // This will be passed into the fragment shader.
		"void main() {              \n"  //
		"  v_Color = a_Color;       \n"  // Pass the color through to the fragment shader.  It will be interpolated across the triangle.
		"  gl_Position = u_MVPMatrix\n"  // gl_Position is a special variable used to store the final position.
		"              * a_Position;\n"  // Multiply the vertex by the matrix to get the final point in normalized screen coordinates.
		"}                          \n";
	const char fragmentShader[] = ""
		"precision mediump float; \n"  // Set the default precision to medium.  We don't need as high of a precision in the fragment shader.
		"varying vec4 v_Color;    \n"  // This is the color from the vertex shader interpolated across the triangle per fragment.
		"void main() {            \n"  //
		"  gl_FragColor = v_Color;\n"  // Pass the color directly through the pipeline.
		"}                        \n";
	GLuint vertexShaderHandle = loadShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fragmentShaderHandle = loadShader(GL_FRAGMENT_SHADER, fragmentShader);
	programHandle = glCreateProgram();
	if (programHandle != 0) {
		glAttachShader(programHandle, vertexShaderHandle);
		glAttachShader(programHandle, fragmentShaderHandle);
		glBindAttribLocation(programHandle, 0, "a_Position");
		glBindAttribLocation(programHandle, 1, "a_Color");
		glLinkProgram(programHandle);
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == 0) {
			glDeleteProgram(programHandle);
			programHandle = 0;
		}
	}
	mvpMatrixHandle = glGetUniformLocation(programHandle, "u_MVPMatrix");
	positionHandle = glGetAttribLocation(programHandle, "a_Position");
	colorHandle = glGetAttribLocation(programHandle, "a_Color");
	glUseProgram(programHandle);
}

