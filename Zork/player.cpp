#include "player.h"
#include <iostream>

Player::Player(const std::string& name, const std::string& description, Room* startRoom)
    : Entity(name, description), currentRoom(startRoom)
{
    type = PLAYER;
}

void Player::Move(const std::string& direction) {
    Room* nextRoom = currentRoom->GetConnectedRoom(direction);
    if (nextRoom) {
        currentRoom = nextRoom;
        std::cout << "You move to: " << currentRoom->name << "\n";
        currentRoom->Look();
    }
    else {
        std::cout << "There is no path to " << direction << "\n";
    }
}

void Player::ShowInventory() const {
    std::cout << "Inventory:\n";
    for (auto obj : inventory)
        std::cout << " - " << obj->name << "\n";
}

void Player::PickUp(Item* obj) {
    inventory.push_back(obj);
    std::cout << "You picked up " << obj->name << "\n";
}

void Player::Drop(Item* obj) {
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (*it == obj) {
            inventory.erase(it);
            std::cout << "You dropped " << obj->name << "\n";
            currentRoom->children.push_back(obj);
            return;
        }
    }
    std::cout << "You don't have that item\n";
}

Room* Player::GetCurrentRoom() const {
    return currentRoom;
}
