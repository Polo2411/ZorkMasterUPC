#include "sword.h"
#include <iostream>

Sword::Sword(const std::string& name,
    const std::string& description,
    int damage,
    const std::string& direction,
    Entity* parent)
    : Item(name, description, direction, parent),
    damage(damage)
{
}

void Sword::Use() {
    std::cout << "Swinging " << name << " dealing " << damage << " damage.\n";
}

int Sword::GetDamage() const {
    return damage;
}

void Sword::SetDamage(int dmg) {
    damage = dmg;
}
