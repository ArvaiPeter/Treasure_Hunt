#include <thread>
#include <chrono>

#include "TreasureHuntGameController.h"
#include "DrawRect.h"

const unsigned int TreasureHuntGameController::m_ScreenWidth = 80;
const unsigned int TreasureHuntGameController::m_ScreenHeight = 50;


TreasureHuntGameController::TreasureHuntGameController()
	: m_View(ConsoleView(m_ScreenWidth, m_ScreenHeight)),
	m_Model(TreasureHuntGameModel()),
	m_GameHasEnded(false)
{
	// SETUP =======================================
	m_Model.LoadLevel(L"level_1.txt");
}

void TreasureHuntGameController::Run() {

	bool prevPlayerMovements[4];
	bool playerMovements[4];

	while (!m_GameHasEnded) {
		// TIMING  =================================
		// TODO: rework
		std::this_thread::sleep_for(std::chrono::microseconds(100));

		// INPUT  =================================
		for (size_t i = 0; i < 4; ++i) {
			playerMovements[i] = (0x8000 & GetAsyncKeyState((unsigned char)("WASD"[i]))) != 0;
		}
		auto player = m_Model.GetPlayer();

		// LOGIC   =================================
		if (playerMovements[0] && playerMovements[0] != prevPlayerMovements[0]) {
			player->Move(0, -1);
		}
		else if (playerMovements[1] && playerMovements[1] != prevPlayerMovements[1]) {
			player->Move(-1, 0);
		}
		else if (playerMovements[2] && playerMovements[2] != prevPlayerMovements[2]) {
			player->Move(0, 1);
		}
		else if (playerMovements[3] && playerMovements[3] != prevPlayerMovements[3]) {
			player->Move(1, 0);
		}

		for (auto i = 0; i < 4; ++i) {
			prevPlayerMovements[i] = playerMovements[i];
			playerMovements[i] = false; 
		}

		auto& lvl = m_Model.GetLevel();


		// DISPLAY =================================
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
}