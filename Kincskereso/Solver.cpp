#include "Solver.h"

#include <list>

// NODE ==============================================
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

// ASTAR PATH FINDER =====================================

AstarPathFinder::AstarPathFinder(const TreasureHuntGameModel& lvl)
	: m_Level(lvl),
	m_StartingNode(nullptr),
	m_EndingNode(nullptr)
{
	for (const auto obj : m_Level.GetLevel()) {
		if (auto env = std::dynamic_pointer_cast<Environment>(obj)) {
			if (env->GetEnvType() == EnvironmentType::WALL) {
				m_Nodes.push_back(std::make_unique<Node>(obj.get(), true));
			}
			else {
				m_Nodes.push_back(std::make_unique<Node>(obj.get()));
			}
		}
		else {
			m_Nodes.push_back(std::make_unique<Node>(obj.get()));
		}
	}

	// connecting neighbouring nodes
	auto lvlDimensions = m_Level.GetLevelDimensions();

	for (auto y = 0; y < lvlDimensions.second; ++y) {
		for (auto x = 0; x < lvlDimensions.first; ++x) {

			auto currIndex = ConvertCoordinates(x, y, lvlDimensions.first);

			if (y > 0) {
				auto upIndex = ConvertCoordinates(x, y - 1, lvlDimensions.first);
				m_Nodes[currIndex]->Neighbours.push_back(m_Nodes[upIndex].get());
			}
			if (y < lvlDimensions.second - 1) {
				auto downIndex = ConvertCoordinates(x, y + 1, lvlDimensions.first);
				m_Nodes[currIndex]->Neighbours.push_back(m_Nodes[downIndex].get());
			}
			if (x > 0) {
				auto leftIndex = ConvertCoordinates(x - 1, y, lvlDimensions.first);
				m_Nodes[currIndex]->Neighbours.push_back(m_Nodes[leftIndex].get());
			}
			if (x < lvlDimensions.first - 1) {
				auto rightIndex = ConvertCoordinates(x + 1, y, lvlDimensions.first);
				m_Nodes[currIndex]->Neighbours.push_back(m_Nodes[rightIndex].get());
			}
		}
	}
}

bool AstarPathFinder::FindPath(const unsigned int& toX, const unsigned int& toY) {
	auto lvlDimensions = m_Level.GetLevelDimensions();
	auto playerIndex = ConvertCoordinates(m_Level.GetPlayer()->X(), m_Level.GetPlayer()->Y(), lvlDimensions.first);
	m_StartingNode = m_Nodes[playerIndex].get();
	m_StartingNode->LocalGoal = 0.0f;
	
	auto endIndex = ConvertCoordinates(toX, toY, lvlDimensions.first);
	m_EndingNode = m_Nodes[endIndex].get();
	m_StartingNode->GlobalGoal = Distance(m_StartingNode, m_EndingNode);

	std::list<Node*> notTestedNodes;
	notTestedNodes.push_back(m_StartingNode);

	while (!notTestedNodes.empty()) {
		
		notTestedNodes.sort([](const Node* a, const Node* b) { return a->GlobalGoal < b->GlobalGoal; });

		while (!notTestedNodes.empty() && notTestedNodes.front()->Visited) {
			notTestedNodes.pop_front();
		}

		if (notTestedNodes.empty()) {
			break;
		}

		auto currNode = notTestedNodes.front();
		currNode->Visited = true;

		for (const auto neighbour : currNode->Neighbours) {
			if (!neighbour->Visited && !neighbour->IsObstacle) {
				notTestedNodes.push_back(neighbour);
			}

			float parentGoal = currNode->LocalGoal + Distance(currNode, neighbour);

			if (parentGoal < neighbour->LocalGoal) {
				neighbour->ParentNode = currNode;
				neighbour->LocalGoal = parentGoal;
				neighbour->GlobalGoal = neighbour->LocalGoal + Distance(neighbour, m_EndingNode);
			}
		}
	}

	return m_EndingNode->ParentNode != nullptr;
}

std::stack<Node*> AstarPathFinder::GetPath() {
	std::stack<Node*> ret;
	auto currNode = m_EndingNode;
	ret.push(currNode);

	while (currNode->ParentNode)
	{
		currNode = currNode->ParentNode;
		ret.push(currNode);
	}

	return ret;
}

void AstarPathFinder::ToggleObstacle(const unsigned int& x, const unsigned int& y) {
	m_Nodes[ConvertCoordinates(x, y, m_Level.GetLevelDimensions().first)]->IsObstacle = !m_Nodes[ConvertCoordinates(x, y, m_Level.GetLevelDimensions().first)]->IsObstacle;
}

bool AstarPathFinder::PathContainsEnemy() {
	auto currNode = m_EndingNode;
	while (currNode->ParentNode) 
	{
		if (auto beast = dynamic_cast<Beast*>(currNode->FieldGameObject)) {
			if (beast->IsAlive()) {
				return true;
			}
		}
		currNode = currNode->ParentNode;
	}

	return false;
}

bool AstarPathFinder::PathContainsTrap() {
	auto currNode = m_EndingNode;
	while (currNode->ParentNode)
	{
		if (auto env = dynamic_cast<Environment*>(currNode->FieldGameObject)) {
			if (env->GetEnvType() == EnvironmentType::TRAP) {
				return true;
			}
		}
		currNode = currNode->ParentNode;
	}

	return false;
}



float AstarPathFinder::Distance(const Node* a, const Node* b) {
	static auto pythagoras = [](const unsigned int& x, const unsigned int& y) {
		return sqrtf( pow(x, 2) + pow(y, 2) );
	};

	return pythagoras(a->X() - b->X(), a->Y() - b->Y());
}

int ConvertCoordinates(const unsigned int& x, const unsigned int& y, const unsigned int& width) {
	return y * width + x;
}