#pragma once

class cEnemy
{
public:
	cEnemy();
	cEnemy(int pX, int pY);
	void setX(int pX);
	void setY(int pY);
	void setNxtPtr(cEnemy* pPtr);
	cEnemy* getNxtPtr();
	//void setPrevPtr(cEnemy* pPtr);
	//cEnemy* getPrevPtr();
	int getPosX();
	int getPosY();
private:
	int mPosX;
	int mPosY;
	cEnemy* mNextPtr;
	//cEnemy* mPrevPtr;
};


class cEnemyStack
{
public:
	cEnemyStack();
	cEnemyStack(int pMaxNumEnemies);
	cEnemy* traverse(cEnemy* pPtr, cEnemy* pTrgtPtr);				// address of last enemy in stack
	cEnemy* whoIsHit(cEnemy* pPtr, int px, int py);
	//int getHitIdx(int px, int py);
	void addEnemy(cEnemy& pE);
	void removeEnemy(int px, int py);
	int getNumEnemies();

	
private:

	int mNumEnemies;
	int mNumMaxEnemies;
	//int mWidth;
	cEnemy* mFirstPointer;


};