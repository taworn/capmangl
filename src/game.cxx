#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "game.hxx"

static GLuint LoadShader(GLenum shaderType, const char *sourceCode)
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
	timeStart = GetTickCount();
	frameCount = 0;

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
	GLuint vertexShaderHandle = LoadShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fragmentShaderHandle = LoadShader(GL_FRAGMENT_SHADER, fragmentShader);
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

	// generates buffer
	const GLfloat verticesData[] = {
		// X, Y, Z, R, G, B, A
		-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	};
	glGenBuffers(1, &verticesId);
	glBindBuffer(GL_ARRAY_BUFFER, verticesId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);

	// model matrix
	rotateMatrix = glm::mat4(1.0f);

	// view matrix
	viewMatrix = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 1.5f),    // camera
		glm::vec3(0.0f, 0.0f, -15.0f),  // looks
		glm::vec3(0.0f, 1.0f, 0.0f)     // head is up
	);

	// projection matrix
	//projectionMatrix = glm::perspective(45.0f, 1.3333f, 1.0f, 25.0f);
	projectionMatrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 25.0f);

	modelX = 0.0f;
	modelY = 0.0f;
	modelDx = 0.0f;
	modelDy = 0.0f;
	angle = 0.0f;
	angleToPlus = 0.05f;
	lastTick = GetTickCount();
}

void Game::fini()
{
}

void Game::fps()
{
	frameCount++;
	ULONGLONG timeUsage = GetTickCount() - timeStart;
	if (timeUsage > 1000) {
		ULONGLONG fps = frameCount * 1000 / timeUsage;
		timeStart = GetTickCount();
		frameCount = 0;
		BOOST_LOG_TRIVIAL(trace) << "FPS: " << fps;
		wchar_t buffer[64];
		wsprintf(buffer, L"FPS: %ld\n", fps);
		OutputDebugStringW(buffer);
	}
}

bool Game::handleKey(HWND hwnd, WPARAM key)
{
	if (key == VK_SPACE) {
		// space
		OutputDebugStringA("SPACE keydown\n");
		return true;
	}
	else if (key == VK_RETURN) {
		// space
		OutputDebugStringA("ENTER keydown\n");
		return true;
	}
	else if (key == 0x57 || key == VK_UP) {
		// up
		OutputDebugStringA("W -or- UP keydown\n");
		modelDy = -0.1f;
		return true;
	}
	else if (key == 0x53 || key == VK_DOWN) {
		// down
		OutputDebugStringA("S -or- DOWN keydown\n");
		modelDy = 0.1f;
		return true;
	}
	else if (key == 0x41 || key == VK_LEFT) {
		// left
		OutputDebugStringA("A -or- LEFT keydown\n");
		modelDx = -0.1f;
		return true;
	}
	else if (key == 0x44 || key == VK_RIGHT) {
		// right
		OutputDebugStringA("D -or- RIGHT keydown\n");
		modelDx = 0.1f;
		return true;
	}
	return false;
}

void Game::render()
{
	if (GetTickCount() - lastTick > 1) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		translateMatrix = glm::mat4(1.0f);
		translateMatrix = glm::translate(translateMatrix, glm::vec3(modelX + modelDx, modelY + modelDy, 0));
		modelX += modelDx;
		modelY += modelDy;
		modelDx = 0.0f;
		modelDy = 0.0f;

		rotateMatrix = glm::mat4(1.0f);
		rotateMatrix = glm::rotate(rotateMatrix, angle, glm::vec3(0, 1, 0));
		angle += angleToPlus;
		if (angle > 89.0f || angle < -89.0f)
			angleToPlus = -angleToPlus;

		// combines model, view and projection matrices
		mvpMatrix = projectionMatrix * viewMatrix * rotateMatrix * translateMatrix;

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, verticesId);

		// passes in the position information
		glVertexAttribPointer(positionHandle, 3, GL_FLOAT, false, 7 * 4, (void*)(0 * 4));
		glEnableVertexAttribArray(positionHandle);

		// passes in the color information
		glVertexAttribPointer(colorHandle, 4, GL_FLOAT, false, 7 * 4, (void*)(3 * 4));
		glEnableVertexAttribArray(colorHandle);

		glUniformMatrix4fv(mvpMatrixHandle, 1, false, &mvpMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);

		SwapBuffers(hdc);
		lastTick = GetTickCount();

		fps();
	}
}

