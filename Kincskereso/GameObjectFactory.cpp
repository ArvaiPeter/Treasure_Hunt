#include "GameObjectFactory.h"


const GameObjectFactory& GameObjectFactory::GetInstance() {
	static GameObjectFactory instance;
	return instance;
}

std::unique_ptr<GameObject> GameObjectFactory::CreateGameObject(const unsigned int& x, const unsigned int& y, const char& repr) const{
	switch (repr)
	{
	case 'k':
		return std::make_unique<Consumable>(x, y, ConsumableType::TREASURE);
	case 'i':
		return std::make_unique<Consumable>(x, y, ConsumableType::POTION);
	case 'c':
		return std::make_unique<Environment>(x, y, EnvironmentType::TRAP);
	case 'a':
		return std::make_unique<Consumable>(x, y, ConsumableType::SWORD);
	case 'h':
		return std::make_unique<Player>(x, y);
	case 'x':
		return std::make_unique<Environment>(x, y, EnvironmentType::WALL);
	case 'j':
		return std::make_unique<Environment>(x, y, EnvironmentType::EXIT);
	case 's':
		return std::make_unique<Beast>(x, y);
	case ' ':
		return std::make_unique<Environment>(x, y, EnvironmentType::PATH);
	default:
		// TODO: error unrecognised game object
		break;
	}
}