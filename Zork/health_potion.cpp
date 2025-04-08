#include "health_potion.h"
#include <iostream>

HealthPotion::HealthPotion(const std::string& name, const std::string& description, int healAmount, Entity* parent)
    : Item(name, description, parent), healAmount(healAmount)
{
}

void HealthPotion::Use() {
    std::cout << "You drink the " << name << " and restore " << healAmount << " health.\n";
}

int HealthPotion::GetHealAmount() const {
    return healAmount;
}
