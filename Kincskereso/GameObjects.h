#pragma once
#include <cstdint>
#include <map>

#include "GameObjectRepresentations.h"
#include "GameObjectModifiers.h"

// ============================ Game Object ==================================
class GameObject
{
public:
	GameObject(const unsigned int& x, const unsigned int& y, const wchar_t repr);
	virtual ~GameObject() = default;

	virtual wchar_t GetRepresentation() const;

	unsigned int X() const;
	unsigned int Y() const;

protected:
	unsigned int m_X;
	unsigned int m_Y;
	wchar_t m_Representation;
};


// ============================ Interfaces ==================================
class IMoveable {
public:
	virtual ~IMoveable() = default;
	virtual void Move(const unsigned int& dX, const unsigned int& dY) = 0;
};

class IInteractable {
protected:
	virtual ~IInteractable() = default;
	virtual void Interact(GameObject* with) = 0;
};

class IDamageable {
public:
	virtual void TakeDamage(const uint8_t& dmg);
	virtual void Heal(const uint8_t& amount);

	virtual bool IsAlive() const;
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

class Environment : public GameObject, public IInteractable, public IModifiable{
public:
	Environment(const unsigned int& x, const unsigned int& y, EnvironmentType type);

	void Interact(GameObject* with) override;

	void Modify(IModifier* modifier) override;

	bool IsWalkable() const;
	EnvironmentType GetEnvType() const;
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

class Consumable : public GameObject, public IInteractable, public IModifiable {
public:
	Consumable(const unsigned int& x, const unsigned int& y, ConsumableType type);

	wchar_t GetRepresentation() const override;
	void Interact(GameObject* with) override;

	void Modify(IModifier* modifier) override;

	void SetConsumed(bool newVal);
	bool IsConsumed() const;
	ConsumableType GetType() const;

protected:
	bool m_Consumed;
	ConsumableType m_Type;
};

// ============================ Beast ==================================
class Beast : public GameObject, public IDamageable, public IInteractable, public IModifiable {
public:
	Beast(const unsigned int& x, const unsigned int& y);

	wchar_t GetRepresentation() const override;
	void Heal(const uint8_t& amount) override;
	void Interact(GameObject* with) override;

	void Modify(IModifier* modifier) override;
};

// ============================ Player ==================================
class Player : public GameObject, public IMoveable, public IDamageable, public IModifiable {
public:
	Player(const unsigned int& x, const unsigned int& y);

	void Move(const unsigned int& dX, const unsigned int& dY) override;

	void Modify(IModifier* modifier) override;

	bool IsArmed() const;
	bool& IsArmed();

	bool HasTreasure() const;
	bool& HasTreasure();
private:
	bool m_Armed;
	bool m_HasTreasure;
};





