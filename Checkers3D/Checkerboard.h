#pragma once

#include <stdio.h>
#include <iostream>
#include <cstdlib>

#include <GLFW\glfw3.h>

#include "Square.h"
#include "Piece.h"

class Gameplay;

class Checkerboard
{
public:
	friend class Gameplay;

	Checkerboard();

	void SetCheckerboard(bool white);

	void PlayerMove(Piece* piece, Square* square, int activeSquareIndex, int chosenPieceIndex);
	void OpponentMove(int pieceIndex, int squareIndex);

	void Take(Piece* piece);

	void Promote(Piece* piece);

	Square* GetWhiteSquares() { return whiteSquares; }
	Square* GetBlackSquares() { return blackSquares; }
	Piece* GetPlayerPieces() { return playerPieces; }
	Piece* GetOpponentPieces() { return opponentPieces; }

	~Checkerboard();

private:
	Square whiteSquares[32];
	Square blackSquares[32];
	Piece playerPieces[12];
	Piece opponentPieces[12];
};

