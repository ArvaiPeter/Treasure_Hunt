#pragma once

#include "ConsoleView.h"
#include "TreasureHuntGameModel.h"

class TreasureHuntGameController
{
public:
	TreasureHuntGameController();

	void Run();

private:
	ConsoleView m_View;
	TreasureHuntGameModel m_Model;

	bool m_GameHasEnded;

	static const unsigned int m_ScreenWidth;
	static const unsigned int m_ScreenHeight;
};

