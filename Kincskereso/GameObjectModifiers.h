#pragma once

#include <memory>

class IModifiable;

// MODIFIER FIELD =================================================
template<typename T>
struct  ModifierField {

	// OriginalValue can be set to defaultValue too, when Apply() is called its value will change anyway
	// when Restore() will only actually restore from the OriginalValue once Apply() has been called
	ModifierField(T defaultValue) : ModifiedValue(defaultValue), OriginalValue(defaultValue), Applied(false) {}


	void SetModifiedValue(T newValue);
	T GetModifiedValue() const;

	void Apply(T& modifiableField);
	void Restore(T& modifiableField);

private:
	T ModifiedValue;
	T OriginalValue;

	bool Applied;
};

// IMODIFIER ========================================================
class IModifier {
public:
	IModifier(IModifiable* subject);
	virtual ~IModifier() = default;

	virtual std::unique_ptr<IModifier> Clone() const = 0;

	void Apply();
	void Restore();

public:
	IModifiable* Subject;
};

struct ModifierComparator {
	bool operator()(const std::unique_ptr<IModifier>& a, const std::unique_ptr<IModifier>& b) {
		return a->Subject < b->Subject;
	}
};

// ENVIRONMENT MODIFIER ==============================================
class EnvironmentModifier : public IModifier {
public:
	EnvironmentModifier(IModifiable* subject, bool walkable) : IModifier(subject), m_Walkable(walkable) {}

	void SetWalkable(bool newVal) {
		m_Walkable.SetModifiedValue(newVal);
	}

	bool GetWalkable() const {
		return m_Walkable.GetModifiedValue();
	}

	std::unique_ptr<IModifier> Clone() const override
	{
		return std::make_unique<EnvironmentModifier>(*this);
	}

private:
	ModifierField<bool> m_Walkable;

	friend class Environment;
};

// CONSUMABLE MODIFIER ================================================
class ConsumableModifier : public IModifier {
public:
	ConsumableModifier(IModifiable* subjecet, bool consumed) : IModifier(subjecet), m_Consumed(consumed) {}
	
	void SetConsumes(bool newVal) {
		m_Consumed.SetModifiedValue(newVal);
	}

	std::unique_ptr<IModifier> Clone() const override
	{
		return std::make_unique<ConsumableModifier>(*this);
	}

private:
	ModifierField<bool> m_Consumed;

	friend class Consumable;
};

// BEAST MODIFIER ===================================================
class BeastModifier : public IModifier {
public:
	BeastModifier(IModifiable* subject, bool isAlive) : IModifier(subject), m_IsAlive(isAlive) {}
	
	void SetAlive(bool newVal) {
		m_IsAlive.SetModifiedValue(newVal);
	}

	std::unique_ptr<IModifier> Clone() const override
	{
		return std::make_unique<BeastModifier>(*this);
	}

private:
	ModifierField<bool> m_IsAlive;

	friend class Beast;
};

// PLAYER MODIFIER ====================================================
class PlayerModifier : public IModifier {
public:
	PlayerModifier(IModifiable* subject, uint8_t health, bool isArmed) : IModifier(subject), m_Health(health), m_IsArmed(isArmed) {}
	
	void AddHealth(int8_t amount) {
		auto newHealth = m_Health.GetModifiedValue() + amount;
		if (newHealth > 2) m_Health.SetModifiedValue(2);
		else if (newHealth < 0) m_Health.SetModifiedValue(0);
		else m_Health.SetModifiedValue(newHealth);
	}

	void SetArmed(bool newVal) {
		m_IsArmed.SetModifiedValue(newVal);
	}

	std::unique_ptr<IModifier> Clone() const override
	{ 
		return std::make_unique<PlayerModifier>(*this);
	}
	
private:
	ModifierField<uint8_t> m_Health;
	ModifierField<bool> m_IsArmed;

	friend class Player;
};

// IMODIFIABLE =========================================================
class IModifiable {
public:
	virtual ~IModifiable() = default;
	virtual void Modify(IModifier* modifier) = 0;
	virtual void Restore(IModifier* modifier) = 0;
};

// IMPLEMENTATIONS ====================================================

// ModifierField =======================================================
template<typename T>
void ModifierField<T>::SetModifiedValue(T newValue) {
	ModifiedValue = newValue;
}

template<typename T>
T ModifierField<T>::GetModifiedValue() const {
	return ModifiedValue;
}

template<typename T>
void ModifierField<T>::Apply(T& modifiableField) {
	OriginalValue = modifiableField;
	modifiableField = ModifiedValue;
	Applied = true;
}

template<typename T>
void ModifierField<T>::Restore(T& modifiableField) {
	if (Applied) {
		modifiableField = OriginalValue;
		Applied = false;
	}
}
