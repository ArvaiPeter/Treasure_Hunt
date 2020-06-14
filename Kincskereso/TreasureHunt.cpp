#include <iostream>

#include "ConsoleView.h"

int main() {

	HANDLE consoleHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(consoleHandle);

	ConsoleView testView(10, 10, &consoleHandle);

	testView.DrawFrame();

	// remove when game loop is ready
	// drawn frame wont be visible othervise
	while (true);

	return 0; 
}