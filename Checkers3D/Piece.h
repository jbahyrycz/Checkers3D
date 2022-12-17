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

	void setcolor(bool a); // 1-b 0-c
	bool getcolor();
	float getposX();
	float getposY();
	float getposZ();

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

