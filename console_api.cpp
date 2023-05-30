#include <conio.h>

#ifndef WINVER
#define WINVER 0x0A00
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#include <windows.h>
#include <clocale>

#include <cstdio>
#include <cstdlib>

#include "logger.h"

#include "console_api.h"

void ConstructConsole(unsigned short nWidth, unsigned short nHeight, const wchar_t* wcTitle,
	unsigned short nFontWidth, unsigned short nFontHeight,
	const wchar_t* wcFont, const wchar_t* wcCodeFormat)
{
	HWND hWnd = GetConsoleWindow();
	if (hWnd == NULL)
	{ exit(Logger::ShowFormatError(L"Invalid Handle", ERROR_INVALID_HANDLE)); }
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE)
	{ exit(Logger::ShowFormatError(L"Invalid Handle", ERROR_INVALID_HANDLE)); }

	SMALL_RECT rectWindow = {
		0x0, 0x0, 0x1, 0x1
	};

	SetConsoleWindowInfo(hConsole, TRUE, &rectWindow);

	COORD coord = { (SHORT)nWidth, (SHORT)nHeight };
	if (!SetConsoleScreenBufferSize(hConsole, coord))
	{ exit(Logger::ShowLastError(L"SetConsoleScreenBufferSize")); }

	if (!SetConsoleActiveScreenBuffer(hConsole))
	{ exit(Logger::ShowLastError(L"SetConsoleActiveScreenBuffer")); }

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = (SHORT)nFontWidth;
	cfi.dwFontSize.Y = (SHORT)nFontHeight;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;

	wcscpy_s(cfi.FaceName, wcFont);
	if (!SetCurrentConsoleFontEx(hConsole, FALSE, &cfi))
	{ exit(Logger::ShowLastError(L"SetCurrentConsoleFontEx")); }

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
	{ exit(Logger::ShowLastError(L"GetConsoleScreenBufferInfo")); }
	if ((SHORT)nWidth > csbi.dwMaximumWindowSize.X)
	{ exit(Logger::ShowLastError(L"Screen Width / Font Width Too Big")); }
	if ((SHORT)nHeight > csbi.dwMaximumWindowSize.Y)
	{ exit(Logger::ShowLastError(L"Screen Height / Font Height Too Big")); }
	
	rectWindow = { 0, 0, (SHORT)(nWidth - 1), (SHORT)(nHeight - 1) };
	if (!SetConsoleWindowInfo(hConsole, TRUE, &rectWindow))
	{ exit(Logger::ShowLastError(L"SetConsoleWindowInfo")); }

	SetConsoleTitle(wcTitle);
	if (_wsetlocale(LC_ALL, wcCodeFormat) == NULL)
	{ exit(Logger::ShowLastError(L"Setlocale")); }

	(void)Logger::LogMessage
		(Logger::LogLevel::LOG_LVL_INFO,
			L"[Function] : " __FUNCTION__ L" - Success!\t[File] : " __FILE__);
}