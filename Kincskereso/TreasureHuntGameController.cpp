#include "TreasureHuntGameController.h"

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
	while (!m_GameHasEnded) {
		// TIMING  =================================

		// INOPUT  =================================
		

		// LOGIC   =================================
		auto& lvl = m_Model.GetLevel();


		// DISPLAY =================================
		auto levelDimensions = m_Model.GetLevelDimensions();
		wchar_t* screenBuffer = m_View.GetScreenBuffer();

		size_t elementCount = 0;
		for (auto x = 0; x < m_ScreenHeight; ++x) {
			for (auto y = 0; y < m_ScreenWidth; ++y) {
				auto screenBuffIndex = y + m_ScreenWidth * x;

				if (y >= levelDimensions.first || x >= levelDimensions.second) {
					screenBuffer[screenBuffIndex] = L' ';
				}
				else {
					screenBuffer[screenBuffIndex] = lvl[elementCount++].GetRepresentation();
				}
			}
		}

		m_View.DrawFrame();
	}
}