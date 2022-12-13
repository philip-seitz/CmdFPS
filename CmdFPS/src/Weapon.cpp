#include "Weapon.h"

cWeapon::cWeapon()
{
	mDamage = 33.4;
	mMagSize = 10;
	mBulletsCount = mMagSize;
	mReloadTime = 2.4;
}

float cWeapon::getDmg()
{
	return mDamage;
}
