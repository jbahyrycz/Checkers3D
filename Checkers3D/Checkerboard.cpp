#include "Checkerboard.h"

Checkerboard::Checkerboard()
{
	blackSquares[0].active = true;
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

void Checkerboard::PlayerMove(Piece* piece, Square* square, int activeSquareIndex, int chosenPieceIndex)
{
	piece->ChangePosition(square->posX, 0.6f, square->posZ); //zmiana wspolrzednych bierki

	blackSquares[piece->squareIndex].playerPieceIndex = -1; //zwalnianie poprzednio zajmowanego pola
	piece->squareIndex = activeSquareIndex; //przypisywanie pola do bierki
	square->playerPieceIndex = chosenPieceIndex; //przypisywanie bierki do pola

	piece->chosen = false; //po ruchu wybrana bierka przestaje byc wybrana
}

void Checkerboard::OpponentMove(int pieceIndex, int squareIndex)
{
	opponentPieces[pieceIndex].ChangePosition(blackSquares[squareIndex].posX, 0.6f, blackSquares[squareIndex].posZ); //zmiana pozycji
	blackSquares[opponentPieces[pieceIndex].squareIndex].opponentPieceIndex = -1; //na poprzednim polu nie ma juz czarnej bierki
	opponentPieces[pieceIndex].squareIndex = squareIndex; //do bierki przypisujemy nowe pole
	blackSquares[squareIndex].opponentPieceIndex = pieceIndex; //do pola przypisujemy nowa bierke
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

Checkerboard::~Checkerboard()
{

}
