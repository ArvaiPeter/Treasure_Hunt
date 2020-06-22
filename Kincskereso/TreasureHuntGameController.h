#pragma once

#include "ConsoleView.h"
#include "TreasureHuntGameModel.h"
#include "InputController.h"

class TreasureHuntGameController
{
public:
	TreasureHuntGameController();

	void Run();

private:
	void DrawFrame();
	void MovePlayer(const unsigned int& toX,
					const unsigned int& toY	);

private:
	ConsoleView m_View;
	TreasureHuntGameModel m_Model;
	InputController m_InputController;

	bool m_GameHasEnded;

	static const unsigned int m_ScreenWidth;
	static const unsigned int m_ScreenHeight;
};

