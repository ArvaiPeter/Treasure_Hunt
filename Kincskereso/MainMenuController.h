#pragma once

#include "InputController.h"
#include "ConsoleView.h"
#include "GameOutcome.h"

class MainMenuController
{
public:
	MainMenuController(InputController& inputController, ConsoleView& consoleView);

	bool Run();
	void SetLastOutCome(GAME_OUTCOME lastOutcome);

private:
	void Display(const uint8_t selectedMenuItem);

private:
	InputController& m_InputController;
	ConsoleView& m_View;
	GAME_OUTCOME m_LastOutcome;

	static std::vector< std::pair<std::wstring, bool> > m_MenuItems;
};

