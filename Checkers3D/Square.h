#pragma once

class Checkerboard;
class Gameplay;
class Analysis;

class Square
{
public:
	friend class Checkerboard;
	friend class Gameplay;
	friend class Analysis;

	Square();

	bool IsEmpty(); //false - puste true - pelny

	~Square();
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

