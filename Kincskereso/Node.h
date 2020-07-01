#pragma once

#include <vector>

#include "GameObjects.h"

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
