#pragma once

#include "Button.h"

#include <string>
#include <vector>
#include <functional>

#define BIT_MASK(x) 1<<x

#define _W 1<<0
#define _A 1<<1
#define _S 1<<2
#define _D 1<<3
#define _F 1<<4
#define _ENTER 1<<5

class InputEvent {
public:
	void Subscribe(std::function<void(BUTTON)> subscriber);
	void RaiseEvent(BUTTON btn);

	std::vector< std::function<void(BUTTON)> > GetSubscribers() const;

private:
	std::vector< std::function<void(BUTTON)> > m_Subscribers;
};

class InputController
{
public:
	InputController();

	void GetInput();

private:
	void RaiseButtonPressed();
	void RaiseInputChanged();

	void RaiseEvent(InputEvent& evemt, char buttons);

public:
	InputEvent ButtonPressedEvent;
	InputEvent ButtonChangedEvent;

private:
	char m_CurrInput;
	char m_PrevInput;

	static const std::string keysWatched;
};

