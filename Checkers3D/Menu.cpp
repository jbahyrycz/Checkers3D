#include "Menu.h"

const char* Menu::vShader = "Shaders/shaderMenu.vert";
const char* Menu::fShader = "Shaders/shaderMenu.frag";

Menu::Menu(Window* window, unsigned int* n)
{
	windowPtr = window;
	nPtr = n;
	*nPtr = 1;

	uniformProjection = 0;
	uniformModel = 0;

	menuShouldClose = false;
}

void Menu::Run()
{
	CreateObjects();
	CreateShaders();
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (GLfloat)windowPtr->GetBufferWidth() / windowPtr->GetBufferHeight(), 0.1f, 100.0f);
	LoadTextures();

	while (!menuShouldClose && !windowPtr->GetShouldClose())
	{
		glfwPollEvents();
		KeyControl(windowPtr->GetKeys());
		RenderPass(projectionMatrix);
		DisplayMenu(uniformModel);

		glUseProgram(0);
		windowPtr->SwapBuffers();
	}
}

void Menu::CreateObjects()
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

void Menu::CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void Menu::RenderPass(glm::mat4 projectionMatrix)
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

void Menu::LoadTextures()
{
	menuTexture = Texture("Textures/menu.png");
	menuTexture.LoadTexture();

	menuSelectedTexture = Texture("Textures/menuSelected.png");
	menuSelectedTexture.LoadTexture();

	menuStartTexture = Texture("Textures/menuStart.png");
	menuStartTexture.LoadTexture();

	menuStartSelectedTexture = Texture("Textures/menuStartSelected.png");
	menuStartSelectedTexture.LoadTexture();

	menuAnalysisTexture = Texture("Textures/menuAnalysis.png");
	menuAnalysisTexture.LoadTexture();

	menuAnalysisSelectedTexture = Texture("Textures/menuAnalysisSelected.png");
	menuAnalysisSelectedTexture.LoadTexture();

	menuSettingsTexture = Texture("Textures/menuSettings.png");
	menuSettingsTexture.LoadTexture();

	menuSettingsSelectedTexture = Texture("Textures/menuSettingsSelected.png");
	menuSettingsSelectedTexture.LoadTexture();

	menuExitTexture = Texture("Textures/menuExit.png");
	menuExitTexture.LoadTexture();

	menuExitSelectedTexture = Texture("Textures/menuExitSelected.png");
	menuExitSelectedTexture.LoadTexture();
}

void Menu::DisplayMenu(GLuint uniformModel)
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.3f, 0.0f));
	model = glm::scale(model, glm::vec3(0.04f, 0.04f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	menuTexture.UseTexture();
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.15f, 0.0f));
	model = glm::scale(model, glm::vec3(0.04f, 0.04f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	if (*nPtr == 1)
	{
		menuStartSelectedTexture.UseTexture();
	}
	else
	{
		menuStartTexture.UseTexture();
	}
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.04f, 0.04f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	if (*nPtr == 2)
	{
		menuAnalysisSelectedTexture.UseTexture();
	}
	else
	{
		menuAnalysisTexture.UseTexture();
	}
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -0.15f, 0.0f));
	model = glm::scale(model, glm::vec3(0.04f, 0.04f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	if (*nPtr == 3)
	{
		menuSettingsSelectedTexture.UseTexture();
	}
	else
	{
		menuSettingsTexture.UseTexture();
	}
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -0.3f, 0.0f));
	model = glm::scale(model, glm::vec3(0.04f, 0.04f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	if (*nPtr == 4)
	{
		menuExitSelectedTexture.UseTexture();
	}
	else
	{
		menuExitTexture.UseTexture();
	}
	meshList[0]->RenderMesh();
}

void Menu::KeyControl(bool* keys)
{
	if (keys[GLFW_KEY_ENTER])
	{
		menuShouldClose = true;

		keys[GLFW_KEY_ENTER] = false;
	}
	if (keys[GLFW_KEY_ESCAPE])
	{
		*nPtr = 4;
		menuShouldClose = true;

		keys[GLFW_KEY_ESCAPE] = false;
	}
	if (keys[GLFW_KEY_UP] && *nPtr > 1)
	{
		*nPtr -= 1;

		keys[GLFW_KEY_UP] = false;
	}
	if (keys[GLFW_KEY_DOWN] && *nPtr < 4)
	{
		*nPtr += 1;

		keys[GLFW_KEY_DOWN] = false;
	}
}

Menu::~Menu()
{

}