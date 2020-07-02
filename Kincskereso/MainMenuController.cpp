#include "MainMenuController.h"

#include "Constants.h"

MainMenuController::MainMenuController(InputController& inputController, ConsoleView& consoleView)
	: m_InputController(inputController),
	m_View(consoleView),
	m_LastOutcome(GAME_OUTCOME::NONE)
{
	m_Model.LoadBanner(L"TreasureHuntBanner.txt");
}

bool MainMenuController::Run() {

	uint8_t selectedMenuItem = 0;

	while (true) {

		m_InputController.GetInput();
		auto buttonsPressed = m_InputController.ButtonsPressed();

		if (buttonsPressed & UP) --selectedMenuItem; // TODO
		else if (buttonsPressed & DOWN) ++selectedMenuItem; // TODO
		else if (buttonsPressed & SELECT) {
			break;
		}

		Display(selectedMenuItem);
	}

	auto menuItems = m_Model.GetMenuItems();

	return menuItems[selectedMenuItem % menuItems.size()].second;
}

void MainMenuController::SetLastOutCome(GAME_OUTCOME lastOutcome) {
	if (lastOutcome != GAME_OUTCOME::NONE) {
		m_Model.LoadBanner(L"GameOverBanner.txt");
	}

	m_LastOutcome = lastOutcome;
}

void MainMenuController::Display(const uint8_t selectedMenuItem) {

	std::vector<DrawRect> screenElements;
	auto menuItems = m_Model.GetMenuItems();

	auto bannerDimensions = m_Model.GetBannerDimensions();
	screenElements.emplace_back(0, 0, bannerDimensions.first, bannerDimensions.second, m_Model.GetBanner());

	if (m_LastOutcome != GAME_OUTCOME::NONE) {

		std::wstring gameOverMessage;
		switch (m_LastOutcome)
		{
		case GAME_OUTCOME::WON:
			gameOverMessage = L"Congratulations, You Won!";
			break;
		case GAME_OUTCOME::SURRENDER:
			gameOverMessage = L"You Escaped Without The Treasure";
			break;
		case GAME_OUTCOME::DEATH:
			gameOverMessage = L"You Died";
			break;
		default:
			break;
		}

		screenElements.emplace_back(
			(bannerDimensions.first / 2) - (gameOverMessage.length() / 2),
			bannerDimensions.second + 1,
			gameOverMessage.length(),
			1,
			gameOverMessage
		);
	}

	for (size_t i = 0; i < menuItems.size(); ++i) {
		std::wstring menuItem = menuItems[i].first;

		if (selectedMenuItem % menuItems.size() == i) {
			menuItem = L"# " + menuItem;
		}

		screenElements.emplace_back(
			(bannerDimensions.first / 2) - (menuItem.length() / 2),
			bannerDimensions.second + (i + MenuYOffset),
			menuItem.length(),
			1,
			menuItem
		);
	}

	m_View.DrawFrame(screenElements);

}

