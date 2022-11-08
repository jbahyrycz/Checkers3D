#include "Square.h"

Square::Square()
{
	posX = 0.0f;
	posY = 0.5f;
	posZ = 0.0f;

	active = false;
	whitePieceIndex = -1;
	blackPieceIndex = -1;
	playerPieceIndex = -1;
	opponentPieceIndex = -1;
}

void Square::SetPosition(float x, float z)
{
	posX = x;
	posZ = z;
}

void Square::ChangeActive()
{
	active = !active;
}

Square::~Square()
{

}