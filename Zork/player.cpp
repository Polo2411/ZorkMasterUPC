#include "player.h"
#include "exit.h"
#include "string_utils.h"  // Para toLower
#include <iostream>
#include <algorithm>

Player::Player(const std::string& name, const std::string& description, Room* startRoom)
    : Creature(name, description, startRoom), currentRoom(startRoom), playerDirection("center")
{
    type = PLAYER;
}

void Player::Move(const std::string& direction) {
    std::string dir = toLower(direction);
    if (!currentRoom->HasDirection(dir)) {
        std::cout << "That direction doesn't exist in this room.\n";
        return;
    }
    playerDirection = dir;
    std::cout << "You move to " << dir << " of " << currentRoom->name << ".\n";

    // Recopilamos las entidades en esta dirección
    auto ents = currentRoom->GetEntities(dir);
    if (!ents.empty()) {
        // Construimos un vector de frases, cada una con "a/an <nombre>"
        std::vector<std::string> itemPhrases;
        for (auto e : ents) {
            std::string art = getIndefiniteArticle(e->name);
            itemPhrases.push_back(art + " " + e->name);
        }

        // Mostramos la lista resultante con "and" antes del último elemento
        if (itemPhrases.size() == 1) {
            std::cout << "Here you see " << itemPhrases[0] << ".\n";
        }
        else if (itemPhrases.size() == 2) {
            std::cout << "Here you see " << itemPhrases[0]
                << " and " << itemPhrases[1] << ".\n";
        }
        else {
            // Si hay 3 o más, unimos con comas y "and" final
            std::cout << "Here you see ";
            for (size_t i = 0; i < itemPhrases.size(); i++) {
                if (i == 0) {
                    std::cout << itemPhrases[i];
                }
                else if (i < itemPhrases.size() - 1) {
                    std::cout << ", " << itemPhrases[i];
                }
                else {
                    std::cout << " and " << itemPhrases[i];
                }
            }
            std::cout << ".\n";
        }
    }
}

void Player::ExitRoom(const std::string& direction) {
    std::string dir = toLower(direction);
    auto ents = currentRoom->GetEntities(dir);
    Exit* exitPtr = nullptr;
    for (auto e : ents) {
        if (e->type == EXIT) {
            exitPtr = dynamic_cast<Exit*>(e);
            break;
        }
    }
    if (!exitPtr) {
        std::cout << "No exit found in " << dir << "\n";
        return;
    }
    if (exitPtr->GetState() == LOCKED) {
        std::cout << "That exit is locked.\n";
        return;
    }
    Room* dest = exitPtr->GetDestination();
    if (!dest) {
        std::cout << "This exit leads nowhere.\n";
        return;
    }
    currentRoom = dest;
    playerDirection = "center";
    std::cout << "You exit towards " << dir << " and arrive at " << dest->name << "\n";
    currentRoom->Look();
}

void Player::TakeItem(const std::string& itemName) {
    std::string searchName = toLower(itemName);
    auto ents = currentRoom->GetEntities(GetPlayerDirection());
    Entity* found = nullptr;
    for (auto e : ents) {
        if (e->type == ITEM) {
            if (toLower(e->name) == searchName) {
                found = e;
                break;
            }
        }
    }
    if (!found) {
        std::cout << "No " << itemName << " here.\n";
        return;
    }
    Item* i = dynamic_cast<Item*>(found);
    if (i) {
        if (CanCarry(i)) {
            inventory.push_back(i);
            currentRoom->RemoveEntity(GetPlayerDirection(), i);
            std::cout << "You picked up " << itemName << "\n";
        }
        else {
            std::cout << "You cannot carry more non-bag items.\n";
        }
    }
    else {
        std::cout << itemName << " is not an item.\n";
    }
}

void Player::DropItem(const std::string& itemName) {
    std::string searchName = toLower(itemName);
    auto it = std::find_if(inventory.begin(), inventory.end(),
        [&](Item* obj) { return toLower(obj->name) == searchName; });
    if (it == inventory.end()) {
        std::cout << "You don't have " << itemName << "\n";
        return;
    }
    Item* itemPtr = *it;
    inventory.erase(it);
    currentRoom->AddEntity(GetPlayerDirection(), itemPtr);
    std::cout << "You dropped " << itemName << " at " << GetPlayerDirection() << "\n";
}

void Player::ShowInventory() const {
    std::cout << "Inventory:\n";
    for (auto obj : inventory) {
        std::cout << " - " << obj->name << "\n";
        if (auto bag = dynamic_cast<Bag*>(obj)) {
            std::cout << "   Contents of " << bag->name << ":\n";
            bag->ListContents();
        }
    }
}

Room* Player::GetCurrentRoom() const {
    return currentRoom;
}

std::string Player::GetPlayerDirection() const {
    return playerDirection;
}

void Player::SetPlayerDirection(const std::string& dir) {
    playerDirection = toLower(dir);
}

std::vector<Item*> Player::GetInventory() const {
    return inventory;
}

void Player::Status() const {
    std::cout << "=== Player Status ===\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Health: " << GetHealth() << "\n";
    std::cout << "Location: " << currentRoom->name << ", at " << playerDirection << "\n";
    ShowInventory();
    std::cout << "=====================\n";
}

bool Player::CanCarry(Item* obj) const {
    if (dynamic_cast<Bag*>(obj))
        return true;
    int countNonBags = 0;
    for (auto x : inventory) {
        if (!dynamic_cast<Bag*>(x)) {
            countNonBags++;
        }
    }
    return (countNonBags < 2);
}