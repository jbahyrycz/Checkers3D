#include "Gameplay.h"

Gameplay::Gameplay()
{
	srand(time(NULL)); //wlaczenie losowosci

	white = true;
	checkerboard.SetCheckerboard(white);

	activeSquareIndex = 0;
	chosenPieceIndex = -1;
}

Gameplay::Gameplay(bool white)
{
	this->white = white;
	checkerboard.SetCheckerboard(white);

	activeSquareIndex = 0;
	chosenPieceIndex = -1;
}

void Gameplay::ChangeChosen()
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

void Gameplay::KeyControl(bool* keys)
{
	if (keys[GLFW_KEY_ENTER])
	{
		if (checkerboard.blackSquares[activeSquareIndex].playerPieceIndex != -1) //jezeli do pola jest przypisana bierka gracza
		{
			ChangeChosen();
		}
		else if (chosenPieceIndex != -1 && CheckIfCorrect()) //jezeli jakas bierka jest wybrana i ruch jest poprawny
		{
			checkerboard.PlayerMove(&checkerboard.playerPieces[chosenPieceIndex], &checkerboard.blackSquares[activeSquareIndex], activeSquareIndex, chosenPieceIndex);
			chosenPieceIndex = -1; // po ruchu ¿adna bierka nie jest ju¿ wybrana
			CalculateMove();
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

void Gameplay::CalculateMove()
{
	int pieceIndex = (std::rand() % 11);
	int squareIndex = (std::rand() % 31);

	if (checkerboard.blackSquares[squareIndex].playerPieceIndex == -1 && checkerboard.blackSquares[squareIndex].opponentPieceIndex == -1 //jeœli wybrane pole jest puste
		&& checkerboard.opponentPieces[pieceIndex].taken == false) //i wybrana bierka nie jest zbita
	{
		checkerboard.OpponentMove(pieceIndex, squareIndex);
	}
	else
	{
		CalculateMove();
	}
}

bool Gameplay::CheckIfCorrect()
{
	if (checkerboard.blackSquares[activeSquareIndex].playerPieceIndex != -1 || checkerboard.blackSquares[activeSquareIndex].opponentPieceIndex != -1)
	{
		return false; //zwroc false jesli na wybranym do ruchu polu stoi jakas bierka
	}
	return true;
}

bool Gameplay::CheckIfFinished()
{
	return false;
}

Gameplay::~Gameplay()
{

}