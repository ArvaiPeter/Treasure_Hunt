#pragma once

#include "ConsoleView.h"
#include "TreasureHuntGameModel.h"
#include "InputController.h"
#include "Solver.h"

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
	std::unique_ptr<AstarPathFinder> m_Solver;

	bool m_GameHasEnded;
	bool m_UserHasControll;

	static const unsigned int m_ScreenWidth;
	static const unsigned int m_ScreenHeight;
};

