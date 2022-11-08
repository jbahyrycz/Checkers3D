#pragma once

#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

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

	void PlayerMove();
	void Move();
	void Move(Piece* piece, Square* square);

	void Take(Piece* piece);

	void Promote(Piece* piece);

	void ChangeChosen();

	void ChangeActiveLeft();
	void ChangeActiveRight();
	void ChangeActiveUp();
	void ChangeActiveDown();

	void ComputerMove();

	int GetActiveSquareIndex() { return activeSquareIndex; }
	int GetChosenPieceIndex() { return chosenPieceIndex; }

	Square* GetWhiteSquares() { return whiteSquares; }
	Square* GetBlackSquares() { return blackSquares; }
	Piece* GetPlayerPieces() { return playerPieces; }
	Piece* GetOpponentPieces() { return opponentPieces; }

	~Checkerboard();

private:
	int activeSquareIndex;
	int chosenPieceIndex;

	Square whiteSquares[32];
	Square blackSquares[32];
	Piece playerPieces[12];
	Piece opponentPieces[12];
};

