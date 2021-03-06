#pragma once

#include "ConsoleView.h"
#include "TreasureHuntGameModel.h"
#include "InputController.h"
#include "Solver.h"
#include "GameOutcome.h"

class TreasureHuntGameController
{
public:
	TreasureHuntGameController(InputController& inputController, ConsoleView& consoleView);

	void ResetGame();

	GAME_OUTCOME Run();


private:
	void DrawFrame();
	void MovePlayer(const unsigned int& toX,
					const unsigned int& toY	);

	void OnButtonPressed(BUTTON btn);

private:
	ConsoleView& m_View;
	InputController& m_InputController;
	TreasureHuntGameModel m_Model;
	std::unique_ptr<MissionControll> m_Solver;

	bool m_GameHasEnded;
	bool m_UserHasControll;
};

