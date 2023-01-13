#include "Enemy.h"
#include <iostream>
#include <math.h>

cEnemy::cEnemy()
{
	mPosX = 0;
	mPosY = 0;
	mHealth = 100.0;
	mNextPtr = NULL;
}

cEnemy::cEnemy(int pX, int pY)
{
	mPosX = pX;
	mPosY = pY;
	mHealth = 100.0;
	mNextPtr = NULL;
}

void cEnemy::setX(int pX)
{
	mPosX = pX;
}
void cEnemy::setY(int pY)
{
	mPosY = pY;
}

void cEnemy::setNxtPtr(cEnemy* pPtr)
{
	mNextPtr = pPtr;
}

cEnemy* cEnemy::getNxtPtr()
{
	return mNextPtr;
}

//void cEnemy::setPrevPtr(cEnemy* pPtr)
//{
//	mPrevPtr = pPtr;
//}

//cEnemy* cEnemy::getPrevPtr()
//{
//	return nullptr;
//}

int cEnemy::getPosX()
{
 	return mPosX;
}

int cEnemy::getPosY()
{
	return mPosY;;
}

float cEnemy::isHit(float pDmg)
{
	mHealth -= pDmg;
	return mHealth;
}

cEnemyStack::cEnemyStack()
{
	mFirstPointer = NULL;
	mNumEnemies = 0;
	mNumMaxEnemies = 5;
	
}

cEnemyStack::cEnemyStack(int pMaxNumEnemies)
{
	mNumMaxEnemies = pMaxNumEnemies;
	mNumEnemies = 0;
	mFirstPointer = NULL;
	
}

cEnemy* cEnemyStack::traverse(cEnemy* pPtr, cEnemy* pTrgtPtr)
{
	if (pPtr->getNxtPtr() == pTrgtPtr)
		return pPtr;
	else
	{
		return traverse(pPtr->getNxtPtr(), pTrgtPtr);
	}
}

cEnemy* cEnemyStack::whoIsHit(cEnemy* pPtr, int px, int py)
{
	if ((pPtr->getPosX() == px) && (pPtr->getPosY() == py))
		return pPtr;
	else
	{
		return whoIsHit(pPtr->getNxtPtr(), px, py);
	}
}

int cEnemyStack::NumCloseEnemies(int pX, int pY)
{
	cEnemy* tmpPtr = mFirstPointer;
	int numClose = 0;

	while (tmpPtr != NULL)
	{
		int eX = tmpPtr->getPosX();
		int eY = tmpPtr->getPosY();
		if (sqrt(pow((eX - pX), 2) + pow((eY - pY), 2)) <= 1.5)
		{
			numClose++;
		}
		tmpPtr = tmpPtr->getNxtPtr();
	}

	return numClose;
}

//int cEnemyStack::getHitIdx(int px, int py)
//{
//	cEnemy* tmpPtr = whoIsHit(mFirstPointer, px, py);
//	return tmpPtr->getPosY() * mWidth + tmpPtr->getPosX();
//}

bool cEnemyStack::addEnemy(cEnemy& pE)
{
	if (mNumEnemies == 0)
	{
		cEnemy* tmpPtr = new cEnemy(pE.getPosX(), pE.getPosY());
		mFirstPointer = tmpPtr;
		mNumEnemies++;
		return true;
	}
	else if (mNumEnemies < mNumMaxEnemies)
	{
		cEnemy* lastPtr = traverse(mFirstPointer, NULL);
		cEnemy* tmpPtr = new cEnemy(pE.getPosX(), pE.getPosY());
		lastPtr->setNxtPtr(tmpPtr);
		mNumEnemies++;
		
		return true;
	}
	else
		return false;
}

bool cEnemyStack::removeEnemy(int px, int py, float pDmg)
{
	cEnemy* HitPtr = whoIsHit(mFirstPointer, px, py);
	if (HitPtr->isHit(pDmg) <= 0.0)
	{
		cEnemy* tmpPtr = HitPtr->getNxtPtr();
		if (tmpPtr != NULL)
			HitPtr->setNxtPtr(NULL);

		if (HitPtr == mFirstPointer)
		{
			mFirstPointer->setNxtPtr(NULL);					// Doesn't seem to be needed but feels a bit safer.
			delete mFirstPointer;
			mFirstPointer = tmpPtr;
		}
		else
		{
			cEnemy* PrevPtr = traverse(mFirstPointer, HitPtr);
			HitPtr->setNxtPtr(NULL);
			delete HitPtr;
			PrevPtr->setNxtPtr(tmpPtr);
		}
		mNumEnemies--;
		return true;
	}
	else
		return false;
}

int cEnemyStack::getNumEnemies()
{
	return mNumEnemies;
}
	
	//if(tmpPtr == nullptr)



