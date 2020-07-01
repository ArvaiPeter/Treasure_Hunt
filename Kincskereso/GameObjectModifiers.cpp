#include "GameObjectModifiers.h"

IModifier::IModifier(IModifiable* subject)
	: Subject(subject) {}

void IModifier::Apply() {
	Subject->Modify(this);
}

void IModifier::Restore() {
	Subject->Restore(this);
}