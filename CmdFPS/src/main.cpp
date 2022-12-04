#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <string>
#include <chrono>
#include <time.h>
#include <ctime>

#include "Enemy.h"
#define PI 3.14;
#define FOV 60.0
using namespace std;

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
	// FOV should be equal to or below 120° (usual FOV cap)

	// Enemy Attributes ==============================================================================

	int maxEnemies = 5;

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
	cEnemy tmp;
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

	// Enemy Attributes for testing ==================================================================
	cEnemyStack stack;
	for (int i = 0; i < 1; i++)
	{
		cEnemy e1;
		while (1)
		{
			int tmpX = rand() % (map_w - 1) + 1;
			int tmpY = rand() % (map_h - 1) + 1;
			if ((map[tmpY * map_w + tmpX] == L' ') && tmpX != xPlayer)
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
		
		// change the position of the enemy every 10 seconds
		/*if (totalTime >= 10.0)
		{
			totalTime = 0.0;
			map[yEnemy * map_w + xEnemy] = L' ';
			do {
				xEnemy = rand() % (map_w - 1) + 1;
				yEnemy = rand() % (map_h - 1) + 1;
			} while ((map[yEnemy * map_w + xEnemy] != L' ') && (xPlayer != xEnemy));
			map[yEnemy * map_w + xEnemy] = L'E';
		}*/

		if (totalTime >= 4.0)
		{
			
			cEnemy e1;
			int tmpX = rand() % (map_w - 1) + 1;
			int tmpY = rand() % (map_h - 1) + 1;
			if ((map[tmpY * map_w + tmpX] == L' ') && tmpX != xPlayer)
			{
				if (stack.getNumEnemies() < 5)
				{
					map[tmpY * map_w + tmpX] = L'E';
					e1.setX(tmpX);
					e1.setY(tmpY);
					stack.addEnemy(e1);
					totalTime = 0.0;
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
			yPlayer -= speed * cos(anglePlayer) * dt;
			xPlayer -= speed * sin(anglePlayer) * dt;
			if (map[int(yPlayer)*map_w + int(xPlayer)] != L' ')
			{
				yPlayer += speed * cos(anglePlayer) * dt;
				xPlayer += speed * sin(anglePlayer) * dt;
			}
		}

			if (GetAsyncKeyState(0x53) & 0x8000)			// S-key
			{
				yPlayer += speed * cos(anglePlayer) * dt;
				xPlayer += speed * sin(anglePlayer) * dt;
				if (map[int(yPlayer) * map_w + int(xPlayer)] != L' ')
				{
					yPlayer -= speed * cos(anglePlayer) * dt;
					xPlayer -= speed * sin(anglePlayer) * dt;
				}
			}

			if (GetAsyncKeyState(0x44) & 0x8000)			// D-key
			{
				yPlayer -= speed * sin(anglePlayer) * dt;
				xPlayer += speed * cos(anglePlayer) * dt;
				if (map[int(yPlayer) * map_w + int(xPlayer)] != L' ')
				{
					yPlayer += speed * sin(anglePlayer) * dt;
					xPlayer -= speed * cos(anglePlayer) * dt;
				}
			}

			if (GetAsyncKeyState(0x41) & 0x8000)			// A-key
			{
				yPlayer += speed * sin(anglePlayer) * dt;
				xPlayer -= speed * cos(anglePlayer) * dt;
				if (map[int(yPlayer) * map_w + int(xPlayer)] != L' ')
				{
					yPlayer -= speed * sin(anglePlayer) * dt;
					xPlayer += speed * cos(anglePlayer) * dt;
				}
			}

			if (GetAsyncKeyState(VK_SPACE) & 0x0001) // & 0x8000)
			{
				float distShot = 0.0;
				float shotX = xPlayer;
				float shotY = yPlayer;
				float shotRayX = -sin(anglePlayer);
				float shotRayY = -cos(anglePlayer);
				bool enemyHit = false;

				while (distShot <= 30)
				{
					shotX += shotRayX * scaler;
					shotY += shotRayY * scaler;
					distShot += scaler;
					if (map[int(shotY) * map_w + (int(shotX))] != L' ')
					{
						if (map[int(shotY) * map_w + (int(shotX))] == L'E')
						{
							enemyHit = true;
						}
						break;
					}
				}

				if (enemyHit)
				{
					map[int(shotY) * map_w + int(shotX)] = L' ';
					stack.removeEnemy(int(shotX), int(shotY));
					totalTime = 0.0;
					/*map[e1.getPosY() * map_w + e1.getPosX()] = L' ';
					e1.setX(0);
					e1.setY(0);*/
					/*do {
						xEnemy = rand() % (map_w - 1) + 1;
						yEnemy = rand() % (map_h - 1) + 1;
					} while ((map[yEnemy * map_w + xEnemy] != L' ') && (xPlayer != xEnemy));
					map[yEnemy * map_w + xEnemy] = L'E';*/

					/*while (1)
					{
						int tmpX = rand() % (map_w - 1) + 1;
						int tmpY = rand() % (map_h - 1) + 1;
						if ((map[tmpY * map_w + tmpX] == L' ') && tmpX != xPlayer)
						{
							map[tmpY * map_w + tmpX] = L'E';
							e1.setX(tmpX);
							e1.setY(tmpY);
							break;
						}
					}*/

				}
			}

			for (int x = 0; x < screen_w; x++)
			{
				float fRayAngle = anglePlayer + fov / 2 - float(x) / (float(screen_w) - 1.0) * fov;
				float fRayX = -sin(fRayAngle);				// because of positive angle to the left (change sign)
				float fRayY = -cos(fRayAngle);				// y axis points downwards
				float testX = xPlayer;
				float testY = yPlayer;
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

			for (int i = 0; i < 20; i++)		// 70
			{
				int k = i - 7;
					for (int j = 0; j < 8; j++)
						sbuf[(i + 25) * screen_w + j + i + 70] = 0x2588;

					if ( i > 8)
					{
						
						
						for (int j = 0; j < k; j++)
							sbuf[(i + 25) * screen_w + j + 77] = 0x2588;
						
					}
					if (i > 8)
					{


						for (int j = 0; j < k; j++)
							sbuf[(i + 23) * screen_w + j + 90] = 0x2588;

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
			sbuf[(int(yPlayer)) * screen_w + int(xPlayer) + 1] = L'o';

			WriteConsoleOutputCharacter(sbuf_h, sbuf, screen_w * screen_h, { 0,0 }, &dwBytesWritten);
			timeEnd = chrono::system_clock::now();
		}

		// End of Game Loop ==============================================================================

		return 0;
	}

