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
	Checkerboard(bool white);

	bool MovePlayerPiece(int pieceIndex, int squareIndex);
	void MoveOpponentPiece(int pieceIndex, int squareIndex);

	void Capture(Piece* piece);
	void Promote(Piece* piece);

	~Checkerboard();

private:
	Square whiteSquares[32];
	Square blackSquares[32];
	Piece playerPieces[12];
	Piece opponentPieces[12];
};

