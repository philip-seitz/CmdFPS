#pragma once

class cEnemy
{
public:
	cEnemy();
	cEnemy(int pX, int pY);
	void setX(int pX);
	void setY(int pY);
	int getPosX();
	int getPosY();
private:
	int mPosX;
	int mPosY;
	cEnemy* mNextPtr;
};