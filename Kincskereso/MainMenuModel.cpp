#include "MainMenuModel.h"

#include <fstream>

#include "Constants.h"

MainMenuModel::MainMenuModel()
	: m_BannerWidth(0),
	m_BannerHeight(0)
{}

void MainMenuModel::LoadBanner(const std::wstring& fileName) {
	if (fileName.empty()) throw "LoadBanner(): fileName cant be empty";

	const std::wstring bannerFilePath = AssetFolderPath + L"\\" + fileName;

	std::wifstream bannerFile(bannerFilePath.c_str(), std::ifstream::in);

	std::wstring line;

	m_BannerWidth = 0;
	m_BannerHeight = 0;
	m_Banner = L"";

	while (std::getline(bannerFile, line)) {
		if (m_BannerWidth == 0) {
			m_BannerWidth = line.length();
		}

		m_Banner += line;
		++m_BannerHeight;
	}

	bannerFile.close();
}

std::wstring MainMenuModel::GetBanner() const {
	return m_Banner;
}

std::pair<unsigned int, unsigned int> MainMenuModel::GetBannerDimensions() const {
	return std::make_pair(m_BannerWidth, m_BannerHeight);
}

std::vector< std::pair<std::wstring, bool> > MainMenuModel::GetMenuItems() const {
	return m_MenuItems;
}

std::vector< std::pair<std::wstring, bool> > MainMenuModel::m_MenuItems = {
	{ L"START", true},
	{ L"EXIT", false}
};
