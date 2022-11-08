#pragma once

class Piece;
class Checkerboard;
class Gameplay;

class Square
{
public:
	friend class Checkerboard;
	friend class Gameplay;

	Square();

	float GetPosX() { return posX; }
	float GetPosZ() { return posZ; }
	bool GetActive() { return active; }

	~Square();

private:
	float posX;
	float posY;
	float posZ;

	bool active;

	int playerPieceIndex;
	int opponentPieceIndex;

	void SetPosition(float x, float z);

	void ChangeActive();
};

