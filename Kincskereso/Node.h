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

// IMPLEMENTATIONS ======================================
Node::Node(GameObject* obj,
	bool isObstacle/*=false*/)
	: GlobalGoal(INFINITY),
	LocalGoal(INFINITY),
	Visited(false),
	IsObstacle(isObstacle),
	ParentNode(nullptr),
	FieldGameObject(obj)
{}

unsigned int Node::X() const {
	return FieldGameObject->X();
}

unsigned int Node::Y() const {
	return FieldGameObject->Y();
}

void Node::ResetNode() {
	GlobalGoal = INFINITY;
	LocalGoal = INFINITY;
	Visited = false;
	ParentNode = nullptr;
}