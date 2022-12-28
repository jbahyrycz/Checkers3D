#pragma once

class Checkerboard;
class Gameplay;

class Piece
{
public:
	friend class Checkerboard;
	friend class Gameplay;

	Piece();
	~Piece();

private:
	void ChangePosition(float x, float y, float z);

	float posX;
	float posY;
	float posZ;

	bool color; //0(f) biale 1 (t)czarne

	int squareIndex;

	bool chosen;
	bool promoted;
	bool captured;

	float finalPosX;
	float finalPosZ;
};

