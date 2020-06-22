#include <thread>
#include <chrono>

#include "TreasureHuntGameController.h"
#include "DrawRect.h"

const unsigned int TreasureHuntGameController::m_ScreenWidth = 80;
const unsigned int TreasureHuntGameController::m_ScreenHeight = 50;


TreasureHuntGameController::TreasureHuntGameController()
	: m_View(ConsoleView(m_ScreenWidth, m_ScreenHeight)),
	m_Model(TreasureHuntGameModel()),
	m_InputController(),
	m_GameHasEnded(false)
{
	// SETUP =======================================
	m_Model.LoadLevel(L"level_1.txt");
}

void TreasureHuntGameController::Run() {

	while (!m_GameHasEnded) {
		// TIMING  =================================
		// TODO: rework
		std::this_thread::sleep_for(std::chrono::microseconds(100));

		// INPUT  =================================
		m_InputController.GetInput();

		// LOGIC   =================================
		auto& lvl = m_Model.GetLevel();
		auto player = m_Model.GetPlayer();

		const char changedInput = m_InputController.ButtonsPressed();

		if (changedInput & UP) {
			player->Move(0, -1);
		}
		else if (changedInput & LEFT) {
			player->Move(-1, 0);
		}
		else if (changedInput & DOWN) {
			player->Move(0, 1);
		}
		else if (changedInput & RIGHT) {
			player->Move(1, 0);
		}

		// DISPLAY =================================
		DrawFrame();
	}
}

void TreasureHuntGameController::DrawFrame() {
	auto lvl = m_Model.GetLevel();
	auto player = m_Model.GetPlayer();
	auto levelDimensions = m_Model.GetLevelDimensions();

	std::vector<DrawRect> screenElements;

	std::wstring pField;
	for (const auto& gameObject : lvl) {
		pField += gameObject->GetRepresentation();
	}


	if (player) {
		auto playerIndex = player->Y() * levelDimensions.first + player->X();
		pField[playerIndex] = player->GetRepresentation();
	}

	screenElements.emplace_back(0, 0, levelDimensions.first, levelDimensions.second, pField);

	m_View.DrawFrame(screenElements);
}