#pragma once

#define BIT_MASK(x) 1<<x

#define UP 1<<0
#define LEFT 1<<1
#define DOWN 1<<2
#define RIGHT 1<<3
#define TOGGLE_SOLVER 1<<4

class InputController
{
public:
	InputController();

	void GetInput();
	// TODO: better names :S
	const char InputChanged();
	const char ButtonsPressed();

private:
	char m_CurrInput;
	char m_PrevInput;
};

