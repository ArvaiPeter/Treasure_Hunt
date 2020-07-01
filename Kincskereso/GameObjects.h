#pragma once
#include <cstdint>
#include <map>

#include "GameObjectRepresentations.h"
#include "GameObjectModifiers.h"

// ============================ Game Object ==================================
class GameObject
{
public:
	GameObject(unsigned int x, unsigned int y, const wchar_t repr);
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
	virtual void Move(unsigned int dX, unsigned int dY) = 0;
};

class IInteractable {
protected:
	virtual ~IInteractable() = default;
	virtual void Interact(GameObject* with) = 0;
};

class IDamageable {
public:
	virtual void TakeDamage(uint8_t dmg);
	virtual void Heal(uint8_t amount);

	virtual bool IsAlive() const;
	virtual uint8_t GetHealth() const;
	virtual uint8_t GetMaxHealth() const;

protected:
	IDamageable(uint8_t health, uint8_t maxHealth, bool alive = true);
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
	Environment(unsigned int x, unsigned int y, EnvironmentType type);

	void Interact(GameObject* with) override;

	void Modify(IModifier* modifier) override;
	void Restore(IModifier* modifier) override;

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
	Consumable(unsigned int x, unsigned int y, ConsumableType type);

	wchar_t GetRepresentation() const override;
	void Interact(GameObject* with) override;

	void Modify(IModifier* modifier) override;
	void Restore(IModifier* modifier) override;

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
	Beast(unsigned int x, unsigned int y);

	wchar_t GetRepresentation() const override;
	void Heal(uint8_t amount) override;
	void Interact(GameObject* with) override;

	void Modify(IModifier* modifier) override;
	void Restore(IModifier* modifier) override;
};

// ============================ Player ==================================
class Player : public GameObject, public IMoveable, public IDamageable, public IModifiable {
public:
	Player(unsigned int x, unsigned int y);

	void Move(unsigned int dX, unsigned int dY) override;

	void Modify(IModifier* modifier) override;
	void Restore(IModifier* modifier) override;

	bool IsArmed() const;
	bool& IsArmed();

	bool HasTreasure() const;
	bool& HasTreasure();
private:
	bool m_Armed;
	bool m_HasTreasure;
};





