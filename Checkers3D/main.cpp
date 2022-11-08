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

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera mainCamera;

Texture stoneTexture;
Texture checkerboardTexture;
Texture plainRedTexture;
Texture plainBrownTexture;
Texture plainWhiteTexture;
Texture plainPinkTexture;
Texture boardWoodTexture;
Texture blackSquareWoodTexture;
Texture whiteSquareWoodTexture;
Texture playerPieceTexture;
Texture opponentPieceTexture;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

static const char* vShader = "Shaders/shader.vert";
static const char* fShader = "Shaders/shader.frag";

Gameplay gameplay;

bool white = true;

void CreateObjects()
{
	unsigned int boardIndices[] = {
		0, 1, 4,
		1, 4, 5,
		1, 2, 6,
		1, 6, 5,
		0, 3, 7,
		0, 7, 4,
		3, 2, 6,
		3, 6, 7
	};

	GLfloat boardVertices[] = {
			-4.0f, -0.5f, 4.0f,			0.0f, 0.0f,
			4.0f, -0.5f, 4.0f,			3.0f, 0.0f,
			4.0f, -0.5f, -4.0f,			0.0f, 0.0f,
			-4.0f, -0.5f, -4.0f,		3.0f, 0.0f,
			-4.0f, 0.5f, 4.0f,			0.0f, 1.0f,
			4.0f, 0.5f, 4.0f,			3.0f, 1.0f,
			4.0f, 0.5f, -4.0f,			0.0f, 1.0f,
			-4.0f, 0.5f, -4.0f,			3.0f, 1.0f
	};

	unsigned int pieceIndices[] = {
		0, 1, 3,
		1, 2, 3,
		0, 5, 3,
		3, 4, 5,
		4, 5, 6,
		4, 6, 7,
		2, 7, 6,
		6, 1, 2,
		0, 1, 5,
		1, 5, 6,
		2, 3, 4,
		2, 4, 7
	};

	GLfloat pieceVertices[] = {
			-0.3f, -0.1f, -0.3f,	0.0f, 0.0f,
			0.3f, -0.1f, -0.3f,		0.0f, 0.0f,
			0.3f, 0.1f, -0.3f,		0.0f, 0.0f,
			-0.3f, 0.1f, -0.3f,		0.0f, 0.0f,
			-0.3f, 0.1f, 0.3f,		0.0f, 0.0f,
			-0.3f, -0.1f, 0.3f,		0.0f, 0.0f,
			0.3f, -0.1f, 0.3f,		0.0f, 0.0f,
			0.3f, 0.1f, 0.3f,		0.0f, 0.0f,
	};

	unsigned int promotedPieceIndices[] = {
		0, 1, 3,
		1, 2, 3,
		0, 5, 3,
		3, 4, 5,
		4, 5, 6,
		4, 6, 7,
		2, 7, 6,
		6, 1, 2,
		0, 1, 5,
		1, 5, 6,
		2, 3, 4,
		2, 4, 7,
		2, 3, 8,
		2, 7, 8,
		4, 7, 8,
		3, 4, 8
	};

	GLfloat promotedPieceVertices[] = {
			-0.3f, -0.1f, -0.3f,	0.0f, 0.0f, //0
			0.3f, -0.1f, -0.3f,		0.0f, 0.0f, //1
			0.3f, 0.1f, -0.3f,		0.0f, 0.0f, //2
			-0.3f, 0.1f, -0.3f,		0.0f, 0.0f, //3
			-0.3f, 0.1f, 0.3f,		0.0f, 0.0f, //4
			-0.3f, -0.1f, 0.3f,		0.0f, 0.0f, //5
			0.3f, -0.1f, 0.3f,		0.0f, 0.0f, //6
			0.3f, 0.1f, 0.3f,		0.0f, 0.0f, //7
			0.0f, 0.7f, 0.0f,		0.0f, 0.0f  //8
	};

	unsigned int squareIndices[] = {
		0, 1, 3,
		3, 2, 1
	};

	GLfloat squareVertices[] = {
			-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,
			0.5f, 0.0f, 0.5f,		1.0f, 0.0f,
			0.5f, 0.0f, -0.5f,		1.0f, 1.0f,
			-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,
	};

	Mesh* boardSides = new Mesh();
	boardSides->CreateMesh(boardVertices, boardIndices, 40, 24);
	meshList.push_back(boardSides);

	Mesh* piece = new Mesh();
	piece->CreateMesh(pieceVertices, pieceIndices, 40, 36);
	meshList.push_back(piece);

	Mesh* promotedPiece = new Mesh();
	promotedPiece->CreateMesh(promotedPieceVertices, promotedPieceIndices, 45, 48);
	meshList.push_back(promotedPiece);

	Mesh* square = new Mesh();
	square->CreateMesh(squareVertices, squareIndices, 20, 6);
	meshList.push_back(square);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void SetTextures()
{
	stoneTexture = Texture("Textures/stone.png");
	stoneTexture.LoadTexture();
	checkerboardTexture = Texture("Textures/checkerboard.png");
	checkerboardTexture.LoadTexture();
	plainRedTexture = Texture("Textures/plainRed.png");
	plainRedTexture.LoadTexture();
	plainBrownTexture = Texture("Textures/plainBrown.png");
	plainBrownTexture.LoadTexture();
	plainWhiteTexture = Texture("Textures/plainWhite.png");
	plainWhiteTexture.LoadTexture();
	plainPinkTexture = Texture("Textures/plainPink.png");
	plainPinkTexture.LoadTexture();
	boardWoodTexture = Texture("Textures/boardWood.png");
	boardWoodTexture.LoadTexture();
	blackSquareWoodTexture = Texture("Textures/blackSquareWood.jpg");
	blackSquareWoodTexture.LoadTexture();
	whiteSquareWoodTexture = Texture("Textures/whiteSquareWood.jpg");
	whiteSquareWoodTexture.LoadTexture();
}

void DisplayGameplay(glm::mat4 projection)
{
	// board

	glm::mat4 model(1.0f);

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(mainCamera.CalculateViewMatrix()));
	boardWoodTexture.UseTexture();
	meshList[0]->RenderMesh();

	// pieces

	for (int i = 0; i < 12; i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(gameplay.GetCheckerboard().GetPlayerPieces()[i].GetPosX(), gameplay.GetCheckerboard().GetPlayerPieces()[i].GetPosY(), gameplay.GetCheckerboard().GetPlayerPieces()[i].GetPosZ()));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		if (gameplay.GetCheckerboard().GetPlayerPieces()[i].GetChosen())
		{
			whiteSquareWoodTexture.UseTexture();
		}
		else
		{
			playerPieceTexture.UseTexture();
		}
		if (gameplay.GetCheckerboard().GetPlayerPieces()[i].GetPromoted())
		{
			meshList[2]->RenderMesh();
		}
		else
		{
			meshList[1]->RenderMesh();
		}

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(gameplay.GetCheckerboard().GetOpponentPieces()[i].GetPosX(), gameplay.GetCheckerboard().GetOpponentPieces()[i].GetPosY(), gameplay.GetCheckerboard().GetOpponentPieces()[i].GetPosZ()));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		if (gameplay.GetCheckerboard().GetOpponentPieces()[i].GetChosen())
		{
			whiteSquareWoodTexture.UseTexture();
		}
		else
		{
			opponentPieceTexture.UseTexture();
		}
		if (gameplay.GetCheckerboard().GetOpponentPieces()[i].GetPromoted())
		{
			meshList[2]->RenderMesh();
		}
		else
		{
			meshList[1]->RenderMesh();
		}
	}

	// squares

	for (int i = 0; i < 32; i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(gameplay.GetCheckerboard().GetBlackSquares()[i].GetPosX(), 0.5f, gameplay.GetCheckerboard().GetBlackSquares()[i].GetPosZ()));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		if (gameplay.GetCheckerboard().GetBlackSquares()[i].GetActive())
		{
			plainPinkTexture.UseTexture();
		}
		else
		{
			blackSquareWoodTexture.UseTexture();
		}
		meshList[3]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(gameplay.GetCheckerboard().GetWhiteSquares()[i].GetPosX(), 0.5f, gameplay.GetCheckerboard().GetWhiteSquares()[i].GetPosZ()));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		whiteSquareWoodTexture.UseTexture();
		meshList[3]->RenderMesh();
	}
}

