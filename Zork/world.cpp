#include "world.h"
#include "sword.h"
#include "bag.h"
#include "health_potion.h"
#include "exit.h"
#include "key.h"
#include "item.h"
#include "enemy.h"  // define Enemy, Demon, DemonKnight
#include "player.h"
#include "string_utils.h"
#include <iostream>
#include <sstream>
#include <map>
#include <functional>
#include <vector>
#include <algorithm>
#include <cctype>

std::vector<std::string> tokenize(const std::string& input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        token = toLower(token);
        tokens.push_back(token);
    }
    return tokens;
}

World::World() {
    // 2 salas
    Room* roomOne = new Room("RoomOne", "This room is sparsely furnished.");
    Room* roomTwo = new Room("RoomTwo", "A second room with a peculiar atmosphere.");

    // RoomOne
    Sword* sword = new Sword("Sword", "A shiny sword on the floor.", 20, "east", roomOne);
    roomOne->AddEntity("east", sword);

    Bag* bag = new Bag("Bag", "A sturdy leather bag.", 5, "south", roomOne);
    roomOne->AddEntity("south", bag);

    Key* key = new Key("Key", "A small key.", "west", roomTwo, roomOne);
    roomOne->AddEntity("west", key);

    // Exit
    Exit* exitOne = new Exit("Door", "A passage to another room.", roomOne, roomTwo);
    exitOne->SetState(CLOSED);
    roomOne->AddEntity("north", exitOne);
    roomTwo->AddEntity("south", exitOne);

    HealthPotion* potion = new HealthPotion("HealthPotion", "Heals you quite a bit.", 50, "east", roomTwo);
    roomTwo->AddEntity("east", potion);

    // Crear Demon en la segunda sala
    Demon* demon = new Demon("Demon", "A fiendish creature from the underworld.", roomTwo);
    enemies.push_back(demon);

    rooms.push_back(roomOne);
    rooms.push_back(roomTwo);

    player = new Player("Player", "An intrepid adventurer.", roomOne);
}

World::~World() {
    for (size_t i = 0; i < rooms.size(); i++) {
        delete rooms[i];
    }
    // Liberar enemigos
    for (size_t i = 0; i < enemies.size(); i++) {
        delete enemies[i];
    }
    delete player;
}

bool World::equalIgnoreCase(const std::string& a, const std::string& b) const {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++) {
        if (std::tolower((unsigned char)a[i]) !=
            std::tolower((unsigned char)b[i])) {
            return false;
        }
    }
    return true;
}

Bag* World::findBag(const std::string& containerName) {
    // en el inventario
    auto inv = player->GetInventory();
    for (size_t i = 0; i < inv.size(); i++) {
        if (equalIgnoreCase(inv[i]->name, containerName)) {
            return dynamic_cast<Bag*>(inv[i]);
        }
    }
    // en la sala (dirección actual)
    auto ents = player->GetCurrentRoom()->GetEntities(player->GetPlayerDirection());
    for (size_t i = 0; i < ents.size(); i++) {
        if (ents[i]->type == ITEM && equalIgnoreCase(ents[i]->name, containerName)) {
            return dynamic_cast<Bag*>(ents[i]);
        }
    }
    return nullptr;
}

Exit* World::findExit(const std::string& exitName) {
    auto ents = player->GetCurrentRoom()->GetEntities(player->GetPlayerDirection());
    for (size_t i = 0; i < ents.size(); i++) {
        if (ents[i]->type == EXIT && equalIgnoreCase(ents[i]->name, exitName)) {
            return dynamic_cast<Exit*>(ents[i]);
        }
    }
    return nullptr;
}

Item* World::findItem(const std::string& itemName) {
    // primero en inventario
    auto inv = player->GetInventory();
    for (size_t i = 0; i < inv.size(); i++) {
        if (equalIgnoreCase(inv[i]->name, itemName)) {
            return inv[i];
        }
    }
    // en sala
    auto ents = player->GetCurrentRoom()->GetEntities(player->GetPlayerDirection());
    for (size_t i = 0; i < ents.size(); i++) {
        if (ents[i]->type == ITEM && equalIgnoreCase(ents[i]->name, itemName)) {
            return dynamic_cast<Item*>(ents[i]);
        }
    }
    return nullptr;
}

