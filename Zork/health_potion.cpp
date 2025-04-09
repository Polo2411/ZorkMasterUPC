#include "health_potion.h"
#include <iostream>

HealthPotion::HealthPotion(const std::string& name,
    const std::string& description,
    int healAmount,
    const std::string& direction,
    Entity* parent)
    : Item(name, description, direction, parent)
{
    // Forzamos que la poci�n restaure siempre 50
    this->healAmount = 50;
}

void HealthPotion::Use() {
    std::cout << "Drinking " << name << " and restoring " << healAmount << " health.\n";
}

int HealthPotion::GetHealAmount() const {
    return healAmount;
}
