#include "player.h"
#include <iostream>
#include <algorithm>

Player::Player(const std::string& name, const std::string& description, Room* startRoom)
    : Creature(name, description, startRoom), currentRoom(startRoom)
{
    type = PLAYER;
    // Player's health is set via Creature (e.g., 100 by default)
}

void Player::Move(const std::string& direction) {
    Room* nextRoom = currentRoom->GetConnectedRoom(direction);
    if (nextRoom) {
        SetLocation(nextRoom);
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
    for (auto obj : inventory) {
        std::cout << " - " << obj->name << " (" << obj->GetDirection() << ")\n";
        // If the item is a Bag, list its contents.
        Bag* bag = dynamic_cast<Bag*>(obj);
        if (bag) {
            std::cout << "   Contents of " << bag->name << ":\n";
            bag->ListContents();
        }
    }
}

void Player::PickUp(Item* obj) {
    if (CanCarry(obj)) {
        inventory.push_back(obj);
        std::cout << "You picked up " << obj->name << "\n";
    }
    else {
        std::cout << "You cannot carry any more items in your hands.\n";
    }
}

void Player::Drop(Item* obj) {
    auto it = std::find(inventory.begin(), inventory.end(), obj);
    if (it != inventory.end()) {
        inventory.erase(it);
        std::cout << "You dropped " << obj->name << "\n";
        currentRoom->children.push_back(obj);
    }
    else {
        std::cout << "You don't have that item\n";
    }
}

Room* Player::GetCurrentRoom() const {
    return currentRoom;
}

std::vector<Item*> Player::GetInventory() const {
    return inventory;
}

void Player::Status() const {
    std::cout << "Status:\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Health: " << GetHealth() << "\n";
    std::cout << "Current Room: " << currentRoom->name << "\n";
    ShowInventory();
}

bool Player::CanCarry(Item* obj) const {
    // Bags do not count against the two-hand limit.
    Bag* bag = dynamic_cast<Bag*>(obj);
    if (bag) return true;

    // Count non-bag items in inventory.
    int count = 0;
    for (auto item : inventory) {
        if (!dynamic_cast<Bag*>(item)) {
            count++;
        }
    }
    return (count < 2);
}