void World::openExit(Exit* exitPtr) {
    if (!exitPtr) return;
    if (exitPtr->GetState() == OPEN) {
        std::cout << "It's already open.\n";
        return;
    }
    if (exitPtr->GetState() == LOCKED) {
        std::cout << "It seems locked.\n";
        return;
    }
    bool hasKey = false;
    Item* chosenKey = nullptr;
    auto inv = player->GetInventory();
    for (size_t i = 0; i < inv.size(); i++) {
        if (auto realKey = dynamic_cast<Key*>(inv[i])) {
            if (realKey->GetOpensRoom() == exitPtr->GetDestination()) {
                hasKey = true;
                chosenKey = realKey;
                break;
            }
        }
    }
    if (!hasKey) {
        std::cout << "You don't have the right key to open this exit.\n";
        return;
    }
    exitPtr->SetState(OPEN);
    std::cout << "You use " << chosenKey->name
        << " to open the " << exitPtr->name << ".\n";
}

void World::closeExit(Exit* exitPtr) {
    if (!exitPtr) return;
    if (exitPtr->GetState() == CLOSED) {
        std::cout << "It's already closed.\n";
        return;
    }
    if (exitPtr->GetState() == LOCKED) {
        std::cout << "It's locked.\n";
        return;
    }
    bool hasKey = false;
    Item* chosenKey = nullptr;
    auto inv = player->GetInventory();
    for (size_t i = 0; i < inv.size(); i++) {
        if (auto realKey = dynamic_cast<Key*>(inv[i])) {
            if (realKey->GetOpensRoom() == exitPtr->GetDestination()) {
                hasKey = true;
                chosenKey = realKey;
                break;
            }
        }
    }
    if (!hasKey) {
        std::cout << "You don't have the key to close this exit.\n";
        return;
    }
    exitPtr->SetState(CLOSED);
    std::cout << "You use " << chosenKey->name
        << " to close the " << exitPtr->name << ".\n";
}

void World::Run() {
    std::cout << "Welcome to Zork\n";
    player->GetCurrentRoom()->Look();

    while (true) {
        std::cout << "\n> ";
        std::string command;
        std::getline(std::cin, command);
        if (command == "quit" || command == "exit") {
            break;
        }
        ProcessCommand(command);
        UpdateEnemies(); // Llamada tras cada comando
    }
}

