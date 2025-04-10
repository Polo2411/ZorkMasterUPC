// vitality_potion.cpp
#include "vitality_potion.h"

VitalityPotion::VitalityPotion(const std::string& name,
    const std::string& description,
    int extraMax,
    int heal,
    const std::string& direction,
    Entity* parent)
    : Item(name, description, direction, parent),
    extraMax(extraMax),
    heal(heal)
{
}

int VitalityPotion::GetExtraMax() const {
    return extraMax;
}
int VitalityPotion::GetHealAmount() const {
    return heal;
}
