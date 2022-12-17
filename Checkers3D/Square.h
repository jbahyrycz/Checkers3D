#pragma once

class Checkerboard;
class Gameplay;

class Square
{
public:
	friend class Checkerboard;
	friend class Gameplay;

	Square();
	~Square();

	void setcolor(bool a);// 0 - c 1 - b
		bool getcolor();
		bool isempty(); //false - puste true - pelny
		float getposX();
		float getposY();
		float getposZ();

private:
	void SetPosition(float x, float z);

	float posX;
	float posY;
	float posZ;

	bool color; //0-c 1-b

	int playerPieceIndex;
	int opponentPieceIndex;

	bool active;
};

