#include "Gameplay.h"

Gameplay::Gameplay()
{
	srand(time(NULL)); //wlaczenie losowosci

	white = (std::rand() % 2);

	checkerboard = Checkerboard(white);
	chosenPieceIndex = 0;

	if (white)
	{
		camera = Camera(glm::vec3(0.0f, 7.0f, 11.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -32.0f);

		playerTexture = Texture("Textures/plainRed.png");
		opponentTexture = Texture("Textures/plainBrown.png");

		activeSquareIndex = 0;
	}
	else
	{
		camera = Camera(glm::vec3(0.0f, 7.0f, -11.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -32.0f);

		playerTexture = Texture("Textures/plainBrown.png");
		opponentTexture = Texture("Textures/plainRed.png");

		activeSquareIndex = 31;
	}

	whiteSquareTexture = Texture("Textures/whiteSquareWood.jpg");
	blackSquareTexture = Texture("Textures/blackSquareWood.jpg");
	boardTexture = Texture("Textures/boardWood.png");
	activeSquareTexture = Texture("Textures/plainPink.png");

	checkerboard.blackSquares[activeSquareIndex].active = true;
}

void Gameplay::CreateObjects()
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

void Gameplay::LoadTextures()
{
	playerTexture.LoadTexture();
	opponentTexture.LoadTexture();
	whiteSquareTexture.LoadTexture();
	blackSquareTexture.LoadTexture();
	boardTexture.LoadTexture();
	activeSquareTexture.LoadTexture();
}

void Gameplay::DisplayGameplay(GLuint uniformModel)
{
	// board

	glm::mat4 model(1.0f);

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	boardTexture.UseTexture();
	meshList[0]->RenderMesh();

	// pieces

	for (int i = 0; i < 12; i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(checkerboard.playerPieces[i].posX, checkerboard.playerPieces[i].posY, checkerboard.playerPieces[i].posZ));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		if (checkerboard.playerPieces[i].chosen)
		{
			whiteSquareTexture.UseTexture();
		}
		else
		{
			playerTexture.UseTexture();
		}
		if (checkerboard.playerPieces[i].promoted)
		{
			meshList[2]->RenderMesh();
		}
		else
		{
			meshList[1]->RenderMesh();
		}

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(checkerboard.opponentPieces[i].posX, checkerboard.opponentPieces[i].posY, checkerboard.opponentPieces[i].posZ));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		opponentTexture.UseTexture();
		if (checkerboard.opponentPieces[i].promoted)
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
		model = glm::translate(model, glm::vec3(checkerboard.blackSquares[i].posX, 0.5f, checkerboard.blackSquares[i].posZ));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		if (checkerboard.blackSquares[i].active)
		{
			activeSquareTexture.UseTexture();
		}
		else
		{
			blackSquareTexture.UseTexture();
		}
		meshList[3]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(checkerboard.whiteSquares[i].posX, 0.5f, checkerboard.whiteSquares[i].posZ));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		whiteSquareTexture.UseTexture();
		meshList[3]->RenderMesh();
	}
}

void Gameplay::KeyControl(bool* keys)
{
	if (keys[GLFW_KEY_ENTER])
	{
		if (checkerboard.blackSquares[activeSquareIndex].playerPieceIndex != -1) //jezeli do pola jest przypisana bierka gracza
		{
			ChangeChosen();
		}
		else if (chosenPieceIndex != -1 //jezeli jakas bierka jest wybrana
			&& checkerboard.blackSquares[activeSquareIndex].playerPieceIndex == -1 //i aktywne pole jest puste
			&& checkerboard.blackSquares[activeSquareIndex].opponentPieceIndex == -1)
		{
			checkerboard.MovePlayerPiece(chosenPieceIndex, activeSquareIndex);
			chosenPieceIndex = -1; // po ruchu zadna bierka nie jest juz wybrana
			//checkerboard.Capture(&checkerboard.opponentPieces[(std::rand() % 11)]);

			CalculateMove(); //komputer sie rusza
			//checkerboard.Capture(&checkerboard.playerPieces[(std::rand() % 11)]);
		}

		keys[GLFW_KEY_ENTER] = false;
	}

	if (keys[GLFW_KEY_LEFT])
	{
		if (white && activeSquareIndex > 0)
		{
			ChangeActiveLeft();
		}
		else if (activeSquareIndex < 31)
		{
			ChangeActiveRight();
		}
		keys[GLFW_KEY_LEFT] = false;
	}

	if (keys[GLFW_KEY_RIGHT])
	{
		if (white && activeSquareIndex < 31)
		{
			ChangeActiveRight();
		}
		else if (activeSquareIndex > 0)
		{
			ChangeActiveLeft();
		}
		keys[GLFW_KEY_RIGHT] = false;
	}

	if (keys[GLFW_KEY_UP])
	{
		if (white && activeSquareIndex < 28)
		{
			ChangeActiveUp();
		}
		else if (activeSquareIndex > 3)
		{
			ChangeActiveDown();
		}
		keys[GLFW_KEY_UP] = false;
	}

	if (keys[GLFW_KEY_DOWN])
	{
		if (white && activeSquareIndex > 3)
		{
			ChangeActiveDown();
		}
		else if (activeSquareIndex < 28)
		{
			ChangeActiveUp();
		}
		keys[GLFW_KEY_DOWN] = false;
	}
}

