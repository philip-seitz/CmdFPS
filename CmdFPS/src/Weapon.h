#pragma once

class cWeapon
{
public:
	cWeapon();
	float getDmg();
	
private:
	int mMagSize;
	int mBulletsCount;
	float mDamage;
	float mReloadTime;
};