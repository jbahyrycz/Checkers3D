#include "Square.h"
//krawedz squera to 1
Square::Square()
{
	posX = 0.0f;
	posY = 0.5f;
	posZ = 0.0f;

	color = 0; //0-c 1-b 

	playerPieceIndex = -1;
	opponentPieceIndex = -1;

	active = false;
}

void Square::SetPosition(float x, float z)
{
	posX = x;
	posZ = z;
}

bool Square::IsEmpty()
{
	if (playerPieceIndex == -1 && opponentPieceIndex == -1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

Square::~Square()
{

}