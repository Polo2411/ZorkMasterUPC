#include "world.h"
#include "sword.h"
#include "bag.h"
#include "health_potion.h"
#include "exit.h"
#include "string_utils.h"  // toLower() ya está definida aquí
#include <iostream>
#include <sstream>
#include <map>
#include <functional>
#include <vector>
#include <algorithm>
#include <cctype>

// Tokeniza la entrada del usuario, convirtiendo cada token a minúsculas.
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
    // Creamos dos salas.
    Room* roomOne = new Room("RoomOne", "This room is sparsely furnished.");
    Room* roomTwo = new Room("RoomTwo", "A second room with a peculiar atmosphere.");

    // En roomOne:
    // - Una Sword en "east".
    Sword* sword = new Sword("Sword", "A shiny sword on the floor.", 20, "east", roomOne);
    roomOne->AddEntity("east", sword);

    // - Una Bag en "south".
    Bag* bag = new Bag("Bag", "A sturdy leather bag.", 5, "south", roomOne);
    roomOne->AddEntity("south", bag);

    // - Una Key en "west" que abre el door (exit) a roomTwo.
    Key* key = new Key("Key", "A small key.", "west", roomTwo, roomOne);
    roomOne->AddEntity("west", key);

    // - Un Exit en "north" que lleva a roomTwo, inicialmente cerrada.
    Exit* exitOne = new Exit("Exit", "A passage to another room.", roomOne, roomTwo);
    exitOne->SetState(CLOSED);  // Cerrada inicialmente.
    roomOne->AddEntity("north", exitOne);

    // En roomTwo:
    // - Una HealthPotion en "east".
    HealthPotion* potion = new HealthPotion("HealthPotion", "Heals you quite a bit.", 50, "east", roomTwo);
    roomTwo->AddEntity("east", potion);

    // - La misma Exit, en sentido opuesto: ubicada en "south" (porque entramos por abajo).
    Exit* exitTwo = new Exit("Exit", "A way back to RoomOne.", roomTwo, roomOne);
    exitTwo->SetState(CLOSED);  // Cerrada inicialmente.
    roomTwo->AddEntity("south", exitTwo);

    rooms.push_back(roomOne);
    rooms.push_back(roomTwo);

    // El jugador inicia en roomOne.
    player = new Player("Player", "An intrepid adventurer.", roomOne);
}

World::~World() {
    for (auto r : rooms) {
        delete r;
    }
    delete player;
}

bool World::equalIgnoreCase(const std::string& a, const std::string& b) const {
    if (a.size() != b.size())
        return false;
    for (size_t i = 0; i < a.size(); i++) {
        if (std::tolower(static_cast<unsigned char>(a[i])) !=
            std::tolower(static_cast<unsigned char>(b[i])))
            return false;
    }
    return true;
}

Bag* World::findBag(const std::string& containerName) {
    // Buscar en el inventario del jugador.
    auto inv = player->GetInventory();
    for (auto i : inv) {
        if (equalIgnoreCase(i->name, containerName)) {
            return dynamic_cast<Bag*>(i);
        }
    }
    // Buscar en la sala, en la dirección actual.
    auto ents = player->GetCurrentRoom()->GetEntities(player->GetPlayerDirection());
    for (auto e : ents) {
        if (e->type == ITEM && equalIgnoreCase(e->name, containerName)) {
            return dynamic_cast<Bag*>(e);
        }
    }
    return nullptr;
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

    // Si hay una Bag abierta, se bloquean "move", "exit" y "drop" (se permite "take" y comandos de contenedor)
    std::string testVerb = tokens[0];
    if (player->HasOpenBag()) {
        if (testVerb == "move" || testVerb == "exit" || testVerb == "drop") {
            std::cout << "You have an open bag. Close it before performing that action.\n";
            return;
        }
    }

    std::map<std::string, std::function<void(const std::vector<std::string>&)>> cmdMap;

    cmdMap["look"] = [this](auto args) {
        player->GetCurrentRoom()->Look();
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

    // El comando "take" admite dos formas:
    // a) "take <item>" para tomar un item del suelo.
    // b) "take <item> from <container>" para extraer un item de dentro de una Bag.
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
                std::cout << "No container called " << containerName << " found here.\n";
                return;
            }
            if (!bagPtr->IsOpen()) {
                std::cout << containerName << " is not open.\n";
                return;
            }
            // Extraer el item de la Bag.
            Item* itemPtr = bagPtr->RemoveItem(itemName);
            if (!itemPtr) {
                // RemoveItem ya informa si no se encuentra el item.
                return;
            }
            // Insertar en el inventario del jugador.
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

    // Comando "open <container>" para abrir una Bag.
    cmdMap["open"] = [this](auto args) {
        if (args.empty()) {
            std::cout << "Open what?\n";
            return;
        }
        std::string containerName = args[0];
        Bag* bagPtr = findBag(containerName);
        if (!bagPtr) {
            std::cout << "No bag called " << containerName << " found.\n";
            return;
        }
        bagPtr->Use();
        };

    // Comando "close <container>" para cerrar una Bag.
    cmdMap["close"] = [this](auto args) {
        if (args.empty()) {
            std::cout << "Close what?\n";
            return;
        }
        std::string containerName = args[0];
        Bag* bagPtr = findBag(containerName);
        if (!bagPtr) {
            std::cout << "No bag called " << containerName << " found.\n";
            return;
        }
        bagPtr->Close();
        };

    // Comando "save <item> in <container>" para depositar un item (del inventario) en una Bag.
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
        // Buscar el item en el inventario del jugador.
        Item* toSave = player->FindItemInInventory(itemName);
        if (!toSave) {
            std::cout << "You don't have " << itemName << ".\n";
            return;
        }
        // Buscar la Bag.
        Bag* bagPtr = findBag(containerName);
        if (!bagPtr) {
            std::cout << "No container called " << containerName << " found.\n";
            return;
        }
        if (!bagPtr->IsOpen()) {
            std::cout << containerName << " is not open.\n";
            return;
        }
        // Quitar el ítem del inventario del jugador.
        player->RemoveItemFromInventory(toSave);
        // Insertarlo en la Bag.
        if (!bagPtr->AddItem(toSave)) {
            // Si falla, revertimos.
            player->InsertItemToInventory(toSave);
        }
        else {
            std::cout << "Saved " << itemName << " in " << containerName << ".\n";
        }
        };

    std::string verb = tokens[0];
    tokens.erase(tokens.begin());
    auto it = cmdMap.find(verb);
    if (it != cmdMap.end()) {
        it->second(tokens);
    }
    else {
        std::cout << "I don't understand " << verb << "\n";
    }
}
