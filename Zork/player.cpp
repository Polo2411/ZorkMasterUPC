#include "player.h"
#include <iostream>
#include <algorithm>

Player::Player(const std::string& name, const std::string& description, Room* startRoom)
    : Creature(name, description, startRoom)
{
    type = PLAYER;
    // Health for the player is set by Creature's constructor (e.g., 100).
}

void Player::Move(const std::string& direction) {
    // Use Creature's location (inherited as 'location') as the current room.
    Room* nextRoom = GetLocation()->GetConnectedRoom(direction);
    if (nextRoom) {
        SetLocation(nextRoom);
        std::cout << "You move to: " << nextRoom->name << "\n";
        nextRoom->Look();
    }
    else {
        std::cout << "There is no path to " << direction << "\n";
    }
}

void Player::ShowInventory() const {
    std::cout << "Inventory:\n";
    for (auto obj : inventory) { // 'inventory' is inherited from Creature.
        std::cout << " - " << obj->name << "\n";
    }
}

void Player::PickUp(Item* obj) {
    inventory.push_back(obj);
    std::cout << "You picked up " << obj->name << "\n";
}

void Player::Drop(Item* obj) {
    auto it = std::find(inventory.begin(), inventory.end(), obj);
    if (it != inventory.end()) {
        inventory.erase(it);
        std::cout << "You dropped " << obj->name << "\n";
        GetLocation()->children.push_back(obj);
    }
    else {
        std::cout << "You don't have that item\n";
    }
}

Room* Player::GetCurrentRoom() const {
    return GetLocation();
}
