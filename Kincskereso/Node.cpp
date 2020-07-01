#include "Node.h"

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