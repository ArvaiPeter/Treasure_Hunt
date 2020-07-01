#include "MainMenuController.h"


MainMenuController::MainMenuController(InputController& inputController, ConsoleView& consoleView)
	: m_InputController(inputController),
	m_View(consoleView),
	m_LastOutcome(GAME_OUTCOME::NONE)
{}

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

	return m_MenuItems[selectedMenuItem % m_MenuItems.size()].second;
}

void MainMenuController::SetLastOutCome(GAME_OUTCOME lastOutcome) {
	// TODO
}

void MainMenuController::Display(const uint8_t selectedMenuItem) {

	std::vector<DrawRect> screenElements;
	const int bannerWidth = 88;
	const int bannerHeight = 7;
	std::wstring banner = L"#######                                                     #     #                     ";
	banner +=			  L"   #    #####  ######   ##    ####  #    # #####  ######    #     # #    # #    # ##### ";
	banner +=			  L"   #    #    # #       #  #  #      #    # #    # #         #     # #    # ##   #   #   ";
	banner +=			  L"   #    #    # #####  #    #  ####  #    # #    # #####     ####### #    # # #  #   #   ";
	banner +=			  L"   #    #####  #      ######      # #    # #####  #         #     # #    # #  # #   #   ";
	banner +=			  L"   #    #   #  #      #    # #    # #    # #   #  #         #     # #    # #   ##   #   ";
	banner +=			  L"   #    #    # ###### #    #  ####   ####  #    # ######    #     #  ####  #    #   #   ";

	screenElements.emplace_back(0, 0, bannerWidth, bannerHeight, banner);

	for (size_t i = 0; i < m_MenuItems.size(); ++i) {
		std::wstring menuItem = m_MenuItems[i].first;

		if (selectedMenuItem % m_MenuItems.size() == i) {
			menuItem = L"# " + menuItem;
		}

		screenElements.emplace_back(
			(bannerWidth / 2) - (menuItem.length() / 2),
			bannerHeight + (i + 1),
			menuItem.length(),
			1,
			menuItem
		);
	}

	m_View.DrawFrame(screenElements);

}

std::vector< std::pair<std::wstring, bool> > MainMenuController::m_MenuItems = {
	{ L"START", true},
	{ L"EXIT", false}
};
