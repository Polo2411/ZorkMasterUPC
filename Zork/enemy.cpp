#include "enemy.h"
#include <iostream>

Enemy::Enemy(const std::string& name, const std::string& description, Room* startRoom)
    : Creature(name, description, startRoom)
{
    // Could set different health or other stats for enemies
}

void Enemy::Attack(Creature* target) {
    std::cout << name << " attacks " << target->name << "\n";
    // Simple example: deals 10 damage
    target->SetHealth(target->GetHealth() - 10);
}

void Enemy::TakeDamage(int dmg) {
    health -= dmg;
    std::cout << name << " takes " << dmg << " damage, health is now " << health << "\n";
    if (!IsAlive())
        std::cout << name << " has been defeated.\n";
}
