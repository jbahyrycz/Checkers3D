#pragma once

#include <ctime>
#include <vector>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Mesh.h"
#include "Camera.h"
#include "Texture.h"
#include "Checkerboard.h"

class Gameplay
{
public:
	Gameplay();

	void CreateObjects();
	void LoadTextures();
	void DisplayGameplay(GLuint uniformModel);
	void KeyControl(bool* keys);

	void CalculateMove();
	void ChangeChosen();
	void ChangeActiveLeft();
	void ChangeActiveRight();
	void ChangeActiveUp();
	void ChangeActiveDown();

	Camera GetCamera() { return camera; };

	~Gameplay();

private:
	Camera camera;

	std::vector<Mesh*> meshList;

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
};

