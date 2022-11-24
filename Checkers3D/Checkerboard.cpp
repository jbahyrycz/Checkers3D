#include "Checkerboard.h"

Checkerboard::Checkerboard()
{

}

Checkerboard::Checkerboard(bool white)
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

	xOffset = 0.0f;

	if (white)
	{
		for (int i = 0; i < 12; i++)
		{
			//white pieces
			blackSquares[i].playerPieceIndex = i; //przyisywanie bierki do pola
			playerPieces[i].squareIndex = i; //przypisywanie pola do bierki
			playerPieces[i].ChangePosition(blackSquares[i].posX, 0.6f, blackSquares[i].posZ); //ustawienie pozycji bierki
			playerPieces[i].finalPosX = -3.5 + xOffset;
			playerPieces[i].finalPosZ = -4.5f;
			//Promote(&playerPieces[i]);
			
			//black pieces
			blackSquares[31 - i].opponentPieceIndex = i;
			opponentPieces[i].squareIndex = 31 - i;
			opponentPieces[i].ChangePosition(blackSquares[31 - i].posX, 0.6f, blackSquares[31 - i].posZ);
			opponentPieces[i].finalPosX = -3.5 + xOffset;
			opponentPieces[i].finalPosZ = 4.5f;
			//Capture(&opponentPieces[i]);

			xOffset += 0.7f;
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
			opponentPieces[i].finalPosX = -3.5 + xOffset;
			opponentPieces[i].finalPosZ = -4.5f;
			//Capture(&opponentPieces[i]);

			//black pieces
			blackSquares[31 - i].playerPieceIndex = i;
			playerPieces[i].squareIndex = 31 - i;
			playerPieces[i].ChangePosition(blackSquares[31 - i].posX, 0.6f, blackSquares[31 - i].posZ);
			playerPieces[i].finalPosX = -3.5 + xOffset;
			playerPieces[i].finalPosZ = 4.5f;
			//Promote(&playerPieces[i]);

			xOffset += 0.7f;
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

void Checkerboard::MovePlayerPiece(int pieceIndex, int squareIndex)
{
	blackSquares[playerPieces[pieceIndex].squareIndex].playerPieceIndex = -1;
	blackSquares[squareIndex].playerPieceIndex = pieceIndex;

	playerPieces[pieceIndex].squareIndex = squareIndex;
	playerPieces[pieceIndex].ChangePosition(blackSquares[squareIndex].posX, 0.6f, blackSquares[squareIndex].posZ);

	playerPieces[pieceIndex].chosen = false;
}

void Checkerboard::MoveOpponentPiece(int pieceIndex, int squareIndex)
{
	blackSquares[opponentPieces[pieceIndex].squareIndex].opponentPieceIndex = -1;
	blackSquares[squareIndex].opponentPieceIndex = pieceIndex;

	opponentPieces[pieceIndex].squareIndex = squareIndex;
	opponentPieces[pieceIndex].ChangePosition(blackSquares[squareIndex].posX, 0.6f, blackSquares[squareIndex].posZ);
}

void Checkerboard::Capture(Piece* piece)
{
	blackSquares[piece->squareIndex].playerPieceIndex = -1; //opuszczanie pola
	blackSquares[piece->squareIndex].opponentPieceIndex = -1;
	piece->squareIndex = -1; //do bierki nie jest juz przypisane zadne pole
	piece->captured = true;
	piece->ChangePosition(piece->finalPosX, -0.5f, piece->finalPosZ); //zmiana polozenia bierki
}

void Checkerboard::Promote(Piece* piece)
{
	piece->promoted = true;
}

Checkerboard::~Checkerboard()
{

}
