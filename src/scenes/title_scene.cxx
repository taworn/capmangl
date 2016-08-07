/**
 * @file title_scene.cxx
 * @desc Title scene module.
 */
#include <windows.h>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include "../game.hxx"
#include "scene.hxx"
#include "title_scene.hxx"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

TitleScene::~TitleScene()
{
	fini();
}

TitleScene::TitleScene() : Scene(), image()
{
	init();
}

void TitleScene::init()
{
	image.init("res\\a.png");
	titleFont = new Font("C:\\WINDOWS\\Fonts\\timesbd.ttf", 128);
}

void TitleScene::fini()
{
	delete titleFont;
}

bool TitleScene::handleKey(HWND hwnd, WPARAM key)
{
	if (key == VK_RETURN) {
		// ENTER
		OutputDebugStringW(L"ENTER keydown\n");
		Game::instance()->changeScene(SCENE_PLAY);
		return true;
	}
	return false;
}

void TitleScene::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	getTextShader()->useProgram();

	RECT rc = getScreenRect();
	float sx = 2.0f / (rc.right - rc.left);
	float sy = 2.0f / (rc.bottom - rc.top);
	float w, h;

	titleFont->setColor(1.0f, 1.0f, 0.5f, 1.0f);
	titleFont->measureText("Capman", &w, &h, sx, sy);
	titleFont->drawText("Capman", 0 - (w / 2), h, sx, sy);

	getNormalFont()->setColor(1.0f, 1.0f, 1.0f, 1.0f);
	getNormalFont()->measureText("Press ENTER to Start", &w, &h, sx, sy);
	getNormalFont()->drawText("Press ENTER to Start", 0 - (w / 2), -0.70f, sx, sy);


	getTextureShader()->useProgram();
	glEnable(GL_TEXTURE_2D);

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		 0.25f,  0.25f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // Top Right
		 0.25f, -0.25f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // Bottom Right
		-0.25f, -0.25f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // Bottom Left
		-0.25f,  0.25f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f  // Top Left 
	};
	GLuint indices[] = {  // Note that we start from 0!
	    0, 1, 3, // First Triangle
	    1, 2, 3  // Second Triangle
	};
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO


	// Load and create a texture 
	GLuint texture2;
	// ===================
	// Texture 2
	// ===================
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getImage());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Bind Textures using texture units
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture1);
	//glUniform1i(glGetUniformLocation(getTextureShader()->getProgram(), "ourTexture1"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(getTextureShader()->getProgram(), "ourTexture2"), 1);


	// translating
	glm::mat4x4 translateMatrix = glm::mat4(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(-0.5f, 0.7f, 0));

	// viewing
	glm::mat4x4 viewMatrix = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 1.5f),    // camera
		glm::vec3(0.0f, 0.0f, -15.0f),  // looks
		glm::vec3(0.0f, 1.0f, 0.0f)     // head is up
	);

	// projecting
	//glm::mat4x4 projectionMatrix = glm::perspective(45.0f, 1.3333f, 1.0f, 25.0f);
	glm::mat4x4 projectionMatrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 25.0f);

	// combines model, view and projection matrices
	glm::mat4x4 mvpMatrix = projectionMatrix * viewMatrix * translateMatrix;

	glUniformMatrix4fv(getTextureShader()->getMVPMatrix(), 1, false, &mvpMatrix[0][0]);

	// Draw container
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);



	computeFPS();
	SwapBuffers(getDevice());
}

