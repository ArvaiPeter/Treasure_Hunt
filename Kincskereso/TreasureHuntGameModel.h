#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "GameObjectFactory.h"

class TreasureHuntGameModel
{
public:
	TreasureHuntGameModel();

	void LoadLevel(const std::wstring& lvlName);

	const std::vector<GameObject>& GetLevel() const;

	std::pair<unsigned int /*width*/, unsigned int /*heigth*/> GetLevelDimensions() const;

private:
	std::wstring m_LevelName;
	std::vector<GameObject> m_Level;

	// TODO: experimantal path or custom path handling
	const std::wstring m_AssetFolderPath = L"..\\Assets";

	const GameObjectFactory& m_GameObjectFactory;
	
	unsigned int m_lvlWidth;
	unsigned int m_lvlHeight;

};
