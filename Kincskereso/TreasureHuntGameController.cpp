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

		const char buttonsPressed = m_InputController.ButtonsPressed();
		unsigned int dx = 0;
		unsigned int dy = 0;

		if (buttonsPressed & UP)				--dy;
		else if (buttonsPressed & LEFT)			--dx;
		else if (buttonsPressed & DOWN)			++dy;
		else if (buttonsPressed & RIGHT)		++dx;

		MovePlayer(dx, dy);

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

void TreasureHuntGameController::MovePlayer(
	const unsigned int& dX,
	const unsigned int& dY
) {
	auto player = m_Model.GetPlayer();
	auto lvl = m_Model.GetLevel();
	auto lvlIndex = (player->Y() + dY) * m_Model.GetLevelDimensions().first + (player->X() + dX);

	if (auto env = std::dynamic_pointer_cast<Environment>(lvl[lvlIndex])) {
		switch (env->GetEnvType()) {
		case EnvironmentType::WALL:
			return;
		case EnvironmentType::TRAP:
			env->Interact(player.get());
			player->Move(dX, dY);
			break;
		case EnvironmentType::EXIT:
			player->Move(dX, dY);
			m_GameHasEnded = true;
			break;
		case EnvironmentType::PATH:
			player->Move(dX, dY);
			break;
		}
	}
	else if (auto consumable = std::dynamic_pointer_cast<Consumable>(lvl[lvlIndex])) {
		consumable->Interact(player.get());
		player->Move(dX, dY);
	}
	else if (auto beast = std::dynamic_pointer_cast<Beast>(lvl[lvlIndex])) {
		if (beast->IsAlive()) {
			beast->Interact(player.get());

			if (player->GetHealth() == 0) {
				m_GameHasEnded = true;
			}
		}
		player->Move(dX, dY);
	}

}