void Gameplay::CalculateMove()
{
	int pieceIndex = (std::rand() % 11);
	int squareIndex = (std::rand() % 31);

	if (checkerboard.blackSquares[squareIndex].playerPieceIndex == -1 && checkerboard.blackSquares[squareIndex].opponentPieceIndex == -1 //jezli wybrane pole jest puste
		&& checkerboard.opponentPieces[pieceIndex].captured == false) //i wybrana bierka nie jest zbita
	{
		checkerboard.MoveOpponentPiece(pieceIndex, squareIndex); //rusz sie losowa bierka
	}
	else
	{
		CalculateMove(); //w innym wypadku powtorz losowanie
	}
}

void Gameplay::ChangeChosen()
{
	if (checkerboard.blackSquares[activeSquareIndex].playerPieceIndex != -1) //jezeli do pola jest przypisana bierka gracza
	{
		if (chosenPieceIndex != -1 && checkerboard.blackSquares[activeSquareIndex].playerPieceIndex != chosenPieceIndex) //jezeli wybrana jest obecnie inna bierka ni¿ ta, stoj¹ca na aktywnym polu
		{
			checkerboard.playerPieces[chosenPieceIndex].chosen = false; //wybrana wczesniej bierka przestaje byc wybrana
			chosenPieceIndex = -1;
		}

		if (checkerboard.playerPieces[checkerboard.blackSquares[activeSquareIndex].playerPieceIndex].chosen == false) //jezeli bierka na aktywnym polu nie jest wybrana
		{
			checkerboard.playerPieces[checkerboard.blackSquares[activeSquareIndex].playerPieceIndex].chosen = true; //bierka staje sie wybrana
			chosenPieceIndex = checkerboard.blackSquares[activeSquareIndex].playerPieceIndex; //indeks wybranej bierki jest przechowany w zmiennej
		}
		else
		{
			checkerboard.playerPieces[checkerboard.blackSquares[activeSquareIndex].playerPieceIndex].chosen = false; //bierka przestaje byc wybrana
			chosenPieceIndex = -1; //zmienna przechowujaca indeks wybranej bierki = -1
		}
	}
}

void Gameplay::ChangeActiveLeft()
{
	checkerboard.blackSquares[activeSquareIndex].active = false;
	checkerboard.blackSquares[activeSquareIndex - 1].active = true;
	activeSquareIndex--;
}

void Gameplay::ChangeActiveRight()
{
	checkerboard.blackSquares[activeSquareIndex].active = false;
	checkerboard.blackSquares[activeSquareIndex + 1].active = true;
	activeSquareIndex++;
}

void Gameplay::ChangeActiveUp()
{
	checkerboard.blackSquares[activeSquareIndex].active = false;
	checkerboard.blackSquares[activeSquareIndex + 4].active = true;
	activeSquareIndex += 4;
}

void Gameplay::ChangeActiveDown()
{
	checkerboard.blackSquares[activeSquareIndex].active = false;
	checkerboard.blackSquares[activeSquareIndex - 4].active = true;
	activeSquareIndex -= 4;
}

Gameplay::~Gameplay()
{

}