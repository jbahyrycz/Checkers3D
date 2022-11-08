#include "Gameplay.h"

Gameplay::Gameplay()
{
	white = true;
	checkerboard.SetCheckerboard(white);
}

Gameplay::Gameplay(bool white)
{
	this->white = white;
	checkerboard.SetCheckerboard(white);
}

void Gameplay::KeyControl(bool* keys)
{
	if (keys[GLFW_KEY_ENTER])
	{
		if (checkerboard.blackSquares[checkerboard.activeSquareIndex].playerPieceIndex != -1) //jezeli do pola jest przypisana bierka gracza
		{
			checkerboard.ChangeChosen();
		}
		else if (checkerboard.chosenPieceIndex != -1 && CheckIfCorrect()) //jezeli jakas bierka jest wybrana i ruch jest poprawny
		{
			checkerboard.Move(&checkerboard.playerPieces[checkerboard.chosenPieceIndex], &checkerboard.blackSquares[checkerboard.activeSquareIndex]);
			checkerboard.ComputerMove();
		}

		keys[GLFW_KEY_ENTER] = false;
	}

	if (keys[GLFW_KEY_LEFT])
	{
		if (white && checkerboard.GetActiveSquareIndex() > 0)
		{
			checkerboard.ChangeActiveLeft();
		}
		else if (checkerboard.GetActiveSquareIndex() < 31)
		{
			checkerboard.ChangeActiveRight();
		}
		keys[GLFW_KEY_LEFT] = false;
	}

	if (keys[GLFW_KEY_RIGHT])
	{
		if (white && checkerboard.GetActiveSquareIndex() < 31)
		{
			checkerboard.ChangeActiveRight();
		}
		else if (checkerboard.GetActiveSquareIndex() > 0)
		{
			checkerboard.ChangeActiveLeft();
		}
		keys[GLFW_KEY_RIGHT] = false;
	}

	if (keys[GLFW_KEY_UP])
	{
		if (white && checkerboard.GetActiveSquareIndex() < 28)
		{
			checkerboard.ChangeActiveUp();
		}
		else if (checkerboard.GetActiveSquareIndex() > 3)
		{
			checkerboard.ChangeActiveDown();
		}
		keys[GLFW_KEY_UP] = false;
	}

	if (keys[GLFW_KEY_DOWN])
	{
		if (white && checkerboard.GetActiveSquareIndex() > 3)
		{
			checkerboard.ChangeActiveDown();
		}
		else if (checkerboard.GetActiveSquareIndex() < 28)
		{
			checkerboard.ChangeActiveUp();
		}
		keys[GLFW_KEY_DOWN] = false;
	}
}

bool Gameplay::CheckIfCorrect()
{
	if (checkerboard.blackSquares[checkerboard.activeSquareIndex].playerPieceIndex != -1 || checkerboard.blackSquares[checkerboard.activeSquareIndex].opponentPieceIndex != -1)
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