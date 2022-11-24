#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Square.h"
#include "Piece.h"
#include "Checkerboard.h"
#include "Gameplay.h"

class App
{
public:
	App() 
	{
		mainWindow = Window(1200, 900);

		uniformProjection = 0;
		uniformModel = 0;
		uniformView = 0;
	}

	void Run()
	{
		mainWindow.Initialize();

		gameplay.CreateObjects();
		CreateShaders();

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.0f);

		gameplay.LoadTextures();

		while (!mainWindow.GetShouldClose())
		{
			glfwPollEvents();

			gameplay.KeyControl(mainWindow.GetKeys());

			glm::mat4 viewMatrix = gameplay.GetCamera().CalculateViewMatrix();
			RenderPass(projectionMatrix, viewMatrix);
			gameplay.DisplayGameplay(uniformModel);

			glUseProgram(0);

			mainWindow.SwapBuffers();
		}
	}

	~App() {}

private:
	GLuint uniformProjection;
	GLuint uniformModel; 
	GLuint uniformView;

	Window mainWindow;
	std::vector<Shader> shaderList;

	static const char* vShader;
	static const char* fShader;

	Gameplay gameplay;

	bool white = true;

	void CreateShaders()
	{
		Shader* shader1 = new Shader();
		shader1->CreateFromFiles(vShader, fShader);
		shaderList.push_back(*shader1);
	}

	void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
	{
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

		glViewport(0, 0, 1200, 900);

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	}
};

const char* App::vShader = "Shaders/shader.vert";
const char* App::fShader = "Shaders/shader.frag";

int main()
{
	App* app = new App;

	app->Run();

	delete app;

	return 0;
}