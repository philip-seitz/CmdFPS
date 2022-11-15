#include <iostream>
#include <Windows.h>
#include <string>
#include <chrono>
#include <ctime>

#define PI 3.14;
#define FOV 100.0
using namespace std;

int main()
{
	// Time Handling =================================================================================

	chrono::time_point<chrono::system_clock> timeStart, timeEnd;
	chrono::duration<float> timeElapsed;
	timeStart = chrono::system_clock::now();

	// Player Attribrutes ============================================================================
	float xPlayer = 15;
	float yPlayer = 4;
	float anglePlayer = 0;
	float fov = FOV/180.0*PI;			// rad = 2pi*angle/360
	// FOV should be equal to or below 120° (usual FOV cap)

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
	
	// Map initialization ============================================================================

	int map_w = 30;
	int map_h = 7;
	wstring map;

	map += L"##############################";		// 0
	map += L"#                            #";		// 1
	map += L"#                            #";		// 2
	map += L"#                            #";		// 3
	map += L"#                            #";		// 4			y 4- 1 , x 15-13.25; 
	map += L"#                            #";		// 5
	map += L"##############################";		// 6

	HANDLE sbuf_h = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	DWORD dwBytesWritten = 0;
	SetConsoleActiveScreenBuffer(sbuf_h);

	// Game Varialbes ================================================================================

	bool wallHit = false;
	float scaler = 0.1;
	
	
	// Start Game Loop ===============================================================================

	timeEnd = chrono::system_clock::now();
	while (1)
	{
		timeElapsed = timeEnd - timeStart;
		float dt = timeElapsed.count();

		timeStart = chrono::system_clock::now();
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			anglePlayer -= 3 * dt;
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			anglePlayer += 3 * dt;
		}
		if (GetAsyncKeyState(0x57) & 0x8000)			// W-key
		{
			yPlayer -= 1 * cos(anglePlayer) * dt;
			xPlayer -= 1 * sin(anglePlayer) * dt;
		}

		if (GetAsyncKeyState(0x53) & 0x8000)			// S-key
		{
			yPlayer += 1 * cos(anglePlayer) * dt;
			xPlayer += 1 * sin(anglePlayer) * dt;
		}

		if (GetAsyncKeyState(0x44) & 0x8000)			// D-key
		{
			 yPlayer -= 1 * sin(anglePlayer) * dt;
			 xPlayer += 1 * cos(anglePlayer) * dt;
		}

		if (GetAsyncKeyState(0x41) & 0x8000)			// A-key
		{
			yPlayer += 1 * sin(anglePlayer) * dt;
			xPlayer -= 1 * cos(anglePlayer) * dt;
		}


		for (int x = 0; x < screen_w; x++)
		{
			float fRayAngle = anglePlayer + fov / 2 - float(x) / (float(screen_w) - 1.0) * fov;
			float fRayX = -sin(fRayAngle);				// because of positive angle to the left (change sign)
			float fRayY = -cos(fRayAngle);				// y axis points downwards
			float testX = xPlayer;
			float testY = yPlayer;
			float distance = 0;
			wallHit = false;

			// making sure the ray isn't being cast too far.
			while(distance < 40)
			{
				testX += fRayX*scaler;
				testY += fRayY*scaler;
				distance+= scaler;

				// safety check (redundant? because of decreased step size)
				if ((testX < 0) || testY < 0)
					break;
				if ((testX > map_w) || testY > map_h)
					break;

				if ((map[int(testY) * map_w + int(testX)] == L'#')||(map[int(testY) * map_w + int(testX)] == L'x'))
				{
					map[int(testY) * map_w + int(testX)] = L'x';		
					// only for testing purposes. map should NOT be changed in run time!
					distance = distance;
					wallHit = true;
					break;
				}
			}
			
			int ceiling = screen_h / 2.0 - screen_h / 2.0 * 1.0 / (1.0 + distance);
			int floor = screen_h / 2.0 + screen_h / 2.0 * 1.0 / (1.0 + distance);
			// rendering walls, floor and ceiling 
			for (int y = 0; y < screen_h; y++)
			{
				if (y < ceiling)
				{
					sbuf[y * screen_w + x] = L' ';
				}
				else if (y > floor)
				{
					sbuf[y * screen_w + x] = L'/';
				}
				else
				{
					sbuf[y * screen_w + x] = L'#';
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


		// Draw the map "on top of the screen" ===========================================================

		for (int x = 0; x < map_w; x++)
		{
			for (int y = 0; y < map_h; y++)
			{
				sbuf[(y) * screen_w + x + 1] = map[y * map_w + x];
			}
		}
		sbuf[(int(yPlayer)) * screen_w + int(xPlayer) + 1] = L'o';

		WriteConsoleOutputCharacter(sbuf_h, sbuf, screen_w * screen_h, { 0,0 }, &dwBytesWritten);
		timeEnd = chrono::system_clock::now();
	}
	
	// End of Game Loop ==============================================================================

	return 0;
}
