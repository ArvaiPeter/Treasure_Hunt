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
	m_GameHasEnded(false),
	m_UserHasControll(true)
{
	// SETUP =======================================
	m_Model.LoadLevel(L"level_1.txt");
	m_Solver = std::make_unique<MissionControll>(m_Model);
}

void TreasureHuntGameController::Run() {

	std::stack<Node*> pathStack;

	bool solved = false;

	while (!m_GameHasEnded) {
		// TIMING  =================================
		// TODO: rework
		if (!m_UserHasControll) {
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}		

		// INPUT  =================================
		m_InputController.GetInput();

		// LOGIC   =================================
		auto& lvl = m_Model.GetLevel();
		auto player = m_Model.GetPlayer();

		const char buttonsPressed = m_InputController.ButtonsPressed();

		if (buttonsPressed & TOGGLE_SOLVER) {
			m_UserHasControll = !m_UserHasControll;
		}
		
		int dx = 0;
		int dy = 0;

		if (m_UserHasControll) {

			if (buttonsPressed & UP)				--dy;
			else if (buttonsPressed & LEFT)			--dx;
			else if (buttonsPressed & DOWN)			++dy;
			else if (buttonsPressed & RIGHT)		++dx;

		}
		else {
			if (!solved) {
				solved = m_Solver->SolveLevel();
			}
			else {
				auto nextStep = m_Solver->GetNextStep();
				dx = nextStep->X() - player->X();
				dy = nextStep->Y() - player->Y();
			}
		}

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

	// PLAYING FIELD
	std::wstring pField;
	for (const auto& gameObject : lvl) {
		pField += gameObject->GetRepresentation();
	}

	if (player) {
		auto playerIndex = player->Y() * levelDimensions.first + player->X();
		pField[playerIndex] = player->GetRepresentation();
	}

	screenElements.emplace_back(0, 0, levelDimensions.first, levelDimensions.second, pField);

	// PLAYER STATS
	std::wstring health = L"HEALTH: ";
	health += std::to_wstring(player->GetHealth());
	screenElements.emplace_back(levelDimensions.first + 3, 0, health.length() , 1, health);
	
	std::wstring weapon = L"WEAPON: ";
	weapon += player->IsArmed() ? L"SWORD" : L"NONE";
	screenElements.emplace_back(levelDimensions.first + 3, 1, weapon.length(), 1, weapon);

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