#include <iostream>

#include "TreasureHuntGameController.h"
#include "MainMenuController.h"
#include "Constants.h"

int main() {
	ConsoleView consoleView(ScreenWidth, ScreenHeight);
	InputController inputController;

	TreasureHuntGameController game(inputController, consoleView);
	MainMenuController menu(inputController, consoleView);

	while (true) {
		auto play = menu.Run();
		if (!play) {
			break;
		}
		auto outcome = game.Run();
		menu.SetLastOutCome(outcome);
		game.ResetGame();
	}

	return 0; 
}