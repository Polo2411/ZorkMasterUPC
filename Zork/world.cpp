#include "world.h"
#include "sword.h"
#include "bag.h"
#include "health_potion.h"
#include "exit.h"
#include "key.h"
#include "item.h"
#include "string_utils.h"
#include <iostream>
#include <sstream>
#include <map>
#include <functional>
#include <vector>
#include <algorithm>
#include <cctype>

// Pasa el input a tokens en minúsculas
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
    // Creamos 2 salas
    Room* roomOne = new Room("RoomOne", "This room is sparsely furnished.");
    Room* roomTwo = new Room("RoomTwo", "A second room with a peculiar atmosphere.");

    // RoomOne
    Sword* sword = new Sword("Sword", "A shiny sword on the floor.", 20, "east", roomOne);
    roomOne->AddEntity("east", sword);

    Bag* bag = new Bag("Bag", "A sturdy leather bag.", 5, "south", roomOne);
    roomOne->AddEntity("south", bag);

    Key* key = new Key("Key", "A small key.", "west", roomTwo, roomOne);
    roomOne->AddEntity("west", key);

    // Creamos UNA sola exit. 
    Exit* exitOne = new Exit("Door", "A passage to another room.", roomOne, roomTwo);
    exitOne->SetState(CLOSED);

    // Añadir la exitOne a RoomOne->"north"
    roomOne->AddEntity("north", exitOne);
    // Y la MISMA exitOne a RoomTwo->"south"
    roomTwo->AddEntity("south", exitOne);

    // RoomTwo
    HealthPotion* potion = new HealthPotion("HealthPotion", "Heals you quite a bit.", 50, "east", roomTwo);
    roomTwo->AddEntity("east", potion);

    rooms.push_back(roomOne);
    rooms.push_back(roomTwo);

    player = new Player("Player", "An intrepid adventurer.", roomOne);
}

World::~World() {
    for (auto r : rooms) {
        delete r;
    }
    delete player;
}

bool World::equalIgnoreCase(const std::string& a, const std::string& b) const {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++) {
        if (std::tolower(static_cast<unsigned char>(a[i])) !=
            std::tolower(static_cast<unsigned char>(b[i]))) {
            return false;
        }
    }
    return true;
}

Bag* World::findBag(const std::string& containerName) {
    // Buscar en inventario
    auto inv = player->GetInventory();
    for (auto i : inv) {
        if (equalIgnoreCase(i->name, containerName)) {
            return dynamic_cast<Bag*>(i);
        }
    }
    // Buscar en la sala actual
    auto ents = player->GetCurrentRoom()->GetEntities(player->GetPlayerDirection());
    for (auto e : ents) {
        if (e->type == ITEM && equalIgnoreCase(e->name, containerName)) {
            return dynamic_cast<Bag*>(e);
        }
    }
    return nullptr;
}

// Busca un Exit en la dirección actual
Exit* World::findExit(const std::string& exitName) {
    auto ents = player->GetCurrentRoom()->GetEntities(player->GetPlayerDirection());
    for (auto e : ents) {
        if (e->type == EXIT && equalIgnoreCase(e->name, exitName)) {
            return dynamic_cast<Exit*>(e);
        }
    }
    return nullptr;
}

// Busca un Item (no solo Bag) en la dirección actual o en el inventario
Item* World::findItem(const std::string& itemName) {
    // Primero en inventario
    auto inv = player->GetInventory();
    for (auto i : inv) {
        if (equalIgnoreCase(i->name, itemName)) {
            return i; // i es un Item*
        }
    }
    // Luego en la sala/dirección actual
    auto ents = player->GetCurrentRoom()->GetEntities(player->GetPlayerDirection());
    for (auto e : ents) {
        if (e->type == ITEM && equalIgnoreCase(e->name, itemName)) {
            return dynamic_cast<Item*>(e);
        }
    }
    return nullptr;
}

// Apertura de Exit (requerimos Key que apunte al Destination)
void World::openExit(Exit* exitPtr) {
    if (!exitPtr) return;
    if (exitPtr->GetState() == OPEN) {
        std::cout << "It's already open.\n";
        return;
    }
    if (exitPtr->GetState() == LOCKED) {
        std::cout << "It's locked.\n";
        return;
    }
    bool hasKey = false;
    Item* chosenKey = nullptr;
    auto inv = player->GetInventory();
    for (auto it : inv) {
        if (auto realKey = dynamic_cast<Key*>(it)) {
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

// Cierre de Exit
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
    for (auto it : inv) {
        if (auto realKey = dynamic_cast<Key*>(it)) {
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
    }
}

void World::ProcessCommand(const std::string& command) {
    auto tokens = tokenize(command);
    if (tokens.empty()) {
        std::cout << "Please type a command.\n";
        return;
    }

    // Si hay una Bag abierta, se bloquean "move", "exit", "drop"
    std::string verb = tokens[0];
    if (player->HasOpenBag()) {
        if (verb == "move" || verb == "exit" || verb == "drop") {
            std::cout << "You have an open bag. Close it before performing that action.\n";
            return;
        }
    }

    // Definimos comandos
    std::map<std::string, std::function<void(const std::vector<std::string>&)>> cmdMap;

    cmdMap["look"] = [this](auto args) {
        // "look" normal, o "look <item>"
        if (args.empty()) {
            // Sin argumentos => look general
            player->GetCurrentRoom()->Look();
            return;
        }
        // Si hay un segundo token => "look sword", etc.
        if (args.size() == 1) {
            std::string itemName = args[0];
            // Buscar item en la dirección actual o en el inventario
            Item* foundItem = findItem(itemName);
            if (!foundItem) {
                std::cout << "You don't see any " << itemName << " here.\n";
                return;
            }
            // Mostrar su nombre y descripción
            std::cout << foundItem->name << "\n";
            std::cout << foundItem->description << "\n";
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
        // Primero buscar la Bag
        Bag* bagPtr = findBag(targetName);
        if (bagPtr) {
            bagPtr->Use();  // abre la Bag
            return;
        }
        // Luego buscar la Exit
        Exit* exitPtr = findExit(targetName);
        if (exitPtr) {
            openExit(exitPtr);
            return;
        }
        // Si no es Bag ni Exit
        // Mensajes diferenciados según "bag" o "exit"? 
        // Como no sabemos qué intentaba el usuario (tal vez 'door'), decimos:
        std::cout << "No exit here named " << targetName << ", and no bag named " << targetName << ".\n";
        };

    cmdMap["close"] = [this](auto args) {
        if (args.empty()) {
            std::cout << "Close what?\n";
            return;
        }
        std::string targetName = args[0];
        // Bag?
        Bag* bagPtr = findBag(targetName);
        if (bagPtr) {
            bagPtr->Close();
            return;
        }
        // Exit?
        Exit* exitPtr = findExit(targetName);
        if (exitPtr) {
            closeExit(exitPtr);
            return;
        }
        // Ni bag ni exit
        std::cout << "No exit here, and no bag named " << targetName << ".\n";
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

    // Ejecutar
    tokens.erase(tokens.begin()); // quita el verb
    auto it = cmdMap.find(verb);
    if (it != cmdMap.end()) {
        it->second(tokens);
    }
    else {
        std::cout << "I don't understand " << verb << "\n";
    }
}
