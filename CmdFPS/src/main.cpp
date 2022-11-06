#include <iostream>
#include <Windows.h>
#include <string>
using namespace std;

int main()
{
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


	// Game Loop ====================================================== 
	while (1)
	{
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
