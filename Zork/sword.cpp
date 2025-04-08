#include "sword.h"
#include <iostream>

Sword::Sword(const std::string& name, const std::string& description, int damage, const std::string& direction, Entity* parent)
    : Item(name, description, direction, parent)
{
    // Forzar que la espada siempre haga 20 de daño
    this->damage = 20;
}

void Sword::Use() {
    std::cout << "Swinging " << name << " dealing " << damage << " damage.\n";
}

int Sword::GetDamage() const {
    return damage;
}
