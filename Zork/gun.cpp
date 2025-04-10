#include "gun.h"

Gun::Gun(const std::string& name,
    const std::string& description,
    int damage,
    const std::string& direction,
    Entity* parent)
    : Item(name, description, direction, parent),
    damage(damage),
    ammoCount(0)  // Arranca con 0 balas
{
}

int Gun::GetDamage() const {
    return damage;
}

void Gun::SetDamage(int dmg) {
    damage = dmg;
}

int Gun::GetAmmoCount() const {
    return ammoCount;
}

void Gun::AddAmmo(int amount) {
    ammoCount += amount;
}
