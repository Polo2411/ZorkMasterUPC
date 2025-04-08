#include "world.h"

#include "sword.h"
#include "key.h"
#include "bag.h"
#include "health_potion.h"
#include "exit.h"
#include "entity.h"

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <functional>

// Tokeniza el comando
std::vector<std::string> tokenize(const std::string& input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

World::World()
{
    // Creamos la primera habitación
    Room* roomOne = new Room(
        "RoomOne",
        "You see a sword lying to the south, a bag to the north, and a key on the west side. "
        "There is an old door to the east."
    );

    // Creamos la segunda habitación
    Room* roomTwo = new Room(
        "RoomTwo",
        "A calmer chamber. There is a Health Potion on the north side."
    );

    // Conexiones reales para moverse
    // Suponiendo que la 1ra hab. se conecta a la 2da por 'east' (para "move east")
    roomOne->Connect("east", roomTwo);
    // Y si quieres poder volver:
    roomTwo->Connect("west", roomOne);

    // Objetos en roomOne con direcciones
    Sword* sword = new Sword("Sword", "A gleaming sword on the floor.", 20, "south", roomOne);
    Bag* bag = new Bag("Bag", "A sturdy bag with limited capacity.", 5, "north", roomOne);
    Key* key = new Key("Key", "A small rusty key.", "west", nullptr, roomOne);

    // Puerta (Exit) para simular estado bloqueado
    // Realmente, la clase Room ya tiene "Connect", pero supongamos que 'Exit' define
    // si se puede usar esa conexión. Por defecto, decimos que "roomOne->Connect(east, roomTwo)" 
    // ya está, pero controlamos si se puede o no mover con un Exit "Door".
    Exit* door = new Exit("Door", "A locked door leading east.", roomOne, roomTwo);
    door->SetState(LOCKED); // Bloqueada inicialmente

    // En la segunda habitación: poción en 'north'
    HealthPotion* potion = new HealthPotion("Health", "A potion that restores health.", 50, "north", roomTwo);

    // Guardamos habitaciones y creamos al jugador en RoomOne
    rooms.push_back(roomOne);
    rooms.push_back(roomTwo);
    player = new Player("Player", "A brave adventurer.", roomOne);
}

World::~World()
{
    for (Room* r : rooms) {
        delete r;
    }
    delete player;
}

void World::Run()
{
    std::cout << "Welcome to Zork\n";
    player->GetCurrentRoom()->Look();

    std::string command;
    while (true) {
        std::cout << "\n> ";
        std::getline(std::cin, command);
        if (command == "exit")
            break;
        ProcessCommand(command);
    }
}

// Comandos con lambdas
void World::ProcessCommand(const std::string& command)
{
    std::vector<std::string> tokens = tokenize(command);
    if (tokens.empty()) {
        std::cout << "Please enter a command.\n";
        return;
    }

    std::map<std::string, std::function<void(const std::vector<std::string>&)>> commandMap;

    // Comando "look"
    commandMap["look"] = [this](const std::vector<std::string>& params) {
        player->GetCurrentRoom()->Look();
        };

    // Comando "move"
    commandMap["move"] = [this](const std::vector<std::string>& params) {
        if (params.size() < 1) {
            std::cout << "Move where?\n";
            return;
        }
        // Ej.: "move east"
        std::string dir = params[0];

        // Verificamos si hay un Exit "Door" y si está bloqueada
        Entity* doorEnt = player->GetCurrentRoom()->FindChild("Door", EXIT);
        if (doorEnt) {
            Exit* door = dynamic_cast<Exit*>(doorEnt);
            // Si la dirección es la misma que la del Exit (roomOne->Connect("east", roomTwo)),
            // y el exit está LOCKED, no dejamos pasar
            // (aunque la room tenga Connect("east", roomTwo))
            if (dir == "east" && door->GetState() == LOCKED) {
                std::cout << "The door is locked.\n";
                return;
            }
            // Si está OPEN, permitimos move
        }

        // Llamamos a Move en el Player
        player->Move(dir);
        };

    // Comando "take"
    commandMap["take"] = [this](const std::vector<std::string>& params) {
        if (params.size() < 2) {
            std::cout << "Take what and from where? e.g., 'take key west'\n";
            return;
        }
        Room* current = player->GetCurrentRoom();
        std::string objectName = params[0];
        std::string direction = params[1];
        Entity* e = current->FindChild(objectName, ITEM);
        if (e) {
            Item* itemPtr = static_cast<Item*>(e);
            if (itemPtr->GetDirection() == direction) {
                if (objectName == "Key")
                    player->PickUp(static_cast<Key*>(e));
                else if (objectName == "Sword")
                    player->PickUp(static_cast<Sword*>(e));
                else if (objectName == "Bag")
                    player->PickUp(static_cast<Bag*>(e));
                else if (objectName == "Health")
                    player->PickUp(static_cast<HealthPotion*>(e));
                else
                    player->PickUp(itemPtr);

                current->children.remove(e);
            }
            else {
                std::cout << "There is no " << objectName << " in the " << direction << " direction.\n";
            }
        }
        else {
            std::cout << "No " << objectName << " found in that direction.\n";
        }
        };

    // Comando "open"
    commandMap["open"] = [this](const std::vector<std::string>& params) {
        if (params.empty()) {
            std::cout << "Open what?\n";
            return;
        }
        // Suponemos que el item a abrir es "door"
        if (params[0] == "door") {
            Entity* doorEnt = player->GetCurrentRoom()->FindChild("Door", EXIT);
            if (!doorEnt) {
                std::cout << "There is no door here.\n";
                return;
            }
            Exit* door = dynamic_cast<Exit*>(doorEnt);
            if (door->GetState() == LOCKED) {
                // Buscamos la Key en el inventario
                bool hasKey = false;
                for (auto itm : player->GetInventory()) {
                    if (itm->name == "Key") {
                        hasKey = true;
                        break;
                    }
                }
                if (hasKey) {
                    door->Open();
                    std::cout << "You unlocked and opened the door.\n";
                }
                else {
                    std::cout << "The door is locked. You need a key.\n";
                }
            }
            else {
                std::cout << "The door is already open.\n";
            }
        }
        else {
            std::cout << "I don't know how to open " << params[0] << "\n";
        }
        };

    // Comando "drop"
    commandMap["drop"] = [this](const std::vector<std::string>& params) {
        if (params.empty()) {
            std::cout << "Drop what?\n";
            return;
        }
        std::string itemName = params[0];
        auto inv = player->GetInventory();
        bool found = false;
        for (auto item : inv) {
            if (item->name == itemName) {
                player->Drop(item);
                found = true;
                break;
            }
        }
        if (!found)
            std::cout << "You don't have that item.\n";
        };

    // Separamos el verbo (primer token)
    std::string verb = tokens[0];
    tokens.erase(tokens.begin());

    auto it = commandMap.find(verb);
    if (it != commandMap.end()) {
        it->second(tokens);
    }
    else {
        std::cout << "I don't understand that command.\n";
    }
}
