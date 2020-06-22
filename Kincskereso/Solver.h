#pragma once

#include <vector>
#include <memory>
#include <stack>

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

};


class AstarPathFinder
{
public:
	AstarPathFinder(const TreasureHuntGameModel& lvl);

	bool FindPath(const unsigned int& toX, const unsigned int& toY); // from is always the player
	std::stack<Node*> GetPath();

	void ToggleObstacle(const unsigned int& x, const unsigned int& y);

	bool PathContainsEnemy();
	bool PathContainsTrap();

private:
	float Distance(const Node* a, const Node* b);

private:
	const TreasureHuntGameModel& m_Level;
	Node* m_StartingNode;
	Node* m_EndingNode;
	std::vector< std::unique_ptr<Node> > m_Nodes;
};

// TODO: move to separate file

int ConvertCoordinates(const unsigned int& x, const unsigned int& y, const unsigned int& width);
