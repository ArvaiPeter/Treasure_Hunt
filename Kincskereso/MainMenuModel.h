#pragma once

#include <string>
#include <vector>

class MainMenuModel
{
public:
	MainMenuModel();

	void LoadBanner(const std::wstring& filename);

	std::wstring GetBanner() const;

	std::pair<unsigned int /*width*/, unsigned int /*height*/> GetBannerDimensions() const; 
	std::vector< std::pair<std::wstring, bool> > GetMenuItems() const;

private:
	std::wstring m_Banner;

	unsigned int m_BannerWidth;
	unsigned int m_BannerHeight;

	static std::vector< std::pair<std::wstring, bool> > m_MenuItems;
};

