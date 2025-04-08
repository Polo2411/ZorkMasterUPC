#include "creature.h"
#include <algorithm>
#include <iostream>

Creature::Creature(const std::string& name, const std::string& description, Room* startRoom)
    : Entity(name, description), location(startRoom)
{
    type = CREATURE;
    health = 100;
    if (location)
        location->children.push_back(this);
}

void Creature::SetHealth(int hp) {
    health = hp;
}

int Creature::GetHealth() const {
    return health;
}

bool Creature::IsAlive() const {
    return (health > 0);
}

void Creature::AddItem(Item* itm) {
    inventory.push_back(itm);
}

void Creature::RemoveItem(Item* itm) {
    auto it = std::find(inventory.begin(), inventory.end(), itm);
    if (it != inventory.end())
        inventory.erase(it);
}

Room* Creature::GetLocation() const {
    return location;
}

void Creature::SetLocation(Room* newRoom) {
    if (location)
        location->children.remove(this);
    location = newRoom;
    if (location)
        location->children.push_back(this);
}
