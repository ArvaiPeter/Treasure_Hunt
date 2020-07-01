#pragma once

#include "TreasureHuntGameModel.h"
#include "Route.h"

#include <set>

struct PathInfo {
	std::vector<Node*> Enemies;
	std::vector<Node*> Traps;
};

class MissionControll
{
public:
	MissionControll(const TreasureHuntGameModel& lvl);

	bool FindPath(Node* from, Node* to, std::list<Node*>& outPath); // from is always the player
	std::set< std::list<Node*> > FindAllPaths(Node* from, Node* to);

	std::vector<Route> FindRoutes(Route& route);
	Node* FindBestPotion(Node* from);
	bool SolveLevel();
	bool WouldSurvivePath(const std::list<Node*>& path);

	void SetObstacle(Node* node, bool value);

	PathInfo GetPathInfo(std::list<Node*> path) const;

	const Node* GetNextStep();

private:
	float Distance(const Node* a, const Node* b);
	void ResetNodes();

	void ApplyModifiers(const Route& route);
	void ResetModifiers(const Route& route);

	PlayerModifier* GetPlayerMod(Route& route);

private:
	const TreasureHuntGameModel& m_Level;
	std::vector< std::shared_ptr<Node> > m_Nodes;
	std::vector<Node*> m_WayPoints; // contains all GameObjects that might need to be visited -> consumables, exit
	std::list<Node*>::iterator m_NextStep;
	std::list< std::list<Node*> >::iterator m_CurrentPath;
	std::list< std::list<Node*> > m_FoundPaths;
};

// TODO: move to separate file
int ConvertCoordinates(const unsigned int& x, const unsigned int& y, const unsigned int& width);