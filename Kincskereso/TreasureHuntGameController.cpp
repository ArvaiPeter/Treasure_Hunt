#include <thread>
#include <chrono>

#include "TreasureHuntGameController.h"
#include "DrawRect.h"




TreasureHuntGameController::TreasureHuntGameController(InputController& inputController, ConsoleView& consoleView)
	: m_View(consoleView),
	m_InputController(inputController),
	m_Model(TreasureHuntGameModel()),
	m_GameHasEnded(false),
	m_UserHasControll(true)
{
	// SETUP =======================================
	m_Model.LoadLevel(L"level_1.txt");
	m_Solver = std::make_unique<MissionControll>(m_Model);
	m_InputController.ButtonPressedEvent.Subscribe(
		std::bind(&TreasureHuntGameController::OnButtonPressed, this, std::placeholders::_1));
}

void TreasureHuntGameController::ResetGame() {
	m_UserHasControll = true;
	m_GameHasEnded = false;
	m_Model.ResetModel(); 
	m_Solver = std::make_unique<MissionControll>(m_Model);
}

GAME_OUTCOME TreasureHuntGameController::Run() {
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

		// maybe move to separate thread with timing
		if (!m_UserHasControll) {
			auto player = m_Model.GetPlayer();
			int dx = 0;
			int dy = 0;

			auto nextStep = m_Solver->GetNextStep();
			dx = nextStep->X() - player->X();
			dy = nextStep->Y() - player->Y();

			MovePlayer(dx, dy);
		}

		// DISPLAY =================================
		DrawFrame();
	}

	if (!m_Model.GetPlayer()->IsAlive()) {
		return GAME_OUTCOME::DEATH;
	}
	else if (m_Model.GetPlayer()->HasTreasure()) {
		return GAME_OUTCOME::WON;
	}
	else {
		return GAME_OUTCOME::SURRENDER;
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

	std::wstring controll = L"CONTROLL: ";
	controll += m_UserHasControll ? L"PLAYER" : L"COMPUTER";
	screenElements.emplace_back(levelDimensions.first + 3, 2, controll.length(), 1, controll);

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

void TreasureHuntGameController::OnButtonPressed(BUTTON btn) {
	
	int dx = 0;
	int dy = 0;
	
	if (m_UserHasControll) {
		switch (btn) {
		case BUTTON::UP:
			--dy;
			break;
		case BUTTON::LEFT:
			--dx;
			break;
		case BUTTON::DOWN:
			++dy;
			break;
		case BUTTON::RIGHT:
			++dx;
			break;
		}

		MovePlayer(dx, dy);
	}

	if (btn == BUTTON::TOGGLE_SOLVER) {
		m_UserHasControll = !m_UserHasControll;

		if (!m_UserHasControll) {
			m_Solver = std::make_unique<MissionControll>(m_Model);
			m_Solver->SolveLevel();
		}
	}
}