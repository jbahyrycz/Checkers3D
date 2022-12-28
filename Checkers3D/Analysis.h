#pragma once

#include <iostream>
#include <fstream>

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
#include "Material.h"
#include "PointLight.h"
#include "Checkerboard.h"

class Analysis
{
public:
	Analysis(Window* widndow, unsigned int* n, unsigned int style); // style: 0 - black/white, 1 - brown/red

	void Run();

	void CalcAverageNormals(unsigned int* indices, unsigned int indicesCount, GLfloat* vertices, unsigned int verticesCount, unsigned int vLength, unsigned int normalOffset);
	void CreateObjects();
	void CreateShaders();
	void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
	void LoadTextures();
	void DisplayAnalysis(GLuint uniformModel);
	void KeyControl(bool* keys);

	~Analysis();

private:
	Window* windowPtr;
	unsigned int* nPtr;

	std::vector<Mesh*> meshList;
	std::vector<Shader> shaderList;

	std::vector<unsigned int> moveList;

	Camera camera;

	const char* whitePieceTextureLoc;
	const char* blackPieceTextureLoc;
	const char* whiteSquareTextureLoc;
	const char* blackSquareTextureLoc;

	Texture playerTexture;
	Texture opponentTexture;
	Texture whiteSquareTexture;
	Texture blackSquareTexture;
	Texture boardTexture;

	GLuint uniformProjection;
	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformEyePosition;
	GLuint uniformSpecularIntensity;
	GLuint uniformShininess;

	Material shinyMaterial;
	Material dullMaterial;

	PointLight light;

	static const char* vShader;
	static const char* fShader;

	Checkerboard checkerboard;

	int indexCounter;
	unsigned int go; // 1 - ruch bialych, 0 - ruch czarnych

	bool white;
	bool analysisShouldClose;
};

