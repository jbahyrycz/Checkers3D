#pragma once

class Checkerboard;
class Gameplay;

class Piece
{
public:
	friend class Checkerboard;
	friend class Gameplay;

	Piece();

	float GetPosX() { return posX; }
	float GetPosY() { return posY; }
	float GetPosZ() { return posZ; }
	bool GetChosen() { return chosen; }
	bool GetPromoted() { return promoted; }
	bool GetTaken() { return taken; }
	int GetSquareIndex() { return squareIndex; }

	~Piece();

private:
	float posX;
	float posY;
	float posZ;

	bool chosen;
	bool promoted;
	bool taken;

	int squareIndex;

	void ChangePosition(float x, float y, float z);
};

