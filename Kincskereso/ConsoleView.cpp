#include "ConsoleView.h"

ConsoleView::ConsoleView(const int& screenWidth, const int& screenHeight, const HANDLE* consoleHandle)
	: IView(screenWidth, screenHeight), m_ConsoleHandle(consoleHandle), m_BytesWritten()
{
	m_Screen = new wchar_t[m_ScreenWidth * m_ScreenHeight];
	for (int i = 0; i < m_ScreenWidth * m_ScreenHeight; ++i) {
		m_Screen[i] = L'*'; // for testing;
	}
}

ConsoleView::~ConsoleView() {
	delete m_Screen;
	// m_ConsoleHandle is not owned by this class -> not deleting it
}

void ConsoleView::DrawFrame() {
	WriteConsoleOutputCharacter(*m_ConsoleHandle, m_Screen, m_ScreenWidth * m_ScreenHeight, { 0,0 }, &m_BytesWritten);
}
