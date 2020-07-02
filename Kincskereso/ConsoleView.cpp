#include "ConsoleView.h"

ConsoleView::ConsoleView(const unsigned int& screenWidth, const unsigned int& screenHeight)
	: m_ScreenWidth(screenWidth),
	m_ScreenHeight(screenHeight),
	m_Screen(new wchar_t[m_ScreenWidth * m_ScreenHeight]),
	m_ConsoleHandle(CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL)),
	m_BytesWritten()
{
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

ConsoleView::~ConsoleView() {
	delete m_Screen;
}

void ConsoleView::DrawFrame(const std::vector<DrawRect>& elementsToDraw) {
	
	ClearScreen();

	for (const auto& screenElement : elementsToDraw) {
		for (auto y = 0; y < m_ScreenHeight; ++y) {
			for (auto x = 0; x < m_ScreenWidth; ++x) {
				if (
					x >= screenElement.X &&
					x < screenElement.X + screenElement.Width &&
					y >= screenElement.Y &&
					y < screenElement.Y + screenElement.Height
					)
				{
					unsigned int elementIndex = (y - screenElement.Y) * screenElement.Width + (x - screenElement.X);
					unsigned int screenIndex = y * m_ScreenWidth + x;
					m_Screen[screenIndex] = screenElement.Data[elementIndex];
				}
			}
		}
	}
	
	if (m_Screen) {
		WriteConsoleOutputCharacter(m_ConsoleHandle, m_Screen, m_ScreenWidth * m_ScreenHeight, { 0,0 }, &m_BytesWritten);
	}
}

void ConsoleView::ClearScreen() {
	for (auto i = 0; i < m_ScreenWidth * m_ScreenHeight; ++i) {
		m_Screen[i] = L' ';
	}
}