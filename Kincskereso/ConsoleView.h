#pragma once

#include <Windows.h>
#include <memory>
#include <string>
#include <vector>

#include "DrawRect.h"

class ConsoleView
{
public:
	ConsoleView(const unsigned int& screenWidth, const unsigned int& screenHeight);
	~ConsoleView();

	void DrawFrame(const std::vector<DrawRect>& elementsToDraw);

private:
	void ClearScreen();

	//TODO: may not be necessary
	unsigned int ConvertIndexingFrom2D(const unsigned int& x, const unsigned int& y);

private:
	unsigned int m_ScreenWidth;
	unsigned int m_ScreenHeight;

	wchar_t* m_Screen;
	const HANDLE m_ConsoleHandle;
	DWORD m_BytesWritten;
};

