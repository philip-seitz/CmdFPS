#include "Enemy.h"

cEnemy::cEnemy()
{
	mPosX = 0;
	mPosY = 0;
	mNextPtr = nullptr;
}

cEnemy::cEnemy(int pX, int pY)
{
	mPosX = pX;
	mPosY = pY;
	mNextPtr = nullptr;
}

void cEnemy::setX(int pX)
{
	mPosX = pX;
}
void cEnemy::setY(int pY)
{
	mPosY = pY;
}

int cEnemy::getPosX()
{
	return mPosX;
}

int cEnemy::getPosY()
{
	return mPosY;;
}
