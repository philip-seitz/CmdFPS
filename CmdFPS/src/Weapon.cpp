#include "Weapon.h"

cWeapon::cWeapon()
{
	mDamage = 33.4;
	mMagSize = 20;
	mBulletCount = mMagSize;
	mReloadTime = 2.4;
}

cWeapon::cWeapon(int pMag, int pDmg)
{
	mMagSize = pMag;
	mDamage = pDmg;
	mBulletCount = mMagSize;
	mReloadTime = 2.4;
}

float cWeapon::getDmg()
{
	return mDamage;
}

int cWeapon::getBulletCount()
{
	return mBulletCount;
}

int cWeapon::getMagSize()
{
	return mMagSize;
}

float cWeapon::getReloadTime()
{
	return mReloadTime;
}

void cWeapon::reduceBullets()
{
	mBulletCount--;
}

void cWeapon::reload()
{
	mBulletCount = mMagSize;
}
