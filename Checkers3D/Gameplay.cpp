#include "Gameplay.h"

const char* Gameplay::vShader = "Shaders/shaderGameplay.vert";
const char* Gameplay::fShader = "Shaders/shaderGameplay.frag";

Gameplay::Gameplay(Window* window, unsigned int* n, unsigned int color)
{
	srand(time(NULL)); //wlaczenie losowosci

	windowPtr = window;
	nPtr = n;

	uniformProjection = 0;
	uniformModel = 0;
	uniformView = 0;
	uniformEyePosition = 0;
	uniformSpecularIntensity = 0;
	uniformShininess = 0;

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);

	light = PointLight(1.0f, 1.0f, 1.0f,
					0.6f, 0.8f,
					0.0f, 5.0f, 0.0f,
					0.05f, 0.03f, 0.02f);

	white = (std::rand() % 2);

	gameplayShouldClose = false;

	checkerboard = Checkerboard(white);
	chosenPieceIndex = 0;

	if (color == 0)
	{
		whitePieceTextureLoc = "Textures/plainWhite.jpg";
		blackPieceTextureLoc = "Textures/plainBlack.jpg";
		whiteSquareTextureLoc = "Textures/whiteSquareWood.jpg";
		blackSquareTextureLoc = "Textures/blackSquareWood.jpg";
	}
	else
	{
		whitePieceTextureLoc = "Textures/plainRed.png";
		blackPieceTextureLoc = "Textures/plainBrown.png";
		whiteSquareTextureLoc = "Textures/whiteSquareWood.jpg";
		blackSquareTextureLoc = "Textures/blackSquareWood.jpg";
	}

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

		playerTexture = Texture(blackPieceTextureLoc);
		opponentTexture = Texture(whitePieceTextureLoc);

		activeSquareIndex = 31;
	}

	whiteSquareTexture = Texture("Textures/whiteSquareWood.jpg");
	blackSquareTexture = Texture("Textures/blackSquareWood.jpg");
	boardTexture = Texture("Textures/boardWood.png");
	activeSquareTexture = Texture("Textures/plainPink.png");

	checkerboard.blackSquares[activeSquareIndex].active = true;
}

void Gameplay::Run()
{
	CreateObjects();
	CreateShaders();
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (GLfloat)windowPtr->GetBufferWidth() / windowPtr->GetBufferHeight(), 0.1f, 100.0f);
	LoadTextures();

	while (!gameplayShouldClose && !windowPtr->GetShouldClose())
	{
		glfwPollEvents();
		KeyControl(windowPtr->GetKeys());
		glm::mat4 viewMatrix = camera.CalculateViewMatrix();
		RenderPass(projectionMatrix, viewMatrix);
		DisplayGameplay(uniformModel);

		glUseProgram(0);
		windowPtr->SwapBuffers();
	}
}

void Gameplay::CalcAverageNormals(unsigned int* indices, unsigned int indicesCount, GLfloat* vertices, unsigned int verticesCount, unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indicesCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = (indices[i + 1]) * vLength;
		unsigned int in2 = (indices[i + 2]) * vLength;

		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticesCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
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
			-4.0f, -0.5f, 4.0f,			0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			4.0f, -0.5f, 4.0f,			3.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			4.0f, -0.5f, -4.0f,			0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			-4.0f, -0.5f, -4.0f,		3.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			-4.0f, 0.5f, 4.0f,			0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
			4.0f, 0.5f, 4.0f,			3.0f, 1.0f,		0.0f, 0.0f, 0.0f,
			4.0f, 0.5f, -4.0f,			0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
			-4.0f, 0.5f, -4.0f,			3.0f, 1.0f,		0.0f, 0.0f, 0.0f
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
			-0.3f, -0.1f, -0.3f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.3f, -0.1f, -0.3f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.3f, 0.1f, -0.3f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			-0.3f, 0.1f, -0.3f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			-0.3f, 0.1f, 0.3f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			-0.3f, -0.1f, 0.3f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.3f, -0.1f, 0.3f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.3f, 0.1f, 0.3f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
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
			-0.3f, -0.1f, -0.3f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.3f, -0.1f, -0.3f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.3f, 0.1f, -0.3f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			-0.3f, 0.1f, -0.3f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			-0.3f, 0.1f, 0.3f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			-0.3f, -0.1f, 0.3f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.3f, -0.1f, 0.3f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.3f, 0.1f, 0.3f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 0.7f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int squareIndices[] = {
		0, 1, 3,
		3, 2, 1
	};

	GLfloat squareVertices[] = {
			-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
			-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	CalcAverageNormals(boardIndices, 24, boardVertices, 64, 8, 5);
	Mesh* boardSides = new Mesh();
	boardSides->CreateMesh(boardVertices, boardIndices, 64, 24);
	meshList.push_back(boardSides);

	CalcAverageNormals(pieceIndices, 36, pieceVertices, 64, 8, 5);
	Mesh* piece = new Mesh();
	piece->CreateMesh(pieceVertices, pieceIndices, 64, 36);
	meshList.push_back(piece);

	CalcAverageNormals(promotedPieceIndices, 48, promotedPieceVertices, 72, 8, 5);
	Mesh* promotedPiece = new Mesh();
	promotedPiece->CreateMesh(promotedPieceVertices, promotedPieceIndices, 72, 48);
	meshList.push_back(promotedPiece);

	CalcAverageNormals(squareIndices, 6, squareVertices, 32, 8, 5);
	Mesh* square = new Mesh();
	square->CreateMesh(squareVertices, squareIndices, 32, 6);
	meshList.push_back(square);
}

void Gameplay::CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void Gameplay::RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	shaderList[0].UseShader();
	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();

	glViewport(0, 0, 1200, 900);

	// Clear the window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

	shaderList[0].SetPointLight(&light);
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
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(checkerboard.whiteSquares[i].posX, 0.5f, checkerboard.whiteSquares[i].posZ));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		whiteSquareTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
	}
}

void Gameplay::KeyControl(bool* keys)
{
	if (keys[GLFW_KEY_ESCAPE])
	{
		gameplayShouldClose = true;
		*nPtr = 0;

		keys[GLFW_KEY_ESCAPE] = false;
	}
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
			bool move = false;
			move = checkerboard.MovePlayerPiece(chosenPieceIndex, activeSquareIndex);
			chosenPieceIndex = -1; // po ruchu zadna bierka nie jest juz wybrana
			//checkerboard.Capture(&checkerboard.opponentPieces[(std::rand() % 11)]);

			if (move == true)
			{
				CalculateMove(); //komputer sie rusza
			}
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