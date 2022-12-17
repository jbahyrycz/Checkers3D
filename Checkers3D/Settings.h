#pragma once

#include <vector>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

class Settings
{
public:
	Settings(Window* mainWindow, unsigned int* n, unsigned int* color);

	void Run();

	void CreateObjects();
	void CreateShaders();
	void RenderPass(glm::mat4 projectionMatrix);
	void LoadTextures();
	void DisplaySettings(GLuint uniformModel);
	void KeyControl(bool* keys);

	~Settings();

private:
	Window* windowPtr;
	unsigned int* nPtr;
	unsigned int* colorPtr;

	GLuint uniformProjection;
	GLuint uniformModel;

	std::vector<Mesh*> meshList;
	std::vector<Shader> shaderList;

	static const char* vShader;
	static const char* fShader;

	Texture settingsTexture;
	Texture settingsBlackAndWhiteTexture;
	Texture settingsBlackAndWhiteSelectedTexture;
	Texture settingsBrownAndRedTexture;
	Texture settingsBrownAndRedSelectedTexture;
	Texture settingsExitTexture;
	Texture settingsExitSelectedTexture;

	bool settingsShouldClose;
};