#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <string>
#include <chrono>
#include <time.h>
#include <ctime>

#include "Enemy.h"
#include "Weapon.h"
#define PI 3.14;
#define FOV 100.0
using namespace std;

struct RadarIdx
{
	int iX;
	int iY;
};

int main()
{
	srand(time(0));										// TODO: how does rand() work (with rng seed (srand()) etc.)
	// Time Handling =================================================================================

	chrono::time_point<chrono::system_clock> timeStart, timeEnd;
	chrono::duration<float> timeElapsed;
	timeStart = chrono::system_clock::now();

	// Player Attribrutes ============================================================================
	float xPlayer = 4;
	float yPlayer = 4;
	float anglePlayer = 0;
	float fov = FOV / 180.0 * PI;			// rad = 2pi*angle/360
	int radarSize = 10;
	// FOV should be equal to or below 120° (usual FOV cap)

	// Enemy Attributes ==============================================================================

	int maxEnemies = 0;
	float spawnInterval = 2.0;

	// Screen Buffer initialization ==================================================================

	int screen_w = 120;
	int screen_h = 40;
	wchar_t* sbuf = new wchar_t[screen_w * screen_h];

	// Draw the Screen with blanks so the console shows 
	for (int x = 0; x < screen_w; x++)
	{
		for (int y = 0; y < screen_h; y++)
		{
			sbuf[y * screen_w + x] = L' ';
		}
	}
	cEnemy e1;

	// Map initialization ============================================================================

	int map_w = 12;
	int map_h = 12;
	int depth = int(sqrt(pow(map_w, 2) + pow(map_h, 2)));
	wstring map;

	map += L"############";		// 0
	map += L"#          #";		// 1
	map += L"#    #     #";		// 2
	map += L"#          #";		// 3
	map += L"#          #";		// 4			y 4- 1 , x 15-13.25; 
	map += L"#          #";		// 5
	map += L"#      #####";		// 6
	map += L"#          #";		// 7
	map += L"#          #";		// 8
	map += L"#          #";		// 9
	map += L"#          #";		// 10
	map += L"############";		// 11

	// Spawn the first enemy  ========================================================================

	cEnemyStack stack(maxEnemies);
	for (int i = 0; i < 2; i++)
	{
		while (1)
		{
			int tmpX = rand() % (map_w - 1) + 1;
			int tmpY = rand() % (map_h - 1) + 1;
			int tmpIdx = tmpY * map_w + tmpX;
			if ((map[tmpIdx] == L' ') && tmpIdx != int(yPlayer)*map_w + int(xPlayer))
			{
				if (stack.getNumEnemies() < maxEnemies)
				{
					map[tmpY * map_w + tmpX] = L'E';
					e1.setX(tmpX);
					e1.setY(tmpY);
					stack.addEnemy(e1);
					break;
				}
				else
				{
					break;
				}
			}
		}
	}

	// Weapons ====================================================================================

	cWeapon currWeapon;
	cWeapon floorWeapon;
	bool reloading = false;
	float reloadTime = 0.0;
	while (1)
	{
		int tmpX = rand() % (map_w - 1) + 1;
		int tmpY = rand() % (map_h - 1) + 1;
		int tmpIdx = tmpY * map_w + tmpX;
		if ((map[tmpIdx] == L' ') && tmpIdx != int(yPlayer) * map_w + int(xPlayer))
		{
			map[tmpY * map_w + tmpX] = L'W';
			break;
		}
	}
	
	HANDLE sbuf_h = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	DWORD dwBytesWritten = 0;
	SetConsoleActiveScreenBuffer(sbuf_h);

	// Game Varialbes ================================================================================

	float scaler = 0.1;
	float speed = 2.0;
	bool gameOver = false;


	// Start Game Loop ===============================================================================

	timeEnd = chrono::system_clock::now();
	float totalTime = 0;			// über time elapsed hochzählen und dann counter zeug machen
	while (!gameOver)
	{
		timeElapsed = timeEnd - timeStart;
		float dt = timeElapsed.count();
		totalTime += dt;
		RadarIdx radar[10];
		int numRadar = 0;
		
		
		if (totalTime >= spawnInterval)
		{
			while (stack.getNumEnemies() < maxEnemies)
			{
				int tmpX = rand() % (map_w - 1) + 1;
				int tmpY = rand() % (map_h - 1) + 1;
				int tmpIdx = tmpY * map_w + tmpX;
				if ((map[tmpIdx] == L' ') && tmpIdx != int(yPlayer) * map_w + int(xPlayer))
				{
					if (stack.getNumEnemies() < maxEnemies)
					{
						map[tmpY * map_w + tmpX] = L'E';
						e1.setX(tmpX);
						e1.setY(tmpY);
						stack.addEnemy(e1);
						totalTime = 0.0;
						break;
					}
					else
					{
						break;
					}
				}
			}
		}

		timeStart = chrono::system_clock::now();
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			anglePlayer -= speed * dt;
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			anglePlayer += speed * dt;
		}
		if (GetAsyncKeyState(0x57) & 0x8000)			// W-key
		{
			int tmpIdx = int(yPlayer - speed * cos(anglePlayer) * dt) * map_w + int(xPlayer - speed * sin(anglePlayer) * dt);
			if (map[tmpIdx] == L' ' || map[tmpIdx] == L'W')
			{
				yPlayer -= speed * cos(anglePlayer) * dt;
				xPlayer -= speed * sin(anglePlayer) * dt;
			}
			/*if (map[int(yPlayer)*map_w + int(xPlayer)] != L' ')
			{
				yPlayer += speed * cos(anglePlayer) * dt;
				xPlayer += speed * sin(anglePlayer) * dt;
			}*/
		}

		if (GetAsyncKeyState(0x53) & 0x8000)			// S-key
		{
			int tmpIdx = int(yPlayer + speed * cos(anglePlayer) * dt) * map_w + int(xPlayer + speed * sin(anglePlayer) * dt);
			if (map[tmpIdx] == L' ' || map[tmpIdx] == L'W')
			{
				yPlayer += speed * cos(anglePlayer) * dt;
				xPlayer += speed * sin(anglePlayer) * dt;
			}
			/*if (map[int(yPlayer) * map_w + int(xPlayer)] != L' ')
			{
				yPlayer -= speed * cos(anglePlayer) * dt;
				xPlayer -= speed * sin(anglePlayer) * dt;
			}*/
		}

		if (GetAsyncKeyState(0x44) & 0x8000)			// D-key
		{
			int tmpIdx = int(yPlayer - speed * sin(anglePlayer) * dt) * map_w + int(xPlayer + speed * cos(anglePlayer) * dt);
			if (map[tmpIdx] == L' ' ||map[tmpIdx] == L'W')
			{
				yPlayer -= speed * sin(anglePlayer) * dt;
				xPlayer += speed * cos(anglePlayer) * dt;
			}
			
			/*if (map[int(yPlayer) * map_w + int(xPlayer)] != L' ')
			{
				yPlayer += speed * sin(anglePlayer) * dt;
				xPlayer -= speed * cos(anglePlayer) * dt;
			}*/
		}

		if (GetAsyncKeyState(0x41) & 0x8000)			// A-key
		{
			int tmpIdx = int(yPlayer + speed * sin(anglePlayer) * dt) * map_w + int(xPlayer - speed * cos(anglePlayer) * dt);
			if (map[tmpIdx] == L' ' || map[tmpIdx] == L'W')
			{
				yPlayer += speed * sin(anglePlayer) * dt;
				xPlayer -= speed * cos(anglePlayer) * dt;
			}
			/*if (map[int(yPlayer) * map_w + int(xPlayer)] != L' ')
			{
				yPlayer -= speed * sin(anglePlayer) * dt;
				xPlayer += speed * cos(anglePlayer) * dt;
			}*/
		}

		if (GetAsyncKeyState(0x45) & 0x0001)		// E-Key
		{
			if (map[int(yPlayer) * map_w + int(xPlayer)] == L'W')
			{
				currWeapon = floorWeapon;
				map[int(yPlayer) * map_w + int(xPlayer)] = L' ';
			}

		}

		if (GetAsyncKeyState(0x52) & 0x0001)		// R-Key
		{
			if (currWeapon.getBulletCount() < currWeapon.getMagSize())
				reloading = true;

		}

		if ((GetAsyncKeyState(VK_SPACE) & 0x0001) && !reloading) // & 0x8000)
		{
			float distShot = 0.0;
			float shotX = xPlayer;
			float shotY = yPlayer;
			float shotRayX = -sin(anglePlayer);
			float shotRayY = -cos(anglePlayer);
			bool enemyHit = false;

			if (currWeapon.getBulletCount() > 0)
			{
				currWeapon.reduceBullets();
				while (distShot <= 30)
				{
					shotX += shotRayX * scaler;
					shotY += shotRayY * scaler;
					distShot += scaler;
					if (map[int(shotY) * map_w + (int(shotX))] != L' ' && map[int(shotY) * map_w + (int(shotX))] != L'W')
					{
						if (map[int(shotY) * map_w + (int(shotX))] == L'E')
						{
							enemyHit = true;
						}
						break;
					}
				}
			}

			if (enemyHit && stack.removeEnemy(int(shotX), int(shotY), currWeapon.getDmg()))
			{
				map[int(shotY) * map_w + int(shotX)] = L' ';
					
				totalTime = 0.0;
			}
		}

		for (int x = 0; x < screen_w; x++)
		{
			float fRayAngle = anglePlayer + fov / 2 - float(x) / (float(screen_w) - 1.0) * fov;
			float fRayX = -sin(fRayAngle);				// because of positive angle to the left (change sign)
			float fRayY = -cos(fRayAngle);				// y axis points downwards
			float testX = xPlayer;
			float testY = yPlayer;
			int lastRadarX = int(xPlayer);
			int lastRadarY = int(yPlayer);
			float distance = 0;
			short shade_w[] = {		0x2588,			// full 
									0x2593,			// dark shade
									0x2592,			// medium shade
									0x2591};		// light shade		
											
			wchar_t shade_f[] = {	L'#',
									L'x',
									L'-'};
			bool wallHit = false;
			bool cornerHit = false;
			bool enemyVisible = false;

			// making sure the ray isn't being cast too far.
			while (distance < 30)
			{
				testX += fRayX * scaler;
				testY += fRayY * scaler;
				distance += scaler;

				// safety check (redundant? because of decreased step size)
				if ((testX < 0) || testY < 0)
					break;
				if ((testX > map_w) || testY > map_h)
					break;

				if (map[int(testY) * map_w + int(testX)] == L'E')
				{
					enemyVisible = true;
					wallHit = true;
					break;
				}

				else if ((map[int(testY) * map_w + int(testX)] == L'#') || (map[int(testY) * map_w + int(testX)] == L'x'))
				{
					// map[int(testY) * map_w + int(testX)] = L'x';
					// only for testing purposes. map should NOT be changed in run time!
					// distance = distance;
					wallHit = true;
					break;
				}

				else if (x == int((screen_w-1 )/ 2))
				{
					if (numRadar < radarSize)
					{
						if ((int(testY) * map_w + int(testX) != lastRadarY * map_w + lastRadarX) && distance <= 4.0)
						{
							radar[numRadar].iX = int(testX);
							radar[numRadar].iY = int(testY);
							lastRadarX = int(testX);
							lastRadarY = int(testY);
							numRadar++;

						}
					}
				}
				
			}
			float distances[4];
			float angles[4];

			if (wallHit)
			{
				int tmp = 0;
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < 2; j++)
					{
							
						float vecBackX = (xPlayer - (int(testX) + i));
						float vecBackY = (yPlayer - (int(testY) + j));
						float d = sqrt(pow(vecBackX, 2) + pow(vecBackY, 2));
						float dot = vecBackX / d * fRayX + vecBackY / d * fRayY;
						angles[tmp] = 180 - acos(dot)*180/PI;
						distances[tmp] = d;
						tmp++;
					}
				}
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						if (distances[j + 1] < distances[j])
						{
							float tmp = distances[j + 1];
							distances[j + 1] = distances[j];
							distances[j] = tmp;
							tmp = angles[j + 1];
							angles[j + 1] = angles[j];
							angles[j] = tmp;
						}
					}
				}

				if ((angles[0] <= 0.8) || (angles[1]) <= 0.8)
					cornerHit = true;
			}

			int ceiling = int(screen_h / 2.0) - int(screen_h / 2.0 * 1.0 / (1.0 + distance));
			int floor = int(screen_h / 2.0) + int(screen_h / 2.0 * 1.0 / (1.0 + distance));
			// rendering walls, floor and ceiling 
			for (int y = 0; y < screen_h; y++)
			{
				if (y < ceiling)
				{
					sbuf[y * screen_w + x] = L' ';
				}
				else if (y > floor)
				{ 	
					if (y >= 33.0)
						sbuf[y * screen_w + x] = shade_f[0];
					else if (y >= 27.0)
						sbuf[y * screen_w + x] = shade_f[1];
					else if (y >= 20.0)
						sbuf[y * screen_w + x] = shade_f[2];
						
				}
				else		// drawing the walls with shading
				{
					// draw the enemy (testing)
					if (enemyVisible)
					{
						if (cornerHit)
						{
							sbuf[y * screen_w + x] = L' ';
						}
						else if (distance <= 0.2)
						{
							sbuf[y * screen_w + x] = L'H';
						}
						else if (distance <= int(depth / 5))
						{ 
							sbuf[y * screen_w + x] = L'L';
						}
						else if (distance <= int(depth / 3))
						{
							sbuf[y * screen_w + x] = L'I';
						}
						else
						{
							sbuf[y * screen_w + x] = L'~';
						}
					}

					else if (cornerHit)
					{
						sbuf[y * screen_w + x] = L' ';
					}
					else if (distance <= 0.2)
						sbuf[y * screen_w + x] = shade_w[0];
					else if (distance <= int(depth/5))
						sbuf[y * screen_w + x] = shade_w[1];
					else if (distance <= int(depth/3))
						sbuf[y * screen_w + x] = shade_w[2];
					else
						sbuf[y * screen_w + x] = shade_w[3];
				}
			}
		}

		// PROBLEM:
		// Because of the length of the scalor (1 at the time), and mostly because i replaced every hit with an x
		// it was could check negative indexes of the map as the rays went throug the the gap (x, wasn't recognized 
		// as wall). For example: 
		// if y was -0.34 it would test the map with y = 0... which is fine.
		// if y was -1.23 it would test the map with y = 1 which is out of bounds. 
		// SOLVED:
		// out of bounds check whith if statements and adding the x as part of the wall for checks
		// To decrease the chance of going to far forwards with the test you can also reduce the distance of the step
		// by introducing a scaler with 0.1.

		// Draw x-hair on top of the screen

		for (int x = 0; x < 3; x++)
		{
			for (int y = 0; y < 2; y++)
			{
				sbuf[(y + 18)*screen_w + x + 58] = L'X';
			}
		}

		// Draw Weapon
		// Problem that happened because of the weapon drawing:
		// Because the screen buffer is a dynamic array its memory is allocated on the heap.
		// On the heap you don't have out of bounds errors, IT JUST WRITES there. Even though
		// the memory doesn't belong to the array.
		// In my case I drew the weapon (in the screen buffer) partially out of the bounds of the 
		// array. 
		// Since the enemies which are also on the heap, were initialized before the weapon drawing 
		// happened, it overwrote some of the pointers of the enemies, which then destroyed my 
		// linked list :(
		// In short:
		// Went out of bounds of sbuf and screwed my linked list memory.

		if (!reloading)
		{
			for (int i = 0; i < 15; i++)		// 70
			{
				int k = i - 7;
				for (int j = 0; j < 8; j++)
				{
					if ((i + 25) * screen_w + j + i + 70 < screen_w * screen_h)
						sbuf[(i + 25) * screen_w + j + i + 70] = 0x2588;
				}
				if (i >= 7)
				{
					for (int j = 0; j < k; j++)
					{
						if ((i + 25) * screen_w + j + i + 70 < screen_w * screen_h)
							sbuf[(i + 25) * screen_w + j + 77] = 0x2588;
					}
				}
				if (i >= 7)
				{
					for (int j = 0; j < k; j++)
					{
						if ((i + 25) * screen_w + j + i + 70 < screen_w * screen_h)
							sbuf[(i + 25) * screen_w + j + 90] = 0x2588;
					}
				}
			}
		}
		else
		{
			reloadTime += dt;
			if (reloadTime >= currWeapon.getReloadTime())
			{	
				currWeapon.reload();
				reloading = false;
				reloadTime = 0.0;
			}
		}

		// Draw the map "on top of the screen" ===========================================================

		for (int x = 0; x < map_w; x++)
		{
			for (int y = 0; y < map_h; y++)
			{
				sbuf[(y)*screen_w + x + 1] = map[y * map_w + x];
			}
		}

		for (int idx = 0; idx < numRadar; idx++)
		{
			sbuf[radar[idx].iY * screen_w + radar[idx].iX + 1] = L'.';
		}

		// Draw ammo count ==============================================================================

		// blank space for ammo display
		for (int x = 0; x < currWeapon.getBulletCount() + 2; x++)
		{
			for (int y = 0; y < 3; y++)
			{

				sbuf[(y + 35) * screen_w + (x + 4)] = L' ';
			}
		}

		// draw the bullet count (mw2 like)
		for (int x = 0; x < currWeapon.getBulletCount(); x++)
		{
			sbuf[36 * screen_w + (x + 5)] = L'|';
		}

		sbuf[(int(yPlayer)) * screen_w + int(xPlayer) + 1] = L'o';
		sbuf[screen_h * screen_w - 1] = '\0';
		WriteConsoleOutputCharacter(sbuf_h, sbuf, screen_w * screen_h, { 0,0 }, &dwBytesWritten);
		timeEnd = chrono::system_clock::now();
	}

		// End of Game Loop ==============================================================================

		return 0;
}

