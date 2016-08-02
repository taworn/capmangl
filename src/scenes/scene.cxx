#include <windows.h>
#include <assert.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <boost/log/trivial.hpp>
#include <GL/glew.h>
#include "scene.hxx"
#include "../game.hxx"

Scene::~Scene()
{
	fini();
}

Scene::Scene()
	: hdc(0), vertexShaderHandle(0), fragmentShaderHandle(0), programHandle(0), positionHandle(0), colorHandle(0), mvpMatrixHandle(0)
	, screenRect(), frameCount(0), fps(0), timeStart(0)
{
	Game *game = Game::instance();
	hdc = game->getDevice();

	int params[4];
	glGetIntegerv(GL_VIEWPORT, params);
	screenRect.left = params[0];
	screenRect.top = params[1];
	screenRect.right = screenRect.left + params[2];
	screenRect.bottom = screenRect.top + params[3];

	timeStart = GetTickCount();

	init();
}

void Scene::init()
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
	vertexShaderHandle = loadShader(GL_VERTEX_SHADER, vertexShader);
	fragmentShaderHandle = loadShader(GL_FRAGMENT_SHADER, fragmentShader);
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

void Scene::fini()
{
	glUseProgram(0);
	glDetachShader(programHandle, fragmentShaderHandle);
	glDetachShader(programHandle, vertexShaderHandle);
	glDeleteProgram(programHandle);
	glDeleteShader(fragmentShaderHandle);
	glDeleteShader(vertexShaderHandle);
}

GLuint Scene::loadShader(GLenum shaderType, const char *sourceCode)
{
	GLint result = GL_FALSE;
	GLuint id = glCreateShader(shaderType);
	glShaderSource(id, 1, &sourceCode, NULL);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	return result ? id : 0;
}

void Scene::computeFPS()
{
	frameCount++;
	ULONGLONG timeCurrent = GetTickCount();
	ULONGLONG timeUsage = timeCurrent - timeStart;
	if (timeUsage > 1000) {
		fps = frameCount * 1000 / timeUsage;
		timeStart = timeCurrent;
		frameCount = 0;
		BOOST_LOG_TRIVIAL(trace) << "FPS: " << fps;
		wchar_t buffer[64];
		wsprintf(buffer, L"FPS: %ld\n", fps);
		OutputDebugStringW(buffer);
	}
}

void Scene::drawFPS()
{
}

bool Scene::handleKey(HWND hwnd, WPARAM key)
{
	return false;
}

void Scene::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	computeFPS();
	drawFPS();
	SwapBuffers(getDevice());
}

