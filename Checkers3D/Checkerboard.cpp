#include "Checkerboard.h"

Checkerboard::Checkerboard()
{
	srand(time(NULL)); //wlaczenie losowosci

	blackSquares[0].ChangeActive();
	activeSquareIndex = 0;
	chosenPieceIndex = -1;
}

void Checkerboard::SetCheckerboard(bool white)
{
	// black squares

	float xOffset = 0.0f;
	float zOffset = 0.0f;

	for (int i = 0; i < 32; i++)
	{
		blackSquares[i].SetPosition(-3.5f + xOffset, 3.5f - zOffset);

		xOffset += 2.0f;

		if ((i + 1) % 4 == 0)
		{
			zOffset += 1.0f;

			if ((i + 1) % 8 == 0)
			{
				xOffset = 0.0f;
			}
			else
			{
				xOffset = 1.0f;
			}
		}
	}

	if (white)
	{
		for (int i = 0; i < 12; i++)
		{
			//white pieces
			blackSquares[i].playerPieceIndex = i; //przyisywanie bierki do pola
			playerPieces[i].squareIndex = i; //przypisywanie pola do bierki

			playerPieces[i].ChangePosition(blackSquares[i].posX, 0.6f, blackSquares[i].posZ); //ustawienie pozycji bierki

			//black pieces
			blackSquares[31 - i].opponentPieceIndex = i;
			opponentPieces[i].squareIndex = 31 - i;

			opponentPieces[i].ChangePosition(blackSquares[31 - i].posX, 0.6f, blackSquares[31 - i].posZ);
		}
	}
	else
	{
		for (int i = 0; i < 12; i++)
		{
			//white pieces
			blackSquares[i].opponentPieceIndex = i;
			opponentPieces[i].squareIndex = i;

			opponentPieces[i].ChangePosition(blackSquares[i].posX, 0.6f, blackSquares[i].posZ);

			//black pieces
			blackSquares[31 - i].playerPieceIndex = i;
			playerPieces[i].squareIndex = 31 - i;

			playerPieces[i].ChangePosition(blackSquares[31 - i].posX, 0.6f, blackSquares[31 - i].posZ);
		}
	}

	// white squares

	xOffset = 0.0f;
	zOffset = 0.0f;

	for (int i = 0; i < 32; i++)
	{
		whiteSquares[i].SetPosition(-2.5f + xOffset, 3.5f - zOffset);

		xOffset += 2.0f;

		if ((i + 1) % 4 == 0)
		{
			zOffset += 1.0f;

			if ((i + 1) % 8 == 0)
			{
				xOffset = 0.0f;
			}
			else
			{
				xOffset = -1.0f;
			}
		}
	}
}

void Checkerboard::Move()
{
	playerPieces[chosenPieceIndex].ChangePosition(blackSquares[activeSquareIndex].posX, 0.6f, blackSquares[activeSquareIndex].posZ); //zmiana wspolrzednych bierki

	blackSquares[playerPieces[chosenPieceIndex].squareIndex].playerPieceIndex = -1; //zwalnianie poprzednio zajmowanego pola
	playerPieces[chosenPieceIndex].squareIndex = activeSquareIndex; //przypisywanie pola do bierki
	blackSquares[activeSquareIndex].playerPieceIndex = chosenPieceIndex; //przypisywanie bierki do pol

	playerPieces[chosenPieceIndex].ChangeChosen(); //wybrana bierka przestaje byc wybrana
	chosenPieceIndex = -1; //po ruchu indeks wybranej bierki zmienia sie na -1

	ComputerMove();
}

void Checkerboard::Promote(Piece* piece)
{
	piece->promoted = true;
}

void Checkerboard::Take(Piece* piece)
{
	static float xOffset = 0;
	piece->taken = true;
	piece->ChangePosition(-3.5f + xOffset, -0.5f, 4.5f);
	xOffset += 0.7;
}

void Checkerboard::ChangeChosen()
{
	if (chosenPieceIndex != -1 && blackSquares[activeSquareIndex].playerPieceIndex != chosenPieceIndex) //jezeli wybrana jest obecnie inna bierka ni¿ ta, stoj¹ca na aktywnym polu
	{
		playerPieces[chosenPieceIndex].ChangeChosen(); //wybrana wczesniej bierka przestaje byc wybrana
		chosenPieceIndex = -1;
	}

	if (playerPieces[blackSquares[activeSquareIndex].playerPieceIndex].chosen == false) //jezeli bierka na aktywnym polu nie jest wybrana
	{
		playerPieces[blackSquares[activeSquareIndex].playerPieceIndex].ChangeChosen(); //bierka staje sie wybrana
		chosenPieceIndex = blackSquares[activeSquareIndex].playerPieceIndex; //indeks wybranej bierki jest przechowany w zmiennej
	}
	else
	{
		playerPieces[blackSquares[activeSquareIndex].playerPieceIndex].ChangeChosen(); //bierka przestaje byc wybrana
		chosenPieceIndex = -1; //zmienna przechowujaca indeks wybranej bierki = -1
	}
}

void Checkerboard::ChangeActiveLeft()
{
	blackSquares[activeSquareIndex].ChangeActive();
	blackSquares[activeSquareIndex - 1].ChangeActive();
	activeSquareIndex--;
}

void Checkerboard::ChangeActiveRight()
{
	blackSquares[activeSquareIndex].ChangeActive();
	blackSquares[activeSquareIndex + 1].ChangeActive();
	activeSquareIndex++;
}

void Checkerboard::ChangeActiveUp()
{
	blackSquares[activeSquareIndex].ChangeActive();
	blackSquares[activeSquareIndex + 4].ChangeActive();
	activeSquareIndex += 4;
}

void Checkerboard::ChangeActiveDown()
{
	blackSquares[activeSquareIndex].ChangeActive();
	blackSquares[activeSquareIndex - 4].ChangeActive();
	activeSquareIndex -= 4;
}

void Checkerboard::ComputerMove()
{
	int pieceIndex = (std::rand() % 11);
	int squareIndex = (std::rand() % 31);

	if (blackSquares[squareIndex].playerPieceIndex == -1 && blackSquares[squareIndex].opponentPieceIndex == -1 //jeœli wybrane pole jest puste
		&& opponentPieces[pieceIndex].taken == false) //i wybrana bierka nie jest zbita
	{
		opponentPieces[pieceIndex].ChangePosition(blackSquares[squareIndex].posX, 0.6f, blackSquares[squareIndex].posZ); //zmiana pozycji
		blackSquares[opponentPieces[pieceIndex].squareIndex].opponentPieceIndex = -1; //na poprzednim polu nie ma juz czarnej bierki
		opponentPieces[pieceIndex].squareIndex = squareIndex; //do bierki przypisujemy nowe pole
		blackSquares[squareIndex].opponentPieceIndex = pieceIndex; //do pola przypisujemy nowa bierke
	}
	else
	{
		ComputerMove();
	}
}

Checkerboard::~Checkerboard()
{

}
