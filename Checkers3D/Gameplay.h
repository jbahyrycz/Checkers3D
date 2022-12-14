#pragma once

#include <ctime>
#include <vector>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Checkerboard.h"

class Gameplay
{
public:
	Gameplay(Window* widndow, unsigned int* n);

	void Run();

	void CreateObjects();
	void CreateShaders();
	void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
	void LoadTextures();
	void DisplayGameplay(GLuint uniformModel);
	void KeyControl(bool* keys);

	void CalculateMove();
	void ChangeChosen();
	void ChangeActiveLeft();
	void ChangeActiveRight();
	void ChangeActiveUp();
	void ChangeActiveDown();

	~Gameplay();

private:
	Window* windowPtr;
	unsigned int* nPtr;

	Camera camera;

	GLuint uniformProjection;
	GLuint uniformModel;
	GLuint uniformView;

	std::vector<Mesh*> meshList;
	std::vector<Shader> shaderList;

	static const char* vShader;
	static const char* fShader;

	Texture playerTexture;
	Texture opponentTexture;
	Texture whiteSquareTexture;
	Texture blackSquareTexture;
	Texture boardTexture;
	Texture activeSquareTexture;

	Checkerboard checkerboard;

	unsigned int activeSquareIndex;
	unsigned int chosenPieceIndex;

	bool white;
	bool gameplayShouldClose;
};
