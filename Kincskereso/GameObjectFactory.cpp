#include "GameObjectFactory.h"


const GameObjectFactory& GameObjectFactory::GetInstance() {
	static GameObjectFactory instance;
	return instance;
}

GameObject GameObjectFactory::CreateGameObject(const unsigned int& x, const unsigned int& y, const char& repr) const{
	switch (repr)
	{
	case 'k':
		return Consumable(x, y, ConsumableType::TREASURE);
	case 'i':
		return Consumable(x, y, ConsumableType::POTION);
	case 'c':
		return Environment(x, y, EnvironmentType::TRAP);
	case 'a':
		return Consumable(x, y, ConsumableType::SWORD);
	case 'h':
		return Player(x, y);
	case 'x':
		return Environment(x, y, EnvironmentType::WALL);
	case 'j':
		return Environment(x, y, EnvironmentType::EXIT);
	case 's':
		return Beast(x, y);
	case ' ':
		return Environment(x, y, EnvironmentType::PATH);
	default:
		// TODO: error unrecognised game object
		break;
	}
}