#pragma once
#ifndef _CONSOLE_API_H_
#define _CONSOLE_API_H_

void ConstructConsole(unsigned short nWidth, unsigned short nHeight, const wchar_t* wcTitle = L"Default Title",
	unsigned short nFontWidth = 10, unsigned short nFontHeight = 16,
		const wchar_t* wcFont = L"Consolas", const wchar_t* wcCodeFormat = L"C");

#endif //_CONSOLE_API_H_
