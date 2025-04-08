#include "health_potion.h"
#include <iostream>

HealthPotion::HealthPotion(const std::string& name, const std::string& description, int healAmount, const std::string& direction, Entity* parent)
    : Item(name, description, direction, parent)
{
    // Forzar que la poción siempre restaure 50 puntos
    this->healAmount = 50;
}

void HealthPotion::Use() {
    std::cout << "Drinking " << name << " and restoring " << healAmount << " health.\n";
}

int HealthPotion::GetHealAmount() const {
    return healAmount;
}

