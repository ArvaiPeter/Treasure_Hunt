#include "InputController.h"

#include <Windows.h>

InputController::InputController()
	: m_CurrInput(0), m_PrevInput(0) {}

void InputController::GetInput() {

	m_PrevInput = m_CurrInput;
	m_CurrInput = 0;

	for (size_t i = 0; i < 5; ++i) {
		if (0x8000 & GetAsyncKeyState((unsigned char)("WASDF"[i]))) {
			m_CurrInput |= BIT_MASK(i);
		}
	}
}

const char InputController::InputChanged() {
	return (char)(m_CurrInput ^ m_PrevInput);
}

const char InputController::ButtonsPressed() {
	return (char)((m_CurrInput ^ m_PrevInput) & m_CurrInput);
}