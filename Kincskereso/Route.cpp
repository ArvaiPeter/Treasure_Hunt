#include "Route.h"

Route::Route(const Route& other) {
	wayPoints = other.wayPoints;
	paths = other.paths;

	modifiers.clear();
	for (const auto& mod : other.modifiers) {
		modifiers.push_back(mod->Clone());
	}

	successful = other.successful;
}

Route::Route(Route&& other) noexcept {
	wayPoints = other.wayPoints;
	paths = other.paths;

	modifiers.clear();
	for (const auto& mod : other.modifiers) {
		modifiers.push_back(mod->Clone());
	}

	successful = other.successful;
}

Route& Route::operator=(const Route& other) {
	wayPoints = other.wayPoints;
	paths = other.paths;

	modifiers.clear();
	for (const auto& mod : other.modifiers) {
		modifiers.push_back(mod->Clone());
	}

	successful = other.successful;
	return *this;
}

Route& Route::operator=(Route&& other) noexcept{
	wayPoints = other.wayPoints;
	paths = other.paths;

	modifiers.clear();
	for (const auto& mod : other.modifiers) {
		modifiers.push_back(mod->Clone());
	}

	successful = other.successful;
	return *this;
}

void Route::AddPath(const std::list<Node*>& path)
{
	paths.push_back(path);
}

bool Route::ContainsWayPoint(const Node* wayPoint) {
	return std::find(wayPoints.begin(), wayPoints.end(), wayPoint) != wayPoints.end();
}

bool Route::IsModified(const GameObject* obj) const {
	for (auto& mod : modifiers) {
		if (dynamic_cast<GameObject*>(mod->Subject) == obj) {
			return true;
		}
	}

	return false;
}