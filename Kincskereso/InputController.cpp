#include "InputController.h"

#include <Windows.h>

// INPUT EVENT =============================================
void InputEvent::Subscribe(std::function<void(BUTTON)> subscriber)
{
	m_Subscribers.push_back(subscriber);
}

void InputEvent::RaiseEvent(BUTTON btn) {
	for (const auto& sub : m_Subscribers) {
		sub(btn);
	}
}

std::vector< std::function<void(BUTTON)> > InputEvent::GetSubscribers() const {
	return m_Subscribers;
}


// INPUT CONTROLLER ==========================================
const std::string InputController::keysWatched = "WASDF\x0D";

InputController::InputController()
	: m_CurrInput(0), m_PrevInput(0) {}

void InputController::GetInput() {

	m_PrevInput = m_CurrInput;
	m_CurrInput = 0;


	for (size_t i = 0; i < keysWatched.length(); ++i) {
		if (0x8000 & GetAsyncKeyState((unsigned char)(keysWatched[i]))) {
			m_CurrInput |= BIT_MASK(i);
		}
	}

	RaiseButtonPressed();
	RaiseInputChanged();
}

void InputController::RaiseButtonPressed() {
	char buttonsPressed = (char)((m_CurrInput ^ m_PrevInput) & m_CurrInput);
	RaiseEvent(ButtonPressedEvent, buttonsPressed);
}

void InputController::RaiseInputChanged() {
	char changedButtons = (char)(m_CurrInput ^ m_PrevInput);
	
	RaiseEvent(ButtonChangedEvent, changedButtons);
}

void InputController::RaiseEvent(InputEvent& event, char buttons) {
	if (buttons & _W) event.RaiseEvent(BUTTON::UP);
	if (buttons & _A) event.RaiseEvent(BUTTON::LEFT);
	if (buttons & _S) event.RaiseEvent(BUTTON::DOWN);
	if (buttons & _D) event.RaiseEvent(BUTTON::RIGHT);
	if (buttons & _F) event.RaiseEvent(BUTTON::TOGGLE_SOLVER);
	if (buttons & _ENTER) event.RaiseEvent(BUTTON::SELECT);
}