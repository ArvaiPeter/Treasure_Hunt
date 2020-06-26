#pragma once

#include <vector>
#include <memory>
#include <stack>
#include <list>
#include <queue>
#include <set>

#include "GameObjects.h"
#include "TreasureHuntGameModel.h"

struct Node {

	Node(GameObject* obj,
		bool isObsacle = false);

	float GlobalGoal;
	float LocalGoal;
	bool Visited;
	bool IsObstacle;
	Node* ParentNode;
	GameObject* FieldGameObject;
	std::vector<Node*> Neighbours;

	unsigned int X() const;
	unsigned int Y() const;

	void ResetNode();
};

struct PathInfo {
	std::vector<Node*> Enemies;
	std::vector<Node*> Traps;
};

struct Route {
	std::list<Node*> wayPoints; // endpoints of paths
	std::list< std::list<Node*> > paths; // actual route between endpoints, contains already known goos paths
	std::vector< std::shared_ptr<IModifier> > modifiers;

	bool successful = false;

	void AddPath(const std::list<Node*>& path);
	bool ContainsWayPoint(const Node* wayPoint);
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

	std::shared_ptr<PlayerModifier> GetPlayerMod(Route& route);

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
