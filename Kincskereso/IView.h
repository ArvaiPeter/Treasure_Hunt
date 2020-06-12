#pragma once

#include <Windows.h>

class IView {

public:
	IView(const int& screenWidth, const int& screenHeight)
		: m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight) {}

	virtual ~IView() = default;

	virtual void DrawFrame() = 0;

protected:
	int m_ScreenWidth;
	int m_ScreenHeight;
};