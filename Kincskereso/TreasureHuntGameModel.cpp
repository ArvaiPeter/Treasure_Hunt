#include "TreasureHuntGameModel.h"

#include <memory>

#include "Constants.h"

TreasureHuntGameModel::TreasureHuntGameModel()
	: m_GameObjectFactory(GameObjectFactory::GetInstance()),
	m_lvlWidth(0),
	m_lvlHeight(0)
{}

void TreasureHuntGameModel::ResetModel(const std::wstring& lvlName /*=L""*/) {
	std::wstring lvlFileName = lvlName.empty() ? m_LevelName : lvlName;

	m_Level.clear();
	m_Player.reset();

	LoadLevel(lvlFileName);
}

void TreasureHuntGameModel::LoadLevel(const std::wstring& lvlName) {
	if (lvlName.empty()) throw "LoadLevel() lvlName can't be empty";

	m_LevelName = lvlName;
	//finding file
	const std::wstring fullLvlPath = LevelsFolder + m_LevelName;

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
			
			if (lvlElement == GameObjectRepr::PLAYER) {
				m_Player.reset(m_GameObjectFactory.CreateGameObject(x, y, lvlElement));
				m_Level.emplace_back(m_GameObjectFactory.CreateGameObject(x, y, ' '));
			}
			else {
				m_Level.emplace_back(m_GameObjectFactory.CreateGameObject(x, y, lvlElement));
			}		
			++x;
		}
		++y;
	}
	m_lvlHeight = y;

	lvlFile.close();
}

const std::vector< std::shared_ptr<GameObject> >& TreasureHuntGameModel::GetLevel() const {
	return m_Level;
}

std::shared_ptr<Player> TreasureHuntGameModel::GetPlayer() const{
	auto ret = std::dynamic_pointer_cast<Player>(m_Player);
	return ret;
}

std::pair<unsigned int, unsigned int> TreasureHuntGameModel::GetLevelDimensions() const {
	return { m_lvlWidth, m_lvlHeight };
}