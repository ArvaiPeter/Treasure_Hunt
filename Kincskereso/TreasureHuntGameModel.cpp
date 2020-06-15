#include "TreasureHuntGameModel.h"


TreasureHuntGameModel::TreasureHuntGameModel() 
	: m_GameObjectFactory(GameObjectFactory::GetInstance()) {}

void TreasureHuntGameModel::LoadLevel(const std::wstring& lvlName) {
	// TODO: error handling

	m_LevelName = lvlName;
	//finding file
	const std::wstring fullLvlPath = m_AssetFolderPath + L"\\" + m_LevelName;

	//reading file and populating m_Level;
	std::ifstream lvlFile(fullLvlPath.c_str(), std::ifstream::in);

	std::string line;

	unsigned int y = 0;
	while (std::getline(lvlFile, line)) {
		if (m_lvlWidth == 0) {
			m_lvlWidth = line.length();
		}

		unsigned int x = 0;
		for (const auto& lvlElement : line) {
			auto element = m_GameObjectFactory.CreateGameObject(x, y, lvlElement);
			m_Level.push_back(element);
			++x;
		}

		++y;
	}
	m_lvlHeight = y;
}

const std::vector<GameObject>& TreasureHuntGameModel::GetLevel() const {
	return m_Level;
}

std::pair<unsigned int, unsigned int> TreasureHuntGameModel::GetLevelDimensions() const {
	return { m_lvlWidth, m_lvlHeight };
}