#pragma once

#include <vector>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

class Menu
{
public:
	Menu(Window* mainWindow, unsigned int* nPtr);

	void Run();

	void CreateObjects();
	void CreateShaders();
	void RenderPass(glm::mat4 projectionMatrix);
	void LoadTextures();
	void DisplayMenu(GLuint uniformModel);
	void KeyControl(bool* keys);

	~Menu();

private:
	Window* windowPtr;
	unsigned int* nPtr;

	GLuint uniformProjection;
	GLuint uniformModel;

	std::vector<Mesh*> meshList;
	std::vector<Shader> shaderList;

	static const char* vShader;
	static const char* fShader;

	Texture menuTexture;
	Texture menuSelectedTexture;
	Texture menuStartTexture;
	Texture menuStartSelectedTexture;
	Texture menuSettingsTexture;
	Texture menuSettingsSelectedTexture;
	Texture menuExitTexture;
	Texture menuExitSelectedTexture;

	bool menuShouldClose;
};