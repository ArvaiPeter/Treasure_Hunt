#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "GameObjectFactory.h"

class TreasureHuntGameModel
{
public:
	TreasureHuntGameModel();

	void ResetModel(const std::wstring& lvlName = L"");
	void LoadLevel(const std::wstring& lvlName);

	const std::vector< std::shared_ptr<GameObject> >& GetLevel() const;
	std::shared_ptr<Player> GetPlayer() const;

	std::pair<unsigned int /*width*/, unsigned int /*heigth*/> GetLevelDimensions() const;

private:
	std::wstring m_LevelName;
	std::vector< std::shared_ptr<GameObject> > m_Level;

	std::shared_ptr<GameObject> m_Player;

	const GameObjectFactory& m_GameObjectFactory;
	
	unsigned int m_lvlWidth;
	unsigned int m_lvlHeight;

};
