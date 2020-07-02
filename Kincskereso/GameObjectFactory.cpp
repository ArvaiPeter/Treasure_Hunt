#include "GameObjectFactory.h"


const GameObjectFactory& GameObjectFactory::GetInstance() {
	static GameObjectFactory instance;
	return instance;
}

GameObject* GameObjectFactory::CreateGameObject(const unsigned int& x, const unsigned int& y, const char& repr) const{
	switch (repr)
	{
	case 'k':
		return new Consumable(x, y, ConsumableType::TREASURE);
	case 'i':
		return new Consumable(x, y, ConsumableType::POTION);
	case 'c':
		return new Environment(x, y, EnvironmentType::TRAP);
	case 'a':
		return new Consumable(x, y, ConsumableType::SWORD);
	case 'h':
		return new Player(x, y);
	case 'x':
		return new Environment(x, y, EnvironmentType::WALL);
	case 'j':
		return new Environment(x, y, EnvironmentType::EXIT);
	case 's':
		return new Beast(x, y);
	case ' ':
		return new Environment(x, y, EnvironmentType::PATH);
	default:
		throw "Unrecognised Game Object";
		break;
	}
}