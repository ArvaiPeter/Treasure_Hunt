#include "MainMenuController.h"

#include "Constants.h"

MainMenuController::MainMenuController(InputController& inputController, ConsoleView& consoleView)
	: m_InputController(inputController),
	m_View(consoleView),
	m_LastOutcome(GAME_OUTCOME::NONE),
	m_SelectedMenuItemIndex(0),
	m_MenuItemSelected(false)
{
	m_Model.LoadBanner(L"TreasureHuntBanner.txt");
	m_InputController.ButtonPressedEvent.Subscribe(
		std::bind(&MainMenuController::OnButtonPressed, this, std::placeholders::_1)
	);
}

bool MainMenuController::Run() {

	m_MenuItemSelected = false;
	m_SelectedMenuItemIndex = 0;

	while (!m_MenuItemSelected) {

		m_InputController.GetInput();
		
		Display();
	}

	auto menuItems = m_Model.GetMenuItems();

	return menuItems[m_SelectedMenuItemIndex % menuItems.size()].second;
}

void MainMenuController::SetLastOutCome(GAME_OUTCOME lastOutcome) {
	if (lastOutcome != GAME_OUTCOME::NONE) {
		m_Model.LoadBanner(L"GameOverBanner.txt");
	}

	m_LastOutcome = lastOutcome;
}

void MainMenuController::Display() {

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

		if (m_SelectedMenuItemIndex % menuItems.size() == i) {
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

void MainMenuController::OnButtonPressed(BUTTON btn) {
	switch (btn) {
	case BUTTON::UP:
		--m_SelectedMenuItemIndex;
		break;
	case BUTTON::DOWN:
		++m_SelectedMenuItemIndex;
		break;
	case BUTTON::SELECT:
		m_MenuItemSelected = true;
		break;
	}
}

