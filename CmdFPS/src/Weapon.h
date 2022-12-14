#pragma once

class cWeapon
{
public:
	cWeapon();
	cWeapon(int pMag,int pDmg);
	float getDmg();
	int getBulletCount();
	int getMagSize();
	float getReloadTime();
	void reduceBullets();
	void reload();
	
private:
	int mMagSize;
	int mBulletCount;
	float mDamage;
	float mReloadTime;
};