int main()
{
	mainWindow = Window(1200, 900);
	mainWindow.Initialize();

	CreateObjects();
	CreateShaders();

	SetTextures();

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.0f);

	white = false;
	if (white)
	{
		mainCamera = Camera(glm::vec3(0.0f, 7.0f, 11.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -32.0f, 5.0f, 0.5f);
		gameplay = Gameplay(true);

		playerPieceTexture = plainRedTexture;
		playerPieceTexture.LoadTexture();

		opponentPieceTexture = plainBrownTexture;
		opponentPieceTexture.LoadTexture();
	}
	else
	{
		mainCamera = Camera(glm::vec3(0.0f, 7.0f, -11.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -32.0f, 5.0f, 0.5f);
		gameplay = Gameplay(false);

		playerPieceTexture = plainBrownTexture;
		playerPieceTexture.LoadTexture();

		opponentPieceTexture = plainRedTexture;
		opponentPieceTexture.LoadTexture();
	}

	while (!mainWindow.GetShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		glfwPollEvents();

		mainCamera.KeyControl(mainWindow.GetKeys(), deltaTime);
		//mainCamera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

		gameplay.KeyControl(mainWindow.GetKeys());

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

		DisplayGameplay(projection);

		glUseProgram(0);

		mainWindow.SwapBuffers();
	}

	return 0;
}