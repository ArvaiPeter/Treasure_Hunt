
#include "GameObjects.h"
#include "GameObjectRepresentations.h"


// ============================ Game Object ==================================
GameObject::GameObject(unsigned int x, unsigned int y, const wchar_t  repr)
	: m_X(x), m_Y(y), m_Representation(repr) {}

wchar_t GameObject::GetRepresentation() const {
	return m_Representation;
}

unsigned int GameObject::X() const {
	return m_X;
}

unsigned int GameObject::Y() const {
	return m_Y;
}

// ============================ Interfaces ==================================
IDamageable::IDamageable(uint8_t health, uint8_t maxHealth, bool alive /*= true*/)
	: m_Health(health), m_MaxHealth(maxHealth), m_Alive(alive) {}

void IDamageable::TakeDamage(uint8_t dmg) {
	m_Health -= dmg;
	m_Alive = m_Health > 0;
}

void IDamageable::Heal(uint8_t amount) {
	if (!m_Alive)
	{
		return; // maybe throw exception
	}

	m_Health += amount;
	if (m_Health > m_MaxHealth) {
		m_Health = m_MaxHealth;
	}
}

bool IDamageable::IsAlive() const {
	return m_Alive;
}

uint8_t IDamageable::GetHealth() const{
	return m_Health;
}

uint8_t IDamageable::GetMaxHealth() const {
	return m_MaxHealth;
}

// ============================ Environment ==================================
Environment::Environment(unsigned int x, unsigned int y, EnvironmentType type)
	: GameObject(x,y, EnvRepr[type]),
	m_Type(type),
	m_Walkable((type != EnvironmentType::WALL) ? true : false) { }

void Environment::Interact(GameObject* with) {
	Player* player = dynamic_cast<Player*>(with);
	if (player) {
		if (m_Type == EnvironmentType::TRAP) {
			m_Type = EnvironmentType::WALL;
			m_Representation = GameObjectRepr::WALL;
			m_Walkable = false;
		}
	}
	else {
		//todo ERROR
	}
}

void Environment::Modify(IModifier* modifier) {
	if (auto envMod = dynamic_cast<EnvironmentModifier*>(modifier)) {
		envMod->m_Walkable.Apply(m_Walkable);
	}
}

void Environment::Restore(IModifier* modifier) {
	if (auto envMod = dynamic_cast<EnvironmentModifier*>(modifier)) {
		envMod->m_Walkable.Restore(m_Walkable);
	}
}

bool Environment::IsWalkable() const {
	return m_Walkable;
}

EnvironmentType Environment::GetEnvType() const {
	return m_Type;
}

// ============================ Consumable ==================================
Consumable::Consumable(unsigned int x, unsigned int y, ConsumableType type)
	: GameObject(x, y, ConsumableRepr[type]), m_Type(type), m_Consumed(false) {}

wchar_t Consumable::GetRepresentation() const {
	return m_Consumed ? GameObjectRepr::PATH : m_Representation	;
}

void Consumable::Interact(GameObject* with) {
	if (m_Consumed) {
		return;
	}

	if (auto player = dynamic_cast<Player*>(with)) {
		m_Consumed = true;
		if (m_Type == ConsumableType::POTION) {
			if (player->GetHealth() < player->GetMaxHealth()) {
				player->Heal(1);
			}
		}
		else if (m_Type == ConsumableType::SWORD) {
			player->IsArmed() = true;
		}
		else if (m_Type == ConsumableType::TREASURE) { // Treasure
			player->HasTreasure() = true;
		}
		else {
			m_Consumed = false; //?
			// TODO error -> new unhandled enum value
		}
	}
	else {
		// TODO error 
	}
}

void Consumable::Modify(IModifier* modifier) {
	if (auto consumableMod = dynamic_cast<ConsumableModifier*>(modifier)) {
		consumableMod->m_Consumed.Apply(m_Consumed);
	}
}

void Consumable::Restore(IModifier* modifier) {
	if (auto consumableMod = dynamic_cast<ConsumableModifier*>(modifier)) {
		consumableMod->m_Consumed.Restore(m_Consumed);
	}
}

void Consumable::SetConsumed(bool newVal) {
	m_Consumed = newVal;
}

bool Consumable::IsConsumed() const{
	return m_Consumed;
}

ConsumableType Consumable::GetType() const {
	return m_Type;
}

// ============================ Beast ==================================
Beast::Beast(unsigned int x, unsigned int y)
	: GameObject(x, y, GameObjectRepr::BEAST), IDamageable(1, 1) {}

wchar_t Beast::GetRepresentation() const {
	return m_Alive ? m_Representation : GameObjectRepr::PATH;
}

void Beast::Heal(uint8_t amount) {
	// healing of Beasts is not possible
	// todo exception
	return;
}

void Beast::Interact(GameObject* with) {
	if (m_Alive) {
		Player* player = dynamic_cast<Player*>(with);

		if (player) {
			if (player->IsArmed()) {
				TakeDamage(m_MaxHealth);
				player->TakeDamage(1);
			}
			else {
				player->TakeDamage(player->GetHealth());
			}
		}
		else {
			// with is not an instance of Player -> error
			// Non-player gameobjects can only interact with the player
		}
	}
}

void Beast::Modify(IModifier* modifier) {
	if (auto beastMod = dynamic_cast<BeastModifier*>(modifier)) {
		beastMod->m_IsAlive.Apply(m_Alive);
	}
}

void Beast::Restore(IModifier* modifier) {
	if (auto beastMod = dynamic_cast<BeastModifier*>(modifier)) {
		beastMod->m_IsAlive.Restore(m_Alive);
	}
}

// ============================ Player ==================================
Player::Player(unsigned int x, unsigned int y)
	: GameObject(x, y, GameObjectRepr::PLAYER), IMoveable(), IDamageable(2, 2), m_Armed(false), m_HasTreasure(false) {}

void Player::Move(unsigned int dX, unsigned int dY) {
	m_X += dX;
	m_Y += dY;
}

void Player::Modify(IModifier* modifier) {
	if (auto playerMod = dynamic_cast<PlayerModifier*>(modifier)) {
		playerMod->m_Health.Apply(m_Health);
		playerMod->m_IsArmed.Apply(m_Armed);
	}
}

void Player::Restore(IModifier* modifier) {
	if (auto playerMod = dynamic_cast<PlayerModifier*>(modifier)) {
		playerMod->m_Health.Restore(m_Health);
		playerMod->m_IsArmed.Restore(m_Armed);
	}
}

bool Player::IsArmed() const {
	return m_Armed;
}

bool& Player::IsArmed() {
	return m_Armed;
}

bool Player::HasTreasure() const {
	return m_HasTreasure;
}

bool& Player::HasTreasure() {
	return m_HasTreasure;
}