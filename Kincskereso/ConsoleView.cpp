#include "ConsoleView.h"

ConsoleView::ConsoleView(const unsigned int& screenWidth, const unsigned int& screenHeight)
	: m_ScreenWidth(screenWidth),
	m_ScreenHeight(screenHeight),
	m_Screen(new wchar_t[m_ScreenWidth * m_ScreenHeight]),
	m_ConsoleHandle(CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL)),
	m_BytesWritten()
{
	for (size_t i = 0; i < m_ScreenWidth * m_ScreenHeight; ++i) {
		m_Screen[i] = L' ';
	}

	// resizing window
	_COORD coord;
	coord.X = m_ScreenWidth;
	coord.Y = m_ScreenHeight;

	_SMALL_RECT rect;
	rect.Left = 0;
	rect.Top = 0;
	rect.Right = m_ScreenWidth - 1;
	rect.Bottom = m_ScreenHeight - 1;

	SetConsoleScreenBufferSize(m_ConsoleHandle, coord);
	SetConsoleWindowInfo(m_ConsoleHandle, TRUE, &rect);

	SetConsoleActiveScreenBuffer(m_ConsoleHandle);
}

void ConsoleView::DrawFrame() {
	WriteConsoleOutputCharacter(m_ConsoleHandle, m_Screen, m_ScreenWidth*m_ScreenHeight, { 0,0 }, &m_BytesWritten);
}

void ConsoleView::SetScreenBuffer(wchar_t* newScreenBuff) {
	m_Screen = newScreenBuff;
}

unsigned int ConsoleView::ConvertIndexingFrom2D(const unsigned int& x, const unsigned int& y) {
	return x * m_ScreenWidth + y;
}
