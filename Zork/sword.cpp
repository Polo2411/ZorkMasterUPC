#include "sword.h"
#include <iostream>

Sword::Sword(const std::string& name, const std::string& description, int damage, Entity* parent)
    : Item(name, description, parent), damage(damage)
{
    // Sword specific; damage is stored here.
}

void Sword::Use() {
    std::cout << "You swing the " << name << ", dealing " << damage << " damage.\n";
}

int Sword::GetDamage() const {
    return damage;
}
