#pragma once

#include <list>
#include <vector>
#include <memory>

#include "GameObjects.h"
#include "Node.h"

struct Route {
	Route() = default;
	Route(const Route& other);
	Route(Route&& other);
	Route& operator=(const Route& orhet);
	Route& operator=(Route&& other);

	std::list<Node*> wayPoints; // waypoints of paths
	std::list< std::list<Node*> > paths; // actual route between endpoints, contains already known good paths
	std::vector< std::unique_ptr<IModifier> > modifiers;

	bool successful = false;

	void AddPath(const std::list<Node*>& path);

	bool ContainsWayPoint(const Node* wayPoint);
	bool IsModified(const GameObject* obj) const;
};
