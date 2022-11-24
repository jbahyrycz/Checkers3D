#include "Piece.h"

Piece::Piece()
{
	posX = 0.0f;
	posY = 0.6f;
	posZ = 0.0f;

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