#include "Settings.h"

const char* Settings::vShader = "Shaders/shaderMenu.vert";
const char* Settings::fShader = "Shaders/shaderMenu.frag";

Settings::Settings(Window* window, unsigned int* n, unsigned int* color)
{
	windowPtr = window;
	nPtr = n;
	*nPtr = 0;

	colorPtr = color;

	uniformProjection = 0;
	uniformModel = 0;

	settingsShouldClose = false;
}

void Settings::Run()
{
	CreateObjects();
	CreateShaders();
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (GLfloat)windowPtr->GetBufferWidth() / windowPtr->GetBufferHeight(), 0.1f, 100.0f);
	LoadTextures();

	while (!settingsShouldClose && !windowPtr->GetShouldClose())
	{
		glfwPollEvents();
		KeyControl(windowPtr->GetKeys());
		RenderPass(projectionMatrix);
		DisplaySettings(uniformModel);

		glUseProgram(0);
		windowPtr->SwapBuffers();
	}
}

void Settings::CreateObjects()
{
	unsigned int buttonIndices[] = {
		0, 1, 2,
		2, 3, 1
	};

	GLfloat buttonVertices[] = {
			-4.0f, -1.5f, -1.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
			4.0f, -1.5f, -1.0f,			1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
			-4.0f, 1.5f, -1.0f,			0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			4.0f, 1.5f, -1.0f,			1.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};

	Mesh* button = new Mesh();
	button->CreateMesh(buttonVertices, buttonIndices, 32, 6);
	meshList.push_back(button);
}

void Settings::CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void Settings::RenderPass(glm::mat4 projectionMatrix)
{
	shaderList[0].UseShader();
	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();

	glViewport(0, 0, 1200, 900);

	// Clear the window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void Settings::LoadTextures()
{
	settingsTexture = Texture("Textures/menuSettings.png");
	settingsTexture.LoadTexture();

	settingsBlackAndWhiteTexture = Texture("Textures/settingsBlackAndWhite.png");
	settingsBlackAndWhiteTexture.LoadTexture();

	settingsBlackAndWhiteSelectedTexture = Texture("Textures/settingsBlackAndWhiteSelected.png");
	settingsBlackAndWhiteSelectedTexture.LoadTexture();

	settingsBrownAndRedTexture = Texture("Textures/settingsBrownAndRed.png");
	settingsBrownAndRedTexture.LoadTexture();

	settingsBrownAndRedSelectedTexture = Texture("Textures/settingsBrownAndRedSelected.png");
	settingsBrownAndRedSelectedTexture.LoadTexture();

	settingsExitTexture = Texture("Textures/menuExit.png");
	settingsExitTexture.LoadTexture();

	settingsExitSelectedTexture = Texture("Textures/menuExitSelected.png");
	settingsExitSelectedTexture.LoadTexture();
}

void Settings::DisplaySettings(GLuint uniformModel)
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.3f, 0.0f));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	settingsTexture.UseTexture();
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.1f, 0.0f));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	if (*nPtr == 0)
	{
		settingsBlackAndWhiteSelectedTexture.UseTexture();
	}
	else
	{
		settingsBlackAndWhiteTexture.UseTexture();
	}
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.0f));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	if (*nPtr == 1)
	{
		settingsBrownAndRedSelectedTexture.UseTexture();
	}
	else
	{
		settingsBrownAndRedTexture.UseTexture();
	}
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -0.3f, 0.0f));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	if (*nPtr == 2)
	{
		settingsExitSelectedTexture.UseTexture();
	}
	else
	{
		settingsExitTexture.UseTexture();
	}
	meshList[0]->RenderMesh();
}

void Settings::KeyControl(bool* keys)
{
	if (keys[GLFW_KEY_ENTER])
	{
		if (*nPtr == 2)
		{
			*nPtr = 0;
			settingsShouldClose = true;
		}
		else
		{
			*colorPtr = *nPtr;
		}

		keys[GLFW_KEY_ENTER] = false;
	}
	if (keys[GLFW_KEY_ESCAPE])
	{
		*nPtr = 0;
		settingsShouldClose = true;
		
		keys[GLFW_KEY_ESCAPE] = false;
	}
	if (keys[GLFW_KEY_UP] && *nPtr > 0)
	{
		*nPtr -= 1;

		keys[GLFW_KEY_UP] = false;
	}
	if (keys[GLFW_KEY_DOWN] && *nPtr < 2)
	{
		*nPtr += 1;

		keys[GLFW_KEY_DOWN] = false;
	}
}

Settings::~Settings()
{

}