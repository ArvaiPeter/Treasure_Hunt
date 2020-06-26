#pragma once

class IModifiable;

class IModifier {
public:
	IModifier(IModifiable* subject);
	virtual ~IModifier() = default;

	template<typename T>
	static void Swap(T& a, T& b) {
		T temp = a;
		a = b;
		b = temp;
	}

	void Apply();
	void Restore();

public:
	bool Applied = false;
	IModifiable* Subject;
};

class EnvironmentModifier : public IModifier {
public:
	EnvironmentModifier(IModifiable* subject, bool walkable) : IModifier(subject), m_Walkable(walkable) {}

	void SetWalkable(bool newVal) {
		bool wasApplied = Applied;

		if (Applied) Restore();
		m_Walkable = newVal;
		if (wasApplied) Apply();
	}

	bool GetWalkable() const { return m_Walkable; }

private:
	bool m_Walkable;

	friend class Environment;
};

class ConsumableModifier : public IModifier {
public:
	ConsumableModifier(IModifiable* subjecet, bool consumed) : IModifier(subjecet), m_Consumed(consumed) {}
	
	void SetConsumes(bool newVal) {
		bool wasApplied = Applied;

		if (Applied) Restore();
		m_Consumed = newVal;
		if (wasApplied) Apply();
	}

	bool GetConsumed() const { return m_Consumed; }

private:
	bool m_Consumed;

	friend class Consumable;
};

class BeastModifier : public IModifier {
public:
	BeastModifier(IModifiable* subject, bool isAlive) : IModifier(subject), m_IsAlive(isAlive) {}
	
	void SetAlive(bool newVal) {
		bool wasApplied = Applied;

		if (Applied) Restore();
		m_IsAlive = newVal;
		if (wasApplied) Apply();
	}

	bool GetAlive() const { return m_IsAlive; }

private:
	bool m_IsAlive;

	friend class Beast;
};

class PlayerModifier : public IModifier {
public:
	PlayerModifier(IModifiable* subject, uint8_t health, bool isArmed) : IModifier(subject), m_Health(health), m_IsArmed(isArmed) {}
	
	void AddHealth(int8_t amount) {
		bool wasApplied = Applied;

		if (Applied) Restore();
		auto newHealth = m_Health + amount;
		
		if (newHealth >= 2) m_Health = 2;
		else if (newHealth < 0) m_Health = 0;
		else m_Health = newHealth;

		if (wasApplied) Apply();
	}

	uint8_t GetHealth() const { return m_Health; }

	void SetArmed(bool newVal) {
		bool wasApplied = Applied;

		if (Applied) Restore();
		m_IsArmed = newVal;
		if (wasApplied) Apply();
	}

	bool GetArmed() const { return m_IsArmed; }
	
private:
	uint8_t m_Health;
	bool m_IsArmed;

	friend class Player;
};

class IModifiable {
public:
	virtual ~IModifiable() = default;
	virtual void Modify(IModifier* modifier) = 0;
};