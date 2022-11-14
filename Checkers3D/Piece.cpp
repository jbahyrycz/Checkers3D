#include "Piece.h"

Piece::Piece()
{
	posX = 0.0f;
	posY = 0.0f;
	posZ = 0.0f;

	chosen = false;
	promoted = false;
	taken = false;
	squareIndex = -1;
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