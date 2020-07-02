#pragma once

#include <memory>

#include "GameObjects.h"

class GameObjectFactory
{
public:
	static const GameObjectFactory& GetInstance();

	GameObject* CreateGameObject(const unsigned int& x, const unsigned int& y, const char& repr) const;

	GameObjectFactory(const GameObjectFactory& ohter) = delete;
	GameObjectFactory& operator=(const GameObjectFactory& other) = delete;

private:
	GameObjectFactory() = default;
};
