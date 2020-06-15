#pragma once
#include <cstdint>
#include <map>

#include "GameObjectRepresentations.h"

// ============================ Game Object ==================================
class GameObject
{
public:
	GameObject(const unsigned int& x, const unsigned int& y, const wchar_t repr);
	virtual ~GameObject() = default;

	wchar_t GetRepresentation() const;

protected:
	unsigned int m_X;
	unsigned int m_Y;
	wchar_t m_Representation;
};


// ============================ Game Object Modifiers ==================================
class IMoveable {
protected:
	virtual void Move(const unsigned int& dX, const unsigned int& dY) = 0;
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
	Environment(const unsigned int& x, const unsigned int& y, EnvironmentType type);

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
	Consumable(const unsigned int& x, const unsigned int& y, ConsumableType type);

	void Interact(GameObject* with) override;
	bool IsConsumed();

protected:
	bool m_Consumed;
	ConsumableType m_Type;
};

// ============================ Beast ==================================
class Beast : public GameObject, public IDamageable, IInteractable {
public:
	Beast(const unsigned int& x, const unsigned int& y);

	void Heal(const uint8_t& amount) override;
	void Interact(GameObject* with) override;
};

// ============================ Player ==================================
class Player : public GameObject, IMoveable, public IDamageable {
public:
	Player(const unsigned int& x, const unsigned int& y);

	void Move(const unsigned int& newX, const unsigned int& newY) override;

	bool IsArmed() const;
	bool& IsArmed();

	bool HasTreasure() const;
	bool& HasTreasure();
private:
	bool m_Armed;
	bool m_HasTreasure;
};





