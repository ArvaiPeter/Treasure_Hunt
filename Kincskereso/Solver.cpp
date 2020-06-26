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

void Node::ResetNode() {
	GlobalGoal = INFINITY;
	LocalGoal = INFINITY;
	Visited = false;
	ParentNode = nullptr;
}

// ROUTE ================================================

void Route::AddPath(const std::list<Node*>& path) 
{
	paths.push_back(path); 
}

bool Route::ContainsWayPoint(const Node* wayPoint) {
	return std::find(wayPoints.begin(), wayPoints.end(), wayPoint) != wayPoints.end();
}

// MISSION CONTROLL =====================================
MissionControll::MissionControll(const TreasureHuntGameModel& lvl)
	: m_Level(lvl)
{
	for (const auto obj : m_Level.GetLevel()) {
		if (auto env = std::dynamic_pointer_cast<Environment>(obj)) {
			if (env->GetEnvType() == EnvironmentType::WALL) {
				m_Nodes.push_back(std::make_shared<Node>(obj.get(), true));
			}
			else {
				m_Nodes.push_back(std::make_shared<Node>(obj.get()));

				if (env->GetEnvType() == EnvironmentType::EXIT) {
					m_WayPoints.push_back(m_Nodes.back().get());
				}
			}
		}
		else {
			m_Nodes.push_back(std::make_shared<Node>(obj.get()));

			if (auto consumable = std::dynamic_pointer_cast<Consumable>(obj)) {
				m_WayPoints.push_back(m_Nodes.back().get());
			}
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

bool MissionControll::FindPath(Node* from, Node* to, std::list<Node*>& outPath) {
	ResetNodes();

	auto lvlDimensions = m_Level.GetLevelDimensions();

	Node* m_StartingNode = from;
	m_StartingNode->LocalGoal = 0.0f;
	
	auto m_EndingNode = to;
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
			if(!neighbour->Visited && !neighbour->IsObstacle) {
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

	if (m_EndingNode->ParentNode == nullptr) {
		return false;
	}

	auto currNode = m_EndingNode;
	outPath.push_front(currNode);

	while (currNode->ParentNode)
	{
		currNode = currNode->ParentNode;
		outPath.push_front(currNode);
	}

	return true;
}

std::set< std::list<Node*> > MissionControll::FindAllPaths(Node* from, Node* to) {
	std::set< std::list<Node*> > allPaths;
	std::vector<Node*> disabledNodes;

	std::list<Node*> path;
	while (FindPath(from, to, path)) {
		auto emplaceInfo = allPaths.emplace(path);	
		if (!emplaceInfo.second) {
			// found the same path twice
			// only happens when there are no obstacles on the path that we can disable and go around
			break;
		}

		auto pathInfo = GetPathInfo(path);

		for (auto enemy : pathInfo.Enemies) {
			SetObstacle(enemy, true);
			disabledNodes.push_back(enemy);
		}
		for (auto trap : pathInfo.Traps) {
			SetObstacle(trap, true);
			disabledNodes.push_back(trap);
		}
		

		path.clear();
	}

	for (auto node : disabledNodes) {
		SetObstacle(node, false);
	}

	return allPaths;
}

std::vector<Route> MissionControll::FindRoutes(Route& currRoute) {

	std::vector<Route> newRoutes;

	Route currRouteCopy = currRoute;

	auto end = currRoute.wayPoints.begin();
	auto start = end++;

	for (auto _ : currRoute.paths) { ++start; ++end; }
	ApplyModifiers(currRoute);

	for (auto path : FindAllPaths(*start, *end)) {

		bool saveRoute = true;
		auto pathInfo = GetPathInfo(path);

		if (WouldSurvivePath(path)) {
			currRoute.AddPath(path);

			for (auto trap : pathInfo.Traps) {
				currRoute.modifiers.push_back(
					std::make_shared<EnvironmentModifier>(dynamic_cast<IModifiable*>(trap->FieldGameObject), false)
				);
			}

			auto playerMod = GetPlayerMod(currRoute);

			if (auto consumable = dynamic_cast<Consumable*>((*end)->FieldGameObject)) {
				currRoute.modifiers.push_back(
					std::make_shared<ConsumableModifier>(dynamic_cast<IModifiable*>(consumable), true)
				);
								
				switch (consumable->GetType())
				{
				case ConsumableType::POTION:
					playerMod->AddHealth(1);
					break;
				case ConsumableType::SWORD:
					playerMod->SetArmed(true);
					break;
				case ConsumableType::TREASURE:
					break;
				}
			}
			else if (auto beast = dynamic_cast<Beast*>((*end)->FieldGameObject)) {
				currRoute.modifiers.push_back(std::make_shared<BeastModifier>(dynamic_cast<IModifiable*>(beast), false));
				playerMod->AddHealth(-1);
			}
			else if (auto environment = dynamic_cast<Environment*>((*end)->FieldGameObject)) {
				if (environment->GetEnvType() == EnvironmentType::EXIT) {
					currRoute.successful = true;
				}
			}
		}
		else {
			
			auto sword = std::find_if(m_WayPoints.begin(), m_WayPoints.end(), [](Node* node)
				{
					if (auto consumable = dynamic_cast<Consumable*>(node->FieldGameObject)) {
						return consumable->GetType() == ConsumableType::SWORD;
					}
					return false;
				}
			);
			
			if (!m_Level.GetPlayer()->IsArmed() && !currRoute.ContainsWayPoint(*sword)) {
				currRoute.wayPoints.insert(end, *sword);
				end--;
			}
			else if (pathInfo.Enemies.size() > 1) {
				// paths need to be split untill only one enemy remains in each -> in the next iterations a potion can be added between them
				// enemies needed to be added from closest to furthest -> GetPathInfo scanns the path in this order so no problem

				for (auto i = pathInfo.Enemies.end(); i != pathInfo.Enemies.begin(); --i) {
					currRoute.wayPoints.insert(end, *(i - 1));
					end--;
				}
			}
			else {
				Node* potion = FindBestPotion(*start);

				if (potion) {
					currRoute.wayPoints.insert(end, potion);
					end--;
				}
				else {
					saveRoute = false;
				}
			}
		}
		
		if (saveRoute) {
			Route newRoute = currRoute;
			newRoutes.emplace_back(newRoute);
		}

		// restoring currRoute for the other possible paths
		currRoute = currRouteCopy;
	}

	ResetModifiers(currRoute);

	return newRoutes;
}

bool MissionControll::SolveLevel() {
	Route initialRoute;

	auto player = m_Level.GetPlayer();
	Node* playerNode = m_Nodes[ConvertCoordinates(player->X(), player->Y(), m_Level.GetLevelDimensions().first)].get();

	auto exit = std::find_if(m_Nodes.begin(), m_Nodes.end(), [](std::shared_ptr<Node> node) 
		{
			if (auto env = dynamic_cast<Environment*>(node->FieldGameObject)) {
				return env->GetEnvType() == EnvironmentType::EXIT;
			}
			return false;
		});

	auto treasure = std::find_if(m_Nodes.begin(), m_Nodes.end(), [](std::shared_ptr<Node> node)
		{
			if (auto consumable = dynamic_cast<Consumable*>(node->FieldGameObject)) {
				return consumable->GetType() == ConsumableType::TREASURE;
			}
			return false;
		});
	
	initialRoute.wayPoints.push_front((*exit).get());
	initialRoute.wayPoints.push_front((*treasure).get());
	initialRoute.wayPoints.push_front(playerNode);

	std::queue<Route> routesToExplore;
	routesToExplore.push(initialRoute);

	while (!routesToExplore.empty()) {
		auto nextRoute = routesToExplore.front();
		routesToExplore.pop();

		for (auto newRoute : FindRoutes(nextRoute)) {
			if (newRoute.successful) {
				m_FoundPaths = newRoute.paths;
				m_CurrentPath = m_FoundPaths.begin();
				m_NextStep = m_CurrentPath->begin();
				return true;
			}
			routesToExplore.push(newRoute);
		}
	}

	return false;
}

Node* MissionControll::FindBestPotion(Node* from) {

	std::vector< std::list<Node*> > goodPaths;

	for (auto i = m_WayPoints.begin(); i != m_WayPoints.end(); ++i) {
		if (auto consumable = dynamic_cast<Consumable*>((*i)->FieldGameObject)) {
			if (consumable->GetType() == ConsumableType::POTION && !consumable->IsConsumed()) {

				for (auto path : FindAllPaths(from, *i)) {
					if (WouldSurvivePath(path)) {
						goodPaths.push_back(path);
					}
				}
			}
		}
	}

	std::sort(goodPaths.begin(), goodPaths.end(), [](const std::list<Node*>& a, const std::list<Node*>& b)
		{
			return a.size() < b.size();
		});

	return goodPaths.empty() ? nullptr : goodPaths.front().back();
}

bool MissionControll::WouldSurvivePath(const std::list<Node*>& path) {
	auto pathInfo = GetPathInfo(path);
	auto player = m_Level.GetPlayer();

	return !( (pathInfo.Enemies.size() > 0 && !player->IsArmed()) || (player->GetHealth() <= pathInfo.Enemies.size() ) );
}

void MissionControll::SetObstacle(Node* node, bool value) {
	// todo error handling
	node->IsObstacle = value;
}

PathInfo MissionControll::GetPathInfo(std::list<Node*> path) const {
	PathInfo pathInfo;

	// we dont care about the start and ending node
	for (auto node = ++path.begin(); node != path.end(); ++node) {
		if (auto beast = dynamic_cast<Beast*>((*node)->FieldGameObject)) {
			if (beast->IsAlive()) 
			{
				pathInfo.Enemies.push_back(*node); 
			}
		}
		else if (auto trap = dynamic_cast<Environment*>((*node)->FieldGameObject)) {
			if (trap->GetEnvType() == EnvironmentType::TRAP) {
				pathInfo.Traps.push_back(*node);
			}
		}
	}

	return pathInfo;
}

const Node* MissionControll::GetNextStep() {
	if (++m_NextStep == m_CurrentPath->end()) {
		if (++m_CurrentPath != m_FoundPaths.end()) {
			m_NextStep = m_CurrentPath->begin();
		}
		else {
			return nullptr;
		}
	}
	
	return *m_NextStep;
}


float MissionControll::Distance(const Node* a, const Node* b) {
	static auto pythagoras = [](const unsigned int& x, const unsigned int& y) {
		return sqrtf( pow(x, 2) + pow(y, 2) );
	};

	return pythagoras(a->X() - b->X(), a->Y() - b->Y());
}

void MissionControll::ResetNodes() {
	for (auto i = 0; i < m_Nodes.size(); ++i) {
		m_Nodes[i]->ResetNode();
	}
}

void MissionControll::ApplyModifiers(const Route& route) {
	for (auto mod : route.modifiers) {
		if (auto envMod = std::dynamic_pointer_cast<EnvironmentModifier>(mod))
		{
			auto subject = dynamic_cast<GameObject*>(envMod->Subject);
			auto subjectsNode = std::find_if(m_Nodes.begin(), m_Nodes.end(), [subject](std::shared_ptr<Node> node)
				{
					return node->FieldGameObject == subject;
				}
			);

			if (subjectsNode != m_Nodes.end()) {
				SetObstacle(subjectsNode->get(), !envMod->GetWalkable());
			}
			else {
				//error
			}
		}

		mod->Apply();
	}
}

void MissionControll::ResetModifiers(const Route& route) {
	for (auto mod : route.modifiers) {
		if (auto envMod = std::dynamic_pointer_cast<EnvironmentModifier>(mod))
		{
			auto subject = dynamic_cast<GameObject*>(envMod->Subject);
			auto subjectsNode = std::find_if(m_Nodes.begin(), m_Nodes.end(), [subject](std::shared_ptr<Node> node)
				{
					return node->FieldGameObject == subject;
				}
			);

			if (subjectsNode != m_Nodes.end()) {
				SetObstacle(subjectsNode->get(), !envMod->GetWalkable());
			}
			else {
				//error
			}
		}

		mod->Restore();
	}
}

std::shared_ptr<PlayerModifier> MissionControll::GetPlayerMod(Route& route) {
	auto playerModIt = std::find_if(route.modifiers.begin(), route.modifiers.end(), [](std::shared_ptr<IModifier> mod)
		{
			if (auto playerMod = std::dynamic_pointer_cast<PlayerModifier>(mod)) {
				return true;
			}
			return false;
		}
	);

	if (playerModIt == route.modifiers.end()) {
		auto player = m_Level.GetPlayer();

		auto playerMod = std::make_shared<PlayerModifier>(
			dynamic_cast<IModifiable*>(player.get()),
			player->GetHealth(),
			player->IsArmed()		
		);

		route.modifiers.push_back(playerMod);
		return playerMod;
	}
	else {
		return std::dynamic_pointer_cast<PlayerModifier>(*playerModIt);
	}
}

int ConvertCoordinates(const unsigned int& x, const unsigned int& y, const unsigned int& width) {
	return y * width + x;
}