#pragma once

#include "Checkerboard.h"

class Gameplay
{
public:
	Gameplay();

	Gameplay(bool white);

	Checkerboard GetCheckerboard() { return checkerboard; }

	void KeyControl(bool* keys);

	bool CheckIfCorrect(); //bedzie sprawdzac czy ruch, ktory uzytkownik chce wykonac jest zgodny z zasadami

	bool CheckIfFinished(); //bedzie sprawdzac czy gra zakonczona

	~Gameplay();

private:
	Checkerboard checkerboard;
	bool white;
};

