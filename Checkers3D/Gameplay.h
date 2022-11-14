#pragma once

#include "Checkerboard.h"
#include <ctime>

class Gameplay
{
public:
	Gameplay();

	Gameplay(bool white);

	Checkerboard GetCheckerboard() { return checkerboard; }

	void KeyControl(bool* keys);

	bool CheckIfCorrect(); //bedzie sprawdzac czy ruch, ktory uzytkownik chce wykonac jest zgodny z zasadami

	bool CheckIfFinished(); //bedzie sprawdzac czy gra zakonczona

	void ChangeActiveLeft();
	void ChangeActiveRight();
	void ChangeActiveUp();
	void ChangeActiveDown();

	int GetActiveSquareIndex() { return activeSquareIndex; }

	void ChangeChosen();
	void CalculateMove();

	~Gameplay();

private:
	int activeSquareIndex;
	int chosenPieceIndex;
	Checkerboard checkerboard;
	bool white;
};

