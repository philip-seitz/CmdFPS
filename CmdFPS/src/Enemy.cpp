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

cEnemyStack::cEnemyStack()
{
	mFirstPointer = nullptr;
	mNumEnemies = 0;
	
}

cEnemyStack::cEnemyStack(int pMaxNumEnemies)
{
	mNumMaxEnemies = pMaxNumEnemies;
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

//int cEnemyStack::getHitIdx(int px, int py)
//{
//	cEnemy* tmpPtr = whoIsHit(mFirstPointer, px, py);
//	return tmpPtr->getPosY() * mWidth + tmpPtr->getPosX();
//}

void cEnemyStack::addEnemy(cEnemy& pE)
{
	if (mNumEnemies == 0)
	{
		cEnemy* tmpPtr = new cEnemy(pE.getPosX(), pE.getPosY());
		mFirstPointer = tmpPtr;
		mNumEnemies++;
	}
	else if (mNumEnemies < 5)
	{
		cEnemy* lastPtr = traverse(mFirstPointer, nullptr);
		cEnemy* tmpPtr = new cEnemy(pE.getPosX(), pE.getPosY());
		lastPtr->setNxtPtr(tmpPtr);
		mNumEnemies++;
	}
}

void cEnemyStack::removeEnemy(int px, int py)
{
	cEnemy* HitPtr = whoIsHit(mFirstPointer, px, py);
	cEnemy* tmpPtr = HitPtr->getNxtPtr();
	if (tmpPtr != nullptr)
		HitPtr->setNxtPtr(nullptr);

	if (HitPtr == mFirstPointer)
	{
		delete mFirstPointer;
		mFirstPointer = tmpPtr;
		mNumEnemies--;
	}
	else
	{
		cEnemy* PrevPtr = traverse(mFirstPointer, HitPtr);
		delete HitPtr;
		PrevPtr->setNxtPtr(tmpPtr);
		mNumEnemies--;
	}
}

int cEnemyStack::getNumEnemies()
{
	return mNumEnemies;
}
	
	//if(tmpPtr == nullptr)



