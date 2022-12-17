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

Square::~Square()
{

}

void Square::setcolor(bool a)
{
	color = a;
}

bool Square::getcolor()
{
	return color;
}

bool Square::isempty()
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

float Square::getposX()
{
	return posX;
}
float Square::getposY()
{
	return posY;
}
float Square::getposZ()
{
	return posZ;
}