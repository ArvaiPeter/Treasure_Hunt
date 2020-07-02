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

	// Uses the A* pathfinding algorithm to find the shortest path between endpoints
	bool FindPath(Node* from, Node* to, std::list<Node*>& outPath);

	// Uses FindPath() to find all paths between two endpoints
	std::set< std::list<Node*> > FindAllPaths(Node* from, Node* to);

	// Iterative solution to finding the optimal route to solve the game using back tracking
	std::vector<Route> FindRoutes(Route& route);

	bool SolveLevel();

	bool WouldSurvivePath(const std::list<Node*>& path);

	// Sets(or unsets) a node as an obstacle, the A* algorithms uses the obstacle flag to plan around it
	void SetObstacle(Node* node, bool value);

	// Returns how many enemies and traps are along the way
	PathInfo GetPathInfo(std::list<Node*> path) const;

	// once the level had been solved, each call to GetNextStep() returns the next step the player has to take to complete finish
	const Node* GetNextStep();

private:
	// Discance between two nodes, uses Pythagoras' theorem
	float Distance(const Node* a, const Node* b);

	// Resets certain attributes of the nodes. Without this two consecutive calls of FindPath() would return wrong paths;
	void ResetNodes();

	// applies all modifiers belonging to a route
	void ApplyModifiers(const Route& route);
	void ResetModifiers(const Route& route);

	// finds the most optimal potion to get
	Node* FindBestPotion(Node* from);

	PlayerModifier* GetPlayerMod(Route& route);

private:
	const TreasureHuntGameModel& m_Level;
	std::vector< std::shared_ptr<Node> > m_Nodes;
	std::vector<Node*> m_WayPoints; // contains all GameObjects that might need to be visited -> consumables, exit, ...
	std::list<Node*>::iterator m_NextStep;
	std::list< std::list<Node*> >::iterator m_CurrentPath;
	std::list< std::list<Node*> > m_FoundPaths; // contains all pahts to solve the level
};

// converts from 2D coordinates to 1D coordinate
int ConvertCoordinates(const unsigned int& x, const unsigned int& y, const unsigned int& width);