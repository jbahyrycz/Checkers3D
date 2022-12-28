#include "Analysis.h"

const char* Analysis::vShader = "Shaders/shaderGameplay.vert";
const char* Analysis::fShader = "Shaders/shaderGameplay.frag";

Analysis::Analysis(Window* window, unsigned int* n, unsigned int color)
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

	analysisShouldClose = false;

	checkerboard = Checkerboard(white);
	chosenPieceIndex = 0;

	if (color == 0)
	{
		whitePieceTextureLoc = "Textures/plainWhite.jpg";
		blackPieceTextureLoc = "Textures/plainBlack.jpg";
		whiteSquareTextureLoc = "Textures/whiteSquareWood.jpg";
		blackSquareTextureLoc = "Textures/blackSquareWood.jpg";
	}
	else if (color == 1)
	{
		whitePieceTextureLoc = "Textures/plainRed.png";
		blackPieceTextureLoc = "Textures/plainBrown.png";
		whiteSquareTextureLoc = "Textures/whiteSquareWood.jpg";
		blackSquareTextureLoc = "Textures/blackSquareWood.jpg";
	}

	if (white)
	{
		camera = Camera(glm::vec3(0.0f, 7.0f, 11.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -32.0f);

		playerTexture = Texture(whitePieceTextureLoc);
		opponentTexture = Texture(blackPieceTextureLoc);

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
}

void Analysis::Run()
{
	CreateObjects();
	CreateShaders();
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (GLfloat)windowPtr->GetBufferWidth() / windowPtr->GetBufferHeight(), 0.1f, 100.0f);
	LoadTextures();

	while (!analysisShouldClose && !windowPtr->GetShouldClose())
	{
		glfwPollEvents();
		KeyControl(windowPtr->GetKeys());
		glm::mat4 viewMatrix = camera.CalculateViewMatrix();
		RenderPass(projectionMatrix, viewMatrix);
		DisplayAnalysis(uniformModel);

		glUseProgram(0);
		windowPtr->SwapBuffers();
	}
}

void Analysis::CalcAverageNormals(unsigned int* indices, unsigned int indicesCount, GLfloat* vertices, unsigned int verticesCount, unsigned int vLength, unsigned int normalOffset)
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

void Analysis::CreateObjects()
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

void Analysis::CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void Analysis::RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
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

void Analysis::LoadTextures()
{
	playerTexture.LoadTexture();
	opponentTexture.LoadTexture();
	whiteSquareTexture.LoadTexture();
	blackSquareTexture.LoadTexture();
	boardTexture.LoadTexture();
}

void Analysis::DisplayAnalysis(GLuint uniformModel)
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
		blackSquareTexture.UseTexture();
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

void Analysis::KeyControl(bool* keys)
{
	if (keys[GLFW_KEY_ESCAPE])
	{
		analysisShouldClose = true;
		*nPtr = 0;

		keys[GLFW_KEY_ESCAPE] = false;
	}
}

Analysis::~Analysis()
{

}
