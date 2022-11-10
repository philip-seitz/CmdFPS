#include <iostream>
#include <Windows.h>
#include <string>
#define PI 3.14;
#define FOV 60.0
using namespace std;

int main()
{
	// Player Attribrutes =============================================
	float xPlayer = 15;
	float yPlayer = 4;
	float anglePlayer = 0;
	float fov = FOV/180.0*PI;

	// Screen Buffer initialization ===================================
	int screen_w = 120;
	int screen_h = 40;
	wchar_t* sbuf = new wchar_t[screen_w * screen_h];

	for (int x = 0; x < screen_w; x++)
	{
		for (int y = 0; y < screen_h; y++)
		{
			sbuf[y * screen_w + x] = L' ';
		}
	}
	
	// Map initialization =============================================

	int map_w = 30;
	int map_h = 7;
	wstring map;

	map += L"##############################";
	map += L"#                            #";
	map += L"#                            #";
	map += L"#                            #";
	map += L"#                            #";
	map += L"#                            #";
	map += L"##############################";

	HANDLE sbuf_h = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	DWORD dwBytesWritten = 0;
	SetConsoleActiveScreenBuffer(sbuf_h);
	bool wallHit = false;

	// Game Loop ====================================================== 
	while (1)
	{
		for (int i = 0; i < screen_w; i++)
		{
			float fRayAngle = anglePlayer + fov / 2 - float(i) / (float(screen_w) - 1.0) * fov;
			float fRayX = -sin(fRayAngle);				// because of positive angle to the left (change sign)
			float fRayY = -cos(fRayAngle);				// y axis points downwards
			float testX = xPlayer;
			float testY = yPlayer;

			for (int j = 0; j < 40; j++)
			{
				
				testX += fRayX;
				testY += fRayY;

				if ((map[int(testY) * map_w + int(testX)] == L'#')&&(int(testY)>=0)&&(int(testX)>=0))
				{
					// map[int(testY) * map_w + int(testX)] = L'x';
					wallHit = true;
					break;
				}
			}
		}
		// Problem hierbei (getroffenes Stück ersetzen war, dass wenn ich ein Stück wand wegnehme der Ray nach außen gehen kann 
		// Ansonsten funktioniert es soweit eigentlich 

		for (int x = 0; x < map_w; x++)
		{
			for (int y = 0; y < map_h; y++)
			{
				sbuf[(y + 5) * screen_w + x + 5] = map[y * map_w + x];
			}
		}
		WriteConsoleOutputCharacter(sbuf_h, sbuf, screen_w * screen_h, { 0,0 }, &dwBytesWritten);
	}

	// comment from Desktop

	return 0;
}
