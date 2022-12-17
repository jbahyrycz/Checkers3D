#include "Piece.h"
// czarne zwiekszaja posZ
//biale zmniejszaja pozZ
Piece::Piece()
{
	posX = 0.0f;
	posY = 0.6f;
	posZ = 0.0f;

	color = 0; //domyslnie czarne 0 - c 1 - b

	squareIndex = -1;

	chosen = false;
	promoted = false;
	captured = false;

	finalPosX = 0.0f;
	finalPosZ = 0.0f;
}

void Piece::ChangePosition(float x, float y, float z)
{
	posX = x;
	posY = y;
	posZ = z;
}

Piece::~Piece()
{

}

void Piece::setcolor(bool a) //1-b 0-c 
{
	color = a;
}

bool Piece::getcolor()
{
	return color;
}

float Piece::getposX()
{
	return posX;
}

float Piece::getposY()
{
	return posY;
}

float Piece::getposZ()
{
	return posZ;
}