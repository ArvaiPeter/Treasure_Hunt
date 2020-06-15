#include <iostream>

#include "ConsoleView.h"
#include "TreasureHuntGameModel.h"

int main() {

	const unsigned int windowWidth = 80;
	const unsigned int windowHeight = 50;

	TreasureHuntGameModel model;
	model.LoadLevel(L"level_1.txt");

	//maybe move this to ConsoleView
	

	auto levelDimensions = model.GetLevelDimensions();
	ConsoleView testView(windowWidth,windowHeight);

	//TODO move to contoller
	auto& lvl = model.GetLevel();
	wchar_t* newScreenBuff = new wchar_t[windowWidth * windowHeight];

	size_t elementCount = 0;
	for (auto x = 0; x < windowHeight; ++x) {
		for (auto y = 0; y < windowWidth; ++y) {
			auto index = y + windowWidth * x;
			wchar_t c = L'#';

			if (y >= levelDimensions.first || x >= levelDimensions.second) {
				c = L' ';
			}
			else {
				c = lvl[elementCount++].GetRepresentation();
			}

			newScreenBuff[index] = c;
		}
	}

	testView.SetScreenBuffer(newScreenBuff);
	testView.DrawFrame();

	// remove when game loop is ready
	// drawn frame wont be visible othervise
	while (true);

	return 0; 
}