#include "Square.h"

Square::Square()
{
	posX = 0.0f;
	posY = 0.5f;
	posZ = 0.0f;

	playerPieceIndex = -1;
	opponentPieceIndex = -1;

	active = false;
}

void Square::SetPosition(float x, float z)
{
	posX = x;
	posZ = z;
}

Square::~Square()
{

}