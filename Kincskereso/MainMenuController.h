#pragma once

#include "InputController.h"
#include "ConsoleView.h"
#include "GameOutcome.h"
#include "MainMenuModel.h"

class MainMenuController
{
public:
	MainMenuController(InputController& inputController, ConsoleView& consoleView);

	bool Run();
	void SetLastOutCome(GAME_OUTCOME lastOutcome);

private:
	void Display();

	void OnButtonPressed(BUTTON btn);

private:
	InputController& m_InputController;
	ConsoleView& m_View;
	MainMenuModel m_Model;
	GAME_OUTCOME m_LastOutcome;

	uint8_t m_SelectedMenuItemIndex;
	bool m_MenuItemSelected;
};

