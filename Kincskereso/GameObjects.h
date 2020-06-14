#pragma once
#include <cstdint>
#include <map>

#include "GameObjectRepresentations.h"

// ============================ Game Object ==================================
class GameObject
{
public:
	GameObject(const uint16_t& x, const uint16_t& y, const wchar_t& repr);
	virtual ~GameObject() = default;

protected:
	uint16_t m_X;
	uint16_t m_Y;
	wchar_t m_Representation;
};


// ============================ Game Object Modifiers ==================================
class IMoveable {
protected:
	virtual void Move(const uint16_t& dX, const uint16_t& dY) = 0;
};

class IInteractable {
protected:
	virtual void Interact(GameObject* with) = 0;
};

class IDamageable {
public:
	virtual void TakeDamage(const uint8_t& dmg);
	virtual void Heal(const uint8_t& amount);

	virtual uint8_t GetHealth() const;
	virtual uint8_t GetMaxHealth() const;

protected:
	IDamageable(const uint8_t& health, const uint8_t& maxHealth, const bool& alive = true);
	virtual ~IDamageable() = default;

	uint8_t m_Health;
	uint8_t m_MaxHealth;
	bool m_Alive;
};

// ============================ Environment ==================================
enum class EnvironmentType {
	PATH,
	TRAP,
	WALL,
	EXIT
};

static std::map<EnvironmentType, wchar_t> EnvRepr = {
	{EnvironmentType::PATH, GameObjectRepr::PATH},
	{EnvironmentType::WALL, GameObjectRepr::WALL},
	{EnvironmentType::TRAP, GameObjectRepr::TRAP},
	{EnvironmentType::EXIT, GameObjectRepr::EXIT},
};

class Environment : public GameObject, public IInteractable{
public:
	Environment(const uint16_t& x, const uint16_t& y, EnvironmentType type);

	void Interact(GameObject* with) override;
private:
	EnvironmentType m_Type;
	bool m_Walkable;
};

// ============================ Consumable GameObject ==================================
enum class ConsumableType {
	POTION,
	SWORD,
	TREASURE
};

static std::map<ConsumableType, wchar_t> ConsumableRepr = {
	{ConsumableType::POTION, GameObjectRepr::HEALTH_POTION},
	{ConsumableType::SWORD, GameObjectRepr::SWORD},
	{ConsumableType::TREASURE, GameObjectRepr::TREASURE}
};

class Consumable : public GameObject, IInteractable {
public:
	Consumable(const uint16_t& x, const uint16_t& y, ConsumableType type);

	void Interact(GameObject* with) override;
	bool IsConsumed();

protected:
	bool m_Consumed;
	ConsumableType m_Type;
};

// ============================ Beast ==================================
class Beast : public GameObject, public IDamageable, IInteractable {
public:
	Beast(const uint16_t& x, const uint16_t& y);

	void Heal(const uint8_t& amount) override;
	void Interact(GameObject* with) override;
};

// ============================ Player ==================================
class Player : public GameObject, IMoveable, public IDamageable {
public:
	Player(const uint16_t& x, const uint16_t& y);

	void Move(const uint16_t& newX, const uint16_t& newY) override;

	bool IsArmed() const;
	bool& IsArmed();

	bool HasTreasure() const;
	bool& HasTreasure();
private:
	bool m_Armed;
	bool m_HasTreasure;
};





