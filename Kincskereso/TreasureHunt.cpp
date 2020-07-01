#include <iostream>

#include "TreasureHuntGameController.h"
#include "MainMenuController.h"


const unsigned int ScreenWidth = 90;
const unsigned int ScreenHeight = 50;

int main() {
	// remove when game loop is ready
	// drawn frame wont be visible othervise

	ConsoleView consoleView(ScreenWidth, ScreenHeight);
	InputController inputController;

	TreasureHuntGameController game(inputController, consoleView);
	MainMenuController menu(inputController, consoleView);

	while (true) {
		auto play = menu.Run();
		if (!play) {
			break;
		}
		game.Run();
	}

	return 0; 
}