#include "player.h"
#include "exit.h"
#include "string_utils.h"
#include <iostream>
#include <algorithm>
#include <cctype>

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

    auto ents = currentRoom->GetEntities(dir);
    if (!ents.empty()) {
        std::vector<std::string> itemPhrases;
        for (auto e : ents) {
            std::string art = getIndefiniteArticle(e->name);
            itemPhrases.push_back(art + " " + e->name);
        }
        if (itemPhrases.size() == 1) {
            std::cout << "Here you see " << itemPhrases[0] << ".\n";
        }
        else if (itemPhrases.size() == 2) {
            std::cout << "Here you see " << itemPhrases[0]
                << " and " << itemPhrases[1] << ".\n";
        }
        else {
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

void Player::InsertItemToInventory(Item* item) {
    if (!item) return;
    if (CanCarry(item)) {
        inventory.push_back(item);
        std::cout << "You now have " << item->name << " in your inventory.\n";
    }
    else {
        std::cout << "You cannot carry more non-bag items. You drop " << item->name << " on the floor.\n";
        currentRoom->AddEntity(GetPlayerDirection(), item);
    }
}

void Player::RemoveItemFromInventory(Item* item) {
    // Busca el item exacto en el vector inventory y lo quita
    auto it = std::find(inventory.begin(), inventory.end(), item);
    if (it != inventory.end()) {
        inventory.erase(it);
    }
    // Si no se encuentra, no hace nada. 
}

Item* Player::FindItemInInventory(const std::string& itemName) {
    std::string searchName = toLower(itemName);
    for (auto it : inventory) {
        if (toLower(it->name) == searchName) {
            return it;
        }
    }
    return nullptr;
}

void Player::ShowInventory() const {
    std::vector<Item*> nonBags;
    std::vector<Bag*> bags;
    // Separa los �tems del inventario.
    for (auto obj : inventory) {
        if (auto bag = dynamic_cast<Bag*>(obj)) {
            bags.push_back(bag);
        }
        else {
            nonBags.push_back(obj);
        }
    }
    std::cout << "Inventory:\n";
    // Mostrar primero los �tems que no son Bag.
    for (auto item : nonBags) {
        std::cout << " - " << item->name << "\n";
    }
    // Luego, para cada Bag, mostrarla y su contenido.
    for (auto bag : bags) {
        std::cout << " - " << bag->name << "\n";
        std::cout << "   Contents of " << bag->name << ":\n";
        bag->ListContents();
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
    // Devuelve una copia del vector
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
    if (dynamic_cast<Bag*>(obj)) {
        return true;
    }
    int countNonBags = 0;
    for (auto x : inventory) {
        if (!dynamic_cast<Bag*>(x)) {
            countNonBags++;
        }
    }
    return (countNonBags < 2);
}

bool Player::HasOpenBag() const {
    for (auto i : inventory) {
        if (auto bag = dynamic_cast<Bag*>(i)) {
            if (bag->IsOpen())
                return true;
        }
    }
    auto ents = currentRoom->GetEntities(GetPlayerDirection());
    for (auto e : ents) {
        if (e->type == ITEM) {
            if (auto bag = dynamic_cast<Bag*>(e)) {
                if (bag->IsOpen())
                    return true;
            }
        }
    }
    return false;
}

