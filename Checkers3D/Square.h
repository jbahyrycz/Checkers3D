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

private:
	void SetPosition(float x, float z);

	float posX;
	float posY;
	float posZ;

	int playerPieceIndex;
	int opponentPieceIndex;

	bool active;
};

