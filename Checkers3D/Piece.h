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

	int squareIndex;

	bool chosen;
	bool promoted;
	bool captured;

	float finalPosX;
	float finalPosZ;
};

