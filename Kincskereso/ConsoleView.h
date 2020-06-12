#pragma once

#include "IView.h"

#include <memory>

class ConsoleView : public IView
{
public:
	ConsoleView(const int& screenWidth, const int& screenHeight, const HANDLE* consoleHandle);
	~ConsoleView();

	void DrawFrame() override;

private:
	wchar_t* m_Screen;
	const HANDLE* m_ConsoleHandle;
	DWORD m_BytesWritten;
};