void World::ProcessCommand(const std::string& command) {
    auto tokens = tokenize(command);
    if (tokens.empty()) {
        std::cout << "Please type a command.\n";
        return;
    }

    std::string verb = tokens[0];

    if (player->HasOpenBag()) {
        if (verb == "move" || verb == "exit" || verb == "drop") {
            std::cout << "You have an open bag. Close it before performing that action.\n";
            return;
        }
    }

    std::map<std::string, std::function<void(const std::vector<std::string>&)>> cmdMap;

    cmdMap["look"] = [this](auto args) {
        if (args.empty()) {
            player->GetCurrentRoom()->Look();
        }
        else if (args.size() == 1) {
            std::string itemName = args[0];
            Item* itemPtr = findItem(itemName);
            if (!itemPtr) {
                std::cout << "You don't see any " << itemName << " here.\n";
            }
            else {
                std::cout << itemPtr->name << "\n"
                    << itemPtr->description << "\n";
            }
        }
        else {
            std::cout << "Usage: look [itemName]\n";
        }
        };

    cmdMap["move"] = [this](auto args) {
        if (args.empty()) {
            std::cout << "Move where? (north, south, east, west, center)\n";
            return;
        }
        player->Move(args[0]);
        };

    cmdMap["exit"] = [this](auto args) {
        if (args.empty()) {
            std::cout << "Exit which direction?\n";
            return;
        }
        player->ExitRoom(args[0]);
        };

    cmdMap["take"] = [this](auto args) {
        if (args.empty()) {
            std::cout << "Take what?\n";
            return;
        }
        if (args.size() == 1) {
            player->TakeItem(args[0]);
            return;
        }
        if (args.size() == 3 && args[1] == "from") {
            std::string itemName = args[0];
            std::string containerName = args[2];
            Bag* bagPtr = findBag(containerName);
            if (!bagPtr) {
                std::cout << "No bag named " << containerName << " here.\n";
                return;
            }
            if (!bagPtr->IsOpen()) {
                std::cout << containerName << " is not open.\n";
                return;
            }
            Item* itemPtr = bagPtr->RemoveItem(itemName);
            if (!itemPtr) return;
            player->InsertItemToInventory(itemPtr);
        }
        else {
            std::cout << "Usage: take <item> [from <container>]\n";
        }
        };

    cmdMap["drop"] = [this](auto args) {
        if (args.empty()) {
            std::cout << "Drop what?\n";
            return;
        }
        player->DropItem(args[0]);
        };

    cmdMap["status"] = [this](auto args) {
        player->Status();
        };

    cmdMap["open"] = [this](auto args) {
        if (args.empty()) {
            std::cout << "Open what?\n";
            return;
        }
        std::string targetName = args[0];
        Bag* bagPtr = findBag(targetName);
        if (bagPtr) {
            bagPtr->Use();
            return;
        }
        Exit* exitPtr = findExit(targetName);
        if (exitPtr) {
            openExit(exitPtr);
            return;
        }
        std::cout << "No exit here named " << targetName
            << ", and no bag named " << targetName << ".\n";
        };

    cmdMap["close"] = [this](auto args) {
        if (args.empty()) {
            std::cout << "Close what?\n";
            return;
        }
        std::string targetName = args[0];
        Bag* bagPtr = findBag(targetName);
        if (bagPtr) {
            bagPtr->Close();
            return;
        }
        Exit* exitPtr = findExit(targetName);
        if (exitPtr) {
            closeExit(exitPtr);
            return;
        }
        std::cout << "No exit here named " << targetName
            << ", and no bag named " << targetName << ".\n";
        };

    cmdMap["save"] = [this](auto args) {
        if (args.size() < 3) {
            std::cout << "Usage: save <item> in <container>\n";
            return;
        }
        std::string itemName = args[0];
        if (args[1] != "in") {
            std::cout << "Usage: save <item> in <container>\n";
            return;
        }
        std::string containerName = args[2];
        Item* toSave = player->FindItemInInventory(itemName);
        if (!toSave) {
            std::cout << "You don't have " << itemName << ".\n";
            return;
        }
        Bag* bagPtr = findBag(containerName);
        if (!bagPtr) {
            std::cout << "No bag named " << containerName << " here.\n";
            return;
        }
        if (!bagPtr->IsOpen()) {
            std::cout << containerName << " is not open.\n";
            return;
        }
        player->RemoveItemFromInventory(toSave);
        if (!bagPtr->AddItem(toSave)) {
            player->InsertItemToInventory(toSave);
        }
        else {
            std::cout << "Saved " << itemName << " in " << containerName << ".\n";
        }
        };

    // Comando "attack <enemy>"
    cmdMap["attack"] = [this](auto args) {
        if (args.empty()) {
            std::cout << "Attack who?\n";
            return;
        }
        std::string enemyName = args[0];
        // Buscamos un Enemy con ese nombre en la sala
        Room* curr = player->GetCurrentRoom();
        Enemy* targetEnemy = nullptr;

        // Recorremos children de la sala 
        for (std::list<Entity*>::iterator it = curr->children.begin();
            it != curr->children.end(); ++it)
        {
            Entity* ent = *it;
            if (ent->type == CREATURE) {
                Enemy* en = dynamic_cast<Enemy*>(ent);
                if (en && equalIgnoreCase(en->name, enemyName)) {
                    targetEnemy = en;
                    break;
                }
            }
        }
        if (!targetEnemy) {
            std::cout << "No enemy called " << enemyName << " here.\n";
            return;
        }
        // Hacer que el player ataque
        player->AttackEnemy(targetEnemy);
        };

    tokens.erase(tokens.begin());
    std::map<std::string, std::function<void(const std::vector<std::string>&)>>::iterator it = cmdMap.find(verb);
    if (it != cmdMap.end()) {
        it->second(tokens);
    }
    else {
        std::cout << "I don't understand " << verb << "\n";
    }
}

// Llamamos a Update de cada enemigo
void World::UpdateEnemies() {
    for (size_t i = 0; i < enemies.size(); i++) {
        enemies[i]->Update(player);
    }
}
